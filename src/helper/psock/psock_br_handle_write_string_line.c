/**
 * \file helper/psock/psock_br_handle_write_string_line.c
 *
 * \brief Write a string line to the given buffered reader wrapper.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <string.h>

#include "psock_internal.h"

RCPR_IMPORT_psock;

/**
 * \brief Write a string line to the given buffered reader wrapper.
 *
 * \param handle                The \ref psock_br_handle instance to which this
 *                              string is written.
 * \param str                   The ASCII-Z terminated string to write.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status RCPR_SYM(psock_br_handle_write_string_line)(
    RCPR_SYM(psock_br_handle)* handle, const char* str)
{
    status retval;

    /* parameter sanity checks. */
    RCPR_MODEL_ASSERT(prop_psock_br_handle_valid(handle));
    RCPR_MODEL_ASSERT(NULL != str);

    /* get the psock adapter for this buffered reader. */
    psock* sock = psock_br_psock_adapter(handle->br);

    /* write the string to the psock. */
    retval = psock_write_raw_data(sock, str, strlen(str));
    if (STATUS_SUCCESS != retval)
    {
        return retval;
    }

    /* write a newline to the socket. */
    return
        psock_write_raw_int8(sock, '\n');
}
