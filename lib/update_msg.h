/*
 * Copyright (c) 2013-2015 Cisco Systems, Inc. and others.  All rights reserved.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v1.0 which accompanies this distribution,
 * and is available at http://www.eclipse.org/legal/epl-v10.html
 *
 */

#ifndef UPDATEMSG_H_
#define UPDATEMSG_H_

#include "add_path_data_container.h"
#include "bgp_common.h"

enum update_attr_types {
  ATTR_TYPE_ORIGIN = 1,
  ATTR_TYPE_AS_PATH,
  ATTR_TYPE_NEXT_HOP,
  ATTR_TYPE_MED,
  ATTR_TYPE_LOCAL_PREF,
  ATTR_TYPE_ATOMIC_AGGREGATE,
  ATTR_TYPE_AGGEGATOR,
  ATTR_TYPE_COMMUNITIES,
  ATTR_TYPE_ORIGINATOR_ID,
  ATTR_TYPE_CLUSTER_LIST,
  ATTR_TYPE_DPA,
  ATTR_TYPE_ADVERTISER,
  ATTR_TYPE_RCID_PATH,
  ATTR_TYPE_MP_REACH_NLRI = 14,
  ATTR_TYPE_MP_UNREACH_NLRI,
  ATTR_TYPE_EXT_COMMUNITY = 16,
  ATTR_TYPE_AS4_PATH = 17,
  ATTR_TYPE_AS4_AGGREGATOR = 18,

  ATTR_TYPE_AS_PATHLIMIT =
    21, // Deprecated - draft-ietf-idr-as-pathlimit, JunOS will send this

  ATTR_TYPE_IPV6_EXT_COMMUNITY = 25,
  ATTR_TYPE_AIGP, ///< RFC7311 - Accumulated IGP metric

  ATTR_TYPE_BGP_LS = 29, // BGP LS attribute draft-ietf-idr-ls-distribution

  ATTR_TYPE_BGP_LINK_STATE_OLD = 99, // BGP link state Older
  ATTR_TYPE_BGP_ATTRIBUTE_SET = 128,

  /*
   * Below attribute types are for internal use only... These are derived/added
   * based on other attributes
   */
  ATTR_TYPE_INTERNAL_AS_COUNT = 9000, // AS path count - number of AS's
  ATTR_TYPE_INTERNAL_AS_ORIGIN        // The AS that originated the entry
};

/*
 * Structure for attribute type
 */
typedef struct attr_type_tuple {
  uint8_t attr_flags;     ///< Attribute flags - 1 octet
  uint8_t attr_type_code; ///< Attribute type code - 1 octet
} attr_type_tuple;

typedef struct as_path_segment {
  uint8_t seg_type;
  uint8_t seg_len;
  uint8_t count_seg_asn;
  uint32_t *seg_asn;
} as_path_segment;

/**
 * struct defines the MP_UNREACH_NLRI (RFC4760 Section 4)
 */
typedef struct mp_unreach_nlri {
  uint16_t afi;                       ///< Address Family Identifier
  uint8_t safi;                       ///< Subsequent Address Family Identifier
  uint16_t count_wdrawn_routes;       ///< Number of withdrawn routes
  uint16_t count_wdrawn_routes_label; ///< Number of withdrawn routes with label
  uint16_t count_evpn_withdrawn;      ///< Number of EVPN NLRIs withdrawn
  union withdrawn_routes_nlri {       ///< Union for withdrawn routes nlri
    update_prefix_tuple *wdrawn_routes; ///< Withdrawn routes
    update_prefix_label_tuple
      *wdrawn_routes_label;     ///< Withdrawn routes with label
    evpn_tuple *evpn_withdrawn; ///< List of evpn nlris withdrawn
  } withdrawn_routes_nlri;
} mp_unreach_nlri;

/**
 * Node (local and remote) common fields
 */
typedef struct node_descriptor {
  uint16_t type;
  uint16_t len;
  uint32_t asn;             ///< BGP ASN
  uint32_t bgp_ls_id;       ///< BGP-LS Identifier
  uint8_t igp_router_id[8]; ///< IGP router ID
  uint8_t ospf_area_Id[4];  ///< OSPF area ID
  uint32_t
    bgp_router_id; ///< BGP router ID (draft-ietf-idr-bgpls-segment-routing-epe)
  uint8_t hash_bin[16]; ///< binary hash for node descriptor
} node_descriptor;

/**
 * Link Descriptor common fields
 */
