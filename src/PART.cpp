// surement ici qu'il faudra erase() le member -> _members.erase(it);
// si plus personne, _members.size() == 0, il faudra aussi _cannels.erase(it);
void	Server::PART(Client *client, Channel *channel){
	// //void channel
	// if(command != channel->getChannelName())
	// 	sendErrorMsg(ERR_NOSUCHCHANNEL, client->getFd(), "", "", "", "");
	// else{
	//	bool channelNotEmpty = true;

		if (channel == NULL){
			std::cout << "channel null" << std::endl;
			sendErrorMsg(ERR_NOSUCHCHANNEL, client->getFd(), "", "", "", "");
			return;
		}
		// else if (command == channel->getChannelName())
		//channel->removeMember(client, client->getFd());
		std::vector<Client*>	members = currentChannel->getMember();
		for (std::vector<Client*>::iterator it = members.begin(); it != members.end(); it++)
		{
			// std::cout << "nb getmembres1 :" << channel->getMember().size() << std::endl;
			// std::cout << "nb membres1 :" << members.size() << std::endl;

			std::cout << "it->getFd " << (*it)->getFd() << std::endl;
			std::cout << "client->getFd() " << client->getFd() << std::endl;
			if ((*it)->getFd() == client->getFd()) // || autorise a recevoir des messages
			{
				members.erase(it);
				break ;
			}
		}
		std::cout << "nb getmembres2 :" << channel->getMember().size() << std::endl;
		std::cout << "nb membres2 :" << members.size() << std::endl;

		if (channel->getMember().size() > 1)
		//if (members.size() > 2)
		{
			std::string msg = ":" + client->getNick() + "@" + client->getHostname() + " PART " + channel->getChannelName();
			sendMsg(msg, client->getFd());
			sendMsgToAllMembers(msg, client->getFd());
		}
		// if (currentChannel->getAllMembers().size() == 0)
		// 	channelNotEmpty = false;
		else if (channel->getMember().size() == 1)
		//else if (members.size() == 1)
		{
			std::string msg = ":" + client->getNick() + "@" + client->getHostname() + " PART " + channel->getChannelName();
			sendMsg(msg, client->getFd());
			//channel->getChannelName().erase(); // channel a effacer du vector, non ?
		}
		//}
}
