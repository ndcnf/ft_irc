#include "Socket.hpp"

Socket::Socket()
{
	int	option = 1;

	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == ERROR)
		throw (std::exception());
		// throw (std::exception("Socket error"));
		// throw (errno); // verifier

	_validity = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	if (_validity == ERROR)
		throw (std::exception());
		// throw (std::exception("Unable to free the socket"));

	
}

Socket::Socket(Socket const &src)
{
	*this = src;
}

Socket	&Socket::operator=(Socket const &rhs)
{
	_socket = rhs._socket;
	_validity = rhs._validity;

	return (*this);
}

Socket::~Socket()
{
	close(_socket);
}

int	Socket::getSocket()
{
	return (_socket);
}

int	Socket::getValidity()
{
	return (_validity);
}

// bool	Socket::connect(const std::string &ipAddress, unsigned short port)
// {
// 	(void)ipAddress;
// 	(void)port;
// 	return (true);
// }
