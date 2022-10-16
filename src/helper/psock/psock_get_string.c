/**
 * \file helper/psock/psock_get_string.c
 *
 * \brief Convert an opaque string pointer to a real string pointer.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include "psock_internal.h"

/**
 * \brief Unwrap a void string into a real string.
 *
 * This method is used by Idris to handle results that could be NULL.
 *
 * \note This assumes that the string is valid. It is up to the caller to verify
 * that the string is valid (not NULL) before calling this function. Here be
 * dragons.
 *
 * \param opaque_string         The string to unwrap.
 *
 * \returns the \p opaque_string cast as a char*.
 */
char* RCPR_SYM(psock_get_string)(void* opaque_string)
{
    return (char*)opaque_string;
}
