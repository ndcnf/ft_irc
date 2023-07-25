#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/Messages.hpp"
#include "../inc/Channel.hpp"

void	Server::commands(std::string cmd, Client *client) {

	std::string _cmdArray[CMDNBR] = {"CAP", "PING", "NICK", "USER", "JOIN", "MODE", "PRIVMSG", "NOTICE", "TOPIC", "PART", "KICK", "INVITE", "PASS", "QUIT"};

	void	(Server::*functionPtr[])(Client *client) = {
		&Server::CAP,
		&Server::PING,
		&Server::NICK,
		&Server::USER,
		&Server::JOIN,
		&Server::MODE,
		&Server::PRIVMSG,
		&Server::NOTICE,
		&Server::TOPIC,
		&Server::PART,
		&Server::KICK,
		&Server::INVITE,
		&Server::PASS,
		&Server::QUIT
	};

	for (int i = 0; i < CMDNBR; i++) {
		if (cmd.compare(_cmdArray[i]) == 0) {
			(this->*functionPtr[i])(client);
			return;
		}
	}
}

void	Server::CAP(Client *client) {
	if (command[0] == 'L' && command[1] == 'S')
		sendMsg("CAP * LS :", client->getFd());
	else
		return;
}

void	Server::PING(Client *client) {
	std::string ping = token;
	std::string pingContent = command;
	// Construire la réponse PONG avec le même contenu que le message PING
	std::string pongResponse = "PONG " + pingContent;
	_lastPing = time(NULL);
	// Envoyer la réponse PONG au client
	sendMsg(pongResponse, client->getFd());
}

void Server::NICK(Client *client) {
	std::cout << "Votre demande est une commande.: ";
	std::cout << "cmd nick" << std::endl;

	if (client->nickSet == false) {
		std::string nickname = command;
		std::cout << "GetNick() DEBUUUG " << client->getNick() << std::endl;
		std::cout << "1 NICKNAAAAME DEBUUUG " << nickname << std::endl;
		// int numberFd = client->getFd();
		// static_cast<std::string>(numberFd);
		// nickname = nickname + numberFd;
		int numberFd = client->getFd();
		std::stringstream ss;
		ss << numberFd;
		std::string strNumberFd = ss.str();
		nickname += strNumberFd;
		std::cout << "different ou egale DEBUUUG " << nickname << std::endl;
		client->setNick(nickname);
		std::cout << "2 NICKNAAAAME DEBUUUG " << client->getNick() << std::endl;
		std::string msg = ":" + command + " NICK " + nickname;
		sendMsg(msg, client->getFd());
		client->nickSet = true;
	}
	else {
		std::string newNick = command;

		// vérifie si le nouveau surnom dépasse 30 caractères
		std::cout << "getNick " << client->getNick() << std::endl;
		std::cout << "newNick " << newNick << std::endl;
		if(newNick.size() > 30) {
		sendErrorMsg(ERR_ERRONEUSNICKNAME, client->getFd(), newNick, "", "", "");
		std::cerr << "Error: Nickname is longer than 30 characters." << std::endl; //comme dans freenode
		return;
	}


	// vérifie si le surnom existe déjà
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->getNick() == newNick) {
			sendErrorMsg(ERR_NICKNAMEINUSE, client->getFd(), newNick, "", "", "");
			// std::cerr << "Error: Nickname already exists." << std::endl;// message d erreurs a gerer voir avec claire
			return;  // quitte la fonction
		}
  }

	// vérifie si le nouveau surnom respecte les règles
	if (newNick.empty() || newNick[0] == '#' || newNick[0] == ':' || newNick.find_first_of(CHANTYPES) != std::string::npos || newNick.find(' ') != std::string::npos) {
		sendErrorMsg(ERR_ERRONEUSNICKNAME, client->getFd(), newNick, "", "", "");
  	//std::cerr << "Error: Nickname contains invalid characters." << std::endl; // message d erreurs a gerer voir avec claire
		return ;  // quitte la fonction
	}

		// continue avec le reste du code si les conditions sont remplies
		for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
			if (it->getFd() == client->getFd()) {
				std::cout << "1 oldNick DEBUG IS : " << client->getNick() << std::endl;
				std::string oldNick;
				for (std::vector<Client>::iterator innerIt = _clients.begin(); innerIt != _clients.end(); ++innerIt) {
					if (innerIt->getFd() == client->getFd()) {
						oldNick = innerIt->getNick();
						break;
					}
				}

				it->setNick(newNick);
				
				std::string msg;
				// if (oldNick.empty())
				// 	msg = "NICK " + newNick;
				// else
				msg = ":" + oldNick + " NICK " + newNick;
				// std::string msg = ":n1t4r4 NICK nana";
				// std::cout << "MESS : " << msg << std::endl;
				sendMsg(msg, client->getFd());
				break;
			}
		}
	}
}

void	Server::USER(Client *client) { // passe dedant ?
	std::cout << "cmd user" << std::endl; // info only

	std::cout << "TOKEN " << token << std::endl;
	std::cout << "command " << command << std::endl;
		std::size_t colonPos = command.find(':');
		if (colonPos != std::string::npos) {
			std::string UserContent = command.substr(colonPos + 1);
			std::string	msg = "USER : " + UserContent + END_SEQUENCE;
			client->setUser(UserContent);
			std::cout << "USERCONTENT : " << UserContent << std::endl; // DEBUG ONLY
			std::cout << "USERNAME : " << client->getUser() << std::endl; // DEBUG ONLY
			// if (client->isAuthenticated() && !client->getUser().empty() && !client->getNick().empty())
			first_message(client);
			}
		}

