#include "../inc/Server.hpp"
#include "../inc/Client.hpp"

void Server::sendMsg(std::string message, int fd)
{
	std::cout << "SEND: " << message.append(END_SEQUENCE) << std::endl;
 	send(fd, message.c_str(), message.size(), 0);
}

void    Server::first_message(Client *client) {

	std::string	msg = "001 " + client->getNick() + " :" + "\033[34mWelcome on the MoIRes Connection Server " + "!~" + client->getNick() + " @" + client->getHostname() + END_SEQUENCE + RES;
	sendMsg(msg, client->getFd());
}

void Server::sendErrorMsg(int errorCode, int fd, std::string param1="", std::string param2="", std::string param3="", std::string info="")
{
	std::stringstream ss;
	ss << errorCode;
	(void)info;
	std::string errorminator = ss.str();

	switch(errorCode)
	{
		case 400:
			errorminator += " UNKNOWNERROR " + param1 + param2 + " :Unknown error";
			break;
		case 401:
			errorminator += " NOSUCHNICK " + param1 + " :No such nick/channel";
			break;
		case 403:
			errorminator += " NOSUCHCHANNEL " + param1 + " :No such channel";
			break;
		case 404:
			errorminator += " CANNOTSENDTOCHAN " + param1 + " :Cannot send to channel";
			break;
		case 411:
			errorminator += " NORECIPIENT :No recipient given (" + param1 + ")";
			break;
		case 412:
			errorminator += " NOTEXTTOSEND :No text to send";
			break;
		case 421:
			errorminator += " UNKNOWNCOMMAND " + param1 + " :Unknown command";
			break;
		case 431:
			errorminator += " NONICKNAMEGIVE :No nickname given";
			break;
		case 432:
			errorminator += " ERRONEUSNICKNAME " + param1 + " :Erroneous nickname";
			break;
		case 433:
			errorminator += " NICKNAMEINUSE " + param1 + " :Nickname is already in use";
			break;
		case 436:
			errorminator += " NICKCOLLISION " + param1 + " :Nickname collision KILL from " + param2 + "@" + param3;
			break;
		case 441:
			errorminator += " USERNOTINCHANNEL " + param1 + " " + param2 + " :They aren't on that channel";
			break;
		case 442:
			errorminator += " NOTONCHANNEL " + param1 + " :You're not on that channel";
			break;
		case 443:
			errorminator += " USERONCHANNEL " + param1 + " " + param2 + " :is already on channel";
			break;
		case 461:
			errorminator += " NEEDMOREPARAMS " + param1 + param2 + " :Not enough parameters";
			break;
		case 462:
			errorminator += " ALREADYREGISTRED :You may not reregister";
			break;
		case 464:
			errorminator += " PASSWDMISMATCH :Password incorrect";
			break;
		case 471:
			errorminator += " " + param1 + " " + param2 + " :Cannot join channel (+l)";
			break;
		case 472:
			errorminator += " " + param1 + " " + param2 + " :is not a recognised channel mode.";
			break;
		case 473:
			errorminator += " ERR_INVITEONYLCHAN " + param1 + " :Cannot join channel. (+i)";
			break;
		case 475:
			errorminator += " ERR_BADPASSWORD " + param2 + " :Cannot join channel (incorrect channel key)";
			break;
		case 482:
			errorminator += + " " + param1 + " " + param2 + " :You're not a channel operator";
			break;
		case 501:
			errorminator += " UMODEUNKNOWNFLAG :Unknown MODE flag";
			break;
		case 650:
			errorminator += param1 + ":<channel> [:<topic>]";
			break;
		case 696:
			errorminator += + " " + param1 + " " + param2 + " " + param3 + " *" + " :You must specify a parameter for the " + param3 + " mode";
			break;
		default:
			errorminator += " " + param1 + " :Unknown error";
			break;
	}
	errorminator = ": " + errorminator + "\r\n" ;
	sendMsg(errorminator, fd);
}

