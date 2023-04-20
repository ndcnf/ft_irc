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
	int					option = 1;
	int					validity;
	struct sockaddr_in	addr;

	// _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == ERROR)
		throw (std::exception());
		// throw (std::exception("Socket error"));
		// throw (errno); // verifier

	validity = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	if (validity == ERROR)
		throw (std::exception());
		// throw (std::exception("Unable to free the socket"));

	validity = fcntl(_socket, F_SETFL, O_NONBLOCK);
	if (_socket == ERROR)
		throw (std::exception());

	_sockets.push_back(_socket);

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);

	validity = bind(_socket, (struct sockaddr *)&addr, sizeof(addr));
	if (validity == ERROR)
		throw (std::exception()); // binding failed

	validity = listen(_socket, BACKLOG);
	if (validity == ERROR)
		throw (std::exception()); //listen failed

	return (true);
}


bool	Server::pollConnection()
{
	bzero(&_addr, sizeof(_addr));
	pollfd				srvFds;
	int					cliSocket;
	unsigned int		len;

	char				buf[250];
	int					ret;
	char				basic[] = "je te vois";

	while (true)
	{
		// // struct sockaddr_in	addr;
		srvFds.fd = _socket;
		srvFds.events = POLLIN;

		std::cout << "Hello, I'm your server. How may I help you?" << std::endl;
		_validity = poll(&srvFds, 1, TIMEOUT_NO); // just 1 ?
		if (_validity == ERROR || _validity == 0)
			throw (std::exception()); // poll failed

		std::cout << "inbetween poll and accept. _validity = " << _validity << std::endl;

		len = sizeof(_addr);
		cliSocket = accept(_socket, (struct sockaddr *)&_addr, &len);
		std::cout << "cliSocket: " << cliSocket << std::endl;
		// if (cliSocket > 0)
		if (cliSocket != ERROR)
		{
			std::cout << "Welcome, " << inet_ntoa(_addr.sin_addr) << std::endl;
			ret = recv(cliSocket, &buf, sizeof(buf), 0);
			write(1, buf, ret);
			send(cliSocket, basic, sizeof(basic), 0);

		}
		else
			throw (std::exception()); // accept failed

	}




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
	
	std::cout << "Socket(s): | ";
	for (std::vector<int>::iterator	it = _sockets.begin(); it != _sockets.end(); it++)
		std::cout << (*it) << " | ";
	std::cout << std::endl;
	/////////////////////////////////////////////////////////////////////////////////////
}

int	Server::getSocket()
{
	return (_socket);
}

// int	Server::getValidity()
// {
// 	return (_validity);
// }
