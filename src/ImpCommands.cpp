#include "../inc/Server.hpp"
#include "../inc/Client.hpp"


void	Server::NICK(Client *client){

	if (command.empty()){
		// sendErrMsgServer(431, client);
		// return;
		client->getNick();
	}
	if (command.size() < 2 || command.size() > 9){
		sendErrMsgServer(432, client);
		return;
	}
	if (!command.compare(client->getNick())){
		sendErrMsgServer(433, client);
		return;
	}
	client->setNick(command);
	first_message(client);
}

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

void	Server::commands(std::string cmd) {
		
	std::string _cmdArray[12] = {"NICK", "USER", "JOIN", "MODE", "PRIVMSG", "NOTICE", "TOPIC", "PART", "KICK", "INVITE", "PASS", "QUIT"};

	void	(Server::*functionPtr[])(void) = {
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
			(this->*functionPtr[i])();
			return;
		}
	}
}

void	Server::NICK() {
	std::cout << "cmd nick" << std::endl;
}

void	Server::USER() {
	std::cout << "cmd user" << std::endl;
}

void	Server::JOIN() {
	std::cout << "cmd join" << std::endl;
}

void	Server::MODE() {
	std::cout << "cmd mode" << std::endl;
}

void	Server::PRIVMSG() {
	std::cout << "cmd privmsg" << std::endl;
}

void	Server::NOTICE() {
	std::cout << "cmd notice" << std::endl;
}

void	Server::TOPIC() {
	std::cout << "cmd topic" << std::endl;
}

void	Server::PART() {
	std::cout << "cmd part" << std::endl;
}

void	Server::KICK() {
	std::cout << "cmd Kick" << std::endl;
}

void	Server::INVITE() {
	std::cout << "cmd invite" << std::endl;
}

void	Server::PASS() {
	std::cout << "cmd pass" << std::endl;
}

void	Server::QUIT() {
	std::cout << "cmd quit" << std::endl;
}