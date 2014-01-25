/*
 * openflow.h
 *
 *  Created on: 02.03.2013
 *      Author: andi
 */

#ifndef OPENFLOW_COMMON_H_
#define OPENFLOW_COMMON_H_ 1

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif

#ifdef SWIG
#define OFP_ASSERT(EXPR)        /* SWIG can't handle OFP10_ASSERT. */
#elif !defined(__cplusplus)
/* Build-time assertion for use in a declaration context. */
#define OFP_ASSERT(EXPR)                                                \
        extern int (*build_assert(void))[ sizeof(struct {               \
                    unsigned int build_assert_failed : (EXPR) ? 1 : -1; })]
#else /* __cplusplus */
#define OFP_ASSERT(_EXPR) typedef int build_assert_failed[(_EXPR) ? 1 : -1]
#endif /* __cplusplus */

#ifndef SWIG
#define OFP_PACKED __attribute__((packed))
#else
#define OFP_PACKED              /* SWIG doesn't understand __attribute. */
#endif


namespace rofl {

#define OFP_MAX_TABLE_NAME_LEN 32
#define OFP_MAX_PORT_NAME_LEN  16

#define OFP_TCP_PORT  6633
#define OFP_SSL_PORT  6633

//#define OFP_VERSION_UNKNOWN 0

#define OFP_ETH_ALEN 6          /* Bytes in an Ethernet address. */

namespace openflow {

	enum ofp_version_t {
		OFP_VERSION_UNKNOWN = 0,
	};

	enum ofp_buffer_t {
		OFP_NO_BUFFER = 0xffffffff,
	};

	/* Header on all OpenFlow packets. */
	struct ofp_header {
		uint8_t version;    /* OFP10_VERSION. */
		uint8_t type;       /* One of the OFP10T_ constants. */
		uint16_t length;    /* Length including this ofp10_header. */
		uint32_t xid;       /* Transaction id associated with this packet.
							   Replies use the same id as was in the request
							   to facilitate pairing. */
	};
	OFP_ASSERT(sizeof(struct ofp_header) == 8);

	enum ofp_type {
		/* Immutable messages. */
		OFPT_HELLO 					= 0,    /* Symmetric message */
		OFPT_ERROR 					= 1,	/* Symmetric message */
		OFPT_ECHO_REQUEST 			= 2,	/* Symmetric message */
		OFPT_ECHO_REPLY				= 3,    /* Symmetric message */
		OFPT_EXPERIMENTER			= 4,    /* Symmetric message */

		/* Switch configuration messages. */
		OFPT_FEATURES_REQUEST		= 5,    /* Controller/switch message */
		OFPT_FEATURES_REPLY			= 6,    /* Controller/switch message */
		OFPT_GET_CONFIG_REQUEST		= 7,    /* Controller/switch message */
		OFPT_GET_CONFIG_REPLY		= 8,    /* Controller/switch message */
		OFPT_SET_CONFIG				= 9,    /* Controller/switch message */

		/* Asynchronous messages. */
		OFPT_PACKET_IN				= 10,   /* Async message */
		OFPT_FLOW_REMOVED			= 11,   /* Async message */
		OFPT_PORT_STATUS			= 12,   /* Async message */

		/* Controller command messages. */
		OFPT_PACKET_OUT				= 13,   /* Controller/switch message */
		OFPT_FLOW_MOD				= 14,   /* Controller/switch message */
		OFPT_GROUP_MOD				= 15,   /* Controller/switch message */
		OFPT_PORT_MOD				= 16,   /* Controller/switch message */
		OFPT_TABLE_MOD				= 17,   /* Controller/switch message */

		/* Multipart messages. */
		OFPT_MULTIPART_REQUEST		= 18,   /* Controller/switch message */
		OFPT_MULTIPART_REPLY		= 19,   /* Controller/switch message */
		OFPT_STATS_REQUEST			= 18,   /* Controller/switch message */
		OFPT_STATS__REPLY			= 19,   /* Controller/switch message */

		/* Barrier messages. */
		OFPT_BARRIER_REQUEST		= 20,   /* Controller/switch message */
		OFPT_BARRIER_REPLY			= 21,   /* Controller/switch message */

		/* Queue Configuration messages. */
		OFPT_QUEUE_GET_CONFIG_REQUEST	= 22,  /* Controller/switch message */
		OFPT_QUEUE_GET_CONFIG_REPLY		= 23,  /* Controller/switch message */

		/* Controller role change request messages. */
		OFPT_ROLE_REQUEST    		= 24, /* Controller/switch message */
		OFPT_ROLE_REPLY				= 25, /* Controller/switch message */

		/* Asynchronous message configuration. */
		OFPT_GET_ASYNC_REQUEST		= 26, /* Controller/switch message */
		OFPT_GET_ASYNC_REPLY		= 27, /* Controller/switch message */
		OFPT_SET_ASYNC				= 28, /* Controller/switch message */

		/* Meters and rate limiters configuration messages. */
		OFPT_METER_MOD				= 29, /* Controller/switch message */
	};

