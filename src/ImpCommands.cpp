#include "../inc/Server.hpp"
#include "../inc/Client.hpp"


void	Server::NICK(Client *client, std::vector<std::string> arg){

	if (arg.empty() || arg[0].empty()){
		sendErrMsgServer(431, client->getFd(), client);
		return;
	}
	if (arg[1].size() < 2 || arg[1].size() > 9){
		sendErrMsgServer(432, client->getFd(), client);
		return;
	}
	if (!arg[1].compare(client->getNick())){
		sendErrMsgServer(433, client->getFd(), client);
		return;
	}
	client->setNick(arg[1]);
	first_message(client);
}
