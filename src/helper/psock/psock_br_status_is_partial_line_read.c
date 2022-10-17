/**
 * \file helper/psock/psock_br_status_is_partial_line_read.c
 *
 * \brief Return true if the given status indicates a partial line read.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include "psock_internal.h"

RCPR_IMPORT_resource;

/**
 * \brief Return true if the given status is a partial line read status.
 *
 * \param s                     The status to check.
 *
 * \returns true if the status is a partial line read status and false
 * otherwise.
 */
bool RCPR_SYM(psock_br_status_is_partial_line_read)(status s)
{
    return s == ERROR_PSOCK_BR_READ_LINE_TRUNCATED;
}
