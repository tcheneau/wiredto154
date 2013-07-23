#include "server.hpp"

#include "dispatcher.hpp"

#include <cassert>
#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>

Server::Server(boost::asio::io_service& io_service, int port)
	: socket_(io_service, udp::endpoint(udp::v4(), port)),
	  recv_buffer_(128) {
	start_receive();
}

void Server::start_receive() {
	socket_.async_receive_from(
			boost::asio::buffer(recv_buffer_), remote_endpoint_,
			boost::bind(&Server::handle_receive, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
}

void Server::handle_send(boost::shared_ptr<std::string> /*message*/,
		const boost::system::error_code& /*error*/,
		std::size_t /*bytes_transferred*/) {
	std::cout << "just sent a message to " << remote_endpoint_ << std::endl;
}

void Server::handle_receive(const boost::system::error_code& error,
		std::size_t read_size/*bytes_transferred*/) {
#ifdef DEBUG
	std::cout << "received a message on port "
			  << socket_.local_endpoint().port()
			  << " (" << read_size << " bytes)"
			  << std::endl;
#endif /* DEBUG */
	recv_buffer_.resize(read_size);

	if (error)
		throw boost::system::system_error(error);
	// parse the frame and determine what to do with it
	Dispatcher::dispatch(socket_.local_endpoint().port(), recv_buffer_, socket_);

	start_receive(); /* make oneself ready for the next packet */
}



