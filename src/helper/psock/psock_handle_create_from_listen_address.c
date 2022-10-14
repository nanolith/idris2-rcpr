/**
 * \file helper/psock/psock_handle_create_from_listen_address.c
 *
 * \brief Create a psock handle from a listen address and port.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <arpa/inet.h>
#include <string.h>

#include "psock_internal.h"

RCPR_IMPORT_allocator;
RCPR_IMPORT_psock;
RCPR_IMPORT_resource;

typedef RCPR_SYM(psock_handle) psock_handle;

/**
 * \brief Create a \ref psock_handle from a listen address and port.
 *
 * \note that the returned \ref psock_handle should be checked for errors and
 * NULL.
 *
 * \param listen_address        The listen address from which this handle is
 *                              created.
 * \param port                  The port from which this handle is created.
 *
 * \returns a \ref psock_handle or NULL.
 */
RCPR_SYM(psock_handle)* RCPR_SYM(psock_handle_create_from_listen_address)(
    const char* listen_address, uint16_t port)
{
    psock_handle* retval = NULL;
    allocator* alloc = NULL;
    bool owns_allocator = true;
    status stat;
    struct sockaddr_in addr;

    /* create a malloc allocator. */
    stat = malloc_allocator_create(&alloc);
    if (STATUS_SUCCESS != stat)
    {
        goto done;
    }

    /* use it to allocate a psock handle. */
    stat = allocator_allocate(alloc, (void**)&retval, sizeof(*retval));
    if (STATUS_SUCCESS != stat)
    {
        retval = NULL;
        goto cleanup_allocator;
    }

    /* clear this handle. */
    memset(retval, 0, sizeof(*retval));

    /* initialize it as a resource. */
    resource_init(&retval->hdr, &RCPR_SYM(psock_handle_resource_release));

    /* assign the allocator to the handle. */
    retval->alloc = alloc;
    retval->owns_allocator = true;

    /* at this point, the return value is up to the caller, and ownership of the
     * allocator passes to the handle / caller. */
    owns_allocator = false;

    /* attempt to translate the listen address to an IP address. */
    if (1 != inet_pton(AF_INET, listen_address, &addr.sin_addr))
    {
        stat = ERROR_PSOCK_CREATE_FROM_ADDRESS_BIND;
        goto done;
    }

    /* create a listening psock. */
    stat =
        psock_create_from_listen_address(
            &retval->sock, retval->alloc, (const struct sockaddr*)&addr,
            sizeof(addr));
    if (STATUS_SUCCESS != stat)
    {
        goto done;
    }

    /* set the port. */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    /* success. */
    stat = STATUS_SUCCESS;
    goto done;

cleanup_allocator:
    if (owns_allocator)
    {
        stat = resource_release(allocator_resource_handle(alloc));
        (void)stat;
    }

done:
    if (NULL != retval)
    {
        retval->last_status = stat;
    }

    return retval;
}
