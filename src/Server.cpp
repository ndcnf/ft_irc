#include "../inc/Server.hpp"
#include "../inc/Client.hpp"


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
	token = rhs.token;
	command = rhs.command;
	_socket = rhs._socket;
	_validity = rhs._validity;
	_port = rhs._port;
	_sockets = rhs._sockets;
	_addr = rhs._addr;
	_pfds = rhs._pfds;
	_clients = rhs._clients;
	// _quit = rhs._quit;
	_password = rhs._password;

	return (*this);
}

Server::~Server()
{
	delete currentClient;
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
		throw (Server::ServException(ERRMSG"socket stream"));

	validity = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	if (validity == ERROR)
		throw (Server::ServException(ERRMSG"unable to free the socket"));

	validity = fcntl(_socket, F_SETFL, O_NONBLOCK);
	if (_socket == ERROR)
		throw (Server::ServException(ERRMSG"socket"));

	_sockets.push_back(_socket); // optionnel, peut-etre que le vecteur des sockets sera inutile

	bzero(&_addr, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port);

	validity = bind(_socket, (struct sockaddr *)&_addr, sizeof(_addr));
	if (validity == ERROR)
		throw (Server::ServException(ERRMSG"binding failed")); // binding failed

	validity = listen(_socket, BACKLOG);
	if (validity == ERROR)
		throw (Server::ServException(ERRMSG "validity listen failed")); //listen failed

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

						// New client
						currentClient = addClient(clientSock); // new way to handle client via Client class
						std::cout << "Bonjour, " << inet_ntoa(_addr.sin_addr) << ":" << ntohs(_addr.sin_port) << std::endl;
						capOrNOt(currentClient); // CAP LS NICK USER
						// getPing(buf, currentClient->getFd()); // PING PONG
					}
					else
						std::cout << ERRMSG << strerror(errno) << std::endl;
				}
				else
				{
					// std::cout << "client " << _pfds[i].fd << " request your attention." << std::endl; // only for DEBUG
					bzero(&buf, sizeof(buf));
					int	bytesNbr = recv(_pfds[i].fd, buf, sizeof(buf), 0);
					for (std::vector<Client>::iterator	it = _clients.begin(); it != _clients.end(); it++)
					{
						if ((*it).getFd() == _pfds[i].fd)
							std::cout << "I'm the " << _pfds[i].fd << std::endl;
					}
					int	sender = _pfds[i].fd;
					//getPing(buf, currentClient);
					inputClient(buf, currentClient);

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
						continue;
						// for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++)
						// {
						// 	int	dest = (*it).fd;

						// 	if (dest != _socket && dest != sender)
						// 	{
						// 		// VERIFIER QUE CE N'EST PAS UNE COMMANDE MAIS DU TEXTE A ENVOYER
						// 		if (send(dest, buf, bytesNbr, 0) == ERROR)
						// 			std::cout << ERRMSG << strerror(errno) << (*it).fd << std::endl;
						// 		// else
						// 		// 	sendMsg(buf, dest);//debug ONLY !!! to erase
						// 	}
						// }
					}
				}
			}
		}
	}
	// getPing(buf, currentClient->getFd()); // PING PONG
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VERENA
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Server::parseNick(char* buf, Client* client) { // pas de USER mais mal parser, a voir
	std::string s_buf(buf);

	std::cout << "DEBUG PARSE NIKC " << buf << std::endl;
	std::size_t nickPos = s_buf.find("nick");
	if (nickPos != std::string::npos) {
		std::string nickToUp = "NICK";
		for (std::size_t i = 0; i < nickToUp.length(); i++) {
			nickToUp[i] = std::toupper(nickToUp[i]);
		}
		s_buf.replace(nickPos, nickToUp.length(), nickToUp);
	}
	nickPos = s_buf.find("NICK ");
	if (nickPos != std::string::npos) {
		std::size_t newlinePos = s_buf.find('\n', nickPos); // ou find('', nickPos) ?
		if (newlinePos != std::string::npos) {
			std::string nickname = s_buf.substr(nickPos + 5, newlinePos - nickPos - 5 - 1);
			if (nickname.empty()) {
				std::cout << "NICK " << client->getNick() << std::endl;
			}
			// std::string msg = "NICK " + nickname + END_SEQUENCE;
			// sendMsg(msg, client->getFd()); // fait buger ? ou pas ? @Verena
			std::cout << "NICKNAME: " << nickname << std::endl;
			client->setNick(nickname);
			// command.clear(); ??
		}
	}
}

