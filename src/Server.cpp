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
		// throw (std::exception("Socket error"));
		// throw (errno); // verifier

	validity = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	if (validity == ERROR)
		throw (Server::ServException(ERRMSG"unable to free the socket"));
		// throw (std::exception("Unable to free the socket"));

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
						addClient(clientSock); // new way to handle client via Client class
						//////////////////////////

						std::cout << "Bonjour, " << inet_ntoa(_addr.sin_addr) << ":" << ntohs(_addr.sin_port) << std::endl;
						capOrNOt(clientSock);
						// doit renvoyer le CAP au client comme ceci 
						/* CAP LS
							NICK n1t4r4
							USER n1t4r4 n1t4r4 localhost :Verena Ferraro*/
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

std::string	Server::parsePing(std::string token, int clientSocket) {
	std::string ping = "PING";
	std::string pong = "PONG";
	// std::string	content;
	std::string parsePing = token.substr(token.find(ping));
	// std::cout << "parsePing : " << parsePing << std::endl;
	// send(clientSocket, parsePing.c_str(), parsePing.size(), 0);
	sendMsg(parsePing, clientSocket);
	return parsePing;
}

void Server::sendMsg(std::string message, int fd)
{
	std::cout << "RESPONSE IS <" << message << ">" << std::endl;
	send(fd, message.append(END_SEQUENCE).c_str(), message.size(), 0);
}

void	Server::inputClient(char *buf)
{
	if (buf[0] == '/')
	{
		std::cout << "Votre demande est une commande." << std::endl;
		cmdSelection(buf);
	}
	else
		std::cout << "Juste du texte." << std::endl;
}

