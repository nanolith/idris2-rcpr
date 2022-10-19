module RCPR

import public Control.Monad.Either
import public Control.Monad.Trans

public export
CIO : Type -> Type
CIO = EitherT Int IO

public export
interface HasIO io => LiftCIO io where
  liftCIO : CIO a -> io a

export %inline
LiftCIO CIO where
  liftCIO = id

export
runCIOAsEither : CIO a -> IO (Either Int a)
runCIOAsEither (MkEitherT io) = io

export
runCIOWith : Lazy (Int -> IO a) -> CIO a -> IO a
runCIOWith f (MkEitherT io) = io >>= either f pure

export
dispErr : Int -> String
dispErr val =
    "Error " ++ (show val)

export
runCIO : CIO () -> IO ()
runCIO = runCIOWith (putStrLn . dispErr)

export %inline
primCIO : PrimIO a -> CIO a
primCIO = primIO
