#ifndef __PARSEBGP_MRT_H
#define __PARSEBGP_MRT_H

#include "parsebgp_error.h"
#include "parsebgp_bgp.h"
#include <inttypes.h>
#include <stdlib.h>

/**
 * MRT Address Families
 */
typedef enum {

  /** IPv4 Address */
  PARSEBGP_MRT_AFI_IPV4 = 1,

  /** IPv6 Address */
  PARSEBGP_MRT_AFI_IPV6 = 2,

} parsebgp_mrt_afi_t;

/**
 * MRT ASN Types (2 or 4 byte)
 */
typedef enum {

  /** 2-byte ASN */
  PARSEBGP_MRT_ASN_2_BYTE = 0,

  /** 4-byte ASN */
  PARSEBGP_MRT_ASN_4_BYTE = 1,

} parsebgp_mrt_asn_type_t;

/**
 * Table Dump Message
 */
typedef struct parsebgp_mrt_table_dump {

  /** View Number (normally zero) */
  uint16_t view_number;

  /** Sequence Number */
  uint16_t sequence;

  /** IP Address of the prefix */
  uint8_t prefix[16];

  /** Length of the prefix mask */
  uint8_t prefix_len;

  /** Status (unused) */
  uint8_t status;

  /** Time prefix was heard (in seconds since the unix epoch) */
  uint32_t originated_time;

  /** Peer IP Address */
  uint8_t peer_ip[16];

  /** Peer ASN (2-byte) */
  uint16_t peer_asn;

  /** Path Attributes */
  parsebgp_bgp_update_path_attrs_t path_attrs;

} parsebgp_mrt_table_dump_t;

/**
 * Table Dump V2 Peer Entry
 *
 * Note that the first two fields (peer_asn_type and peer_ip_afi) are decoded
 * from the "Peer Type" field.
 */
typedef struct parsebgp_mrt_table_dump_v2_peer_entry {

  /** Peer ASN Type (2 or 4 byte) */
  parsebgp_mrt_asn_type_t asn_type;

  /** Peer IP AFI */
  parsebgp_mrt_afi_t ip_afi;

  /** Peer BGP ID */
  uint8_t bgp_id[4];

  /** Peer IP Address */
  uint8_t ip[16];

  /** Peer ASN */
  uint32_t asn;

} parsebgp_mrt_table_dump_v2_peer_entry_t;

/**
 * Table Dump V2 Peer Index Table
 */
typedef struct parsebgp_mrt_table_dump_v2_peer_index {

  /** Collector BGP ID */
  uint8_t collector_bgp_id[4];

  /** View Name Length */
  uint16_t view_name_len;

  /** View Name */
  char *view_name;

  /** Number of Peer Entries */
  uint16_t peer_count;

  /** Array of (peer_count) Peer Entries */
  parsebgp_mrt_table_dump_v2_peer_entry_t *peer_entries;

} parsebgp_mrt_table_dump_v2_peer_index_t;

/**
 * Table Dump V2 RIB Entry
 */
typedef struct  parsebgp_mrt_table_dump_v2_rib_entry {

  /** Peer Index (refers to index of peer in the peer_entries field of the most
      recently parsed peer index table) */
  uint16_t peer_index;

  /** Time prefix was heard (in seconds since the unix epoch) */
  uint32_t originated_time;

  /** Path Attributes */
  parsebgp_bgp_update_path_attrs_t path_attrs;

} parsebgp_mrt_table_dump_v2_rib_entry_t;

/**
 * Table Dump V2 AFI/SAFI-specific RIB
 */
typedef struct parsebgp_mrt_table_dump_v2_afi_safi_rib {

  /** Sequence Number */
  uint32_t sequence;

  /** Length of the prefix mask */
  uint8_t prefix_len;

  /** IP Address of the prefix */
  uint8_t prefix[16];

  /** Number of RIB entries */
  uint16_t entry_count;

  /** Array of (entry_count) RIB entries */
  parsebgp_mrt_table_dump_v2_rib_entry_t *entries;

} parsebgp_mrt_table_dump_v2_afi_safi_rib_t;

