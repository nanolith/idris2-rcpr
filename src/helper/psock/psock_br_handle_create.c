/**
 * \file helper/psock/psock_br_handle_create.c
 *
 * \brief Create a psock buffered reader handle from a psock handle.
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
typedef RCPR_SYM(psock_br_handle) psock_br_handle;

/**
 * \brief Create a \ref psock_br_handle from a \ref psock_handle.
 *
 * \note The returned \ref psock_br_handle should be checked for errors and
 * NULL. Furthermore, this \ref psock_br_handle is a separate instance from the
 * \ref psock_handle but is dependent on it. It must be released BEFORE the
 * underlying \ref psock_handle is released. Reads from this handle are
 * buffered: reading from the underlying \ref psock_handle instance will be out
 * of order and will lead to data corruption once this \ref psock_br_handle
 * instance is read from.
 *
 * \param parent_handle         The parent \ref psock_handle from which this
 *                              instance is created.
 *
 * \returns a \ref psock_br_handle or NULL.
 */
RCPR_SYM(psock_br_handle)* RCPR_SYM(psock_br_handle_create)(
    RCPR_SYM(psock_handle)* parent_handle)
{
    psock_br_handle* retval = NULL;
    status stat;

    /* parameter sanity checks. */
    RCPR_MODEL_ASSERT(prop_psock_br_handle_valid(parent_handle));

    /* attempt to allocate the new handle structure. */
    stat =
        allocator_allocate(
            parent_handle->alloc, (void**)&retval, sizeof(*retval));
    if (STATUS_SUCCESS != stat)
    {
        retval = NULL;
        goto done;
    }

    /* clear the structure. */
    memset(retval, 0, sizeof(*retval));

    /* initialize the resource. */
    resource_init(&retval->hdr, &RCPR_SYM(psock_br_handle_resource_release));

    /* copy allocator. */
    retval->owns_allocator = false;
    retval->alloc = parent_handle->alloc;

    /* create a psock_br instance from a psock. */
    stat =
        psock_br_create_from_psock(
            &retval->br, retval->alloc, parent_handle->sock);
    if (STATUS_SUCCESS != stat)
    {
        goto done;
    }

    /* success. */
    stat = STATUS_SUCCESS;
    goto done;

done:
    if (NULL != retval)
    {
        retval->last_status = stat;
    }

    return retval;
}
