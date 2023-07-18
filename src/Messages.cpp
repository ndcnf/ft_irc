#include "../inc/Server.hpp"
#include "../inc/Client.hpp"

void Server::sendMsg(std::string message, int fd)
{
	send(fd, message.append(END_SEQUENCE).c_str(), message.size(), 0);
}

// void Server::sendMsg(std::string message, int fd)
// {
//     std::cout << "MESSAGE : " << message << std::endl; //debug only
//     size_t messageSize = message.size() + strlen(END_SEQUENCE);
//     message.append(END_SEQUENCE);
//     send(fd, message.c_str(), messageSize, 0);
// }


void    Server::first_message(Client *client) {

	// std::string    msg = ":"+ client->getHostname() + "001 " + client->getUser() + " : " + "\033[34mWelcome on the MoIRes Connection Server " + client->getUser() + "!~" + client->getUser() + "@" + client->getHostname() + "\r\n" + RES;
	//std::string    msg = "001 " + client->getUser() + ": " + "\033[34mWelcome on the MoIRes Connection Server " + client->getUser() + "!~" + client->getUser() + "@" + client->getHostname() + "NICK " + client->getNick() + END_SEQUENCE + RES;
	std::string	msg = "001 " + client->getNick() + "\033[34m*: " + "\033[34mWelcome on the MoIRes Connection Server " + "!~" + client->getNick() + " @" + client->getHostname() + END_SEQUENCE + RES;
	sendMsg(msg, client->getFd());
}

void	Server::sendErrMsgServer(int errorCode, Client *client) {

	std::string	errorminator;

	switch (errorCode)
	{
		case 400:
				errorminator = " UNKNOWNERROR : Unknown error";
				break;
		case 401:
				errorminator = " NOSUCHNICK : No such nick/channel";
				break;
		case 403:
				errorminator = " NOSUCHCHANNEL : No such channel";
				break;
		case 404:
				errorminator = " CANNOTSENDTOCHAN : Cannot send to channel";
				break;
		case 411:
				errorminator = " NORECIPIENT : No recipient given";
				break;
		case 412:
				errorminator = " NOTEXTTOSEND : No text to send";
				break;
		case 421:
				errorminator = " UNKNOWNCOMMAND : Unknown command";
				break;
		case 431:
				errorminator = " NONICKNAMEGIVE : No nickname given";
				break;
		case 432:
				errorminator = " ERRONEUSNICKNAME : Erroneous nickname\r\n";
				break;
		case 433:
				errorminator = " NICKNAMEINUSE : Nickname is already in use";
				break;
		case 436:
				errorminator = " NICKCOLLISION : Nickname collision KILL from ";
				break;
		case 441:
				errorminator = " USERNOTINCHANNEL : They aren't on that channel";
				break;
		case 442:
				errorminator = " NOTONCHANNEL : You're not on that channel";
				break;
		case 443:
				errorminator = " USERONCHANNEL : is already on channel";
				break;
		case 461:
				errorminator = " NEEDMOREPARAMS : Not enough parameters";
				break;
		case 462:
				errorminator = " ALREADYREGISTRED : You may not reregister";
				break;
		case 464:
				errorminator = " PASSWDMISMATCH :Password incorrect";
				break;
		case 471:
				errorminator = "NOJOINCHANNEL : Cannot join channel";
				break;
		case 472:
				errorminator = " : is not a recognised channel mode";
				break;
		case 473:
				errorminator = "ERR_INVITEONYLCHAN : Cannot join channel";
				break;
		case 475:
				errorminator = "ERR_BADPASSWORD : Cannot join channel (incorrect channel key)";
				break;
		case 482:
				errorminator = "You're not a channel operator";
				break;
		case 501:
				errorminator = "UMODEUNKNOWNFLAG : Unknown MODE flag";
				break;
		default:
				errorminator = ":Unknown error";
				break;
	}

	std::cout << errorminator << client->getFd() << std::endl;

	// @Claire : j'ai mis en parametre la classe client mais du coup, quand on appelle la fonction, le 3e
	//           parametre est client, ce qui ne joue pas.... a voir comment manager differemment...
	//           par contre le message s'affiche bien cote serveur, a voir si il s'affiche cote client

	//Conditions en switch ou voir HARL avec les niveaux d erreur, ou autres...

	//msg == le message à renvoyer avec les getter necessaires.
	sendMsg(errorminator, client->getFd());


}
