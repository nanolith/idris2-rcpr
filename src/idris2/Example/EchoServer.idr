module Example.EchoServer

import RCPR
import RCPR.Psock

%default total

echoLoop : Nat -> PsockBufferedReaderHandle -> IO ()
echoLoop Z _ = pure ()
echoLoop (S x) handle = do
    withReadLine handle $ brWriteStringLine handle
    status <- psockBufferedReaderHandleGetStatus handle
    if status == 0
        then echoLoop x handle
        else pure ()

dispatchSocket : PsockHandle -> IO ()
dispatchSocket handle = do
    runCIO $ withBufferedReader handle $ echoLoop 1000

acceptAndDispatch : Nat -> PsockHandle -> IO ()
acceptAndDispatch Z _ = pure ()
acceptAndDispatch (S x) handle = do
    runCIO $ withAcceptedPsockHandle handle $ dispatchSocket
    acceptAndDispatch x handle
        
main : IO ()
main =
    runCIO $
        withPsockHandleFromListenAddress "0.0.0.0" 9001 $ acceptAndDispatch 1000
