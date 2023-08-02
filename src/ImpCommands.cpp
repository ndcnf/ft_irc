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

	if (passIsValid == false) {
		sendErrorMsg(ERR_NEEDMOREPARAMS, client->getFd(), "", "PASSWORD REQUIERED", "", "");
		exit(EXIT_FAILURE);
	}
	if (client->nickSet == false) {
		std::string nickname = command;
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

void	Server::USER(Client *client, Channel *channel) {
	(void)channel;
	std::cout << "cmd user" << std::endl; // info only

	if (passIsValid == false) {
		sendErrorMsg(ERR_NEEDMOREPARAMS, client->getFd(), client->getNick(), "PASSWORD REQUIERED", "", "");
		exit(EXIT_FAILURE);
	}

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
	bool						clientCanJoin = false;
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
				channelExists = true;

				if (channel->getLimitMode())
				{
					//Si opti (notamment si clientCanJoin obsolete) au lieu de la condition actuelle:
					// if (static_cast<int>(channel->getMember().size()) >= channel->getNbLimit())
					// {
					// 	sendErrorMsg(ERR_CHANNELISFULL, client->getFd(), client->getNick(), channel->getChannelName(), "", "");
					// 	return ;
					// }



					if (static_cast<int>(channel->getMember().size()) < channel->getNbLimit())
						clientCanJoin = true;
					else
					{
						sendErrorMsg(ERR_CHANNELISFULL, client->getFd(), client->getNick(), channel->getChannelName(), "", "");
						clientCanJoin = false;
						return ;
					}
				}
				else
					// pour l'instant, c'est bon. Autre verifications pour invitations-only + mot de passe correct
					clientCanJoin = true;

				//verifier s'il y invitation seulement et si oui, si la personne est sur la liste.
				//verifier si mot de passe et si ok, si oui.

				if (clientCanJoin)
				{
					std::cout << "Channel [" + (*itc) + "] already exist. You'll join 'em" << std::endl;
					currentChannel = (*it);
					currentChannel->addMember(client);
					break;
				}
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
	std::string					msg = "";
	std::string					modes;
	std::string					chanName;
	std::vector<std::string>	modesVec;
	std::vector<std::string>	args;
	size_t						pos = 0;
	size_t						endPos;
	bool						isAdded = false;

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
		endPos = command.find(" ", (pos + 1));
		if (endPos == std::string::npos)
			endPos = command.size();
		tempura = command.substr((pos + 1), ((endPos - pos) - 1));
		args.push_back(tempura);
		pos = endPos;
	}

	if (args.size() > 3)
	{
		//trop d'arguments pour notre realite
		std::cout << "trop d'arguments" << std::endl;
		return;
	}

	pos = 0;
	for (std::vector<std::string>::iterator it = modesVec.begin(); it != modesVec.end(); it++)
	{
		if ((*it).find("+") != std::string::npos)
			isAdded = true;
		else
			isAdded = false;

		if ((*it).find("t") != std::string::npos)
		{
			channel->setTopicMode(isAdded);
			if (isAdded)
				msg = ":" + client->getNick() + " MODE " + channel->getChannelName() + " " + (*it) + " :Channel topic is restricted to operator(s)";
			else
				msg = ":" + client->getNick() + " MODE " + channel->getChannelName() + " " + (*it) + " :Channel topic can be set by everyone";	
		}
		else if ((*it).find("o") != std::string::npos)
		{
			if (args.size() == 0)
			{
				sendErrorMsg(ERR_NEEDMOREPARAMS, client->getFd(), channel->getChannelName(), "Invalid channel limit", "", "");
				return ;
			}

			if (channel->setOperator(isAdded, args.back()))
			{
				if (isAdded)
					msg = ":" + client->getNick() + " MODE " + channel->getChannelName() + " " + (*it) + " " + args.back() + " :has been granted operator status.";			
				else
					msg = ":" + client->getNick() + " MODE " + channel->getChannelName() + " " + (*it) + " " + args.back() + " :has been removed from operators";		
			}
		}
		else if ((*it).find("l") != std::string::npos)
		{
			if (args.size() == 0 && (isAdded))
			{
				sendErrorMsg(ERR_NEEDMOREPARAMS, client->getFd(), "", "", "", "");
				return ;
			}

			if (isAdded)
			{
				int limit = std::atoi(args.front().c_str());
				if (limit <= 0)
				{
					sendErrorMsg(ERR_UNKNOWNMODE, client->getFd(), "", "", "", "");
					return ;
				}
				channel->setLimit(isAdded, limit);
				msg = ":" + client->getNick() + " MODE " + channel->getChannelName() + " " + (*it) + " " + args.front() + " :Channel limit set to " + args.front();			
			}
			else
			{
				channel->setLimit(isAdded, 0);
				msg = ":" + client->getNick() + " MODE " + channel->getChannelName() + " " + (*it) + " :Channel limit removed";
			}
		}
		else if ((*it).find("k") != std::string::npos)
		{
			if (args.size() == 0 && (isAdded))
			{
				sendErrorMsg(ERR_NEEDMOREPARAMS, client->getFd(), "", "", "", "");
				return ;
			}

			if (isAdded)
			{
				std::string	password;
				if (args.size() == 1)
					password = args.front();
				else if (args.size() == 2)
				{
					if (channel->isNumber(args.front()))
						password = args[1];
					else
						password = args.front();
				}
				else
					password = args[1];
					
				std::cout << "VERIF PASSWORD ENTRE : " + password << std::endl;
			}


		}

		if (!msg.empty())
		{
			sendMsg(msg, client->getFd());
			sendMsgToAllMembers(msg, client->getFd());
		}
	}





}

