#ifndef __PARSEBGP_UTILS_H
#define __PARSEBGP_UTILS_H

#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>

ssize_t extract_from_buffer(uint8_t **buffer, int *buf_len, void *output_buf,
                            ssize_t output_len);

/**
 *  Simple function to swap bytes around from network to host or
 *  host to networking.  This method will convert any size byte variable,
 *  unlike ntohs and ntohl.
 *
 * @param [in/out] var   Variable containing data to update
 * @param [in]     size  Size of var - Default is size of var
 */
void SWAP_BYTES(void *var, int size);

/** Convenience function to allocate and zero memory */
void *malloc_zero(const size_t size);

#endif /*  __PARSEBGP_UTILS_H */