	/* OFPT_ERROR: Error message (datapath -> controller). */
	struct ofp_error_msg {
		struct ofp_header header;

		uint16_t type;
		uint16_t code;
		uint8_t data[0];          /* Variable-length data.  Interpreted based
									 on the type and code.  No padding. */
	};
	OFP_ASSERT(sizeof(struct ofp_error_msg) == 12);


	enum ofp_flow_mod_command {
		OFPFC_ADD,              /* New flow. */
		OFPFC_MODIFY,           /* Modify all matching flows. */
		OFPFC_MODIFY_STRICT,    /* Modify entry strictly matching wildcards and
								   priority. */
		OFPFC_DELETE,           /* Delete all matching flows. */
		OFPFC_DELETE_STRICT     /* Delete entry strictly matching wildcards and
								   priority. */
	};


	/* What changed about the physical port */
	enum ofp_port_reason {
		OFPPR_ADD,              /* The port was added. */
		OFPPR_DELETE,           /* The port was removed. */
		OFPPR_MODIFY            /* Some attribute of the port has changed. */
	};


	enum ofp_instruction_type {
		OFPIT_GOTO_TABLE = 1,       /* Setup the next table in the lookup
									   pipeline */
		OFPIT_WRITE_METADATA = 2,   /* Setup the metadata field for use later in
									   pipeline */
		OFPIT_WRITE_ACTIONS = 3,    /* Write the action(s) onto the datapath action
									   set */
		OFPIT_APPLY_ACTIONS = 4,    /* Applies the action(s) immediately */
		OFPIT_CLEAR_ACTIONS = 5,    /* Clears all actions from the datapath
									   action set */
		OFPIT_METER = 6,				/* Apply meter (rate limiter) */
		OFPIT_EXPERIMENTER = 0xFFFF  /* Experimenter instruction */
	};


	/* Generic ofp_instruction structure */
	struct ofp_instruction {
		uint16_t type;                /* Instruction type */
		uint16_t len;                 /* Length of this struct in bytes. */
		uint8_t pad[4];               /* Align to 64-bits */
	};
	OFP_ASSERT(sizeof(struct ofp_instruction) == 8);




	/* Action header that is common to all actions.  The length includes the
	 * header and any padding used to make the action 64-bit aligned.
	 * NB: The length of an action *must* always be a multiple of eight. */
	struct ofp_action_header {
		uint16_t type;                  /* One of OFPAT_*. */
		uint16_t len;                   /* Length of action, including this
										   header.  This is the length of action,
										   including any padding to make it
										   64-bit aligned. */
		uint8_t pad[4];
	};
	OFP_ASSERT(sizeof(struct ofp_action_header) == 8);

	enum ofp_action_type {
		OFPAT_OUTPUT 			= 0, 	/* Output to switch port. */
		// OF1.0 only actions
		OFPAT_SET_VLAN_VID      = 1, 	/* Set the 802.1q VLAN id. */
		OFPAT_SET_VLAN_PCP		= 2,    /* Set the 802.1q priority. */
		OFPAT_STRIP_VLAN		= 3,    /* Strip the 802.1q header. */
		OFPAT_SET_DL_SRC		= 4,    /* Ethernet source address. */
		OFPAT_SET_DL_DST		= 5,    /* Ethernet destination address. */
		OFPAT_SET_NW_SRC		= 6,    /* IP source address. */
		OFPAT_SET_NW_DST		= 7,    /* IP destination address. */
		OFPAT_SET_NW_TOS		= 8,    /* IP ToS (DSCP field, 6 bits). */
		OFPAT_SET_TP_SRC		= 9,    /* TCP/UDP source port. */
		OFPAT_SET_TP_DST		= 10,   /* TCP/UDP destination port. */
		// OF1.0 only actions (end)
		// Please note: #0 and #11 needs special care in OF10 and OF12
		OFPAT_COPY_TTL_OUT 		= 11, 	/* Copy TTL "outwards" -- from next-to-outermost to outermost */
		OFPAT_COPY_TTL_IN 		= 12, 	/* Copy TTL "inwards" -- from outermost to next-to-outermost */
		OFPAT_SET_MPLS_TTL 		= 15, 	/* MPLS TTL */
		OFPAT_DEC_MPLS_TTL 		= 16, 	/* Decrement MPLS TTL */
		OFPAT_PUSH_VLAN 		= 17, 	/* Push a new VLAN tag */
		OFPAT_POP_VLAN 			= 18, 	/* Pop the outer VLAN tag */
		OFPAT_PUSH_MPLS 		= 19, 	/* Push a new MPLS tag */
		OFPAT_POP_MPLS 			= 20, 	/* Pop the outer MPLS tag */
		OFPAT_SET_QUEUE 		= 21, 	/* Set queue id when outputting to a port */
		OFPAT_GROUP 			= 22, 	/* Apply group. */
		OFPAT_SET_NW_TTL 		= 23, 	/* IP TTL. */
		OFPAT_DEC_NW_TTL 		= 24, 	/* Decrement IP TTL. */
		OFPAT_SET_FIELD 		= 25, 	/* Set a header field using OXM TLV format. */
		OFPAT_EXPERIMENTER		= 0xffff
	};



