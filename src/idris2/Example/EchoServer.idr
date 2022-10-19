module Example.EchoServer

import RCPR
import RCPR.Psock

%default total

echoLoop : Nat -> PsockBufferedReaderHandle -> CIO ()
echoLoop Z _ = pure ()
echoLoop (S x) handle = do
    withReadLine handle $ brWriteStringLine handle
    echoLoop x handle

dispatchSocket : PsockHandle -> IO ()
dispatchSocket handle = do
    runCIO $ withBufferedReader handle $ \br => runCIO $ echoLoop 1000 br

acceptAndDispatch : Nat -> PsockHandle -> IO ()
acceptAndDispatch Z _ = pure ()
acceptAndDispatch (S x) handle = do
    runCIO $ withAcceptedPsockHandle handle $ dispatchSocket
    acceptAndDispatch x handle
        
main : IO ()
main =
    runCIO $
        withPsockHandleFromListenAddress "0.0.0.0" 9001 $ acceptAndDispatch 1000
