module Example.EchoServer

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

acceptAndDispatch : Nat -> PsockHandle -> IO ()
acceptAndDispatch Z _ = pure ()
acceptAndDispatch (S x) handle = do
    withAcceptedPsockHandle handle $ flip withBufferedReader $ echoLoop 1000
    status <- psockHandleGetStatus handle
    if status == 0
        then acceptAndDispatch x handle
        else pure ()
        
main : IO ()
main =
    withPsockHandleFromListenAddress "0.0.0.0" 9001 $ acceptAndDispatch 1000
