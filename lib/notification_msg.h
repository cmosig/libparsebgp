/*
 * Copyright (c) 2013-2015 Cisco Systems, Inc. and others.  All rights reserved.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v1.0 which accompanies this distribution,
 * and is available at http://www.eclipse.org/legal/epl-v10.html
 *
 */

#ifndef NOTIFICATIONMSG_H_
#define NOTIFICATIONMSG_H_

#include "parsebgp_bgp_common.h"
#include <inttypes.h>
#include <unistd.h>

/**
 * defines the NOTIFICATION BGP header per RFC4271
 *  \see http://www.iana.org/assignments/bgp-parameters/bgp-parameters.xhtml
 */
enum notify_error_codes {
  NOTIFY_MSG_HDR_ERR = 1,
  NOTIFY_OPEN_MSG_ERR,
  NOTIFY_UPDATE_MSG_ERR,
  NOTIFY_HOLD_TIMER_EXPIRED,
  NOTIFY_FSM_ERR,
  NOTIFY_CEASE
};

/**
 * Defines header error codes
 *  \see http://www.iana.org/assignments/bgp-parameters/bgp-parameters.xhtml
 */
enum msg_hdr_subcodes {
  MSG_HDR_CONN_NOT_SYNC = 1,
  MSG_HDR_BAD_MSG_LEN,
  MSG_HDR_BAD_MSG_TYPE
};

/**
 * Defines open error codes
 *  \see http://www.iana.org/assignments/bgp-parameters/bgp-parameters.xhtml
 */
enum open_subcodes {
  OPEN_UNSUPPORTED_VER = 1,
  OPEN_BAD_PEER_AS,
  OPEN_BAD_BGP_ID,
  OPEN_UNSUPPORTED_OPT_PARAM,
  OPEN_code5_deprecated,
  OPEN_UNACCEPTABLE_HOLD_TIME
};
/**
 * Defines open error codes
 *  \see http://www.iana.org/assignments/bgp-parameters/bgp-parameters.xhtml
 */
enum update_subcodes {
  UPDATE_MALFORMED_ATTR_LIST = 1,
  UPDATE_UNRECOGNIZED_WELL_KNOWN_ATTR,
  UPDATE_MISSING_WELL_KNOWN_ATTR,
  UPDATE_ATTR_FLAGS_ERROR,
  UPDATE_ATTR_LEN_ERROR,
  UPDATE_INVALID_NEXT_HOP_ATTR,
  UPDATE_OPT_ATTR_ERROR,
  UPDATE_INVALID_NET_FIELD,
  UPDATE_MALFORMED_AS_PATH
};

/**
 * Per RFC4486 - cease subcodes
 */
enum cease_subcodes {
  CEASE_MAX_PREFIXES = 1,
  CEASE_ADMIN_SHUT,
  CEASE_PEER_DECONFIG,
  CEASE_ADMIN_RESET,
  CEASE_CONN_REJECT,
  CEASE_OTHER_CONFIG_CHG,
  CEASE_CONN_COLLISION,
  CEASE_OUT_OF_RESOURCES
};

/**
 * Decoded/parsed BGP notification message
 */
typedef struct libparsebgp_notify_msg {
  uint8_t error_code; ///< Indicates the type of error
                      ///<   NOTIFY_ERROR_CODES enum for errors
  uint8_t
    error_subcode; ///< specific info about the nature of the reported error
                   ///<   values depend on the error code
  char error_text[255]; ///< Decoded notification message
} libparsebgp_notify_msg;

/**
 * Parses a notification message stored in a byte buffer
 *
 * @details
 *      Reads the notification message from buffer.  The parsed data will be
 *      returned via parsed_msg.
 *
 * @param [in]      data        Pointer to raw bgp payload data, starting at the
 * notification message
 * @param [in]      size        Size of the data buffer, to prevent overrun when
 * reading
 * @param [out]     parsed_msg  Reference pointer to where to store the parsed
 * notification message
 *
 * @return True if error, false if no error reading/parsing the notification
 * message
 */
ssize_t
libparsebgp_notification_parse_notify(libparsebgp_notify_msg *parsed_msg,
                                      uint8_t **data, size_t size);

#endif /* NOTIFICATIONMSG_H_ */
