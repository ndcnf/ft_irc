#include "Server.hpp"

Server::Server()
{
	// struct sockaddr_in	addr;

	// addr.sin_family = AF_INET;
	// addr.sin_addr.s_addr = INADDR_ANY;
	// addr.sin_port = htons(_port);
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
	struct sockaddr_in	addr;

	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// _socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == ERROR)
		throw (std::exception());
		// throw (std::exception("Socket error"));
		// throw (errno); // verifier

	_validity = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	if (_validity == ERROR)
		throw (std::exception());
		// throw (std::exception("Unable to free the socket"));

	_validity = fcntl(_socket, F_SETFL, O_NONBLOCK);
	if (_socket == ERROR)
		throw (std::exception());

	_sockets.push_back(_socket);

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);

	_validity = bind(_socket, (struct sockaddr *)&addr, sizeof(addr));
	if (_validity == ERROR)
		throw (std::exception());

	_validity = listen(_socket, BACKLOG);
	if (_validity == ERROR)
		throw (std::exception());

	std::cout << "End of socket: " << _socket << std::endl; // DEBUG ONLY

	return (true);
}

void	Server::allSockets()
{
	if (_sockets.empty())
	{
		std::cout << "No live socket at the moment." << std::endl;
		return;
	}

	// DEBUG ONLY ///////////////////////////////////////////////////////////////////////
	std::cout << "Number of sockets: " << _sockets.size() << std::endl;

	for (std::vector<int>::iterator	it = _sockets.begin(); it != _sockets.end(); it++)
		std::cout << (*it) << std::endl;
	/////////////////////////////////////////////////////////////////////////////////////
}

int	Server::getSocket()
{
	return (_socket);
}

int	Server::getValidity()
{
	return (_validity);
}
