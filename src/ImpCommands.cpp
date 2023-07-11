#include "../inc/Server.hpp"
#include "../inc/Client.hpp"


void	Server::commands(std::string cmd, Client *client) {
		
	std::string _cmdArray[12] = {"NICK", "USER", "JOIN", "MODE", "PRIVMSG", "NOTICE", "TOPIC", "PART", "KICK", "INVITE", "PASS", "QUIT"};

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

	for (int i = 0; i < 12; i++) {
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
	std::string newNick = client->getNick();
	client->setIsAuthenticated(true);
	std::cout << "Votre nick est : " << newNick << std::endl;
	// std::string msg = "You're now known as " + client->getNick() + END_SEQUENCE;
	std::string msg = ":" + oldNick + " " + token + " " + newNick + END_SEQUENCE;
	// std::string msg = ":vferraro " + token + " " + newNick + END_SEQUENCE;
	sendMsg(msg, client->getFd());
}

void	Server::USER(Client *client) {
	std::cout << "cmd user" << std::endl;
	client->setUser(command);
}

void	Server::JOIN(Client *client) {
	std::cout << "cmd join" << std::endl;
	(void)client;
}

void	Server::MODE(Client *client) {
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
		sendErrMsgServer(462, client);
	}

	if (_password != getPassword()){
		sendErrMsgServer(464, client);
	}
	if (_password.empty()){
		sendErrMsgServer(461, client);
	}
	client->setIsAuthenticated(true);
	first_message(client);
}

void	Server::QUIT(Client *client) {
	std::cout << "cmd quit" << std::endl;
	(void)client;
}