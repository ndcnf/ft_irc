#include "Server.hpp"

Server::Server()
{
	struct sockaddr_in	addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);
}

Server::Server(Server const &src)
{
	*this = src;
}

Server	&Server::operator=(Server const &rhs)
{
	_port = rhs._port;
	_sockets = rhs._sockets;

	return (*this);
}

Server::~Server()
{

}

void	Server::setPort(int port)
{
	_port = port;
}

int		Server::getPort()
{
	return (_port);
}

void	Server::allSockets()
{
	if (_sockets.empty())
	{
		std::cout << "No live socket at the moment." << std::endl;
		return;
	}

	for (std::vector<Socket>::iterator	it = _sockets.begin(); it != _sockets.end(); it++)
		std::cout << (*it).getSocket() << std::endl;
}

