/**
 * \file helper/psock/psock_internal.h
 *
 * \brief Internal psock helper library.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <rcpr/allocator.h>
#include <rcpr/psock.h>
#include <rcpr/resource/protected.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

struct RCPR_SYM(psock_handle)
{
    RCPR_SYM(resource) hdr;
    bool owns_allocator;
    RCPR_SYM(allocator)* alloc;
    RCPR_SYM(psock)* psock;
    uint8_t buffer[4096];
    size_t size;
    status last_status;
};

/**
 * \brief Release a psock_handle resource.
 *
 * \param r         The resource to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status RCPR_SYM(psock_handle_resource_release)(RCPR_SYM(resource)* r);

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
