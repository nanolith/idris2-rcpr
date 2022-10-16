/**
 * \file helper/psock/psock_br_handle_read_line.c
 *
 * \brief Read a (potentially partial) line from a \ref psock_br_handle
 * instance.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <stdlib.h>
#include <string.h>

#include "psock_internal.h"

RCPR_IMPORT_psock;

/**
 * \brief Read a (potentially partial) line from a \ref psock_br_handle
 * instance.
 *
 * \note This function can return NULL on error. The status of this operation
 * should be checked by calling \ref psock_br_handle_get_status before
 * attempting to dereference this value. This can return STATUS_SUCCESS when a
 * complete line or the end of a partial line is read, or
 * ERROR_PSOCK_BR_READ_LINE_TRUNCATED if only a partial line is read. It is up
 * to the caller to assemble partial lines.
 *
 * \param handle                The \ref psock_br_handle instance from which to
 *                              read.
 *
 * \returns a (possibly partial) line or NULL.
 */
char* RCPR_SYM(psock_br_handle_read_line)(RCPR_SYM(psock_br_handle)* handle)
{
    char* retval = NULL;
    char buffer[4096];
    size_t buffer_size = sizeof(buffer);

    /* parameter sanity checks. */
    RCPR_MODEL_ASSERT(prop_psock_br_handle_valid(handle));

    /* attempt to read a (partial maybe) line from the buffered reader. */
    handle->last_status = psock_br_read_line(buffer, &buffer_size, handle->br);

    /* decode the status. */
    switch (handle->last_status)
    {
        case STATUS_SUCCESS:
        case ERROR_PSOCK_BR_READ_LINE_TRUNCATED:
            retval = malloc(buffer_size);
            if (NULL == retval)
            {
                handle->last_status = ERROR_GENERAL_OUT_OF_MEMORY;
                goto done;
            }
            memcpy(retval, buffer, buffer_size);
            goto done;

        default:
            goto done;
    }

done:
    memset(buffer, 0, sizeof(buffer));

    return retval;
}