	struct ofp_oxm_hdr {
		uint16_t oxm_class;		/* oxm_class */
		uint8_t  oxm_field;		/* includes has_mask bit! */
		uint8_t  oxm_length;	/* oxm_length */
	};



	// OXM_OF_VLAN_PCP 		/* 3 bits */
	// OXM_OF_IP_DSCP 		/* 6 bits */
	// OXM_OF_IP_ECN		/* 2 bits */
	// OXM_OF_IP_PROTO		/* 8 bits */
	// OXM_OF_ICMPV4_TYPE
	// OXM_OF_ICMPV4_CODE
	// OXM_OF_ICMPV6_TYPE
	// OXM_OF_ICMPV6_CODE
	// OXM_OF_MPLS_TC		/* 3 bits */
	struct ofp_oxm_ofb_uint8_t {
		struct ofp_oxm_hdr hdr;		/* oxm header */
		uint8_t byte;
		uint8_t mask;
	};


	// OXM_OF_ETH_TYPE
	// OXM_OF_VLAN_VID (mask)
	// OXM_OF_TCP_SRC
	// OXM_OF_TCP_DST
	// OXM_OF_UDP_SRC
	// OXM_OF_UDP_DST
	// OXM_OF_SCTP_SRC
	// OXM_OF_SCTP_DST
	// OXM_OF_ARP_OP
	struct ofp_oxm_ofb_uint16_t {
		struct ofp_oxm_hdr hdr;		/* oxm header */
		uint16_t word;				/* network byte order */
		uint16_t mask;
	};


	// OXM_OF_IN_PORT
	// OXM_OF_IN_PHY_PORT
	// OXM_OF_IPV4_SRC (mask)
	// OXM_OF_IPV4_DST (mask)
	// OXM_OF_ARP_SPA (mask)
	// OXM_OF_ARP_THA (mask)
	// OXM_OF_IPV6_FLABEL (mask)
	// OXM_OF_MPLS_LABEL
	struct ofp_oxm_ofb_uint32_t {
		struct ofp_oxm_hdr hdr;		/* oxm header */
		uint32_t dword;				/* network byte order */
		uint32_t mask;				/* only valid, when oxm_hasmask=1 */
	};


	// OXM_OF_IPV6_ND_SLL
	// OXM_OF_IPV6_ND_TLL
	struct ofp_oxm_ofb_uint48_t {
		struct ofp_oxm_hdr hdr;		/* oxm header */
		uint8_t value[6];
		uint8_t mask[6];			/* only valid, when oxm_hasmask=1 */
	};

	// OXM_OF_METADATA (mask)
	struct ofp_oxm_ofb_uint64_t {
		struct ofp_oxm_hdr hdr;		/* oxm header */
		uint8_t word[8];
		uint8_t mask[8];
	#if 0
		uint64_t qword;				/* network byte order */
		uint64_t mask;				/* only valid, when oxm_hasmask=1 */
	#endif
	};


	// OXM_OF_ETH_DST (mask)
	// OXM_OF_ETH_SRC (mask)
	struct ofp_oxm_ofb_maddr {
		struct ofp_oxm_hdr hdr;		/* oxm header */
		uint8_t addr[OFP_ETH_ALEN];
		uint8_t mask[OFP_ETH_ALEN]; /* only valid, when oxm_hasmask=1 */
	};


	// OXM_OF_IPV6_SRC (mask)
	// OXM_OF_IPV6_DST (mask)
	// OXM_OF_IPV6_ND_TARGET
	struct ofp_oxm_ofb_ipv6_addr {
		struct ofp_oxm_hdr hdr;		/* oxm header */
		uint8_t addr[16];
		uint8_t mask[16];			/* only valid, when oxm_hasmask=1 */
	};


	/* OXM Class IDs.
	 * The high order bit differentiate reserved classes from member classes.
	 * Classes 0x0000 to 0x7FFF are member classes, allocated by ONF.
	 * Classes 0x8000 to 0xFFFE are reserved classes, reserved for standardisation.
	 */
	enum ofp_oxm_class {
		OFPXMC_NXM_0			= 0x0000, 	/* Backward compatibility with NXM */
		OFPXMC_NXM_1			= 0x0001,	/* Backward compatibility with NXM */
		OFPXMC_OPENFLOW_BASIC	= 0x8000,	/* Basic class for OpenFlow */
		OFPXMC_EXPERIMENTER		= 0xFFFF,	/* Experimenter class */
	};


