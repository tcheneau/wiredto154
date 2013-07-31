/*
* Conditions Of Use
*
* This software was developed by employees of the National Institute of
* Standards and Technology (NIST), and others.
* This software has been contributed to the public domain.
* Pursuant to title 15 United States Code Section 105, works of NIST
* employees are not subject to copyright protection in the United States
* and are considered to be in the public domain.
* As a result, a formal license is not needed to use this software.
*
* This software is provided "AS IS."
* NIST MAKES NO WARRANTY OF ANY KIND, EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTY OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT
* AND DATA ACCURACY.  NIST does not warrant or make any representations
* regarding the use of the software or the results thereof, including but
* not limited to the correctness, accuracy, reliability or usefulness of
* this software.
*/

/* Tony Cheneau <tony.cheneau@nist.gov> */

#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <map>
#include <signal.h>
#include <string>
#include <sstream>

namespace po = boost::program_options;
namespace ip = boost::asio::ip;
using boost::asio::ip::udp;
using namespace std;

#define FILE_PREFIX "capture"
#define MAX_PACKET_SIZE 65535
#define char_to_uint16(tab, x) ((tab[x] << 8) + tab[x+1])

sig_atomic_t capture_exit = false;
void exit_signal(int) { capture_exit = true; }

class PcapDumper {
private:
	FILE * _file;
	string _filename;
	/* from the pcap.h */
	struct pcap_file_header
	{
		uint32_t    magic;
		uint16_t    version_major;
		uint16_t    version_minor;
		int32_t     thiszone;
		uint32_t    sigfigs;
		uint32_t    snaplen;
		uint32_t    linktype;
	};

	struct pcap_pkthdr {
			uint32_t ts_sec;     /* time stamp (second) */
			uint32_t ts_msec;    /* time stamp (microseconds) */
			uint32_t caplen;     /* length of portion present */
			uint32_t len;        /* length this packet (off wire) */
	};

public:
	PcapDumper(string filename):
		_filename(filename)
	{
		open_file(_filename.c_str());
		write_header(_file);
	}

	~PcapDumper() {
		if (_file) {
			if (fclose(_file)) perror("fclose()");
		}
	}

	void open_file(const char * filename) {
		_file = fopen(filename, "w+");
		if (_file==NULL) {
			perror("PcapDumper::open_file()");
			exit(EXIT_FAILURE);
		}

	}

	void write_header(FILE * fd) {
		struct pcap_file_header header;

		/* see pcap-savefile(5) */
		header.magic = 0xa1b2c3d4;
		header.version_major = 2;
		header.version_minor = 4;
		header.thiszone = 0;
		header.sigfigs = 0;
		header.snaplen = 127; /* max MTU on IEEE 802.15.4 links */
		header.linktype = 195; /* IEEE 802.15.4 + FCS, see http://www.tcpdump.org/linktypes.html */


		if (fwrite(&header, 1, sizeof(header), _file) != sizeof(header)) {
			perror("write");
			exit(EXIT_FAILURE);
		}
	}

	void write_data(char * data, size_t size) {
		struct pcap_pkthdr header;
		struct timeval tv;

		memset(&header, 0, sizeof(header));

		gettimeofday(&tv, NULL);
		/* force casting to 32 bit (struct timeval could be storing in 64 bit format)*/
		header.ts_sec = (uint32_t) tv.tv_sec;
		header.ts_msec = (uint32_t) tv.tv_usec;

		header.caplen = size;
		header.len = size;

		if (_file == NULL) throw;

		if (fwrite(&header, sizeof(char), sizeof(header), _file) != sizeof(header))
			goto err;

		if (fwrite(data, sizeof(char), size, _file) != size)
			goto err;

		return;
	err:
		perror("write_data()");
		exit(EXIT_FAILURE);

	}
};

/* TODO: point to a place where the frame format is documented */
bool parse_frame(list<uint16_t> & good_nodes, list<uint16_t> & bad_nodes,
				 char ** p, size_t & p_size) {
	int offset = 0;
	char * frame = * p;
	uint16_t node_id;
	uint16_t size_good, size_bad;

	if (frame[offset] != 0x02) /* outbound frame */
		return false; /* only parse outbound frame */
	++offset;

	node_id = char_to_uint16(frame, offset);
	offset += 2;

	good_nodes.push_back(node_id);

	size_good = char_to_uint16(frame, offset);
	offset += 2;

	if (size_good * sizeof(uint16_t) > (p_size - offset))
		return false;

	for(int i=0; i < size_good; ++i) {
		good_nodes.push_back(char_to_uint16(frame, offset));
		offset += 2;
	}

	size_bad = char_to_uint16(frame, offset);
	offset += 2;

	if (size_bad * sizeof(uint16_t) > (p_size - offset))
		return false;

	for(int i=0; i < size_bad; ++i) {
		bad_nodes.push_back(char_to_uint16(frame, offset));
		offset += 2;
	}

	/* skip the 8 bytes timestamp */
	offset += 8;
	if (p_size < offset)
		return false;

	p_size = p_size - offset;
	* p = &frame[offset];

	return true;
}

