#include "../inc/Server.hpp"
#include "../inc/Client.hpp"


void	Server::commands(std::string cmd, Client *client) {
		
	std::string _cmdArray[CMDNBR] = {"NICK", "USER", "JOIN", "MODE", "PRIVMSG", "NOTICE", "TOPIC", "PART", "KICK", "INVITE", "PASS", "QUIT"};
	//std::string _cmdArray[CMDNBR] = {"USER", "JOIN", "MODE", "PRIVMSG", "NOTICE", "TOPIC", "PART", "KICK", "INVITE", "PASS", "QUIT"};

	void	(Server::*functionPtr[])(Client *client) = {
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

void Server::NICK(Client *client) {
	std::cout << "Votre demande est une commande.: ";
	std::cout << "cmd nick" << std::endl;

	std::string newNick = command;

	// vérifie si le nouveau surnom dépasse 9 caractères
	if(newNick.size() > 9) {
		std::cerr << "Error: Nickname is longer than 9 characters." << std::endl;
		return;  // quitte la fonction
	}

	// vérifie si le surnom existe déjà
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->getNick() == newNick) {
			std::cerr << "Error: Nickname already exists." << std::endl;// message d erreurs a gerer voir avec claire
			std::string msg = "433 " + client->getNick() + " " + newNick + ":Nickname is already in use";
			sendMsg(msg, client->getFd());
			return;  // quitte la fonction
		}
	}

	// vérifie si le nouveau surnom respecte les règles
	if (newNick.empty() || newNick[0] == '#' || newNick[0] == ':' || newNick.find_first_of(CHANTYPES) != std::string::npos || newNick.find(' ') != std::string::npos) {
		std::cerr << "Error: Nickname contains invalid characters." << std::endl; // message d erreurs a gerer voir avec claire
		//std::string msg = 
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
			// std::size_t pos = oldNick.find("\nNICK");
			// if (pos != std::string::npos) {
			// 	oldNick = oldNick.substr(0, pos);
			// }
			std::cout << "2 oldNick DEBUG IS : " << oldNick << std::endl;
			
			it->setNick(newNick);
			std::cout << "DEBUG NICKNAME SET : " << it->getNick() << std::endl; // nana
			std::cout << "DEBUG NEWNICK SET : " << newNick << std::endl; // nana
			std::cout << "TOKEN DEBUG IS : " << token << std::endl; // NICK
			std::cout << "3 oldNick DEBUG IS : " << oldNick << std::endl;
			
			std::string msg;
			if (oldNick.empty())
				msg = "NICK " + newNick;
			else
				msg = ":" + oldNick + " NICK " + newNick;
			// std::string msg = ":n1t4r4 NICK nana";
			std::cout << "MESS : " << msg << std::endl;
			sendMsg(msg, client->getFd());
			break;
		}
	}
}



// void	Server::NICK(Client *client) {
// 	std::cout << "Votre demande est une commande.: ";
// 	std::cout << "cmd nick" << std::endl;
// 	// cmdSelection(buf, client);
// 	std::string oldNick = client->getNick();
// 	client->setNick(command);
// 	std::cout << "DEBUG NICKNAME SET : " << client->getNick() << std::endl;
// 	std::string newNick = client->getNick();
// 	std::cout << "Votre nick est : " << newNick << std::endl;
// 	// std::string msg = "You're now known as " + client->getNick() + END_SEQUENCE;
// 	// std::string msg = ":" + oldNick + " " + token + " " + newNick + END_SEQUENCE;
// 	std::string msg = ":" + oldNick + " " + token + " " + newNick + END_SEQUENCE;
// 	// std::string msg = ":n1t4r4 " + token + " " + newNick + END_SEQUENCE;
// 	sendMsg(msg, client->getFd());
// }

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
			first_message(client);
		}
}

void	Server::JOIN(Client *client) {
	std::cout << "cmd join" << std::endl;
	(void)client;
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
	std::cout << "cmd pass" << std::endl;
	(void)client;
}

void Server::QUIT(Client *client) {
	std::cout << "Client " << client->getNick() << " has quit." << std::endl;

	// Envoyer un message de départ aux autres clients si nécessaire mais on peut l'enlever car pas demande apparement
	std::string quitMessage = "Client " + client->getNick() + " has quit.";
	for (size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i].getFd() != client->getFd()) {
			sendMsg(quitMessage, _clients[i].getFd());
		}
	}

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