void	Server::parseUser(char *buf, Client *client) { // debug only ou utile ?
	if (strstr(buf, "USER") != 0) {
		std::string str(buf);
		std::size_t colonPos = str.find(':');
		if (colonPos != std::string::npos) {
			std::string UserContent = str.substr(colonPos + 1);
			std::string	msg = "USER : " + UserContent + END_SEQUENCE;
			client->setUser(UserContent);
			std::cout << "USERCONTENT : " << UserContent << std::endl; // DEBUG ONLY
			std::cout << "USERNAME : " << client->getUser() << std::endl; // DEBUG ONLY
			// sendMsg(msg, client->getFd()); // DEBUG ONLY pas necessaire a priori (pas dans freenode)
		}
	}
}

void	Server::getPing(char *buf, Client *client) {
		// if (client->getIPAddress() == clientPingIP && client->getPort() == clientPingPort) { // solution ?
			if (static_cast<std::string>(buf).find("PING") == 0) {
			// Extraire le contenu du message PING (après le token "PING")
			std::string	str(buf);
			std::size_t colonPos = str.find(' ');
			if (colonPos != std::string::npos) {
				std::string pingContent = str.substr(colonPos + 1);
				// Construire la réponse PONG avec le même contenu que le message PING
				std::string pongResponse = "PONG " + pingContent + END_SEQUENCE;
				_lastPing = time(NULL);
				// Envoyer la réponse PONG au client
				sendMsg(pongResponse, client->getFd());
			}
	}
}

void Server::parseCommand(char* buf)
{
	std::string input(buf);
	size_t spacePos = input.find(' ');

	// Trouver le retour à la ligne
	size_t newlinePos = input.find('\n');

	if (spacePos != std::string::npos && newlinePos != std::string::npos && spacePos < newlinePos)
	{
		token = input.substr(0, spacePos);
		command = input.substr(spacePos + 1, newlinePos - spacePos - 1);
	}
	else
	{
		token = input;
		command.clear();
	}

	// Affichage des résultats
	std::cout << "Token: " << token << std::endl;
	std::cout << "Command: " << command << std::endl;
}


void	Server::inputClient(char *buf, Client *client) // retourner une veleur ? un string ? return buff
{
	std::cout << "buf iCAv: " << buf << std::endl;

	if (static_cast<std::string>(buf).find("CAP LS") == 0) { // a sortir de la boucle ???
		parseNick(buf, client);
		parseUser(buf, client);
		first_message(client);
	}
	else if (static_cast<std::string>(buf).find("PING") == 0)
	{
		std::cout << "ping dans input client" << std::endl;
		getPing(buf, client);
	}
	else if (buf != 0)
	{
		parseCommand(buf);
		commands(token, client);
	}
}

Client* Server::addClient(int fd)
{
	Client* client = new Client(fd); // Allouer dynamiquement un nouvel objet Client
	_clients.push_back(*client);
	return client;
}

std::string	Server::getPassword() {
	return _password;
} //@Verena to print the password entered

void	Server::setPassword(std::string pass)  {
	_password = pass;
}

 void	Server::capOrNOt(Client *client) {
	std::string version = "2.0";
	std::string channel = "#mychannel";

	//print clientNumber (socket)
	std::cout << "NEW CLIENT : " << client->getFd() << std::endl; // check and debug only

	// // Connexion au serveur
	if (connect(client->getFd(), (struct sockaddr*)&_addr, sizeof(_addr)) < 0) {
		std::cerr <<  "other connexion detected"  << std::endl;
		sendMsg("PING", client->getFd()); // a voir si utile et necessaire
		return;
	}
}

