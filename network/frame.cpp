#include "frame.hpp"

#define MSB_UINT16(x) ((x>>8) & 0xff)
#define LSB_UINT16(x) (x & 0xff)

Frame::frame Frame::build_outbound_frame(const Node<>::node_list good_nodes,
										 const Node<>::node_list bad_nodes,
										 const Frame::frame & message) {
	Frame::frame outbound_frame;
	uint16_t good_n_size = good_nodes.size();
	uint16_t bad_n_size = bad_nodes.size();

	// encode the list of nodes that should receive the packet correctly
	outbound_frame.push_back(OUTBOUND_FRAME);
	outbound_frame.push_back(MSB_UINT16(good_n_size));
	outbound_frame.push_back(LSB_UINT16(good_n_size));
	for(Node<>::node_list::const_iterator i = good_nodes.begin(); i != good_nodes.end(); ++i) {
		uint16_t node_id = (* i)->get_id();
		outbound_frame.push_back(MSB_UINT16(node_id));
		outbound_frame.push_back(LSB_UINT16(node_id));
	}

	// encode the list of nodes that should receive a damaged packet
	outbound_frame.push_back(MSB_UINT16(bad_n_size));
	outbound_frame.push_back(LSB_UINT16(bad_n_size));
	for(Node<>::node_list::const_iterator i = bad_nodes.begin(); i != bad_nodes.end(); ++i) {
		uint16_t node_id = (* i)->get_id();
		outbound_frame.push_back(MSB_UINT16(node_id));
		outbound_frame.push_back(LSB_UINT16(node_id));
	}

	// add the initial frame


	return outbound_frame;
}
