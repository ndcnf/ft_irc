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
	// struct sockaddr_in	addr;

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


bool	Server::pollConnection()
{
	// bzero(&_addr, sizeof(_addr));

	pollfd				pfd;
	int					clientSock;
	int					pollCounter;
	// unsigned int		j = 0;

	// bool				isServer = true;

	// unsigned int		len;

	char				buf[250];
	socklen_t			addrlen;
	// int					ret = 0;
	// char				msg[] = "Je te vois...";
	// char				basic[] = "je te vois";

	// newCliFds.fd = _socket;
	// newCliFds.events = POLLIN;

	// cliFds.push_back(newCliFds);

	bzero(&pfd, sizeof(pfd));
	/* *********************************************************
	 * The first fd will be the one from the server.
	 * The subsequent ones, will be from the client(s).
	 * ********************************************************* */
	// if (isServer)
	// {
		pfd.fd = _socket;
		pfd.events = POLLIN;
		_pfds.push_back(pfd);
		// isServer = false;
	// }

	while (true)
	// for (int i = 0; i < 10; i++) // DEBUG ONLY, pour eviter une boucle infini inarretable
	{
		// // struct sockaddr_in	addr;
		// srvFds.fd = _socket;
		// srvFds.events = POLLIN;

		std::cout << "Hello, I'm your server. Have you some time for a poll?" << std::endl;

		pollCounter = poll(&pfd, static_cast<nfds_t>(_pfds.size()), TIMEOUT_NO_P);
		std::cout << "Waiting for a client..." << std::endl;

		// pollCounter = poll(_pfds.data(), static_cast<nfds_t>(_pfds.size()), TIMEOUT_NO_P);
		if (pollCounter == ERROR)
		{
			// throw (std::exception()); // poll failed
			std::cout << "poll error: " << strerror(errno) << std::endl;
			exit (ERROR);
		}


		// for(unsigned long i = 0; i < _pfds.size(); i++)

		/* *********************************************************
		* The first entry in the vector is the server.
		* The subsequent ones, are from the client(s), so we skip
		* to the first client (begin + 1)
		* ********************************************************* */
		// for(std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++)

		// std::cout << _pfds.size() << std::endl;
		for (unsigned int j = 0; j < _pfds.size(); j++)
		// while (j <= _pfds.size())
		// for(std::map<int, sockaddr_in>::iterator it = _clients.begin() + 1; it != _clients.end(); it++)
		{
			// std::cout << "i: " << i << std::endl;
			// // std::cout << "revents = " << (*it).revents << std::endl;
			// std::cout << "vector fd : " << _pfds[j].fd << std::endl;
			// std::cout << "events ? " << _pfds[j].events << std::endl;

			// _pfds[i].revents = pfd.revents;
			// (*it).revents = pfd.revents;
			// _pfds[0].revents = &pfd.revents;

			// _pfds[j].revents = pfd.revents;
			// if (_pfds[j].revents == 0)
			// {
			// 	j++;
			// 	continue ;
			// }

			if (_pfds[j].revents & POLLIN)
			// if (pfd.revents & POLLIN)
			{
				// if ((*it).fd == clientSock)
				if (_pfds[j].fd == _socket)
				{
					// struct sockaddr_in	addr;
					// bzero(&_addr, sizeof(_addr));
					// bzero(&_addr, sizeof(_addr));
					// _addr.sin_family = AF_INET;
					// addr.sin_addr.s_addr = INADDR_ANY;
					// _addr.sin_port = htons(_port);


					addrlen = sizeof(_addr);
					clientSock = accept(_socket, (sockaddr*)(&_addr), &addrlen);
					if (clientSock != ERROR)
					{
						// _addr.sin_family = AF_INET;
						// // addr.sin_addr.s_addr = INADDR_ANY;
						// _addr.sin_port = htons(_port);
						// std::cout << _port << std::endl;
						_clients[clientSock] = _addr;
						std::cout << "T'es un serveur, non ? Bonjour, " << inet_ntoa(_addr.sin_addr) << ":" << ntohs(_addr.sin_port) << std::endl;
						pfd.fd = clientSock;
						pfd.events = POLLIN; //uniquement POLLIN ?
						_pfds.push_back(pfd);
					}
					else
						std::cout << "accept error: " << strerror(errno) << std::endl;



					std::cout << "size of _pfds vector = " << _pfds.size() << std::endl;
					std::cout << "Un nouveau client ! Bonjour, " << inet_ntoa(_addr.sin_addr) << std::endl;
				}
				else
				{
					std::cout << "Une requete client !" << std::endl;
					int	bytesNbr = recv(pfd.fd, buf, sizeof(buf), 0);
					int	sender = pfd.fd;
					if (bytesNbr <= 0)
					{
						if (bytesNbr == 0)
							// Connection closed
							std::cout << "bye bye from " << sender << std::endl;
						else
						{
							std::cout << "Erreur recv()" << strerror(errno) << std::endl;
						}
						close(_pfds[j].fd);
						// _pfds.erase(_pfds[j].fd);
					}
					else
					{
						for (unsigned int i = 0; i < _pfds.size(); i++)
						{
							int	dest = pfd.fd;

							if (dest != _socket && dest != sender)
							{
								if (send(dest, buf, bytesNbr, 0) == ERROR)
									std::cout << "erreur de send()" << std::endl;
							}
						}
					}

				}

			}
			else
				std::cout << "Erreur: Pas pollin" << std::endl;
			// j++;
		}





		// struct sockaddr_in	addr;
		// bzero(&_addr, sizeof(_addr));

		// socklen_t			addrlen;

		// addrlen = sizeof(addr);
		// clientSock = accept(_socket, (sockaddr*)(&addr), &addrlen);
		// if (clientSock != ERROR)
		// {
		// 	// addr.sin_family = AF_INET;
		// 	// addr.sin_addr.s_addr = INADDR_ANY;
		// 	// addr.sin_port = htons(_port);
		// 	std::cout << _port << std::endl;
		// 	_clients[clientSock] = addr;
		// 	std::cout << "Un nouveau client ! Bonjour, " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << std::endl;
		// 	pfd.fd = clientSock;
		// 	pfd.events = POLLIN | POLLOUT; //uniquement POLLIN ?
		// 	_pfds.push_back(pfd);
		// }
		// else
		// 	std::cout << "accept error: " << strerror(errno) << std::endl;



		// std::cout << "size of _pfds vector = " << _pfds.size() << std::endl;

		// if (!_clients.empty())
		// {
		// // for(unsigned long i = 0; i < _pfds.size(); i++)

		// /* *********************************************************
		// * The first entry in the vector is the server.
		// * The subsequent ones, are from the client(s), so we skip
		// * to the first client (begin + 1)
		// * ********************************************************* */
		// for(std::vector<pollfd>::iterator it = _pfds.begin() + 1; it != _pfds.end(); it++)
		// // for(std::map<int, sockaddr_in>::iterator it = _clients.begin() + 1; it != _clients.end(); it++)
		// {
		// 	std::cout << "revents = " << (*it).revents << std::endl;
		// 	std::cout << "vector fd : " << (*it).fd << std::endl;

		// 	if ((*it).revents & POLLIN)
		// 	{
		// 		// if ((*it).fd == clientSock)
		// 		if ((*it).fd == _socket)
		// 			std::cout << "Un nouveau client ! Bonjour, " << inet_ntoa(addr.sin_addr) << std::endl;
		// 		else
		// 			std::cout << "Une requete client !" << std::endl;

		// 	}
		// 	else
		// 		std::cout << "Erreur: Pas pollin" << std::endl;
		// }



			//////////////////////////////////////
			// if ((*it).revents == 0)
			// 	continue;
			// if ((*it).revents != POLLIN)
			// {
			// 	std::cout << "error: " << strerror(errno) << std::endl;
			// 	_quit = true;
			// 	break;
			// }
			// if ((*it).fd == _socket)
			// {
			// 	std::cout << "Un nouveau client ! Bonjour, " << inet_ntoa(_addr.sin_addr) << std::endl;
			// }
			// else
			// {
			// 	do
			// 	{
			// 		std::cout << "Une requete client !" << std::endl;
			// 		clientSock = accept(_socket, NULL, NULL);
			// 		if (clientSock < 0)
			// 		{
			// 			if (errno != EWOULDBLOCK)
			// 			{
			// 				perror("  accept() failed");
			// 				_quit = true;
			// 			}
			// 			std::cout << "clientSock loop !" << std::endl;
			// 			break;
			// 		}
			// 	} while (clientSock != ERROR);
			// }














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



		// }


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

bool	Server::connection()
{
	int		fd_count = 1;

	pollfd	pfd;			// for vector _pfds
	pollfd	pfds[5];		// temp, should be replaced by vector and other pollfd
	int		clientSock;
	int		pollCounter;
	char	buf[250];
	socklen_t	addrlen;
	bool	disconnected;

	bzero(&pfds, sizeof(pfds));

	pfds[0].fd = _socket;	//temp
	pfds[0].events = POLLIN;	//temp

	pfd.fd = _socket;
	pfd.events = POLLIN;
	_pfds.push_back(pfd);

	while (true)
	{
		// pollCounter = poll(pfds, fd_count, TIMEOUT_NO_P);	//temp

		// std::cout << "_pfds.data(): " << _pfds.data() << std::endl;						//DEBUG
		// std::cout << "_pfds.front(): " << &_pfds.front() << std::endl;					//DEBUG
		// std::cout << "pfds: " << pfds << std::endl;										//DEBUG
		fd_count = _pfds.size(); //DEBUG, pour eviter de tout remplacer sans etre sure

		pollCounter = poll(_pfds.data(), fd_count, TIMEOUT_NO_P);	//to think about again

		if (pollCounter == ERROR)
			std::cout << "erreur de poll() : " << strerror(errno) << std::endl;

		// for (int i = 0; i < fd_count; i++)
		// for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++)
		disconnected = false;
		for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++)
		{
			// if (pfds[i].revents & POLLIN)
			if ((*it).revents & POLLIN)
			{
				// if (pfds[i].fd == _socket)
				if ((*it).fd == _socket)
				{
					addrlen = sizeof(_addr);
					clientSock = accept(_socket, (struct sockaddr *)&_addr, &addrlen);

					if (clientSock != ERROR)
					{
						fd_count = _pfds.size();
						// pfds[fd_count].fd = clientSock;
						// pfds[fd_count].events = POLLIN;
						// fd_count++;

						pfd.fd = clientSock;
						pfd.events = POLLIN;
						_pfds.push_back(pfd);

						// std::cout << "Adresse : " << inet_ntop(_addr.sin_family, (void*)&(_addr.sin_addr), buf, addrlen) << ":" << ntohs(_addr.sin_port) << std::endl;
						std::cout << "Bonjour, " << inet_ntoa(_addr.sin_addr) << ":" << ntohs(_addr.sin_port) << std::endl;
					}
					else
						std::cout << "erreur d'accept() : " << strerror(errno) << std::endl;
				}
				else
				{
					// std::cout << "client " << pfds[i].fd << " request your attention." << std::endl;
					std::cout << "client " << (*it).fd << " request your attention." << std::endl;
					// int	bytesNbr = recv(pfds[i].fd, buf, sizeof(buf), 0);
					int	bytesNbr = recv((*it).fd, buf, sizeof(buf), 0);
					// int	sender = pfds[i].fd;
					int	sender = (*it).fd;

					if (bytesNbr <= 0)
					{
						if (bytesNbr == 0)
							std::cout << "socket " << sender << " is gone." << std::endl;
						else
							std::cout << "erreur de recv() : " << strerror(errno) << std::endl;

						// close(pfds[i].fd);
						close((*it).fd);
						// pfds[i] = pfds[fd_count - 1]; // This line seems problematic
						_pfds.erase(it);
						// fd_count--;
					}
					else
					{
						// for (int j = 0; j < fd_count; j++)
						for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++)
						{
							// int	dest = pfds[j].fd;
							int	dest = (*it).fd;

							if (dest != _socket && dest != sender)
							{
								if (send(dest, buf, bytesNbr, 0) == ERROR)
									// std::cout << "erreur de send() : " << strerror(errno) << j << std::endl;
									std::cout << "erreur de send() : " << strerror(errno) << (*it).fd << std::endl;
							}
						}
					}
				}
			}
			// it++;
			// else
			// {
			// 	disconnected = true;
			// }

			// if (disconnected)
			// {
			// 	it = _pfds.erase(it - 1);
			// 	disconnected = false;
			// }
		}
	}
}