/**
 * Table Dump V2 Subtypes
 */
typedef enum parsebgp_mrt_table_dump_v2_subtype {

  /** Peer Index Table */
  PEER_INDEX_TABLE = 1,

  /** IPv4 Unicast RIB */
  RIB_IPV4_UNICAST = 2,

  /** IPv4 Multicast RIB */
  RIB_IPV4_MULTICAST = 3,

  /** IPv6 Unicast RIB */
  RIB_IPV6_UNICAST = 4,

  /** IPv6 Multicast RIB */
  RIB_IPV6_MULTICAST = 5,

  /** Generic RIB */
  RIB_GENERIC = 6,

} parsebgp_mrt_table_dump_v2_subtype_t;

/*
 * Table Dump V2 (a union of the possible subtype messages)
 */
typedef union parsebgp_mrt_table_dump_v2 {

  /** Peer Index Table */
  parsebgp_mrt_table_dump_v2_peer_index_t peer_index;

  /** AFI/SAFI-specific RIB Table */
  parsebgp_mrt_table_dump_v2_afi_safi_rib_t afi_safi_rib;

  /** Generic RIB Table */
  // TODO
  //parsebgp_mrt_table_dump_v2_generic_rib_t generic_rib;

} parsebgp_mrt_table_dump_v2_t;


typedef enum {

  /** 1    Idle */
  PARSEBGP_MRT_FSM_CODE_IDLE = 1,

  /** 2    Connect */
  PARSEBGP_MRT_FSM_CODE_CONNECT = 2,

  /** 3    Active */
  PARSEBGP_MRT_FSM_CODE_ACTIVE = 3,

  /** 4    OpenSent */
  PARSEBGP_MRT_FSM_CODE_OPENSENT = 4,

  /** 5    OpenConfirm */
  PARSEBGP_MRT_FSM_CODE_OPENCONFIRM = 5,

  /** 6    Established */
  PARSEBGP_MRT_FSM_CODE_ESTABLISHED = 6,

} parsebgp_mrt_fsm_code_t;


/**
 * BGP4MP State Change information
 */
typedef struct parsebgp_mrt_bgp4mp_state_change {

  /** FSM code of the old state (parsebgp_mrt_fsm_code_t) */
  uint16_t old_state;

  /** FSM code of the new state (parsebgp_mrt_fsm_code_t) */
  uint16_t new_state;

} parsebgp_mrt_bgp4mp_state_change_t;

/**
 * BGP4MP Message Subtypes
 */
typedef enum {

  /** 0    BGP4MP_STATE_CHANGE */
  PARSEBGP_MRT_BGP4MP_STATE_CHANGE = 0,

  /** 1    BGP4MP_MESSAGE */
  PARSEBGP_MRT_BGP4MP_MESSAGE = 1,

  /** 4    BGP4MP_MESSAGE_AS4 */
  PARSEBGP_MRT_BGP4MP_MESSAGE_AS4 = 4,

  /** 5    BGP4MP_STATE_CHANGE_AS4 */
  PARSEBGP_MRT_BGP4MP_STATE_CHANGE_AS4 = 5,

  /** 6    BGP4MP_MESSAGE_LOCAL */
  PARSEBGP_MRT_BGP4MP_MESSAGE_LOCAL = 6,

  /** 7    BGP4MP_MESSAGE_AS4_LOCAL */
  PARSEBGP_MRT_BGP4MP_MESSAGE_AS4_LOCAL = 7,

} parsebgp_mrt_bgp4mp_subtype_t;

/**
 * BGP4MP Message
 */