	/* OXM Flow match field types for OpenFlow basic class. */
	enum oxm_ofb_match_fields {
		OFPXMT_OFB_IN_PORT = 0,			/* Switch input port. */				// required
		OFPXMT_OFB_IN_PHY_PORT = 1,		/* Switch physical input port. */
		OFPXMT_OFB_METADATA = 2,		/* Metadata passed between tables. */
		OFPXMT_OFB_ETH_DST = 3,			/* Ethernet destination address. */		// required
		OFPXMT_OFB_ETH_SRC = 4,			/* Ethernet source address. */			// required
		OFPXMT_OFB_ETH_TYPE = 5,		/* Ethernet frame type. */				// required
		OFPXMT_OFB_VLAN_VID = 6,		/* VLAN id. */
		OFPXMT_OFB_VLAN_PCP = 7,		/* VLAN priority. */
		OFPXMT_OFB_IP_DSCP = 8,			/* IP DSCP (6 bits in ToS field). */
		OFPXMT_OFB_IP_ECN = 9,			/* IP ECN (2 bits in ToS field). */
		OFPXMT_OFB_IP_PROTO = 10,		/* IP protocol. */						// required
		OFPXMT_OFB_IPV4_SRC = 11,		/* IPv4 source address. */				// required
		OFPXMT_OFB_IPV4_DST = 12,		/* IPv4 destination address. */			// required
		OFPXMT_OFB_TCP_SRC = 13,		/* TCP source port. */					// required
		OFPXMT_OFB_TCP_DST = 14,		/* TCP destination port. */				// required
		OFPXMT_OFB_UDP_SRC = 15,		/* UDP source port. */					// required
		OFPXMT_OFB_UDP_DST = 16,		/* UDP destination port. */				// required
		OFPXMT_OFB_SCTP_SRC = 17,		/* SCTP source port. */
		OFPXMT_OFB_SCTP_DST = 18,		/* SCTP destination port. */
		OFPXMT_OFB_ICMPV4_TYPE = 19,	/* ICMP type. */
		OFPXMT_OFB_ICMPV4_CODE = 20,	/* ICMP code. */
		OFPXMT_OFB_ARP_OP = 21,			/* ARP opcode. */
		OFPXMT_OFB_ARP_SPA = 22,		/* ARP source IPv4 address. */
		OFPXMT_OFB_ARP_TPA = 23,		/* ARP target IPv4 address. */
		OFPXMT_OFB_ARP_SHA = 24,		/* ARP source hardware address. */
		OFPXMT_OFB_ARP_THA = 25,		/* ARP target hardware address. */
		OFPXMT_OFB_IPV6_SRC = 26,		/* IPv6 source address. */				// required
		OFPXMT_OFB_IPV6_DST = 27,		/* IPv6 destination address. */			// required
		OFPXMT_OFB_IPV6_FLABEL = 28,	/* IPv6 Flow Label */
		OFPXMT_OFB_ICMPV6_TYPE = 29,	/* ICMPv6 type. */
		OFPXMT_OFB_ICMPV6_CODE = 30,	/* ICMPv6 code. */
		OFPXMT_OFB_IPV6_ND_TARGET = 31,	/* Target address for ND. */
		OFPXMT_OFB_IPV6_ND_SLL = 32,	/* Source link-layer for ND. */
		OFPXMT_OFB_IPV6_ND_TLL = 33,	/* Target link-layer for ND. */
		OFPXMT_OFB_MPLS_LABEL = 34,		/* MPLS label. */
		OFPXMT_OFB_MPLS_TC = 35,		/* MPLS TC. */
		OFPXMT_OFB_MPLS_BOS = 36,		/* MPLS BoS bit. */
		OFPXMT_OFB_PBB_ISID = 37,		/* PBB I-SID. */
		OFPXMT_OFB_TUNNEL_ID = 38,		/* Logical Port Metadata. */
		OFPXMT_OFB_IPV6_EXTHDR = 39,	/* IPv6 Extension Header pseudo-field */
		/* max value */
		OFPXMT_OFB_MAX,
	};


	/* The VLAN id is 12-bits, so we can use the entire 16 bits to indicate
	* special conditions.
	*/
	enum ofp_vlan_id {
		OFPVID_PRESENT = 0x1000, /* Bit that indicate that a VLAN id is set */
		OFPVID_NONE = 0x0000, /* No VLAN id was set. */
	};





	/* Fields to match against flows */
	struct ofp_match {
		uint16_t type;			/* One of OFPMT_* */
		uint16_t length;		/* Length of ofp_match (excluding padding) */
		/* Followed by:
		 * - Exactly (length - 4) (possibly 0) bytes containing OXM TLVs, then
		 * - Exactly ((length + 7)/8*8 - length) (between 0 and 7) bytes of
		 * all-zero bytes
		 * In summary, ofp_match is padded as needed, to make its overall size
		 * a multiple of 8, to preserve alignement in structures using it.
		 */
		uint8_t oxm_fields[4];
		/* OXMs start here - Make compiler happy */
	};
	OFP_ASSERT(sizeof(struct ofp_match) == 8);


	/* The match type indicates the match structure (set of fields that compose the
	* match) in use. The match type is placed in the type field at the beginning
	* of all match structures. The "OpenFlow Extensible Match" type corresponds
	* to OXM TLV format described below and must be supported by all OpenFlow
	* switches. Extensions that define other match types may be published on the
	* ONF wiki. Support for extensions is optional.
	*/
	enum ofp_match_type {
		OFPMT_STANDARD = 0, /* Deprecated. */
		OFPMT_OXM = 1, 		/* OpenFlow Extensible Match */
	};


