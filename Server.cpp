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
	_socket = rhs._socket;
	_validity = rhs._validity;
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

bool	Server::createSocket()
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

	_sockets.push_back(_socket);


	


	std::cout << "End of socket: " << _socket << std::endl;




	// if (!sock.empty())
	// {
		// for (std::vector<Socket>::iterator	it = _sockets.begin(); it != _sockets.end(); it++)
		// {
		// }
		// _sockets.push_back(sock);
		// _sockets.insert(_socket, _sockets.back());

	// }
	return (true);
}

void	Server::allSockets()
{
	if (_sockets.empty())
	{
		std::cout << "No live socket at the moment." << std::endl;
		return;
	}

	// for (std::vector<Socket>::iterator	it = _sockets.begin(); it != _sockets.end(); it++)
	// 	std::cout << (*it).getSocket() << std::endl;
	std::cout << "Collection of sockets: " << _sockets.size() << std::endl;

	for (std::vector<int>::iterator	it = _sockets.begin(); it != _sockets.end(); it++)
		std::cout << (*it) << std::endl;
}

int	Server::getSocket()
{
	return (_socket);
}

int	Server::getValidity()
{
	return (_validity);
}
