/**
 * \file helper/psock/psock_internal.h
 *
 * \brief Internal psock helper library.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <arpa/inet.h>
#include <rcpr/allocator.h>
#include <rcpr/psock.h>
#include <rcpr/resource/protected.h>
#include <rcprhelper/psock.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

struct RCPR_SYM(psock_handle)
{
    RCPR_SYM(resource) hdr;
    bool owns_allocator;
    RCPR_SYM(allocator)* alloc;
    RCPR_SYM(psock)* sock;
    status last_status;
    struct sockaddr_in addr;
    socklen_t addrlen;
};

struct RCPR_SYM(psock_br_handle)
{
    RCPR_SYM(resource) hdr;
    bool owns_allocator;
    RCPR_SYM(allocator)* alloc;
    RCPR_SYM(psock_br)* br;
    status last_status;
};

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
    const char* listen_address, uint16_t port);

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
    RCPR_SYM(psock_handle)* accept_handle);

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
    RCPR_SYM(psock_handle)* parent_handle);

/**
 * \brief Release a \ref psock_handle resource.
 *
 * \param r         The resource to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status RCPR_SYM(psock_handle_resource_release)(RCPR_SYM(resource)* r);

/**
 * \brief Release a \ref psock_br_handle resource.
 *
 * \param r         The resource to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status RCPR_SYM(psock_br_handle_resource_release)(RCPR_SYM(resource)* r);

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