	/* Group numbering. Groups can use any number up to OFPG_MAX. */
	enum ofp_group {
		/* Last usable group number. */
		OFPG_MAX        = 0xffffff00,

		/* Fake groups. */
		OFPG_ALL        = 0xfffffffc,  /* Represents all groups for group delete
										  commands. */
		OFPG_ANY        = 0xffffffff   /* Wildcard group used only for flow stats
										  requests. Selects all flows regardless of
										  group (including flows with no group).
										  */
	};

	/* Bucket for use in groups. */
	struct ofp_bucket {
		uint16_t len;                   /* Length the bucket in bytes, including
										   this header and any padding to make it
										   64-bit aligned. */
		uint16_t weight;                /* Relative weight of bucket.  Only
										   defined for select groups. */
		uint32_t watch_port;            /* Port whose state affects whether this
										   bucket is live.  Only required for fast
										   failover groups. */
		uint32_t watch_group;           /* Group whose state affects whether this
										   bucket is live.  Only required for fast
										   failover groups. */
		uint8_t pad[4];
		struct ofp_action_header actions[0]; /* The action length is inferred
											   from the length field in the
											   header. */
	};
	OFP_ASSERT(sizeof(struct ofp_bucket) == 16);



	/* Values for ’type’ in ofp_error_message. These values are immutable: they
	 * will not change in future versions of the protocol (although new values may
	 * be added). */
	enum ofp_error_type {
		OFPET_HELLO_FAILED 			= 0, 	/* Hello protocol failed. */
		OFPET_BAD_REQUEST 			= 1, 	/* Request was not understood. */
		OFPET_BAD_ACTION 			= 2, 	/* Error in action description. */
		OFPET_BAD_INSTRUCTION 		= 3, 	/* Error in instruction list. */
		OFPET_BAD_MATCH 			= 4, 	/* Error in match. */
		OFPET_FLOW_MOD_FAILED 		= 5, 	/* Problem modifying flow entry. */
		OFPET_GROUP_MOD_FAILED 		= 6, 	/* Problem modifying group entry. */
		OFPET_PORT_MOD_FAILED 		= 7, 	/* Port mod request failed. */
		OFPET_TABLE_MOD_FAILED 		= 8, 	/* Table mod request failed. */
		OFPET_QUEUE_OP_FAILED 		= 9, 	/* Queue operation failed. */
		OFPET_SWITCH_CONFIG_FAILED 	= 10, 	/* Switch config request failed. */
		OFPET_ROLE_REQUEST_FAILED 	= 11, 	/* Controller Role request failed. */
		OFPET_METER_MOD_FAILED		= 12, 	/* Error in meter. */
		OFPET_TABLE_FEATURES_FAILED = 13, 	/* Setting table features failed. */
		OFPET_EXPERIMENTER 			= 0xffff /* Experimenter error messages. */
	};


	/* ofp_error_msg 'code' values for OFPET_HELLO_FAILED.  'data' contains an
	 * ASCII text string that may give failure details. */
	enum ofp_hello_failed_code {
		OFPHFC_INCOMPATIBLE			= 0,    /* No compatible version. */
		OFPHFC_EPERM				= 1,	/* Permissions error. */
	};

	/* ofp_error_msg ’code’ values for OFPET_BAD_REQUEST. ’data’ contains at least
	* the first 64 bytes of the failed request. */
	enum ofp_bad_request_code {
		OFPBRC_BAD_VERSION 			= 0, /* ofp_header.version not supported. */
		OFPBRC_BAD_TYPE 			= 1, /* ofp_header.type not supported. */
		OFPBRC_BAD_STAT 			= 2, /* ofp_stats_request.type not supported. */
		OFPBRC_BAD_EXPERIMENTER 	= 3, /* Experimenter id not supported
									 	  * (in ofp_experimenter_header or
									 	  * ofp_stats_request or ofp_stats_reply). */
		OFPBRC_BAD_EXP_TYPE 		= 4, /* Experimenter type not supported. */
		OFPBRC_EPERM 				= 5, /* Permissions error. */
		OFPBRC_BAD_LEN 				= 6, /* Wrong request length for type. */
		OFPBRC_BUFFER_EMPTY 		= 7, /* Specified buffer has already been used. */
		OFPBRC_BUFFER_UNKNOWN 		= 8, /* Specified buffer does not exist. */
		OFPBRC_BAD_TABLE_ID 		= 9, /* Specified table-id invalid or does not
								  	  	  * exist. */
		OFPBRC_IS_SLAVE 			= 10, /* Denied because controller is slave. */
		OFPBRC_BAD_PORT 			= 11, /* Invalid port. */
		OFPBRC_BAD_PACKET 			= 12, /* Invalid packet in packet-out. */
		OFPBRC_MULTIPART_BUFFER_OVERFLOW = 13, /* ofp_multipart_request
												* overflowed the assigned buffer. */
	};


