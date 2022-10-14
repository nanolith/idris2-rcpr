/**
 * \file helper/psock/psock_handle_get_status.c
 *
 * \brief Get the status of the last operation on the given \ref psock_handle
 * instance.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include "psock_internal.h"

RCPR_IMPORT_resource;

/**
 * \brief Return the latest status code from a \ref psock_handle operation.
 *
 * \param handle                The \ref psock_handle to interrogate.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status RCPR_SYM(psock_handle_get_status)(RCPR_SYM(psock_handle)* handle)
{
    return handle->last_status;
}
