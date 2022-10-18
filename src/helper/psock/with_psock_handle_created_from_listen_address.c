/**
 * \file helper/psock/with_psock_handle_created_from_listen_address.c
 *
 * \brief Create a psock handle from a listen address and use it to call the
 * given callback before releasing it.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include "psock_internal.h"

RCPR_IMPORT_resource;

typedef RCPR_SYM(psock_handle) psock_handle;

/**
 * \brief Create a psock handle from a listen address, use it to call the given
 * callback, then release it.
 *
 * \param listen_address        The listen address from which this handle is
 *                              created.
 * \param port                  The port from which this handle is created.
 * \param callback_fn           The callback function to call with the created
 *                              handle on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status RCPR_SYM(with_psock_handle_created_from_listen_address)(
    const char* listen_address, uint16_t port,
    void (*callback_fn)(RCPR_SYM(psock_handle)*))
{
    status retval, release_retval;
    psock_handle* handle;

    /* create the handle. */
    handle =
        RCPR_SYM(psock_handle_create_from_listen_address)(listen_address, port);
    if (NULL == handle)
    {
        retval = ERROR_GENERAL_OUT_OF_MEMORY;
        goto done;
    }

    /* decode the last status to ensure that this handle was created
     * successfully. */
    retval = handle->last_status;
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_handle;
    }

    /* call the callback function. */
    callback_fn(handle);

    /* clean up. */
    retval = STATUS_SUCCESS;
    goto cleanup_handle;

cleanup_handle:
    release_retval = resource_release(&handle->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}
