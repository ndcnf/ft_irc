#include "../inc/Server.hpp"
#include "../inc/Client.hpp"


void	Server::commands(std::string cmd, Client *client) {
		
	std::string _cmdArray[CMDNBR] = {"NICK", "USER", "JOIN", "MODE", "PRIVMSG", "NOTICE", "TOPIC", "PART", "KICK", "INVITE", "PASS", "QUIT"};

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

void	Server::NICK(Client *client) {
	std::cout << "Votre demande est une commande.: ";
	std::cout << "cmd nick" << std::endl;
	// cmdSelection(buf, client);
	std::string oldNick = client->getNick();
	client->setNick(command);
	std::cout << "DEBUG NICKNAME SET : " << client->getNick() << std::endl;
	std::string newNick = client->getNick();
	std::cout << "Votre nick est : " << newNick << std::endl;
	// std::string msg = "You're now known as " + client->getNick() + END_SEQUENCE;
	// std::string msg = ":" + oldNick + " " + token + " " + newNick + END_SEQUENCE;
	std::string msg = ":n1t4r4 " + token + " " + newNick + END_SEQUENCE;
	sendMsg(msg, client->getFd());
}

void	Server::USER(Client *client) { // ne passe jamais dedant car pas besoin de le gerer mais je la laisse pour faire joli
	(void)client;
	std::cout << "cmd user" << std::endl; // est gerer directement avec le cap ls
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