typedef struct parsebgp_mrt_bgp4mp {

  /** Peer ASN */
  uint32_t peer_asn;

  /** Local ASN */
  uint32_t local_asn;

  /** Interface Index */
  uint16_t interface_index;

  /** Address Family (parsebgp_mrt_afi_t) */
  uint16_t afi;

  /** Peer IP Address */
  uint8_t peer_ip[16];

  /** Local IP Address */
  uint8_t local_ip[16];

  union {

    /** State Change info (PARSEBGP_MRT_BGP4MP_STATE_CHANGE and
        PARSEBGP_MRT_BGP4MP_STATE_CHANGE_AS4) */
    parsebgp_mrt_bgp4mp_state_change_t state_change;

    /** BGP Message (used by PARSEBGP_MRT_BGP4MP_MESSAGE_* types) */
    parsebgp_bgp_msg_t bgp_msg;

  } data;

} parsebgp_mrt_bgp4mp_t;

typedef enum {

  /** 11   OSPFv2 */
  PARSEBGP_MRT_TYPE_OSPF_V2 = 11,

  /** 12   TABLE_DUMP */
  PARSEBGP_MRT_TYPE_TABLE_DUMP = 12,

  /** 13   TABLE_DUMP_V2 */
  PARSEBGP_MRT_TYPE_TABLE_DUMP_V2 = 13,

  /** 16   BGP4MP */
  PARSEBGP_MRT_TYPE_BGP4MP = 16,

  /** 17   BGP4MP_ET */
  PARSEBGP_MRT_TYPE_BGP4MP_ET = 17,

  /** 32   ISIS */
  PARSEBGP_MRT_TYPE_ISIS = 32,

  /** 33   ISIS_ET */
  PARSEBGP_MRT_TYPE_ISIS_ET = 33,

  /** 48   OSPFv3 */
  PARSEBGP_MRT_TYPE_OSPF_V3 = 48,

  /** 49   OSPFv3_ET */
  PARSEBGP_MRT_TYPE_OSPF_V3_ET = 49

} parsebgp_mrt_msg_type_t;

/*
 * MRT Message Structure
 */
typedef struct parsebgp_mrt_msg {

  /** Timestamp of the message in seconds since the unix epoch */
  uint32_t timestamp_sec;

  /** Message type (parsebgp_mrt_msg_type_t) */
  uint16_t type;

  /** Message sub-type */
  uint16_t subtype;

  /** Message length EXCLUDING common header */
  uint32_t len;

  /** Microseconds portion of the message timestamp. This field is unused unless
      parsing one of the *_ET message types. */
  uint32_t timestamp_usec;

  union {

    /** Type 12: TABLE_DUMP */
    parsebgp_mrt_table_dump_t table_dump;

    /** Type 13: TABLE_DUMP_V2 */
    parsebgp_mrt_table_dump_v2_t table_dump_v2;

    /** Types 16 and 17: BGP4MP */
    parsebgp_mrt_bgp4mp_t bgp4mp;

  } types;

} parsebgp_mrt_msg_t;

/**
 * Decode (parse) a single MRT message from the given buffer into the given MRT
 * message structure.
 *
 * @param [in] msg      Pointer to the MRT Message structure to fill
 * @param [in] buf      Pointer to the start of a raw MRT message
 * @param [in,out] len  Length of the data buffer (used to prevent overrun).
 *                      Updated to the number of bytes read from the buffer.
 * @return OK (0) if a message was parsed successfully, or an error code
 * otherwise
 */
parsebgp_error_t parsebgp_mrt_decode(parsebgp_mrt_msg_t *msg,
                                     uint8_t *buf, size_t *len);

/** Destroy the given MRT message structure
 *
 * @param msg           Pointer to message structure to destroy
 *
 * This function *does not* free the passed structure itself as it is assumed to
 * be a member of a parsebgp_msg_t structure.
 */
void parsebgp_mrt_destroy_msg(parsebgp_mrt_msg_t *msg);

#endif /* __PARSEBGP_MRT_H */