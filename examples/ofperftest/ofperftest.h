#ifndef ofperftest_H
#define ofperftest_H 1

#include <map>
#include "rofl/common/cmacaddr.h"
#include "rofl/common/caddress.h"
#include "rofl/common/crofbase.h"
#include "rofl/common/openflow/cofdpt.h"

using namespace rofl;

class ofperftest :
		public crofbase
{
private:

	struct fibentry_t {
		uint32_t 		port_no;	// port where a certain is attached
		time_t 			timeout;	// timeout event for this FIB entry
	};

	// a very simple forwarding information base
	std::map<cofdpt*, std::map<uint16_t, std::map<cmacaddr, struct fibentry_t> > > fib;

	unsigned int 		fib_check_timeout; 		// periodic timeout for removing expired FIB entries
	unsigned int		flow_stats_timeout;		// periodic timeout for requesting flow stats
	unsigned int		fm_delete_all_timeout;	// periodic purging of all FLOW-MODs

	enum ofperftest_timer_t {
		ETHSWITCH_TIMER_BASE = ((0x6271)),
		ETHSWITCH_TIMER_FIB,
		ETHSWITCH_TIMER_FLOW_MOD_DELETE_ALL,
	};

public:

	ofperftest();

	virtual
	~ofperftest();

	virtual void
	handle_timeout(int opaque);

	virtual void
	handle_dpath_open(cofdpt *dpt);

	virtual void
	handle_dpath_close(cofdpt *dpt);

	virtual void
	handle_packet_in(cofdpt *dpt, cofmsg_packet_in *msg);


private:

	void
	drop_expired_fib_entries();

	void
	flow_mod_delete_all();
};

#endif