	/* ofp_error_msg 'code' values for OFPET_BAD_ACTION.  'data' contains at least
	 * the first 64 bytes of the failed request. */
	enum ofp_bad_action_code {
		OFPBAC_BAD_TYPE				= 0,	/* Unknown action type. */
		OFPBAC_BAD_LEN				= 1,	/* Length problem in actions. */
		OFPBAC_BAD_EXPERIMENTER		= 2,   	/* Unknown experimenter id specified. */
		OFPBAC_BAD_EXP_TYPE			= 3, 	/* Unknown action type for experimenter id. */
		OFPBAC_BAD_OUT_PORT			= 4,    /* Problem validating output port. */
		OFPBAC_BAD_ARGUMENT			= 5,    /* Bad action argument. */
		OFPBAC_EPERM				= 6,	/* Permissions error. */
		OFPBAC_TOO_MANY				= 7,    /* Can't handle this many actions. */
		OFPBAC_BAD_QUEUE			= 8,	/* Problem validating output queue. */
		OFPBAC_BAD_OUT_GROUP		= 9,	/* Invalid group id in forward action. */
		OFPBAC_MATCH_INCONSISTENT	= 10, 	/* Action can't apply for this match. */
		OFPBAC_UNSUPPORTED_ORDER	= 11,	/* Action order is unsupported for the action
					  	  	  	  	  	  	 * list in an Apply-Actions instruction */
		OFPBAC_BAD_TAG				= 12,	/* Actions uses an unsupported
									  	     * tag/encap. */
		OFPBAC_BAD_SET_TYPE			= 13, 	/* Unsupported type in SET_FIELD action. */
		OFPBAC_BAD_SET_LEN			= 14, 	/* Length problem in SET_FIELD action. */
		OFPBAC_BAD_SET_ARGUMENT		= 15, 	/* Bad argument in SET_FIELD action. */
	};

	/* ofp_error_msg 'code' values for OFPET_BAD_INSTRUCTION.  'data' contains at least
	 * the first 64 bytes of the failed request. */
	enum ofp_bad_instruction_code {
		OFPBIC_UNKNOWN_INST			= 0,    /* Unknown instruction. */
		OFPBIC_UNSUP_INST			= 1,    /* Switch or table does not support the
									  	  	 * instruction. */
		OFPBIC_BAD_TABLE_ID			= 2,    /* Invalid Table-ID specified. */
		OFPBIC_UNSUP_METADATA		= 3,    /* Metadata value unsupported by datapath. */
		OFPBIC_UNSUP_METADATA_MASK	= 4,	/* Metadata mask value unsupported by
									  	  	 * datapath. */
		OFPBIC_BAD_EXPERIMENTER		= 5,    /* Specific experimenter instruction
									  	  	 * unsupported. */
		OFPBIC_BAD_EXP_TYPE			= 6, 	/* Unknown instruction for experimenter id. */
		OFPBIC_BAD_LEN				= 7, 	/* Length problem in instructions. */
		OFPBIC_EPERM				= 8, 	/* Permissions error. */
	};

	/* ofp_error_msg 'code' values for OFPET_BAD_MATCH. 'data' contains at least
	* the first 64 bytes of the failed request. */
	enum ofp_bad_match_code {
		OFPBMC_BAD_TYPE 			= 0, 	/* Unsupported match type specified by the match */
		OFPBMC_BAD_LEN 				= 1, 	/* Length problem in match. */
		OFPBMC_BAD_TAG 				= 2, 	/* Match uses an unsupported tag/encap. */
		OFPBMC_BAD_DL_ADDR_MASK 	= 3, 	/* Unsupported datalink addr mask - switch
											 * does not support arbitrary datalink
											 * address mask. */
		OFPBMC_BAD_NW_ADDR_MASK 	= 4, 	/* Unsupported network addr mask - switch
											 * does not support arbitrary network
											 * address mask. */
		OFPBMC_BAD_WILDCARDS 		= 5, 	/* Unsupported combination of fields masked
											 * or omitted in the match. */
		OFPBMC_BAD_FIELD 			= 6, 	/* Unsupported field type in the match. */
		OFPBMC_BAD_VALUE 			= 7, 	/* Unsupported value in a match field. */
		OFPBMC_BAD_MASK 			= 8, 	/* Unsupported mask specified in the match,
											 * field is not dl-address or nw-address. */
		OFPBMC_BAD_PREREQ 			= 9, 	/* A prerequisite was not met. */
		OFPBMC_DUP_FIELD 			= 10, 	/* A field type was duplicated. */
		OFPBMC_EPERM 				= 11, 	/* Permissions error. */
	};

	/* ofp_error_msg 'code' values for OFPET_FLOW_MOD_FAILED.  'data' contains
	 * at least the first 64 bytes of the failed request. */
	enum ofp_flow_mod_failed_code {
		OFPFMFC_UNKNOWN				= 0,    /* Unspecified error. */
		OFPFMFC_TABLE_FULL			= 1,    /* Flow not added because table was full. */
		OFPFMFC_BAD_TABLE_ID		= 2,    /* Table does not exist */
		OFPFMFC_OVERLAP				= 3,    /* Attempted to add overlapping flow with
									   	   	 * CHECK_OVERLAP flag set. */
		OFPFMFC_EPERM				= 4,	/* Permissions error. */
		OFPFMFC_BAD_TIMEOUT			= 5,    /* Flow not added because of unsupported
									   	     * idle/hard timeout. */
		OFPFMFC_BAD_COMMAND			= 6,	/* Unsupported or unknown command. */
		OFPFMFC_BAD_FLAGS			= 7,	/* Unsupported or unknown flags. */
	};

