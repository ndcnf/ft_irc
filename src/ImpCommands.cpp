#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/Messages.hpp"
#include "../inc/Channel.hpp"

// struct CommandHandler {
//     void (Server::*functionPtr)(Client *client, const std::vector<std::string>& params);
//     int numParams;
// };

// CommandHandler commandHandlers[CMDNBR] = {
//     {&Server::CAP, 1},      // CAP
//     {&Server::PING, 1},     // PING
//     {&Server::NICK, 1},     // NICK
//     {&Server::USER, 4},     // USER
//     {&Server::JOIN, 1},     // JOIN
//     {&Server::MODE, 2},     // MODE
//     {&Server::PRIVMSG, 2},  // PRIVMSG
//     {&Server::NOTICE, 2},   // NOTICE
//     {&Server::TOPIC, 2},    // TOPIC
//     {&Server::PART, 1},     // PART
//     {&Server::KICK, 2},     // KICK
//     {&Server::INVITE, 2},   // INVITE
//     {&Server::PASS, 1},     // PASS
//     {&Server::QUIT, 0}      // QUIT
// };


void	Server::commands(std::string cmd, Client *client, Channel *channel) {

	std::string _cmdArray[CMDNBR] = {"CAP", "PING", "NICK", "USER", "JOIN", "MODE", "PRIVMSG", "NOTICE", "TOPIC", "PART", "KICK", "INVITE", "PASS", "QUIT"};
	// std::string _cmdArrayTwoArg[CMDNBR] = {"CAP", "PING", "NICK", "USER", "JOIN", "MODE", "PRIVMSG", "NOTICE", "TOPIC", "PART", "KICK", "INVITE", "PASS", "QUIT"};


	void	(Server::*functionPtr[])(Client *client, Channel *channel) = {
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
			(this->*functionPtr[i])(client, channel);
			return;
		}
	}
}

void	Server::CAP(Client *client, Channel *channel) {
	(void)channel;
	if (command[0] == 'L' && command[1] == 'S')
		sendMsg("CAP * LS :", client->getFd());
	else
		return;
}

void	Server::PING(Client *client, Channel *channel) {
	(void)channel;
	std::string ping = token;
	std::string pingContent = command;
	// Construire la réponse PONG avec le même contenu que le message PING
	std::string pongResponse = "PONG " + pingContent;
	_lastPing = time(NULL);
	// Envoyer la réponse PONG au client
	sendMsg(pongResponse, client->getFd());
}

