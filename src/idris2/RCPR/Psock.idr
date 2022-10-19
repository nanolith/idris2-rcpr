module RCPR.Psock

import RCPR
import RCPR.Library
import System.FFI

%default total

export
PsockHandle : Type
PsockHandle = Struct (rcprSym "psock_handle") [] 

export
PsockBufferedReaderHandle : Type
PsockBufferedReaderHandle = Struct (rcprSym "psock_br_handle") [] 

%foreign (librcprhelper "psock_handle_get_status")
prim__PsockHandleGetStatus : PsockHandle -> PrimIO Int

export
psockHandleGetStatus : HasIO io => PsockHandle -> io Int
psockHandleGetStatus handle = primIO (prim__PsockHandleGetStatus handle)

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

export
%foreign (librcprhelper "psock_handle_get")
getPsockHandle : Ptr PsockHandle -> PsockHandle

export
%foreign (librcprhelper "psock_br_handle_get")
getPsockBuffedReaderHandle
    : Ptr PsockBufferedReaderHandle -> PsockBufferedReaderHandle

export
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

export
readLine : PsockBufferedReaderHandle -> CIO String
readLine handle =
    readLineHelper 100 handle ""

%foreign (librcprhelper "psock_br_handle_write_string_line")
prim__brWriteStringLine : PsockBufferedReaderHandle -> String -> PrimIO Int

export
brWriteStringLine : HasIO io => PsockBufferedReaderHandle -> String -> io ()
brWriteStringLine handle str = do
    status <- primIO $ prim__brWriteStringLine handle str
    pure ()

%foreign (librcprhelper "psock_handle_release")
prim__PsockHandleRelease : PsockHandle -> PrimIO Int

export
psockHandleRelease : HasIO io => PsockHandle -> io Int
psockHandleRelease handle = primIO (prim__PsockHandleRelease handle)

%foreign (librcprhelper "psock_br_handle_release")
prim__PsockBufferedReaderHandleRelease : PsockBufferedReaderHandle -> PrimIO Int

export
psockBufferedReaderHandleRelease
    : HasIO io => PsockBufferedReaderHandle -> io Int
psockBufferedReaderHandleRelease handle =
    primIO (prim__PsockBufferedReaderHandleRelease handle)

%foreign (librcprhelper "with_psock_handle_created_from_listen_address")
prim__withPsockHandleFromListenAddress
    : String -> Bits16 -> (Ptr PsockHandle -> PrimIO ()) -> PrimIO Int

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

export
withBufferedReader :
    PsockHandle -> (PsockBufferedReaderHandle -> IO ()) -> CIO ()
withBufferedReader handle fn = do
    stat <- lift $ primIO $ prim__withBufferedReader handle $
        \br => toPrim $ fn $ getPsockBuffedReaderHandle br
    if stat == 0
        then pure ()
        else throwError stat

export
withReadLine : PsockBufferedReaderHandle -> (String -> IO ()) -> CIO ()
withReadLine handle fn = do
    line <- readLine handle
    lift $ fn line