//for PRIVMSG
size_t countSubstring(const std::string& str, std::string& sub) {
	if (sub.length() == 0) 
		return 0;
	size_t count = 0;
	std::size_t lastChar = sub.find_last_not_of(' ');
	if (lastChar != std::string::npos) {
		sub = sub.substr(0, lastChar + 1);
	}
	for (size_t offset = str.find(sub); offset != std::string::npos;
		 offset = str.find(sub, offset + 1)) {
		count++;
	}
	return count;
}

void Server::PRIVMSG(Client* client, Channel* channel) {
	std::cout << "cmd privmsg" << std::endl;

		size_t parsePoint = command.find(':');
		std::string channelName = command.substr(1, parsePoint - 1);  // Get the channel name
		std::string messChan = command.substr(parsePoint + 1);;  // Get the message
		channelName = '#' + channelName;
		// Check how many times the channel name appears in the command
		std::size_t count = countSubstring(command, channelName);
		if (count > 1) {
			std::cout << "2 count donc prive : " << channelName << std::endl;
			// juste parse message apres les : et envoie
			std::cout << "messCHan :" << messChan << std::endl;
			
			std::string msg = ':' + client->getNick() + '@' + client->getHostname() + " " + token + " " + channelName + " :" + messChan;
			// sendMsg(msg, client->getFd());
			sendMsgToAllMembers(msg, client->getFd());
		}
	if (command.find('#') != std::string::npos && count == 1) {
		std::cout << "1 count donc channel : " << channelName << std::endl;

		std::vector<std::string> hashChan;
		std::string allChanMsg;

		// Séparer la commande en mots à l'aide de la fonction parseChan
		size_t pos = command.find("#");
		while (pos != std::string::npos) {
			std::string chanName = parseChan(command, pos);
			hashChan.push_back(chanName);
			pos = command.find('#', pos + 1);
		}
		if (!hashChan.empty()) {
			std::string lastHashWord = hashChan.back();
			std::size_t lastHashPos = command.rfind(lastHashWord);
			if (lastHashPos != std::string::npos) {
				allChanMsg = command.substr(lastHashPos + lastHashWord.size() + 2); // soucis de parsing a verifier
			}
		}
		for (size_t i = 0; i < hashChan.size(); i++) {
			std::cout << "mot avec # : " << hashChan[i] << std::endl;
			std::cout << "message a envoyer : " << allChanMsg << std::endl;

			bool found = false;

			// Comparaison avec les noms de channels existants
			for (size_t j = 0; j < _channels.size(); ++j) {
				if (_channels[j]->getChannelName() == hashChan[i]) {
					found = true;
					// Envoyer le message à tous les membres du channel correspondant
					std::string msg = ':' + client->getNick() + '@' + client->getHostname() + " " + token + " " + hashChan[i] + " :" + allChanMsg;
					sendMsgToAllMembers(msg, client->getFd());
					// break; // On arrête la recherche dès qu'on a trouvé le channel correspondant du coup non !! 
				}
			}
			if (!found) {
				sendErrorMsg(ERR_NOSUCHCHANNEL, client->getFd(), hashChan[i], "", "", "");
			}
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
				std::string nickname = command.substr(0, nickPos);
				
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
				else {
					sendErrorMsg(ERR_NOSUCHNICK, client->getFd(), client->getNick(), "", "", "");
				}
			} 
			else {
				sendErrorMsg(ERR_CANNOTSENDTOCHAN, client->getFd(), channel->getChannelName(), "", "", "");
			}
		} 
		else {
			sendErrorMsg(ERR_CANNOTSENDTOCHAN, client->getFd(), channel->getChannelName(), "", "", "");
		}
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
	std::cout << "password enregistrer chez nous : " << getPassword() << std::endl;
	std::string pass = command;
	if (pass.empty()){
		std::cout << "PASS" << std::endl;
		sendErrorMsg(ERR_NEEDMOREPARAMS, client->getFd(), client->getNick(), "COMMANDE A IMPLEMENTER", "", "");
	}
	if (pass != getPassword()){
		std::cout << "PASS" << std::endl;
		sendErrorMsg(ERR_PASSWDMISMATCH, client->getFd(),"", "", "", "");
		exit(EXIT_FAILURE);
	}
	else {
		if (client->isAuthenticated()){
			//std::cout << "PASS" << std::endl;
			sendErrorMsg(ERR_ALREADYREGISTERED, client->getFd(),"", "", "", "");
		}
		else {
			client->setIsAuthenticated(true);
			passIsValid = true;
				// first_message(client); fait apres dans USER qui arrive apres NICK
		}
	}
}

// void	Server::PASS(Client *client, Channel *channel) {
// 	(void)channel;

// 	if (client->isAuthenticated()){
// 		//std::cout << "PASS" << std::endl;
// 		sendErrorMsg(ERR_ALREADYREGISTERED, client->getFd(),"", "", "", "");
// 	}

// 	if (_password != getPassword()){
// 		std::cout << "PASS" << std::endl;
// 		sendErrorMsg(ERR_PASSWDMISMATCH, client->getFd(),"", "", "", "");
// 	}
// 	if (_password.empty()){
// 		std::cout << "PASS" << std::endl;
// 		sendErrorMsg(ERR_NEEDMOREPARAMS, client->getFd(), client->getNick(), "COMMANDE A IMPLEMENTER", "", "");
// 	}
// 	client->setIsAuthenticated(true);
// 	if (client->isAuthenticated() && !client->getUser().empty() && !client->getNick().empty())
// 		first_message(client);
// }

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