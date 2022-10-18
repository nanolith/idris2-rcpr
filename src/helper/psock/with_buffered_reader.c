/**
 * \file helper/psock/with_buffered_reader.c
 *
 * \brief Create a buffered reader handle from the given psock handle, and use
 * it to call the given callback before releasing it.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include "psock_internal.h"

RCPR_IMPORT_resource;

typedef RCPR_SYM(psock_br_handle) psock_br_handle;

/**
 * \brief Create a buffered reader handle using the given psock handle, use it
 * to call the given callback, then release it.
 *
 * \param handle                The \ref psock_handle to use to create this
 *                              buffered reader handle.
 * \param callback_fn           The callback function to call with the created
 *                              handle on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status RCPR_SYM(with_buffered_reader)(
    RCPR_SYM(psock_handle)* handle,
    void (*callback_fn)(RCPR_SYM(psock_br_handle)*))
{
    status retval, release_retval;
    psock_br_handle* br;

    /* accept a new handle. */
    br = RCPR_SYM(psock_br_handle_create)(handle);
    if (NULL == br)
    {
        retval = ERROR_GENERAL_OUT_OF_MEMORY;
        goto done;
    }

    /* decode the last status to ensure that this handle was created
     * successfully. */
    retval = br->last_status;
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_br;
    }

    /* call the callback function. */
    callback_fn(br);

    /* clean up. */
    retval = STATUS_SUCCESS;
    goto cleanup_br;

cleanup_br:
    release_retval = resource_release(&br->hdr);
    if (STATUS_SUCCESS != release_retval)
    {
        retval = release_retval;
    }

done:
    return retval;
}
