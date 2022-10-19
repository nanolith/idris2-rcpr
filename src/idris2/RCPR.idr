module RCPR

import public Control.Monad.Either
import public Control.Monad.Trans

||| CIO is an Either Monad Transformer wrapper for RCPR C FFI operations.
|||
||| A C FFI operation can either succeed or fail, and when it fails, it returns
||| an error code.
public export
CIO : Type -> Type
CIO = EitherT Int IO

||| LiftCIO provides IO.
public export
interface HasIO io => LiftCIO io where
  liftCIO : CIO a -> io a

||| LiftCIO provides IO.
export %inline
LiftCIO CIO where
  liftCIO = id

||| Run a CIO operation, returning an Either value in the IO Monad.
export
runCIOAsEither : CIO a -> IO (Either Int a)
runCIOAsEither (MkEitherT io) = io

||| Run a CIO operation, using the provided function to handle an error.
export
runCIOWith : Lazy (Int -> IO a) -> CIO a -> IO a
runCIOWith f (MkEitherT io) = io >>= either f pure

||| Display an error message.
export
dispErr : Int -> String
dispErr val =
    "Error " ++ (show val)

||| Run a CIO operation, logging errors to the console.
export
runCIO : CIO () -> IO ()
runCIO = runCIOWith (putStrLn . dispErr)

||| Lift a PrimIO operation to a CIO operation.
export %inline
primCIO : PrimIO a -> CIO a
primCIO = primIO
