#include "../inc/Server.hpp"
#include "../inc/Client.hpp"



void	Server::NICK(Client *client, std::vector<std::string> arg){

	if (arg.empty() || arg[0].empty()){
		sendErrMsgServer(431, client->getFd(), client);
		return;
	}
	std::string nickname = arg[0];

	// if (si le nickname est deja utilisé)
	// {
	//  	sendErrMsgServer(433, client->getFd(), client);
	// }

	client->setNick(nickname);
	first_message(client);
}
