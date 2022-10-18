/**
 * \file rcprhelper/psock.h
 *
 * \brief Idris2 rcprhelper psock library.
 *
 * This library provides a simple helper handle to be used by Idris, as well as
 * a method to create and a method to release this handle.
 *
 * \copyright 2022 Justin Handville.  Please see license.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <rcpr/function_decl.h>
#include <rcpr/psock.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/**
 * \brief The psock helper handle wraps a psock interface and an allocator
 * interface.
 */
typedef struct RCPR_SYM(psock_handle) RCPR_SYM(psock_handle);

/**
 * \brief The buffered psock helper handle wraps a buffered psock interface and
 * an allocator interface. It is created from a psock handle.
 */
typedef struct RCPR_SYM(psock_br_handle) RCPR_SYM(psock_br_handle);

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
 * \brief Return the latest status code from a \ref psock_handle operation.
 *
 * \param handle                The \ref psock_handle to interrogate.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status RCPR_SYM(psock_handle_get_status)(RCPR_SYM(psock_handle)* handle);

/**
 * \brief Return the latest status code from a \ref psock_br_handle operation.
 *
 * \param handle                The \ref psock_br_handle to interrogate.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status RCPR_SYM(psock_br_handle_get_status)(RCPR_SYM(psock_br_handle)* handle);

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
char* RCPR_SYM(psock_br_handle_read_line)(RCPR_SYM(psock_br_handle)* handle);

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
    RCPR_SYM(psock_br_handle)* handle, const char* str);

/**
 * \brief Return true if the given handle is not NULL.
 *
 * \param handle                The \ref psock_handle to check.
 *
 * \returns true if the handle is not NULL and false otherwise.
 */
bool RCPR_SYM(psock_handle_is_valid)(RCPR_SYM(psock_handle)* handle);

/**
 * \brief Return true if the given handle is not NULL.
 *
 * \param handle                The \ref psock_br_handle to check.
 *
 * \returns true if the handle is not NULL and false otherwise.
 */
bool RCPR_SYM(psock_br_handle_is_valid)(RCPR_SYM(psock_br_handle)* handle);

/**
 * \brief Return true if the given status is a partial line read status.
 *
 * \param s                     The status to check.
 *
 * \returns true if the status is a partial line read status and false
 * otherwise.
 */
bool RCPR_SYM(psock_br_status_is_partial_line_read)(status s);

/**
 * \brief Attempt to release a \ref psock_handle instance.
 *
 * \param handle                The \ref psock_handle instance to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status RCPR_SYM(psock_handle_release)(RCPR_SYM(psock_handle)* handle);

/**
 * \brief Attempt to release a \ref psock_br_handle instance.
 *
 * \param handle                The \ref psock_br_handle instance to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status RCPR_SYM(psock_br_handle_release)(RCPR_SYM(psock_br_handle)* handle);

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
char* RCPR_SYM(psock_get_string)(void* opaque_string);

/**
 * \brief Create a psock handle from a listen address, use it to call the given
 * callback, then release it.
 *
 * \param listen_address        The listen address from which this handle is
 *                              created.
 * \param port                  The port from which this handle is created.
 * \param callback_fn           The callback function to call with the created
 *                              handle on success.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status RCPR_SYM(with_psock_handle_created_from_listen_address)(
    const char* listen_address, uint16_t port,
    void (*callback_fn)(RCPR_SYM(psock_handle)*));

/**
 * \brief Helper to cast an opaque \ref psock_handle instance to a
 * \ref psock_handle instance.
 *
 * \param opaque_handle         A type erased handle to cast.
 *
 * \returns The non-opaque reference.
 */
RCPR_SYM(psock_handle)* RCPR_SYM(psock_handle_get)(void* opaque_handle);

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
