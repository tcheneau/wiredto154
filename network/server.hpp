#ifndef WIREDTO154_NETWORK_SERVER_H
#define WIREDTO154_NETWORK_SERVER_H

// The Server class defines a UDP server that listen a specific port for
// incoming packet from a single host. This host is then mapped to a
// node within the simulation.
// For each packet sent to this server, the simulation engines will compute
// the list of (simulated) nodes that should receive the message.


// The following class is heavily inspired from the Boost examples.
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)

#include "frame.hpp"

#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class Server
{
	private:
			static const int IEEE802154_MAX_FRAME_SIZE = 127;
			static const int UDP_MAX_FRAME_SIZE = 65535;
	public:
		typedef boost::shared_ptr<Server> server_ptr;
		typedef std::list<server_ptr> server_list;
		Server(boost::asio::io_service& io_service, int port);

	private:
		void start_receive();

		void handle_receive(const boost::system::error_code& error,
							std::size_t /*bytes_transferred*/);

		void handle_send(boost::shared_ptr<std::string> /*message*/,
						 const boost::system::error_code& /*error*/,
						 std::size_t /*bytes_transferred*/);


		udp::socket socket_;
		udp::endpoint remote_endpoint_;
		Frame::frame recv_buffer_;
};

#endif /* WIREDTO154_NETWORK_SERVER_H */