int main(int argc, char * argv[]) {
	string mcast_addr;
	int mcast_port;
	map< int, boost::shared_ptr<PcapDumper> > dumpers;

	/* set the signal handler in order to exit the program gracefully */
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = exit_signal;
	sigfillset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);

	try {
		po::options_description desc("Allowed options");
		desc.add_options()
				("help", "produce this help message")
				("maddr", po::value<string>(&mcast_addr), "multicast address to listen on")
				("mport", po::value<int> (&mcast_port), "multicast port to listen on")
				;

		po::variables_map vm;
		po::store(po::command_line_parser(argc, const_cast<char **>(argv)).
				  options(desc).run(), vm);
		po::notify(vm);

		if (vm.count("help")) {
			cout << "Usage: options_description [options]\n";
			cout << desc;
			return 0;
		}

		if (!vm.count("maddr")) {
			cerr << "maddr parameter is mandatory, use --help to see the full list of argument"
				 << endl;
			exit(EXIT_FAILURE);
		}

		if (!vm.count("mport")) {
			cerr << "mport parameter is mandatory, use --help to see the full list of arguments"
				 << endl;
		}

	}
	catch(std::exception& e)
	{
		cerr << e.what() << "\n";
		return 1;
	}


	try
	{
		list<uint16_t> good_nodes, bad_nodes;
		char * p = NULL;

		boost::asio::io_service io_service;

		cout << "starting listener on " << mcast_addr
			 << " port " << mcast_port << endl;

		ip::address multicast_address(ip::address::from_string(mcast_addr));
		udp::endpoint listen_endpoint;
		if (multicast_address.is_v6())
			listen_endpoint = udp::endpoint(udp::v6(), mcast_port);
		else
			listen_endpoint = udp::endpoint(udp::v4(), mcast_port);

		udp::socket socket(io_service);
		socket.open(listen_endpoint.protocol());
		socket.set_option(udp::socket::reuse_address(true));
		socket.set_option(ip::multicast::join_group(multicast_address));
		socket.bind(listen_endpoint);

		char packet[MAX_PACKET_SIZE];
		size_t packet_len;

		for (;;) {
			bool is_ok;
			udp::endpoint remote_endpoint;
			boost::system::error_code error;

			memset(packet, 0, MAX_PACKET_SIZE);
			packet_len = socket.receive_from(boost::asio::buffer(packet, MAX_PACKET_SIZE),
											 remote_endpoint, 0, error);
			if (capture_exit) {
				cout << "Exiting pcap-dumper gracefully" << endl;
				return 0;
			}

			if (error && error != boost::asio::error::message_size)
				throw boost::system::system_error(error);

			cout << "received a " << packet_len << " bytes long frame" << endl;

			good_nodes.clear(); bad_nodes.clear();

			p = packet; /* point at the beginning of the inner IEEE 802.15.4 frame */
			/* parse the header in order to find destination nodes for the frame */
			is_ok = parse_frame(good_nodes, bad_nodes, &p, packet_len);
			if (is_ok) {
				/* dump the frame for each good nodes */
				for(list<uint16_t>::const_iterator num_node = good_nodes.begin();
					num_node != good_nodes.end();
					++ num_node) {
					/* fill an map<int, PcapDumper> with this list */
					if (!dumpers.count(* num_node)) {
						stringstream filename;
						filename << FILE_PREFIX
								 << "-"
								 << * num_node
								 << ".pcapng";
						dumpers[* num_node] = boost::shared_ptr<PcapDumper>(new PcapDumper(filename.str()));
					}
					// dump the frame
					dumpers[* num_node]->write_data(p, packet_len);
				}

				/* flip the bits on the CRC so that we can find out the node
				 * received a bad packet */
				p[packet_len -2] ^= 0xff;
				p[packet_len -1] ^= 0xff;

				/* dump the frame for each bad nodes */
				for(list<uint16_t>::const_iterator num_node = bad_nodes.begin();
					num_node != bad_nodes.end();
					++ num_node) {
					/* fill an map<int, PcapDumper> with this list */
					if (!dumpers.count(* num_node)) {
						stringstream filename;
						filename << FILE_PREFIX
								 << "-"
								 << * num_node
								 << ".pcapng";
						dumpers[* num_node] = boost::shared_ptr<PcapDumper>(new PcapDumper(filename.str()));
					}
					// dump the frame
					dumpers[* num_node]->write_data(p, packet_len);
				}
			}
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
