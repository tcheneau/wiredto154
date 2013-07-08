#include "server.hpp"

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>


Server::Server(boost::asio::io_service& io_service, int port)
	: socket_(io_service, udp::endpoint(udp::v4(), port)) {
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
		std::size_t /*bytes_transferred*/) {
	std::cout << socket_.local_endpoint().port() << std::endl;

	if (error)
		throw boost::system::system_error(error);

	boost::shared_ptr<std::string> message;

	socket_.async_send_to(boost::asio::buffer(recv_buffer_), remote_endpoint_,
			boost::bind(&Server::handle_send, this, message,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));

	start_receive(); /* make oneself ready for the next packet */
}

/*
int main()
{
	try
	{
		server_list my_servers;
		boost::asio::io_service io_service;
		for (int i=PORT_FIRST; i <= PORT_LAST; ++i)
			my_servers.push_back(Server_ptr(new Server(io_service, i)));
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
*/