	/* ofp_error_msg 'code' values for OFPET_GROUP_MOD_FAILED.  'data' contains
	 * at least the first 64 bytes of the failed request. */
	enum ofp_group_mod_failed_code {
		OFPGMFC_GROUP_EXISTS 			= 0,   	/* Group not added because a group ADD
												 * attempted to replace an
												 * already-present group. */
		OFPGMFC_INVALID_GROUP         	= 1,   	/* Group not added because Group specified
												 * is invalid. */
		OFPGMFC_WEIGHT_UNSUPPORTED		= 2,    /* Switch does not support unequal load
												 * sharing with select groups. */
		OFPGMFC_OUT_OF_GROUPS			= 3,    /* The group table is full. */
		OFPGMFC_OUT_OF_BUCKETS			= 4,    /* The maximum number of action buckets
												 * for a group has been exceeded. */
		OFPGMFC_CHAINING_UNSUPPORTED	= 5,    /* Switch does not support groups that
												 * forward to groups. */
		OFPGMFC_WATCH_UNSUPPORTED		= 6,    /* This group cannot watch the
												   watch_port or watch_group specified. */
		OFPGMFC_LOOP					= 7,    /* Group entry would cause a loop. */
		OFPGMFC_UNKNOWN_GROUP			= 8,    /* Group not modified because a group
												   MODIFY attempted to modify a
												   non-existent group. */
		OFPGMFC_CHAINED_GROUP 			= 9,	/* Group not deleted because another
												   group is forwarding to it. */
		OFPGMFC_BAD_TYPE				= 10,	/* Unsupported or unknown group type. */
		OFPGMFC_BAD_COMMAND				= 11,	/* Unsupported or unknown command. */
		OFPGMFC_BAD_BUCKET				= 12,	/* Error in bucket. */
		OFPGMFC_BAD_WATCH				= 13,	/* Error in watch port/group. */
		OFPGMFC_EPERM					= 14,	/* Permissions error. */
	};

	/* ofp_error_msg 'code' values for OFPET_PORT_MOD_FAILED.  'data' contains
	 * at least the first 64 bytes of the failed request. */
	enum ofp_port_mod_failed_code {
		OFPPMFC_BAD_PORT				= 0,	/* Specified port number does not exist. */
		OFPPMFC_BAD_HW_ADDR				= 1,	/* Specified hardware address does not
									  	  	  	 * match the port number. */
		OFPPMFC_BAD_CONFIG				= 2,	/* Specified config is invalid. */
		OFPPMFC_BAD_ADVERTISE			= 3,	/* Specified advertise is invalid. */
		OFPPMFC_EPERM					= 4,	/* Permissions error. */
	};

	/* ofp_error_msg 'code' values for OFPET_TABLE_MOD_FAILED.  'data' contains
	 * at least the first 64 bytes of the failed request. */
	enum ofp_table_mod_failed_code {
		OFPTMFC_BAD_TABLE				= 0,	/* Specified table does not exist. */
		OFPTMFC_BAD_CONFIG				= 1,	/* Specified config is invalid. */
		OFPTMFC_EPERM					= 2,	/* Permissions error. */
	};

	/* ofp_error msg 'code' values for OFPET_QUEUE_OP_FAILED. 'data' contains
	 * at least the first 64 bytes of the failed request */
	enum ofp_queue_op_failed_code {
		OFPQOFC_BAD_PORT				= 0,	/* Invalid port (or port does not exist). */
		OFPQOFC_BAD_QUEUE				= 1,	/* Queue does not exist. */
		OFPQOFC_EPERM					= 2,	/* Permissions error. */
	};

	/* ofp_error_msg 'code' values for OFPET_SWITCH_CONFIG_FAILED. 'data' contains
	 * at least the first 64 bytes of the failed request. */
	enum ofp_switch_config_failed_code {
		OFPSCFC_BAD_FLAGS				= 0,	/* Specified flags is invalid. */
		OFPSCFC_BAD_LEN					= 1,	/* Specified len is invalid. */
		OFPSCFC_EPERM					= 2,	/* Permissions error. */
	};

	/* ofp_error_msg ’code’ values for OFPET_ROLE_REQUEST_FAILED. ’data’ contains
	* at least the first 64 bytes of the failed request. */
	enum ofp_role_request_failed_code {
		OFPRRFC_STALE 					= 0, 	/* Stale Message: old generation_id. */
		OFPRRFC_UNSUP 					= 1, 	/* Controller role change unsupported. */
		OFPRRFC_BAD_ROLE 				= 2, 	/* Invalid role. */
	};