typedef struct link_descriptor {
  uint16_t type;
  uint16_t len;
  uint32_t local_id;     ///< Link Local ID
  uint32_t remote_id;    ///< Link Remote ID
  uint8_t intf_addr[16]; ///< Interface binary address
  uint8_t nei_addr[16];  ///< Neighbor binary address
  uint32_t mt_id;        ///< Multi-Topology ID
  bool is_ipv4;          ///< True if IPv4, false if IPv6
} link_descriptor;

/**
 * Prefix descriptor common fields
 */
typedef struct prefix_descriptor {
  uint16_t type;
  uint16_t len;
  char ospf_route_type[32]; ///< OSPF Route type in string form for DB enum
  uint32_t mt_id;           ///< Multi-Topology ID
  uint8_t prefix[16];       ///< Prefix binary address
  uint8_t prefix_bcast[16]; ///< Prefix broadcast/ending binary address
  uint8_t prefix_len;       ///< Length of prefix in bits
} prefix_descriptor;

typedef struct mp_reach_ls {
  uint16_t nlri_type;
  uint16_t nlri_len;
  uint8_t proto_id;
  uint64_t id;
  union nlri_ls {
    struct node_nlri {
      uint16_t type;
      uint16_t len;
      uint16_t count_local_nodes;
      node_descriptor *local_nodes;
    } node_nlri;

    struct link_nlri {
      uint16_t type;
      uint16_t len;
      uint16_t count_local_nodes;
      node_descriptor *local_nodes;
      uint16_t count_remote_nodes;
      node_descriptor *remote_nodes;
      uint16_t count_link_desc;
      link_descriptor *link_desc;
    } link_nlri;

    struct prefix_nlri_ipv4_ipv6 {
      uint16_t type;
      uint16_t len;
      uint16_t count_local_nodes;
      node_descriptor *local_nodes;
      uint16_t count_prefix_desc;
      prefix_descriptor *prefix_desc;
    } prefix_nlri_ipv4_ipv6;
  } nlri_ls;
} mp_reach_ls;

typedef struct link_peer_epe_node_sid {
  bool L_flag;
  bool V_flag;
  uint32_t sid_3;
  uint32_t sid_4;
  u_char ip_raw[16];
} link_peer_epe_node_sid;

typedef struct mp_reach_nlri {
  uint16_t afi;               ///< Address Family Identifier
  uint8_t safi;               ///< Subsequent Address Family Identifier
  uint8_t nh_len;             ///< Length of next hop
  unsigned char next_hop[16]; ///< Next hop address - Pointer to data (normally
                              ///< does not require freeing)
  uint8_t reserved;           ///< Reserved
  uint16_t count_nlri_info;   ///< Number of nlri_info
  uint16_t count_nlri_label_info; ///< Number of nlri with label
  uint16_t count_mp_rch_ls;       ///< number of mp_reach_ls
  uint16_t count_evpn;            ///< number of evpn
  union mp_reach_nlri_info {
    update_prefix_tuple *nlri_info; ///< Withdrawn routes
    update_prefix_label_tuple *nlri_label_info;
    mp_reach_ls *mp_rch_ls;
    evpn_tuple *evpn; ///< List of evpn nlris advertised
  } mp_reach_nlri_info;
} mp_reach_nlri;

/**
 * Extended Community header
 *      RFC4360 size is 8 bytes total (6 for value)
 *      RFC5701 size is 20 bytes total (16 for global admin, 2 for local admin)
 */
typedef struct extcomm_hdr {
  uint8_t high_type; ///< Type high byte
  uint8_t low_type;  ///< Type low byte - subtype
  char val[20];       ///< Value
} extcomm_hdr;

typedef struct bgp_link_state_attrs {
  uint16_t type;
  uint16_t len;
  union node_attr {
    u_char node_flag_bits;
    u_char node_ipv4_router_id_local[4];
    u_char node_ipv6_router_id_local[16];
    u_char node_isis_area_id[8];
    u_char node_name[256];
    u_char mt_id[256];
  } node;
  union link_attr {
    u_char link_admin_group[4];
    uint32_t link_igp_metric;
    u_char link_ipv4_router_id_remote[4];
    u_char link_ipv6_router_id_remote[4];
    int32_t link_max_link_bw;
    int32_t link_max_resv_bw;
    u_char link_name[256];
    uint32_t link_te_def_metric;
    int32_t link_unresv_bw[8];
    link_peer_epe_node_sid link_peer_epe_sid;
  } link;
  union prefix_attr {
    uint32_t prefix_prefix_metric;
    uint32_t prefix_route_tag;
  } prefix;
} bgp_link_state_attrs;

