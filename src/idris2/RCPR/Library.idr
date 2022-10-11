module RCPR.Library

import System.FFI

%default total

export
librcpr : String -> String
librcpr fn =
    "C:rcpr_u0ec71e88_25af_40aa_8dd9_990d596b60de_V0_2_"
        ++ fn ++ ",librcpr-0.2.1"

export
librcprhelper : String -> String
librcprhelper fn = "C:" ++ fn ++ ",librcprhelper"
