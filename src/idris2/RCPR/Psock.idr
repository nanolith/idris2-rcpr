module RCPR.Psock

import RCPR.Library
import System.FFI

%default total

PsockHandle : Type
PsockHandle = Struct (rcprSym "psock_handle") [] 

PsockBufferedReaderHandle : Type
PsockBufferedReaderHandle = Struct (rcprSym "psock_br_handle") [] 

%foreign (librcprhelper "psock_handle_create_from_listen_address")
prim__mkPsockHandleFromListenAddress :
    String -> Bits16 -> PrimIO (Ptr PsockHandle)

%foreign (librcprhelper "psock_handle_accept")
prim__accept : PsockHandle -> PrimIO (Ptr PsockHandle)

%foreign (librcprhelper "psock_br_handle_create")
prim__mkPsockBufferedReaderHandle :
    PsockHandle -> PrimIO (Ptr PsockBufferedReaderHandle)

%foreign (librcprhelper "psock_handle_get_status")
prim_PsockHandleGetStatus : PsockHandle -> PrimIO Int

%foreign (librcprhelper "psock_br_handle_get_status")
prim__PsockBufferedReaderHandleGetStatus :
    PsockBufferedReaderHandle -> PrimIO Int

%foreign (librcprhelper "psock_br_handle_read_line")
prim__readLine : PsockBufferedReaderHandle -> PrimIO (Ptr String)

%foreign (librcprhelper "psock_handle_release")
prim__PsockHandleRelease : PsockHandle -> PrimIO Int

%foreign (librcprhelper "psock_br_handle_release")
prim__PsockBufferedReaderHandleRelease : PsockBufferedReaderHandle -> PrimIO Int
