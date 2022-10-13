/**
 * \file rcprhelper/config.h
 *
 * \brief Generated configuration file data for idris2-rcpr.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#define MAKE_C_VERSION(X,Y) V ## X ## _ ## Y
#define IDRIS2_RCPR_VERSION_SYM \
    MAKE_C_VERSION(@IDRIS2_RCPR_VERSION_MAJOR@, @IDRIS2_RCPR_VERSION_MINOR@)

#define IDRIS2_RCPR_VERSION_STRING \
    "@IDRIS2_RCPR_VERSION_MAJOR@.@IDRIS2_RCPR_VERSION_MINOR@.@IDRIS2_RCPR_VERSION_REL@"
