/**
 * \file helper/psock/with_read_raw_buffer.c
 *
 * \brief Read a raw block of data of the given size, call the given callback
 * with this buffer, then clean up.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <string.h>

#include "psock_internal.h"

RCPR_IMPORT_allocator;
RCPR_IMPORT_psock;

typedef RCPR_SYM(psock_c_array) psock_c_array;

/**
 * \brief Read a raw block of data of a given size, and call a callback with
 * this block exposed as a \ref psock_c_array. This callback can call other C
 * FFI functions with this \ref psock_c_array, and these functions can access
 * the inner details.
 *
 * \param handle                The \ref psock_br_handle to use to perform this
 *                              read.
 * \param size                  The number of bytes to read.
 * \param callback_fn           The callback function to call with the read raw
 *                              array on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status RCPR_SYM(with_read_raw_buffer)(
    RCPR_SYM(psock_br_handle)* handle, size_t size,
    void (*callback_fn)(RCPR_SYM(psock_c_array)*))
{
    status retval, reclaim_retval;
    psock_c_array* array;

    /* parameter sanity checks. */
    RCPR_MODEL_ASSERT(prop_psock_br_handle_valid(handle));
    RCPR_MODEL_ASSERT(NULL != callback_fn);

    /* allocate memory for the array. */
    retval = allocator_allocate(handle->alloc, (void**)&array, sizeof(*array));
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* clear structure. */
    memset(array, 0, sizeof(*array));

    /* set the array size. */
    array->size = size;

    /* allocate memory for the array body. */
    retval = allocator_allocate(handle->alloc, &array->data, size);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_array;
    }

    /* get the psock adapter for this buffered reader. */
    psock* sock = psock_br_psock_adapter(handle->br);

    /* read the data from the psock. */
    retval = psock_read_raw(sock, array->data, &array->size);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_array_data;
    }

    /* verify that we read the entire buffer. */
    if (size != array->size)
    {
        retval = ERROR_PSOCK_READ_INVALID_SIZE;
        goto cleanup_array_data;
    }

    /* call the callback. */
    callback_fn(array);

    /* clean up. */
    retval = STATUS_SUCCESS;
    goto cleanup_array_data;

cleanup_array_data:
    memset(array->data, 0, size);

    reclaim_retval = allocator_reclaim(handle->alloc, array->data);
    if (STATUS_SUCCESS != reclaim_retval)
    {
        retval = reclaim_retval;
    }

cleanup_array:
    memset(array, 0, sizeof(*array));

    reclaim_retval = allocator_reclaim(handle->alloc, array);
    if (STATUS_SUCCESS != reclaim_retval)
    {
        retval = reclaim_retval;
    }

done:
    return retval;
}
