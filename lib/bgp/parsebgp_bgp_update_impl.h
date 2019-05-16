/*
 * Copyright (C) 2017 The Regents of the University of California.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __PARSEBGP_BGP_UPDATE_IMPL_H
#define __PARSEBGP_BGP_UPDATE_IMPL_H

#include "parsebgp_bgp_update.h"
#include "parsebgp_error.h"
#include "parsebgp_opts.h"
#include <stddef.h>

/** Decode an UPDATE message */
parsebgp_error_t parsebgp_bgp_update_decode(parsebgp_opts_t *opts,
                                            parsebgp_bgp_update_t *msg,
                                            const uint8_t *buf, size_t *lenp,
                                            size_t remain);

/** Destroy an UPDATE message */
void parsebgp_bgp_update_destroy(parsebgp_bgp_update_t *msg);

/** Clear an UPDATE message */
void parsebgp_bgp_update_clear(parsebgp_bgp_update_t *msg);

/**
 * Dump a human-readable version of the message to stdout
 *
 * @param msg           Pointer to the parsed UPDATE message to dump
 * @param depth         Depth of the message within the overall message
 *
 * The output from these functions is designed to help with debugging the
 * library and also includes internal implementation information like the names
 * and sizes of structures. It may be useful to potential users of the library
 * to get a sense of their data.
 */
void parsebgp_bgp_update_dump(const parsebgp_bgp_update_t *msg, int depth);

/** Decode PATH ATTRIBUTES */
parsebgp_error_t parsebgp_bgp_update_path_attrs_decode(
  parsebgp_opts_t *opts, parsebgp_bgp_update_path_attrs_t *msg, const uint8_t *buf,
  size_t *lenp, size_t remain);

/** Destroy a Path Attributes message */
void parsebgp_bgp_update_path_attrs_destroy(
  parsebgp_bgp_update_path_attrs_t *msg);

/** Clear a Path Attributes message */
void parsebgp_bgp_update_path_attrs_clear(
  parsebgp_bgp_update_path_attrs_t *msg);

/**
 * Dump a human-readable version of the message to stdout
 *
 * @param msg           Pointer to the parsed Path Attrs message to dump
 * @param depth         Depth of the message within the overall message
 *
 * The output from these functions is designed to help with debugging the
 * library and also includes internal implementation information like the names
 * and sizes of structures. It may be useful to potential users of the library
 * to get a sense of their data.
 */
void parsebgp_bgp_update_path_attrs_dump(
    const parsebgp_bgp_update_path_attrs_t *msg, int depth);

#endif /* __PARSEBGP_BGP_UPDATE_IMPL_H */