void Server::NICK(Client *client, Channel *channel) {
	(void)channel;

	if (client->nickSet == false) {
		std::string nickname = command;
		// int numberFd = client->getFd();
		// static_cast<std::string>(numberFd);
		// nickname = nickname + numberFd;
		int numberFd = client->getFd();
		std::stringstream ss;
		ss << numberFd;
		std::string strNumberFd = ss.str();
		nickname += strNumberFd;
		client->setNick(nickname);
		std::string msg = ":" + command + " NICK " + nickname;
		sendMsg(msg, client->getFd());
		client->nickSet = true;
	}
	else {
		std::string newNick = command;

		// vérifie si le nouveau surnom dépasse 30 caractères
		if(newNick.size() > 30) {
		sendErrorMsg(ERR_ERRONEUSNICKNAME, client->getFd(), newNick, "", "", "");
		std::cerr << "Error: Nickname is longer than 30 characters." << std::endl; //comme dans freenode
		return;
	}


	// vérifie si le surnom existe déjà
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if ((*it)->getNick() == newNick) {
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
		for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
			if ((*it)->getFd() == client->getFd()) {
				std::string oldNick;
				for (std::vector<Client*>::iterator innerIt = _clients.begin(); innerIt != _clients.end(); ++innerIt) {
					if ((*innerIt)->getFd() == client->getFd()) {
						oldNick = (*innerIt)->getNick();
						break;
					}
				}

				(*it)->setNick(newNick);

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

void	Server::USER(Client *client, Channel *channel) { // passe dedant ?
	(void)channel;
	std::cout << "cmd user" << std::endl; // info only

		std::size_t colonPos = command.find(':');
		if (colonPos != std::string::npos) {
			std::string UserContent = command.substr(colonPos + 1);
			std::string	msg = "USER : " + UserContent + END_SEQUENCE;
			client->setUser(UserContent);
			// if (client->isAuthenticated() && !client->getUser().empty() && !client->getNick().empty())
			first_message(client);
			}
		}

void	Server::JOIN(Client *client, Channel *channel) {
	(void)channel;
	std::cout << "cmd join" << std::endl;

	bool						channelExists = false;
	std::string					chanName;
	size_t						pos = 0;
	size_t						hashtagPos = 0;
	std::vector<std::string>	channelsToAdd;
	std::string					msg;
	
	if (command == ":")
		return;

	// if (chanName == channel.getName()) { // juste ?
	// 	message d'erreur ce channel existe deja voir avec le mess erreur claire
	// Nadia: Non, pas de message d'erreur, mais joindre le channel existant en verifiant si droits
	// }
	// else {
		// if (command == 0)
		// 	Type /join #<channel> pas besoin gere tout seul
	// if (pos != std::string::npos || command.find(":") != std::string::npos)

	while ((hashtagPos = command.find("#", hashtagPos)) != std::string::npos)
	{
		chanName = parseChan(command, hashtagPos);
		std::cout << "WHILE JOIN chanName [" + chanName + "]" << std::endl;

		if (chanName[0] != '#')
			chanName = '#' + chanName;
		
		channelsToAdd.push_back(chanName);

		hashtagPos = hashtagPos + 1;
	}

	for (std::vector<std::string>::iterator itc = channelsToAdd.begin(); itc != channelsToAdd.end(); itc++)
	{
		for (std::vector<Channel*>::iterator	it = _channels.begin(); it != _channels.end(); it++)
		{
			if (((*it)->getChannelName() == (*itc)))
			{
				std::cout << "Channel [" + (*itc) + "] already exist. You'll join 'em" << std::endl;
				currentChannel = (*it);
				currentChannel->addMember(client);
				channelExists = true;
				// client aura un mode 'normal' (pas oper/admin)
				break;
			}
		}

		if (!channelExists)
		{
			currentChannel = addChannel((*itc));
			std::cout << "Channel [" + (*itc) + "] created. You're a VIP now." << std::endl;
			currentChannel->addMember(client);
			currentChannel->addOperator(client);
		}

		pos = command.find(" :");
		if (pos != std::string::npos && (std::string::npos + 1) != (*itc).size())
			currentChannel->setTopic(command.substr(pos + 2, command.size()), client);

		// send info to client
		msg = ":" + client->getNick() + "@" + client->getHostname() + " JOIN " + (*itc);
		sendMsg(msg, client->getFd());

		// send info of all members in the channel
		msg = ":" + client->getNick() + "@" + client->getHostname() + " = " + (*itc) + " :" + currentChannel->getAllMembers();
		sendMsg(msg, client->getFd());

		if (!channelExists)
		{
			msg = "MODE " + (*itc) + " +o "+ client->getNick();
			sendMsg(msg, client->getFd());
		}

		msg = ":" + (*itc) + " :End of /NAMES list.";
		sendMsg(msg, client->getFd());

		msg = ":" + client->getNick() + "@" + client->getHostname() + " JOIN " + (*itc);
		sendMsgToAllMembers(msg, client->getFd());

		if (!currentChannel->getTopic().empty())
		{
			msg = "TOPIC " + (*itc) + " :" + currentChannel->getTopic();

			sendMsg(msg, client->getFd());
			sendMsgToAllMembers(msg, client->getFd());
		}
	}

	// rest a ajouter lA GESTION DES ERREURS par claire
	
}

void	Server::MODE(Client *client, Channel *channel) {
	std::cout << "cmd mode" << std::endl;
	std::string					msg;
	std::string					modes;
	std::vector<std::string>	modesVec;
	size_t						pos = 0;
	size_t						endPos;
	std::string					chanName;
	int							argsNum = 0;
	// std::map<char, int>			argsByMode = setArgsByMode();
	std::vector<std::string>	args;

	std::cout << "command recue [" + command + "]" << std::endl;

	// lors de la connexion initiale
	if (command == (client->getNick() + " +i"))
		return;

	chanName = parseChan(command, 0);
	bool	isChannel = channelExists(chanName);

	if (!isChannel)
	{
		//le channel donne n'existe pas ERREUR
		return ;
	}

	//Si le user n'est pas operator du channel: vtff
	if (!channel->isOperator(client))
		return ;

	pos = chanName.size() + 1;
	modes = command.substr(pos, command.find(" ")); // future old way

	// firstPos = command.find(command.find_first_of("+-"), (chanName.size() + 1));
	// lastPos = command.find(" ", firstPos);

	modesVec = parseModeCmd(command.substr(pos)); // new way
	if (modesVec.empty())
	{
		std::cout << "mode invalide" << std::endl;
		// commande invalide
		return ;
	}

	
	std::string	tempura;

	while ((pos = command.find(" ", pos)) != std::string::npos && pos < command.size())
	{
		std::cout << "la verite est ailleurs" << std::endl;
		endPos = command.find(" ", (pos + 1));
		if (endPos == std::string::npos)
			endPos = command.size();
		tempura = command.substr((pos + 1), ((endPos - pos) - 1));
		args.push_back(tempura);
		// argsNum++;
		pos = endPos;
	}

	std::cout << "ARGS SIZE " << args.size() << std::endl;

	for (std::vector<std::string>::iterator it=args.begin(); it != args.end(); it++)
		std::cout << "voila des arguments en beton [" + (*it) + "]" << std::endl;

	if (argsNum > 3)
	{
		//trop d'arguments pour notre realite
		return;
	}


	
	// modeTargetMember(command.substr(pos));

	// future old way
	// std::cout << "modes [" + modes + "]" << std::endl;

	// if ((pos = modes.find("t") != std::string::npos))
	// {
	// 	if ((pos - 1) == modes.find("+"))
	// 	{
	// 		channel->setTopicMode(true);
	// 	}
	// 	else
	// 	{
	// 		channel->setTopicMode(false);
	// 		// isMinus = true;
	// 	}
	// }
	// END of future old way	


	msg = "MODE " + channel->getChannelName() + " " + modes + " " + client->getNick();

	sendMsg(msg, client->getFd());


}

// void Server::PRIVMSG(Client *client, Channel *channel) { // ajouter des messages d erreurs ?
// 	std::cout << "cmd privmsg" << std::endl;
	
// 	if (command.find('#') != std::string::npos) {
// 		// Recherche de l'indice du ':'
// 		std::size_t chanPos = command.find("#");
// 		std::size_t msgPos = command.find(":");

// 		if (msgPos != std::string::npos && chanPos != std::string::npos) {
// 			// Extraction des sous-chaines apres le # pour le channel et apres le : pour le message'
// 			std::string chanComp = command.substr(chanPos + 1, msgPos - chanPos - 2);
// 			std::string allChanMsg = command.substr(msgPos + 1);
// 			chanComp = '#' + chanComp;
// 			// Comparaison avec le nom du channel actuel
// 			if (channel->getChannelName() == chanComp) {
// 				// Faire quelque chose si c'est le bon channel
// 				std::string msg = ':' + client->getNick() + '@' + client->getHostname() + " " + token + " " + chanComp + " :" + allChanMsg;
// 				// sendMsg(msg, client->getFd());
// 				sendMsgToAllMembers(msg, client->getFd());
				
// 			}
// 			else {
// 				sendErrorMsg(ERR_NOSUCHCHANNEL, client->getFd(), chanComp, "", "", "");
// 			}
// 		}
// 		else {
// 			sendErrorMsg(ERR_CANNOTSENDTOCHAN, client->getFd(), channel->getChannelName(), "", "", "");
// 		}
// 	}
// 	else
// 		sendErrorMsg(ERR_CANNOTSENDTOCHAN, client->getFd(), channel->getChannelName(), "", "", "");
// }


void Server::PRIVMSG(Client *client, Channel *channel) {
	
	std::cout << "cmd privmsg" << std::endl;
	if (command.find('#') != std::string::npos) {
		// Recherche de l'indice du ':'
		std::size_t chanPos = command.find("#");
		std::size_t msgPos = command.find(":");

		if (msgPos != std::string::npos && chanPos != std::string::npos) {
			// Extraction des sous-chaines apres le # pour le channel et apres le : pour le message'
			std::string chanComp = command.substr(chanPos + 1, msgPos - chanPos - 2);
			std::string allChanMsg = command.substr(msgPos + 1);
			chanComp = '#' + chanComp;
			// Comparaison avec le nom du channel actuel
			if (channel->getChannelName() == chanComp) {
				// Faire quelque chose si c'est le bon channel
				std::string msg = ':' + client->getNick() + '@' + client->getHostname() + " " + token + " " + chanComp + " :" + allChanMsg;
				// sendMsg(msg, client->getFd());
				sendMsgToAllMembers(msg, client->getFd());
				
			}
			else {
				sendErrorMsg(ERR_NOSUCHCHANNEL, client->getFd(), chanComp, "", "", "");
			}
		}
		else {
			sendErrorMsg(ERR_CANNOTSENDTOCHAN, client->getFd(), channel->getChannelName(), "", "", "");
		}
	}
	else if (command.find('#') == std::string::npos) {
		std::size_t msgPos = command.find(":");
		if (msgPos != std::string::npos) {
			std::string privMsg = command.substr(msgPos + 1);

			// Recherche de l'indice du premier espace avant le ':'
			std::size_t nickPos = command.find(" ");
			if (nickPos != std::string::npos) {
				// Extraction du nickname
				// std::string nickname = command.substr(nickPos + 1, msgPos - nickPos - 1);
				std::string nickname = command.substr(0, nickPos);
				std::cout << "debug message : " << privMsg << std::endl;
				std::cout << "debug nick a qui : " << nickname << std::endl;
				std::cout << "debug nick moi: " << client->getNick() << std::endl;
				                
                // Find the recipient client from _clients vector
                Client* recipientClient = NULL;
                for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
                    if ((*it)->getNick() == nickname) {
                        recipientClient = *it;
                        break;
                    }
                }
                if (recipientClient) {
				// std::string msg = ':' + client->getNick() + "@" + client->getHostname() + " " + token + " " + nickname + " :" + privMsg;
				// std::string msg = ':' + client->getNick() + "!~" + client->getUser() + "@" + client->getHostname() + " " + token + " " + nickname + " :" + privMsg;
				// std::string msg = ':' + client->getNick() + "@" + client->getHostname() + " " + token + " " + nickname + " :" + privMsg;
				// sendMsg(msg, client->getFd());
				std::string privMsgNick = "<" + client->getNick() + "> send you : " + privMsg;
				sendMsg(privMsgNick, recipientClient->getFd());
				}
				else
					sendErrorMsg(ERR_NOSUCHNICK, client->getFd(), client->getNick(), "", "", "");
			} 
			else
				sendErrorMsg(ERR_CANNOTSENDTOCHAN, client->getFd(), channel->getChannelName(), "", "", "");
		} 
		else
			sendErrorMsg(ERR_CANNOTSENDTOCHAN, client->getFd(), channel->getChannelName(), "", "", "");
	}
}



void	Server::NOTICE(Client *client, Channel *channel) { //comme privmsg mais sans les erreurs
	std::cout << "cmd notice" << std::endl;
	(void)client;
	(void)channel;
}

void	Server::TOPIC(Client *client, Channel *channel) {
	std::cout << "cmd topic" << std::endl;
	std::string		topicName;
	std::string		msg;
	size_t			pos = command.find(":");

//	La commande TOPIC peut comporter deux formes :

//	TOPIC #nom_du_canal :nouveau_sujet (pour définir un nouveau sujet)
//	TOPIC #nom_du_canal (pour récupérer le sujet actuel) // gere automatiquement

// Nouveau sujet uniquement pour les operators
// Gestion d'erreur: le channel n'existe pas, pas les droits, texte trop long

	if (channel == NULL)
	{
		sendErrorMsg(ERR_NOSUCHCHANNEL, client->getFd(), "", "", "", "");
		return;
	}

	// le /topic seul est gere automatiquement sans passer par TOPIC
	if (command.find("::") != std::string::npos)
	{
		msg = ": TOPIC " + channel->getChannelName();
		channel->setTopic("", client);
	}
	else
	{
		if (pos != std::string::npos && (std::string::npos + 1) != command.size())
			topicName = command.substr(pos + 1);
		else
			topicName = command;

		std::cout << "commande ds topic : [" + command + "]" << std::endl;
		std::cout << "topicName ds topic : [" + topicName + "]" << std::endl;
		std::cout << "channel name ds topic : [" + channel->getChannelName() + "]" << std::endl;

		channel->setTopic(topicName, client);
		msg = ": 332 " + client->getNick() + " " + channel->getChannelName() + " :" + channel->getTopic();
	}

	sendMsg(msg, client->getFd());
	// lors d'un changement de sujet, envoyer a tous les membres via TOPIC qqch
	sendMsgToAllMembers(msg, client->getFd());

}

// surement ici qu'il faudra erase() le member -> _members.erase(it);
// si plus personne, _members.size() == 0, il faudra aussi _channels.erase(it);
void	Server::PART(Client *client, Channel *channel){
	if (channel == NULL){
		std::cout << "channel null" << std::endl;
		sendErrorMsg(ERR_NOSUCHCHANNEL, client->getFd(), "", "", "", "");
		return;
	}

	channel->removeMember(client, client->getFd());
	std::cout << "Members still on the channel: " << channel->getMember().size() << std::endl;

	std::string msg = ":" + client->getNick() + "@" + client->getHostname() + " PART " + channel->getChannelName();
	sendMsg(msg, client->getFd());
	if (channel->getMember().size() > 0)
		sendMsgToAllMembers(msg, client->getFd());
	else if (channel->getMember().size() == 0)
		removeChannel(channel);

	// if (channel->getMember().size() > 1)
	// //if (members.size() > 2)
	// {
	// 	// std::string msg = ":" + client->getNick() + "@" + client->getHostname() + " PART " + channel->getChannelName();
	// 	sendMsg(msg, client->getFd());
	// 	sendMsgToAllMembers(msg, client->getFd());
	// }
	// else if (channel->getMember().size() == 1)
	// //else if (members.size() == 1)
	// {
	// 	// std::string msg = ":" + client->getNick() + "@" + client->getHostname() + " PART " + channel->getChannelName();
	// 	sendMsg(msg, client->getFd());
	// }
	// else if (channel->getMember().size() == 0)
	// {
	// 	sendMsg(msg, client->getFd());
	// 	removeChannel(channel);
	// }
}

void	Server::KICK(Client *client, Channel *channel) {
	std::cout << "cmd Kick" << std::endl;
	(void)client;
	(void)channel;
}

void	Server::INVITE(Client *client, Channel *channel) {
	std::cout << "cmd invite" << std::endl;
	(void)client;
	(void)channel;
}

void	Server::PASS(Client *client, Channel *channel) {
	(void)channel;

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

void Server::QUIT(Client *client, Channel *channel) {
	(void)channel;
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
		if (_clients[i]->getFd() == clientSocket) {
			_clients.erase(_clients.begin() + i);
			break;
		}
	}
}