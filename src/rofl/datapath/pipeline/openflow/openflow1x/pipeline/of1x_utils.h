/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __OF1X_UTILS_H__
#define __OF1X_UTILS_H__

#include <inttypes.h>
#include "../../of_switch.h" 

/**
* @file of1x_utils.h
* @author Marc Sune<marc.sune (at) bisdn.de>
*/


//
// OpenFlow 1.X specific return values
//

/**
* @ingroup core_of1x 
* Extended flowmod return codes
*/
typedef enum rofl_of1x_fm_result{
	ROFL_OF1X_FM_SUCCESS	= EXIT_SUCCESS,
	ROFL_OF1X_FM_FAILURE	= EXIT_FAILURE,
	ROFL_OF1X_FM_OVERLAP
}rofl_of1x_fm_result_t;


//
// Removal mutex management
//

/*
* Mutex state for flow_entry_removal 
*/
typedef enum of1x_mutex_acquisition_required{
	MUTEX_NOT_ACQUIRED = 0, 			/*mutex has not been acquired and we must take it*/
	MUTEX_ALREADY_ACQUIRED_BY_TIMER_EXPIRATION,	/*mutex was taken when checking for expirations. We shouldn't call the timers functions*/
	MUTEX_ALREADY_ACQUIRED_NON_STRICT_SEARCH	/*mutex was taken when looking for entries with a non strict definition*/
}of1x_mutex_acquisition_required_t;

//
// Pipeline version
//

/*
* Required pipeline version
*/
typedef struct of1x_ver_req_t{
	of_version_t min_ver;	//of_version_t
	of_version_t max_ver;	//of_version_t
}of1x_ver_req_t;

#define OF1X_MIN_VERSION OF_VERSION_10
#define OF1X_MAX_VERSION OF_VERSION_13


//
// Match/Actions masks
//
#if defined(BIG_ENDIAN_DETECTED)
	/*
	* Useful masks
	*/
	//Byte masks
	#define OF1X_8_BYTE_MASK	0xFFFFFFFFFFFFFFFFULL 
	#define OF1X_6_BYTE_MASK	0xFFFFFFFFFFFF0000ULL
	#define OF1X_4_BYTE_MASK	0xFFFFFFFF
	#define OF1X_3_BYTE_MASK	0xFFFFFF00
	#define OF1X_2_BYTE_MASK	0xFFFF
	#define OF1X_1_BYTE_MASK	0xFF
	  
	//Non-multiple of byte masks
	#define OF1X_48_BITS_MASK	0xFFFFFFFFFFFF0000ULL 
	#define OF1X_20_BITS_MASK	0xFFFFF000
	#define OF1X_13_BITS_MASK	0xFFF8
	#define OF1X_12_BITS_MASK	0xFFF0
	#define OF1X_9_BITS_MASK	0xFF80
	#define OF1X_6_BITS_MASK	0xFC
	#define OF1X_4_BITS_MASK	0xF0
	#define OF1X_3_BITS_MASK	0xE0
	#define OF1X_2_BITS_MASK	0xC0
	#define OF1X_1_BIT_MASK		0x80

	#define OF1X_VLAN_PRESENT_MASK	0x0008
	#define OF1X_VLAN_ID_MASK	0x0FFF
	
#elif defined(LITTLE_ENDIAN_DETECTED)
	/*
	* Useful masks
	*/
	//Byte masks
	#define OF1X_8_BYTE_MASK	0xFFFFFFFFFFFFFFFFULL 
	#define OF1X_6_BYTE_MASK	0x0000FFFFFFFFFFFFULL
	#define OF1X_4_BYTE_MASK	0x00000000FFFFFFFF
	#define OF1X_3_BYTE_MASK	0x0000000000FFFFFF
	#define OF1X_2_BYTE_MASK	0x000000000000FFFF
	#define OF1X_1_BYTE_MASK	0x00000000000000FF
	  
	//Non-multiple of byte masks
	#define OF1X_48_BITS_MASK	0x0000FFFFFFFFFFFFULL 
	#define OF1X_20_BITS_MASK	0x0000000000F0FFFF
	#define OF1X_13_BITS_MASK	0x0000000000001FFF
	#define OF1X_12_BITS_MASK	0x000000000000FF0F
	#define OF1X_9_BITS_MASK	0x00000000000001FF
	#define OF1X_6_BITS_MASK	0x000000000000003F
	#define OF1X_4_BITS_MASK	0x000000000000000F
	#define OF1X_3_BITS_MASK	0x0000000000000007
	#define OF1X_2_BITS_MASK	0x0000000000000003
	#define OF1X_1_BIT_MASK		0x0000000000000001	
	
	#define OF1X_VLAN_PRESENT_MASK	0x0010
	#define OF1X_VLAN_ID_MASK	0xFF0F
	
#else
	#error Unknwon endianness
#endif

// Masks and flags (intra byte => not dependent on endianness)
#define OF1X_BIT0_MASK 1<<0
#define OF1X_BIT1_MASK 1<<1
#define OF1X_BIT2_MASK 1<<2
#define OF1X_BIT3_MASK 1<<3
#define OF1X_BIT4_MASK 1<<4
#define OF1X_BIT5_MASK 1<<5
#define OF1X_BIT6_MASK 1<<6
#define OF1X_BIT7_MASK 1<<7

#define OF1X_4MSBITS_MASK 0xF0 // 1111 0000
#define OF1X_4LSBITS_MASK 0x0F // 0000 1111
#define OF1X_6MSBITS_MASK 0xFC // 1111 1100
#define OF1X_2LSBITS_MASK 0x03 // 
#define OF1X_3MSBITS_MASK 0xE0 //1110 0000
#define OF1X_BITS_5AND6_MASK 0x30


//
// Bit alignement MACROS
//

//alignemnt operations not dependant on endiannes
#define OF1X_SHIFT_LEFT(x, y) (x<<y)
#define OF1X_SHIFT_RIGHT(x, y) (x>>y)

//aligment operations dependent on endiannes
#define OF1X_SWAP_AND_SHIFT_64(x, y) do{ \
		x=__bswap_64(x); \
		x>>=y; \
		}while(0)
//NOTE swapping will be done externally. DUMPING?

//Vlan pcp is found in the 3 most significant bits: 1110 0000
#define OF1X_VLAN_PCP_VALUE(x) OF1X_SHIFT_RIGHT(x, 5)
#define OF1X_VLAN_PCP_ALIGN(x) OF1X_SHIFT_LEFT(x, 5)
#define OF1X_MPLS_LABEL_VALUE(x) OF1X_SHIFT_RIGHT(x, 12)
#define OF1X_MPLS_LABEL_ALIGN(x) OF1X_SHIFT_LEFT(x, 12)


/*
* Align to 64bit
*/
#define OF1X_ALIGN_64_BIT(x) (((x/8)+1)*8)

#endif //OF1X_UTILS
