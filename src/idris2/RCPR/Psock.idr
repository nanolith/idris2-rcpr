module RCPR.Psock

import RCPR
import RCPR.Library
import System.FFI

%default total

||| Opaque handle for a Psock instance.
export
PsockHandle : Type
PsockHandle = Struct (rcprSym "psock_handle") [] 

||| Opaque handle for a Buffered Psock Reader instance.
export
PsockBufferedReaderHandle : Type
PsockBufferedReaderHandle = Struct (rcprSym "psock_br_handle") [] 

||| Opaque reference to a C array.
export
CArray : Type
CArray = Struct (rcprSym "psock_c_array") []

%foreign (librcprhelper "psock_br_handle_get_status")
prim__PsockBufferedReaderHandleGetStatus :
    PsockBufferedReaderHandle -> PrimIO Int

%foreign (librcprhelper "psock_br_status_is_partial_line_read")
prim__psockStatusIsPartialRead : Int -> Int

psockStatusIsPartialRead : Int -> Bool
psockStatusIsPartialRead status =
    if prim__psockStatusIsPartialRead status == 0
        then False
        else True

%foreign (librcprhelper "psock_get_string")
getString : Ptr String -> String

%foreign (librcprhelper "psock_handle_get")
getPsockHandle : Ptr PsockHandle -> PsockHandle

%foreign (librcprhelper "psock_br_handle_get")
getPsockBuffedReaderHandle
    : Ptr PsockBufferedReaderHandle -> PsockBufferedReaderHandle

psockBufferedReaderHandleGetStatus
    : HasIO io => PsockBufferedReaderHandle -> io Int
psockBufferedReaderHandleGetStatus handle =
    primIO (prim__PsockBufferedReaderHandleGetStatus handle)

%foreign (librcprhelper "psock_br_handle_read_line")
prim__readLine : PsockBufferedReaderHandle -> PrimIO (Ptr String)

readLinePartial : PsockBufferedReaderHandle -> IO (Ptr String)
readLinePartial handle = primIO (prim__readLine handle)

readLineHelper : Nat -> PsockBufferedReaderHandle -> String -> CIO String
readLineHelper Z handle line = throwError 1
readLineHelper (S x) handle line = do
    pstring <- lift $ readLinePartial handle
    status <- psockBufferedReaderHandleGetStatus handle
    if 0 == status
        then pure $ line ++ getString pstring
        else if psockStatusIsPartialRead status
            then readLineHelper x handle $ line ++ getString pstring
            else throwError status

readLine : PsockBufferedReaderHandle -> CIO String
readLine handle =
    readLineHelper 100 handle ""

%foreign (librcprhelper "psock_br_handle_write_string_line")
prim__brWriteStringLine : PsockBufferedReaderHandle -> String -> PrimIO Int

||| Write a string as a line to the given Buffered Psock instance.
|||
||| Parameters:
||| - @handle is the Buffered Psock Reader instance to which the string is
|||   written.
||| - @str is the string to write.
export
brWriteStringLine : HasIO io => PsockBufferedReaderHandle -> String -> io ()
brWriteStringLine handle str = do
    status <- primIO $ prim__brWriteStringLine handle str
    pure ()

%foreign (librcprhelper "with_psock_handle_created_from_listen_address")
prim__withPsockHandleFromListenAddress
    : String -> Bits16 -> (Ptr PsockHandle -> PrimIO ()) -> PrimIO Int

||| Create a PsockHandle from the given listen address and port, passing this to
||| the provided function.
|||
||| After the function is called, this socket instance is automatically closed.
|||
||| Parameters:
||| - @addr is the listen address (IP address string).
||| - @port is the listen port.
||| - @fn is the function to call with the created handle.
export
withPsockHandleFromListenAddress :
    String -> Bits16 -> (PsockHandle -> IO ()) -> CIO ()
withPsockHandleFromListenAddress addr port fn = do
    stat <- lift $ primIO $ prim__withPsockHandleFromListenAddress addr port $
        \h => toPrim $ fn $ getPsockHandle h
    if stat == 0
        then pure ()
        else throwError stat

%foreign (librcprhelper "with_accepted_psock_handle")
prim__withAcceptedPsockHandle
    : PsockHandle -> (Ptr PsockHandle -> PrimIO ()) -> PrimIO Int

||| Accept a socket connection from the given listen socket handle, create a
||| PsockHandle using this connection, and pass it to the provided function.
|||
||| After the function is called, the accepted connection is automatically
||| closed.
|||
||| Parameters:
||| - @handle is the listening socket handle from which a connection is
|||   accepted.
||| - @fn is the function to call with this accepted connection.
export
withAcceptedPsockHandle :
    PsockHandle -> (PsockHandle -> IO ()) -> CIO ()
withAcceptedPsockHandle handle fn = do
    stat <- lift $ primIO $ prim__withAcceptedPsockHandle handle $
        \h => toPrim $ fn $ getPsockHandle h
    if stat == 0
        then pure ()
        else throwError stat

%foreign (librcprhelper "with_buffered_reader")
prim__withBufferedReader
    : PsockHandle -> (Ptr PsockBufferedReaderHandle -> PrimIO()) -> PrimIO Int

||| Create a buffered reader from the given PsockHandle instance, and call the
||| provided function with this handle.
|||
||| After the function is called, the buffered reader instance is automatically
||| torn down.
|||
||| Parameters:
||| - @handle is the PsockHandle instance from which this buffered reader is
|||   created.
||| - @fn is the function to call with this buffered reader instance.
export
withBufferedReader :
    PsockHandle -> (PsockBufferedReaderHandle -> IO ()) -> CIO ()
withBufferedReader handle fn = do
    stat <- lift $ primIO $ prim__withBufferedReader handle $
        \br => toPrim $ fn $ getPsockBuffedReaderHandle br
    if stat == 0
        then pure ()
        else throwError stat

||| Read a line from the given PsockBufferedReaderHandle instance, and call the
||| provided function with this line.
|||
||| Parameters:
||| - @handle is the buffered reader handle from which this line is read.
||| - @fn is the function to call with this line.
export
withReadLine : PsockBufferedReaderHandle -> (String -> IO ()) -> CIO ()
withReadLine handle fn = do
    line <- readLine handle
    lift $ fn line

%foreign (librcprhelper "with_read_raw_buffer")
prim__withReadRawBuffer
    : PsockBufferedReaderHandle -> (CArray -> PrimIO ()) -> PrimIO Int

||| Read a raw buffer of the given size from the given buffered reader handle,
||| and call the provided function with this raw buffer.
|||
||| After the function is called, the created CArray instance is automatically
||| reclaimed by the allocator.
|||
||| Parameters:
||| - @br is the buffered reader handle instance for this operation.
||| - @fn is the function to call with the raw buffer after it is read.
export
withReadRawBuffer : PsockBufferedReaderHandle -> (CArray -> IO ()) -> CIO ()
withReadRawBuffer br fn = do
    stat <- lift $ primIO $ prim__withReadRawBuffer br $ \arr => toPrim $ fn arr
    if stat == 0
        then pure ()
        else throwError stat
