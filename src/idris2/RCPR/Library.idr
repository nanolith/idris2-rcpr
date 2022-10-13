module RCPR.Library

import System.FFI

%default total

public export
rcprSym : String -> String
rcprSym fn =
    "rcpr_u0ec71e88_25af_40aa_8dd9_990d596b60de_V0_2_" ++ fn

public export
librcpr : String -> String
librcpr fn =
    "C:" ++ (rcprSym fn) ++ ",librcpr-0.2.1"

public export
librcprhelper : String -> String
librcprhelper fn =
    "C:" ++ (rcprSym fn) ++ ",librcprhelper"
