/**
 * \file helper/psock/psock_handle_resource_release.c
 *
 * \brief Release a \ref psock_handle resource.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <string.h>

#include "psock_internal.h"

RCPR_IMPORT_allocator;
RCPR_IMPORT_psock;
RCPR_IMPORT_resource;

typedef RCPR_SYM(psock_handle) psock_handle;

/**
 * \brief Release a \ref psock_handle resource.
 *
 * \param r         The resource to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status RCPR_SYM(psock_handle_resource_release)(RCPR_SYM(resource)* r)
{
    status psock_release_retval = STATUS_SUCCESS;
    status reclaim_retval = STATUS_SUCCESS;
    status allocator_release_retval = STATUS_SUCCESS;
    psock_handle* handle = (psock_handle*)r;

    /* cache allocator. */
    allocator* alloc = handle->alloc;

    /* cache allocator ownership. */
    bool owns_allocator = handle->owns_allocator;

    /* release socket if set. */
    if (NULL != handle->sock)
    {
        psock_release_retval =
            resource_release(psock_resource_handle(handle->sock));
    }

    /* clear the structure. */
    memset(handle, 0, sizeof(*handle));

    /* reclaim memory. */
    reclaim_retval = allocator_reclaim(alloc, handle);

    /* if we own the allocator, release it. */
    if (owns_allocator)
    {
        allocator_release_retval =
            resource_release(allocator_resource_handle(alloc));
    }

    /* decode the return value. */
    if (STATUS_SUCCESS != psock_release_retval)
    {
        return psock_release_retval;
    }
    else if (STATUS_SUCCESS != reclaim_retval)
    {
        return reclaim_retval;
    }
    else
    {
        return allocator_release_retval;
    }
}
