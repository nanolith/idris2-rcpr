/**
 * \file helper/psock/psock_handle_release.c
 *
 * \brief Release a psock handle instance.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include "psock_internal.h"

RCPR_IMPORT_resource;

/**
 * \brief Attempt to release a \ref psock_handle instance.
 *
 * \param handle                The \ref psock_handle instance to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status RCPR_SYM(psock_handle_release)(RCPR_SYM(psock_handle)* handle)
{
    return
        resource_release(&handle->hdr);
}