bool	Server::addClient(int fd)
{
	//comment gerer une nouvelle connexion ? un nouveau client avec un nouvel fd ?
	Client client(fd);
	_clients.push_back(client);
	std::cout << "new client added : " << client.getFd() << std::endl; //DEBUG ONLY
	// int newFD = client.getFd();
	// capOrNOt(newFD);
	std::cout << "fd client APRES : " << client.getFd() << std::endl; //DEBUG ONLY
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VERENA
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string	Server::getPassword() {
	return _password;
} //@Verena to print the password entered

void	Server::setPassword(std::string pass)  {
	_password = pass;
}


// void	Server::capOrNOt() {
// 	std::string capabilities = "CAP LS done\n";
// 	for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++) {
// 		int dest = (*it).fd;
// 		if (dest != _socket) {
// 			if (send(dest, capabilities.c_str(), capabilities.size(), 0) == ERROR)
// 				std::cout << ERRMSG << strerror(errno) << std::endl;
// 		}
// 	getCap();
// 	}
// 	// std::cout << "Capabilities supported: " << Server::getCap() << std::endl; // envoyer la liste de commandes a imprimer (note de @Verena)
// }

 void	Server::capOrNOt(int clientSocket) {
	Client	cl;
	// std::string serverAddress = _addr.str_c();
	int serverPort = Server::getPort();
	std::string nickname = cl.getNick();
	std::string version = "2.0";
	// std::string clientNumber = cl.getFd();
	std::string channel = "#mychannel";

	//print clientNumber (socket)
	std::cout << "NEW CLIENT : " << clientSocket << std::endl;
	// // Création du socket
	// int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	// if (clientSocket == -1) {
	// 	std::cerr << "Erreur lors de la création du socket" << std::endl;
	// 	// return 1;
	// 	return;
	// }

	// Obtention de l'adresse IP du serveur
	char ipAddress[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(_addr.sin_addr), ipAddress, INET_ADDRSTRLEN);
	std::cout << "IP adresse server : " << ipAddress << std::endl;
	std::string serverAddress = ipAddress;

	// Préparation des informations de connexion du serveur
	// sockaddr_in serverAddr{};//_addr
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(serverPort);
	if (inet_pton(AF_INET, serverAddress.c_str(), &(_addr.sin_addr)) <= 0) {
		std::cerr << "Unvalid IP adresse server" << std::endl;
		// return 1;
		return;
	}

	// // Connexion au serveur
	if (connect(clientSocket, (struct sockaddr*)&_addr, sizeof(_addr)) < 0) {
		std::cerr <<  "other connexion detected"  << std::endl;
		// return 1;
		parsePing("PING\n", clientSocket);
		// std::string servCommand = "PONG " + nickname + ":" + nickname + "\r\n";
		// send(clientSocket, servCommand.c_str(), servCommand.size(), 0);
		// std::cout << "PONG : " << parsePing("PING\n", clientSocket) << std::endl;
		return;
	}

	std::cout << "coucou" << std::endl;
	// Envoi de la commande pour demander les capacités du serveur
	std::string capCommand = "CAP LS\r\n";
	send(clientSocket, capCommand.c_str(), capCommand.size(), 0);

	// Envoi des commandes d'authentification
	std::string authCommand = "NICK " + nickname + "\r\n";
	send(clientSocket, authCommand.c_str(), authCommand.size(), 0);

	std::string passCommand = "PASS " + _password + "\r\n";
	send(clientSocket, passCommand.c_str(), authCommand.size(), 0);

	std::string userCommand = "USER " + nickname + " 0 * :" + nickname + "\r\n";
	send(clientSocket, userCommand.c_str(), userCommand.size(), 0);

	send(clientSocket, (" 001 Verena Hi ! Welcome to this awesome IRC server !, Verena\r\n"), 100, 0);
	send(clientSocket, (" 002 Your host is 127.0.0.1 running version 4.20\r\n"), 100, 0);
	send(clientSocket, (" 003 This server was created Verena\r\n"), 100, 0);
	send(clientSocket, (" 004 Verena 127.0.0.1 4.20  none none.\r\n"), 100, 0); // 100 faire un strlen de la string
	// PASS

	parsePing("PING\n", clientSocket);
	std::string servCommand = "PONG " + nickname + ":" + nickname + "\r\n";
	send(clientSocket, servCommand.c_str(), servCommand.size(), 0);


	// Préparation des structures pour la fonction poll()
	struct pollfd fds[1];
	fds[0].fd = clientSocket;
	fds[0].events = POLLIN;

	// Réception et traitement des réponses du serveur
	char buffer[BUFFER_SIZE];
	std::string serverResponse = "CAP LS";
	while (true) {
	// std::cout << "ca bug la !" << std::endl;
		int result = poll(fds, 1, -1);
		if (result == -1) {
			std::cerr << "Erreur lors de l'appel à poll()" << std::endl;
			break;
		}

		if (result > 0) {
			if (fds[0].revents & POLLIN) {
				memset(buffer, 0, sizeof(buffer));
				int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
				if (bytesRead <= 0) {
					break;
				}

				serverResponse += buffer;

				// Vérification si la réponse complète a été reçue
				std::stringstream responseStream(serverResponse);
				std::string line;
				while (std::getline(responseStream, line)) {
					std::cout << "Server response: " << line << std::endl;
					inputClient(buffer);
					// // Vérification des capacités renvoyées
					// if (line.find("CAP") != std::string::npos && line.find("LS") != std::string::npos) {
					// 	//gerer ici les commandes !
					// 	getCap();
					// }
				}

				serverResponse.clear();
			}
			std::cout << "CLIENT SOCKET 1: " << clientSocket << std::endl;
		}
		std::cout << "CLIENT SOCKET 2: " << clientSocket << std::endl;
	}
	// Fermeture du socket
	close(clientSocket);
	std::cout << "CLIENT SOCKET 3: " << clientSocket << std::endl;

	// return 0;
}

std::vector<std::string> Server::getCap() {
	std::vector<std::string> capabilities;

	// Ajoutez le code nécessaire pour récupérer les capacités (CAP LS)
	capabilities.push_back("CAPABILITY1");
	capabilities.push_back("CAPABILITY2");
	capabilities.push_back("CAPABILITY3");
	// ...

	std::cout << "Capabilities supported:";
	for (std::vector<std::string>::const_iterator it = capabilities.begin(); it != capabilities.end(); ++it)
	{
		const std::string& capability = *it;
		std::cout << " " << capability;
	}
	std::cout << std::endl;

	return capabilities;
}
