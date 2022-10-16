/**
 * \file helper/psock/psock_br_handle_resource_release.c
 *
 * \brief Release a \ref psock_br_handle resource.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <string.h>

#include "psock_internal.h"

RCPR_IMPORT_allocator;
RCPR_IMPORT_psock;
RCPR_IMPORT_resource;

typedef RCPR_SYM(psock_br_handle) psock_br_handle;

/**
 * \brief Release a \ref psock_br_handle resource.
 *
 * \param r         The resource to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status RCPR_SYM(psock_br_handle_resource_release)(RCPR_SYM(resource)* r)
{
    status br_release_retval = STATUS_SUCCESS;
    status reclaim_retval = STATUS_SUCCESS;
    status alloc_release_retval = STATUS_SUCCESS;
    psock_br_handle* handle = (psock_br_handle*)r;

    /* parameter sanity checks. */
    RCPR_MODEL_ASSERT(prop_psock_br_handle_valid(handle));

    /* cache allocator. */
    allocator* alloc = handle->alloc;

    /* cache allocator ownership. */
    bool owns_allocator = handle->owns_allocator;

    /* release buffered reader if set. */
    if (NULL != handle->br)
    {
        br_release_retval =
            resource_release(psock_br_resource_handle(handle->br));
    }

    /* clear the structure. */
    memset(handle, 0, sizeof(*handle));

    /* reclaim memory. */
    reclaim_retval = allocator_reclaim(alloc, handle);

    /* if we own the allocator, release it. */
    if (owns_allocator)
    {
        alloc_release_retval =
            resource_release(allocator_resource_handle(alloc));
    }

    /* decode the return values. */
    if (STATUS_SUCCESS != br_release_retval)
    {
        return br_release_retval;
    }
    else if (STATUS_SUCCESS != reclaim_retval)
    {
        return reclaim_retval;
    }
    else
    {
        return alloc_release_retval;
    }
}
