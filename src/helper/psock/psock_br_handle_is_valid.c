/**
 * \file helper/psock/psock_br_handle_is_valid.c
 *
 * \brief Return true if the given psock buffered reader handle is valid.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include "psock_internal.h"

RCPR_IMPORT_resource;

/**
 * \brief Return true if the given handle is not NULL.
 *
 * \param handle                The \ref psock_br_handle to check.
 *
 * \returns true if the handle is not NULL and false otherwise.
 */
bool RCPR_SYM(psock_br_handle_is_valid)(RCPR_SYM(psock_br_handle)* handle)
{
    return handle != NULL;
}
