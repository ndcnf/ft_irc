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
	// pollfd				newCliFds;
	// int					cliSocket;
	// unsigned int		len;

	// char				buf[250];
	// int					ret = 0;
	// char				msg[] = "Je te vois...";
	// char				basic[] = "je te vois";

	int					pollCounter;

	// newCliFds.fd = _socket;
	// newCliFds.events = POLLIN;cc

	// cliFds.push_back(newCliFds);
	srvFds.fd = _socket;
	srvFds.events = POLLIN;

	while (true)
	{
		// // struct sockaddr_in	addr;
		// srvFds.fd = _socket;
		// srvFds.events = POLLIN;

		std::cout << "Hello, I'm your server. Have you some time for a poll?" << std::endl;
		pollCounter = poll(&srvFds, 1, TIMEOUT_NO_P); // just 1 ? plutot remplacer par le nombre de clients online ?
		if (pollCounter == ERROR)
		{
			// throw (std::exception()); // poll failed
			std::cout << "poll error: " << strerror(errno) << std::endl;
			exit (ERROR);
		}

		std::cout << "inbetween poll and accept. _validity = " << pollCounter << std::endl;


		for(unsigned long i = 0; i < cliFds.size(); i++)
		{
		// 	// len = sizeof(_addr);
		// 	// cliSocket = accept(_socket, (struct sockaddr *)&_addr, &len);
		// 	// std::cout << "cliSocket: " << cliSocket << std::endl;
		// 	if ((int)i != srvFds.fd)
		// 	// if (cliSocket != ERROR)
		// 	{
		// 		// std::cout << "Welcome, " << inet_ntoa(_addr.sin_addr) << std::endl;
		// 		ret = recv(i, &buf, sizeof(buf), 0);
		// 		// ret = recv(cliSocket, buf, sizeof(buf), 0);

		// 		std::cout << "RET="<< ret << std::endl;

		// 		write(1, buf, ret);
		// 		send(i, basic, sizeof(basic), 0);
		// 		// send(cliSocket, msg, sizeof(msg), 0);

		// 	}
		// 	else
		// 	{
		// 		len = sizeof(_addr);
		// 		cliSocket = accept(_socket, (struct sockaddr *)&_addr, &len);
		// 		std::cout << "cliSocket: " << cliSocket << std::endl;
		// 		std::cout << "Welcome, " << inet_ntoa(_addr.sin_addr) << std::endl;
		// 		newCliFds.fd = cliSocket;
		// 		newCliFds.events = POLLIN;

		// 		cliFds.push_back(newCliFds);
		// 	}
		// 		// throw (std::exception()); // accept failed
		}


	}




	return (true);
}


// bool	Server::selectConnection()
// {
// 	fd_set	setRd;
// 	fd_set	setWrt;
// 	fd_set	setErr;
// 	int		result;

// 	FD_ZERO(&set);
// 	FD_SET(_socket, &set);

// 	readyOK = select(_socket + 1, &set, )




// 	TIMEOUT_NO_S



// 	return (true);
// }


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
