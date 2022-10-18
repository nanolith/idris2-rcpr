/**
 * \file helper/psock/with_accepted_psock_handle.c
 *
 * \brief Create a psock handle from an accepted socket and use it to call the
 * given callback before releasing it.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include "psock_internal.h"

RCPR_IMPORT_resource;

typedef RCPR_SYM(psock_handle) psock_handle;

/**
 * \brief Create a psock handle from an accepted socket, use it to call the
 * given callback, then release it.
 *
 * \param listen_handle         The handle of the listen socket.
 * \param callback_fn           The callback function to call with the created
 *                              handle on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status RCPR_SYM(with_accepted_psock_handle)(
    RCPR_SYM(psock_handle)* listen_handle,
    void (*callback_fn)(RCPR_SYM(psock_handle)*))
{
    status retval, release_retval;
    psock_handle* handle;

    /* accept a new handle. */
    handle = RCPR_SYM(psock_handle_accept)(listen_handle);
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
