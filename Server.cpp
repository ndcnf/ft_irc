#include "Server.hpp"

Server::Server():	_socket(0),
					_validity(0),
					_port(0)
					// _quit(false)
{

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
	_addr = rhs._addr;
	_pfds = rhs._pfds;
	_clients = rhs._clients;
	// _quit = rhs._quit;

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

	_sockets.push_back(_socket); // optionnel, peut-etre que le vecteur des sockets sera inutile

	bzero(&_addr, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port);

	validity = bind(_socket, (struct sockaddr *)&_addr, sizeof(_addr));
	if (validity == ERROR)
		throw (std::exception()); // binding failed

	validity = listen(_socket, BACKLOG);
	if (validity == ERROR)
		throw (std::exception()); //listen failed

	return (true);
}

void	Server::allSockets()
{
	if (_sockets.empty())
	{
		std::cout << "No live socket at the moment." << std::endl; // Change this message with errno
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

bool	Server::connection()
{
	pollfd		pfd;
	int			clientSock;
	int			pollCounter;
	char		buf[250];
	socklen_t	addrlen;

	bzero(&pfd, sizeof(pfd));

	/* *********************************************************
	 * The first fd will be the one from the server.
	 * The subsequent ones, will be from the client(s).
	 * ********************************************************* */
	pfd.fd = _socket;
	pfd.events = POLLIN;
	_pfds.push_back(pfd);

	while (true)
	{
		pollCounter = poll(_pfds.data(), _pfds.size(), TIMEOUT_NO_P);

		if (pollCounter == ERROR)
			std::cout << ERRMSG << strerror(errno) << std::endl;

		for (unsigned int i = 0; i < _pfds.size(); i++)
		{
			if (_pfds[i].revents & POLLIN)
			{
				if (_pfds[i].fd == _socket)
				{
					addrlen = sizeof(_addr);
					clientSock = accept(_socket, (struct sockaddr *)&_addr, &addrlen);

					if (clientSock != ERROR)
					{
						pfd.fd = clientSock;
						pfd.events = POLLIN;
						_pfds.push_back(pfd);

						std::cout << "Bonjour, " << inet_ntoa(_addr.sin_addr) << ":" << ntohs(_addr.sin_port) << std::endl;
					}
					else
						std::cout << ERRMSG << strerror(errno) << std::endl;
				}
				else
				{
					// std::cout << "client " << _pfds[i].fd << " request your attention." << std::endl; // only for DEBUG
					int	bytesNbr = recv(_pfds[i].fd, buf, sizeof(buf), 0);
					int	sender = _pfds[i].fd;
					inputClient(buf);

					if (bytesNbr <= 0)
					{
						if (bytesNbr == 0)
							std::cout << "socket " << sender << " is gone." << std::endl; //
						else
							std::cout << ERRMSG << strerror(errno) << std::endl;

						close(_pfds[i].fd);
						_pfds.erase(_pfds.begin() + i);
						i--;
					}
					else
					{
						for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++)
						{
							int	dest = (*it).fd;

							if (dest != _socket && dest != sender)
							{
								if (send(dest, buf, bytesNbr, 0) == ERROR)
									std::cout << ERRMSG << strerror(errno) << (*it).fd << std::endl;
							}
						}
					}
				}
			}
		}
	}
}

// void	Server::cmdSelection()
// {
// 	enum Color { red, green, blue };
// 	Color r = red;

// 	switch(r)
// 	{
// 		case red  : std::cout << "red\n";   break;
// 		case green: std::cout << "green\n"; break;
// 		case blue : std::cout << "blue\n";  break;
// 	}

// 	enum commands

// }

void	Server::inputClient(char *buf)
{
	if (buf[0] == '\\')
		std::cout << "Votre demande est une commande." << std::endl;
	else
		std::cout << "Juste du texte." << std::endl;
}
