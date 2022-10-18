/**
 * \file helper/psock/psock_br_handle_get.c
 *
 * \brief Cast an opaque \ref psock_br_handle reference to the concrete type.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include "psock_internal.h"

typedef RCPR_SYM(psock_br_handle) psock_br_handle;

/**
 * \brief Helper to cast an opaque \ref psock_br_handle instance to a
 * \ref psock_br_handle instance.
 *
 * \param opaque_handle         A type erased handle to cast.
 *
 * \returns The non-opaque reference.
 */
RCPR_SYM(psock_br_handle)* RCPR_SYM(psock_br_handle_get)(void* opaque_handle)
{
    return (psock_br_handle*)opaque_handle;
}