/*
 * Union holding the different attributes types
 */
typedef union attr_value {
  uint8_t origin;           ///< Origin of path information
  as_path_segment *as_path; ///< sequence of AS path segments
  u_char next_hop[4]; ///< IP address of the router to be used as next hop to
                      ///< the destinations listed in the NLRI field of the
                      ///< UPDATE message
  u_char originator_id[4];  ///< Originator ID
  uint32_t med;             ///< contains the MED value
  uint32_t local_pref;      ///< contains the local pref value
  u_char aggregator[4];     ///< The attribute contains the last AS number that
                            ///< formed the aggregate route
  u_char **cluster_list;    ///< Cluster List (RFC 4456)
  uint16_t *attr_type_comm; ///< Community list
  extcomm_hdr *ext_comm;    ///< extended community list (RFC 4360)
  mp_unreach_nlri mp_unreach_nlri_data; ///< MP_REACH_NLRI RFC4760
  mp_reach_nlri mp_reach_nlri_data;     ///< MP_UNREACH_NLRI RFC4760
  bgp_link_state_attrs *bgp_ls;         ///< Link state attribute
} attr_val;

/*
 * Structure for path attributes in update message
 */
typedef struct update_path_attrs {
  attr_type_tuple attr_type;     ///< Type of attribute - 2 octets
  uint16_t attr_len;             ///< Length of attribute
  attr_val attr_value;           ///< Attribute value
  uint16_t count_as_path;        ///< Number of as_paths
  uint16_t count_cluster_list;   ///< Number of cluster lists
  uint16_t count_attr_type_comm; ///< Number of communities
  uint16_t count_ext_comm;       ///< Number of extended communities
  uint16_t count_bgp_ls;         ///< Number of bgp-ls
} update_path_attrs;

/*
 * Structure for parsed update message
 */
typedef struct libparsebgp_update_msg_data {
  uint16_t count_wdrawn_route; ///< Number of withdrawn routes
  uint16_t count_path_attr;    ///< Number of path attributes
  uint16_t count_nlri;         ///< Number of NLRI

  uint16_t wdrawn_route_len;           ///< Length of withdrawn routes
  update_prefix_tuple **wdrawn_routes; ///< Array containing withdrawn routes
  uint16_t total_path_attr_len;        ///< PAth attributes length
  update_path_attrs **path_attributes; ///< Array containing path attributes
  update_prefix_tuple **nlri;          ///< Array containing nlri
} libparsebgp_update_msg_data;

/**
 * Parses the update message
 *
 * @details
 *      Reads the update message from socket and parses it.  The parsed output
 * will be added to the DB.
 *
 * @param [in]   data           Pointer to raw bgp payload data, starting at the
 * notification message
 * @param [in]   size           Size of the data available to read; prevent
 * overrun when reading
 * @param [out]  parsed_data    Reference to parsed_update_data; will be updated
 * with all parsed data
 *
 * @return ZERO is error, otherwise a positive value indicating the number of
 * bytes read from update message
 */
ssize_t
libparsebgp_update_msg_parse_update_msg(libparsebgp_update_msg_data *update_msg,
                                        u_char *data, ssize_t size,
                                        bool *has_end_of_rib_marker);

/**
 * Parses the BGP attributes in the update
 *
 * @details
 *     Parses all attributes.  Decoded values are updated in 'parsed_data'
 *
 * @param [in]   data       Pointer to the start of the prefixes to be parsed
 * @param [in]   len        Length of the data in bytes to be read
 * @param [out]  parsed_data    Reference to parsed_update_data; will be updated
 * with all parsed data
 */
ssize_t libparsebgp_update_msg_parse_attributes(update_path_attrs ***update_msg,
                                                u_char *data, uint16_t len,
                                                bool *has_end_of_rib_marker,
                                                uint16_t *count);

/**
 * Destructor for the update message struct
 * @param update_msg struct containing the parsed update message
 *
 */
void libparsebgp_parse_update_msg_destructor(
  libparsebgp_update_msg_data *update_msg);

/**
 * Destructor for the structure containing path attributes
 * @param path_attrs struct containing path attributes
 */
void libparsebgp_parse_update_path_attrs_destructor(
  update_path_attrs *path_attrs);

#endif /* UPDATEMSG_H_ */