void	Server::JOIN(Client *client) {
	std::cout << "cmd join" << std::endl;
	bool	channelExists = false;

	if (command == ":")
		return;

	// if (chanName == channel.getName()) { // juste ?
	// 	message d'erreur ce channel existe deja voir avec le mess erreur claire
	// Nadia: Non, pas de message d'erreur, mais joindre le channel existant en verifiant si droits
	// }
	// else {
		// if (command == 0)
		// 	Type /join #<channel> pas besoin gere tout seul
	if (command[0] != '#')
		command = '#' + command;

	for (std::vector<Channel*>::iterator	it = _channels.begin(); it != _channels.end(); it++)
	{
		if (((*it)->getChannelName() == command))
		{
			std::cout << "Channel [" + command + "] already exist. You'll join 'em" << std::endl;
			currentChannel = (*it);
			currentChannel->addMember(client);
			channelExists = true;
			// client aura un mode 'normal' (pas oper/admin)
			break;
		}
	}

	if (!channelExists)
	{
		currentChannel = addChannel(command);
		std::cout << "Channel [" + command + "] created. You'll be a VIP soon." << std::endl;
		//le client aura un mode operator
		currentChannel->addMember(client);
	}

	// send info to client
	std::string msg = ":" + client->getNick() + "@" + client->getHostname() + " JOIN " + command;
	sendMsg(msg, client->getFd());

	// send info of all members in the channel
	msg = ":" + client->getNick() + "@" + client->getHostname() + " = " + command + " :" + currentChannel->getAllMembers();
	sendMsg(msg, client->getFd());

	msg = ":" + command + " :End of /NAMES list.";
	sendMsg(msg, client->getFd());

	std::vector<Client*>	members = currentChannel->getMember(); // moins moche apres
	msg = ":" + client->getNick() + "@" + client->getHostname() + " JOIN " + command;
	// message a envoyer a tous les membres du channel
	for (std::vector<Client*>::iterator it=members.begin(); it !=members.end(); it++)
	{
		if ((*it)->getFd() != client->getFd()) // || autorise a recevoir des messages
			sendMsg(msg, (*it)->getFd());
	}

	// }
	// rest a ajouter lA GESTION DES ERREURS par claire

// 	Channel	*channel;
	//lui dire que la commande (requete qui vient apres JOIN)= le nom du channel
	//comment introduire la classe channel la dedans ?

// 	channel->_nameChannel = command;

	// creer une fonction pour creer le channel ou le faire direct la ?

// }
}

void	Server::MODE(Client *client) { // channel only ? auto gerer par le client lorqu on se connect
	std::cout << "cmd mode" << std::endl;
	(void)client;
}

void	Server::PRIVMSG(Client *client) {
	std::cout << "cmd privmsg" << std::endl;
	(void)client;
}

void	Server::NOTICE(Client *client) {
	std::cout << "cmd notice" << std::endl;
	(void)client;
}

void	Server::TOPIC(Client *client) {
	std::cout << "cmd topic" << std::endl;
	(void)client;
}

void	Server::PART(Client *client) {
	std::cout << "cmd part" << std::endl;
	(void)client;
}

void	Server::KICK(Client *client) {
	std::cout << "cmd Kick" << std::endl;
	(void)client;
}

void	Server::INVITE(Client *client) {
	std::cout << "cmd invite" << std::endl;
	(void)client;
}

void	Server::PASS(Client *client) {
	if (client->isAuthenticated()){
		//std::cout << "PASS" << std::endl;
		sendErrorMsg(ERR_ALREADYREGISTERED, client->getFd(),"", "", "", "");
	}

	if (_password != getPassword()){
		std::cout << "PASS" << std::endl;
		sendErrorMsg(ERR_PASSWDMISMATCH, client->getFd(),"", "", "", "");
	}
	if (_password.empty()){
		std::cout << "PASS" << std::endl;
		sendErrorMsg(ERR_NEEDMOREPARAMS, client->getFd(), client->getNick(), "COMMANDE A IMPLEMENTER", "", "");
	}
	client->setIsAuthenticated(true);
	if (client->isAuthenticated() && !client->getUser().empty() && !client->getNick().empty())
		first_message(client);
}

void Server::QUIT(Client *client) {
	std::cout << "Client " << client->getNick() << " has quit." << std::endl;

	// Envoyer un message de départ aux autres clients si nécessaire mais on peut l'enlever car pas demande apparement
	// std::string quitMessage = "Client " + client->getNick() + " has quit.";
	// for (size_t i = 0; i < _clients.size(); i++) {
	// 	if (_clients[i].getFd() != client->getFd()) {
	// 		sendMsg(quitMessage, _clients[i].getFd());
	// 	}
	// }

	// Fermer la connexion du client
	int clientSocket = client->getFd();
	close(clientSocket);

	// Supprimer l'objet Client du vecteur _clients
	for (size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i].getFd() == clientSocket) {
			_clients.erase(_clients.begin() + i);
			break;
		}
	}
}
