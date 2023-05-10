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
					bzero(&buf, sizeof(buf));
					int	bytesNbr = recv(_pfds[i].fd, buf, sizeof(buf), 0);
					int	sender = _pfds[i].fd;
					inputClient(buf);

					if (bytesNbr <= 0)
					{
						if (bytesNbr == 0)
						{
							std::cout << "socket " << sender << " is gone." << std::endl;
							// Quand il n'y a plus de client, juste le serveur
							// if (_pfds.size() == 2)
								// std::cout << _pfds.size() << " * cricket noises *" << std::endl;
						}
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
								// VERIFIER QUE CE N'EST PAS UNE COMMANDE MAIS DU TEXTE A ENVOYER
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

void	Server::cmdSelection(char *buf)
{
	std::string	str(buf);
	std::string	token;
	std::string	content;

	if (str.size() < 2)
	{
		std::cout << "ERREUR, pas de commande donnee" << std::endl; // NEEDS IMPROVEMENT
		return ;
	}

	// enum commands
	// {
	// 	JOIN,
	// 	PASS,
	// 	NICK,
	// 	USER,
	// 	// OPER,
	// 	// QUIT,
	// 	PART,
	// 	PRIVMSG,
	// 	NOTICE,

	// 	//commandes operators listes dans le sujet
	// 	KICK,
	// 	INVITE,
	// 	TOPIC,
	// 	MODE,

	// 	//commandes donnees par Nicole
	// 	PONG,
	// 	WHO,
	// 	NUM_COMMANDS
	// };

	token = str.substr(1, str.find(' ') - 1);
	for (unsigned int i = 0; i < token.size(); i++)
		token[i] = toupper(token[i]);

	std::cout << "TOKEN : [" << token << "]" << std::endl; // DEBUG ONLY

	if (str.size() == (token.size() + 1) && (str.size() < token.size() + 2))
	{
		std::cout << token.size() << " | ";
		std::cout << "ERREUR Your command is empty." << std::endl; // ERROR
		return ;
	}

	content = str.substr(token.size() + 2);
	std::cout << "CONTENT : [" << content << "]" << std::endl; // DEBUG ONLY

	//FORET de IF
	if (token == "JOIN")
		std::cout << "join us on : " << content << std::endl;
	else if (token == "NICK")
		std::cout << "nickname : " << std::endl;
	else if (token == "USER")
		std::cout << "user name : " << std::endl;
	else if (token == "PART")
		std::cout << "part + arg ? : " << std::endl;
	else if (token == "PRIVMSG")
		std::cout << "private msg : " << std::endl;
	else if (token == "NOTICE")
		std::cout << "notice (private msg also) : " << std::endl;
	else if (token == "KICK")
		std::cout << "kick : " << std::endl;
	else if (token == "INVITE")
		std::cout << "invite : " << std::endl;
	else if (token == "TOPIC")
		std::cout << "topic : " << std::endl;
	else if (token == "MODE")
		std::cout << "mode (+ i, t, k, o or l) : " << std::endl;
	else if (token == "PONG")
		std::cout << "pong ? : " << std::endl;
	else
		std::cout << "I don't understand this command." << std::endl;

	// commands	cmd;
	// cmd = JOIN;

	// for (int i = 0; i < NUM_COMMANDS; i++)
	// {
	// 	if (token == "JOIN")
	// 		std::cout << "join us..."<< std::endl;
	// 	std::cout << "fooooor"<< std::endl;
	// }

	// std::cout << cmd << std::endl;
	// switch(cmd)
	// {
	// 	case join	: std::cout << "join us..."<< std::endl;	break;
	// 	case nick	: std::cout << "nice nickname..." << std::endl;	break;
	// 	default		: std::cout << "I don't understand you." << std::endl;	break;
	// }

}

void	Server::inputClient(char *buf)
{
	if (buf[0] == '\\')
	{
		std::cout << "Votre demande est une commande." << std::endl;
		cmdSelection(buf);
	}
	else
		std::cout << "Juste du texte." << std::endl;
}
