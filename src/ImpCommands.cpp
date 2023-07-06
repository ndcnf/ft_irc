#include "../inc/Server.hpp"
#include "../inc/Client.hpp"


// void	Server::NICK(Client *client){

// 	if (command.empty()){
// 		// sendErrMsgServer(431, client);
// 		// return;
// 		client->getNick();
// 	}
// 	if (command.size() < 2 || command.size() > 9){
// 		sendErrMsgServer(432, client);
// 		return;
// 	}
// 	if (!command.compare(client->getNick())){
// 		sendErrMsgServer(433, client);
// 		return;
// 	}
// 	client->setNick(command);
// 	// first_message(client);
// }

// void	Server::NICK(Client *client, std::vector<std::string> arg){

// 	if (arg.empty() || arg[0].empty()){
// 		sendErrMsgServer(431, client->getFd(), client);
// 		return;
// 	}
// 	if (arg[1].size() < 2 || arg[1].size() > 9){
// 		sendErrMsgServer(432, client->getFd(), client);
// 		return;
// 	}
// 	if (!arg[1].compare(client->getNick())){
// 		sendErrMsgServer(433, client->getFd(), client);
// 		return;
// 	}
// 	client->setNick(arg[1]);
// 	first_message(client);
// }

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
	std::cout << "Votre nick est : " << newNick << std::endl;
	// std::string msg = "You're now known as " + client->getNick() + END_SEQUENCE;
	// std::string msg = ":" + oldNick + " " + token + " " + newNick + END_SEQUENCE;
	std::string msg = ":vferraro " + token + " " + newNick + END_SEQUENCE;
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
	std::cout << "cmd pass" << std::endl;
	(void)client;
}

void	Server::QUIT(Client *client) {
	std::cout << "cmd quit" << std::endl;
	(void)client;
}