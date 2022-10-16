/**
 * \file helper/psock/psock_handle_accept.c
 *
 * \brief Accept a \ref psock_handle from a listening \ref psock_handle.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <string.h>
#include <unistd.h>

#include "psock_internal.h"

RCPR_IMPORT_allocator;
RCPR_IMPORT_psock;
RCPR_IMPORT_resource;

typedef RCPR_SYM(psock_handle) psock_handle;

/**
 * \brief Accept a connection from the given listening \ref psock_handle
 * instance as a new \ref psock_handle instance.
 *
 * \note The returned \ref psock_handle should be checked for errors and NULL.
 *
 * \param accept_handle         The \ref psock_handle from which a connection is
 *                              accepted.
 *
 * \returns a \ref psock_handle or NULL.
 */
RCPR_SYM(psock_handle)* RCPR_SYM(psock_handle_accept)(
    RCPR_SYM(psock_handle)* accept_handle)
{
    psock_handle* retval = NULL;
    status stat;
    struct sockaddr_in addr;
    int desc;
    socklen_t addrlen = sizeof(addr);

    /* parameter sanity checks. */
    RCPR_MODEL_ASSERT(prop_psock_handle_valid(accept_handle));

    /* attempt to accept a socket from the listening socket. */
    stat =
        psock_accept(
            accept_handle->sock, &desc, (struct sockaddr*)&addr, &addrlen);
    if (STATUS_SUCCESS != stat)
    {
        goto done;
    }

    /* attempt to allocate the new handle structure. */
    stat =
        allocator_allocate(
            accept_handle->alloc, (void**)&retval, sizeof(*retval));
    if (STATUS_SUCCESS != stat)
    {
        retval = NULL;
        goto cleanup_desc;
    }

    /* clear the structure. */
    memset(retval, 0, sizeof(*retval));

    /* initialize resource. */
    resource_init(&retval->hdr, &RCPR_SYM(psock_handle_resource_release));

    /* copy allocator. */
    retval->owns_allocator = false;
    retval->alloc = accept_handle->alloc;

    /* create psock instance from descriptor. */
    stat = psock_create_from_descriptor(&retval->sock, retval->alloc, desc);
    if (STATUS_SUCCESS != stat)
    {
        goto done;
    }

    /* copy address. */
    memcpy(&retval->addr, &addr, addrlen);
    retval->addrlen = addrlen;

    /* success. */
    stat = STATUS_SUCCESS;
    goto done;

cleanup_desc:
    close(desc);

done:
    if (NULL != retval)
    {
        retval->last_status = stat;
    }

    return retval;
}
