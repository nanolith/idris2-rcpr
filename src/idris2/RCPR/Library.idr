module RCPR.Library

import System.FFI

%default total

||| Given a C symbol name, provide the fully qualified RCPR C symbol name.
|||
||| Parameters:
||| - @sym the symbol name to expand.
public export
rcprSym : String -> String
rcprSym sym =
    "rcpr_u0ec71e88_25af_40aa_8dd9_990d596b60de_V0_2_" ++ sym

||| Create the RCPR foreign library definition for the given symbol.
|||
||| Parameters:
||| - @sym the symbol name.
public export
librcpr : String -> String
librcpr sym =
    "C:" ++ (rcprSym sym) ++ ",librcpr-0.2.1.so"

||| Create the RCPR helper foreign library definition for the given symbol.
|||
||| Parameters:
||| - @sym the symbol name.
public export
librcprhelper : String -> String
librcprhelper sym =
    "C:" ++ (rcprSym sym) ++ ",librcprhelper-0.2.1.so"