	/* ofp_error_msg ’code’ values for OFPET_METER_MOD_FAILED. ’data’ contains
	* at least the first 64 bytes of the failed request. */
	enum ofp_meter_mod_failed_code {
		OFPMMFC_UNKNOWN					= 0, 	/* Unspecified error. */
		OFPMMFC_METER_EXISTS 			= 1, 	/* Meter not added because a Meter ADD
		 	 	 	 	 	 	 	 	 	 	 * attempted to replace an existing Meter. */
		OFPMMFC_INVALID_METER 			= 2,	/* Meter not added because Meter specified
		 	 	 	 	 	 	 	 	 	 	 * is invalid. */
		OFPMMFC_UNKNOWN_METER 			= 3, 	/* Meter not modified because a Meter
												 * MODIFY attempted to modify a non-existent
												 * Meter. */
		OFPMMFC_BAD_COMMAND				= 4, 	/* Unsupported or unknown command. */
		OFPMMFC_BAD_FLAGS				= 5, 	/* Flag configuration unsupported. */
		OFPMMFC_BAD_RATE				= 6, 	/* Rate unsupported. */
		OFPMMFC_BAD_BURST				= 7, 	/* Burst size unsupported. */
		OFPMMFC_BAD_BAND				= 8, 	/* Band unsupported. */
		OFPMMFC_BAD_BAND_VALUE 			= 9, 	/* Band value unsupported. */
		OFPMMFC_OUT_OF_METERS 			= 10, 	/* No more meters available. */
		OFPMMFC_OUT_OF_BANDS 			= 11, 	/* The maximum number of properties
		 	 	 	 	 	 	 	 	 	 	 * for a meter has been exceeded. */
	};

	/* ofp_error_msg ’code’ values for OFPET_TABLE_FEATURES_FAILED. ’data’ contains
	* at least the first 64 bytes of the failed request. */
	enum ofp_table_features_failed_code {
		OFPTFFC_BAD_TABLE				= 0,	/* Specified table does not exist. */
		OFPTFFC_BAD_METADATA 			= 1,	/* Invalid metadata mask. */
		OFPTFFC_BAD_TYPE				= 2,	/* Unknown property type. */
		OFPTFFC_BAD_LEN					= 3, 	/* Length problem in properties. */
		OFPTFFC_BAD_ARGUMENT 			= 4, 	/* Unsupported property value. */
		OFPTFFC_EPERM					= 5, 	/* Permissions error. */
	};


	enum ofp_multipart_types {
		/* Description of this OpenFlow switch.
		* The request body is empty.
		* The reply body is struct ofp_desc. */
		OFPMP_DESC = 0,

		/* Individual flow statistics.
		* The request body is struct ofp_flow_stats_request.
		* The reply body is an array of struct ofp_flow_stats. */
		OFPMP_FLOW = 1,

		/* Aggregate flow statistics.
		* The request body is struct ofp_aggregate_stats_request.
		* The reply body is struct ofp_aggregate_stats_reply. */
		OFPMP_AGGREGATE = 2,

		/* Flow table statistics.
		* The request body is empty.
		* The reply body is an array of struct ofp_table_stats. */
		OFPMP_TABLE = 3,

		/* Port statistics.
		* The request body is struct ofp_port_stats_request.
		* The reply body is an array of struct ofp_port_stats. */
		OFPMP_PORT_STATS = 4,

		/* Queue statistics for a port
		* The request body is struct ofp_queue_stats_request.
		* The reply body is an array of struct ofp_queue_stats */
		OFPMP_QUEUE = 5,

		/* Group counter statistics.
		* The request body is struct ofp_group_stats_request.
		* The reply is an array of struct ofp_group_stats. */
		OFPMP_GROUP = 6,

		/* Group description.
		* The request body is empty.
		* The reply body is an array of struct ofp_group_desc_stats. */
		OFPMP_GROUP_DESC = 7,

		/* Group features.
		* The request body is empty.
		* The reply body is struct ofp_group_features. */
		OFPMP_GROUP_FEATURES = 8,

		/* Meter statistics.
		* The request body is struct ofp_meter_multipart_requests.
		* The reply body is an array of struct ofp_meter_stats. */
		OFPMP_METER = 9,

		/* Meter configuration.
		* The request body is struct ofp_meter_multipart_requests.
		* The reply body is an array of struct ofp_meter_config. */
		OFPMP_METER_CONFIG = 10,

		/* Meter features.
		* The request body is empty.
		* The reply body is struct ofp_meter_features. */
		OFPMP_METER_FEATURES = 11,

		/* Table features.
		* The request body is either empty or contains an array of
		* struct ofp_table_features containing the controller’s
		* desired view of the switch. If the switch is unable to
		* set the specified view an error is returned.
		* The reply body is an array of struct ofp_table_features. */
		OFPMP_TABLE_FEATURES = 12,

		/* Port description.
		* The request body is empty.
		* The reply body is an array of struct ofp_port. */
		OFPMP_PORT_DESC = 13,

		/* Experimenter extension.
		* The request and reply bodies begin with
		* struct ofp_experimenter_multipart_header.
		* The request and reply bodies are otherwise experimenter-defined. */
		OFPMP_EXPERIMENTER = 0xffff
	};


}; // end of namespace openflow
}; // end of namespace rofl


#endif /* OPENFLOW_COMMON_H_ */
