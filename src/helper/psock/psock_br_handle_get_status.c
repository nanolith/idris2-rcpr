/**
 * \file helper/psock/psock_br_handle_get_status.c
 *
 * \brief Get the last status from a \ref psock_br_handle instance.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include "psock_internal.h"

/**
 * \brief Return the latest status code from a \ref psock_br_handle operation.
 *
 * \param handle                The \ref psock_br_handle to interrogate.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status RCPR_SYM(psock_br_handle_get_status)(RCPR_SYM(psock_br_handle)* handle)
{
    /* parameter sanity checks. */
    RCPR_MODEL_ASSERT(prop_psock_br_handle_valid(handle));

    return handle->last_status;
}
