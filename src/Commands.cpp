#include "../inc/Server.hpp"
#include "../inc/Client.hpp"


void	Server::cmdSelection(char *buf)
{
	std::string	str(buf);
	std::string	splitStr;
	std::string	command;
	std::string	token;
	std::string	content;

	if (str.size() < 2)
	{
		std::cout << "ERREUR, pas de commande donnee" << std::endl; // NEEDS IMPROVEMENT
		return ;
	}

	// enum commands
	// {
	// 	JOIN,
	// 	PASS,
	// 	NICK,
	// 	USER,
	// 	// OPER,
	// 	// QUIT,
	// 	PART,
	// 	PRIVMSG,
	// 	NOTICE,

	// 	//commandes operators listes dans le sujet
	// 	KICK,
	// 	INVITE,
	// 	TOPIC,
	// 	MODE,

	// 	//commandes donnees par Nicole
	// 	PONG,
	// 	WHO,
	// 	NUM_COMMANDS
	// };

	str = str.substr(1, str.size() - 2);
	token = str.substr(0, str.find(' '));
	//  token = strtok(splitStr, " ");

		for (unsigned int i = 0; i < token.size(); i++)
		token[i] = toupper(token[i]);

	std::cout << "TOKEN : [" << token << "]" << std::endl; // DEBUG ONLY

	if (str.size() == (token.size() + 1) && (str.size() < token.size() + 2))
	{
		std::cout << token.size() << " | ";
		std::cout << "ERREUR Your command is empty." << std::endl; // ERROR
		return ;
	}

	splitStr = str.substr(token.size(), str.find('\n'));
	if (splitStr.size() != 0) {
		content = str.substr(splitStr.size() + 2);
		 for (unsigned int i = 0; i < content.size(); i++)
		 content[i] = toupper(content[i]);
		 if (token.size() < 4)
		 content = str.substr(splitStr.size() + 1);
		 for (unsigned int i = 0; i < content.size(); i++)
		 content[i] = toupper(content[i]);

		std::cout << "CONTENT : [" << content << "]" << std::endl; // DEBUG ONLY

		//FORET de IF attention a modifier le try and catch risque d arreet du prog
		if (token.size() != 0 && content.size() != 0) {
			if ((token.compare("CAP") == 0) && (content.compare("LS") == 0)) {
				std::cout << "CAP LS start" << std::endl;
				// std::cout << "Capabilities supported: " << std::endl; // envoyer la liste de commandes a imprimer (note de @Verena)
				// capOrNOt();
				// try {
				// 	getCap();
				// } catch (const std::exception &e) {
				// 		std::cerr << "End of CAP LS negotiation" << e.what() << std::endl;
				// }
				// return ;
			}
			else if ((token.compare("CAP") == 0) && (content.compare("LS") != 0)) {
				std::cerr << ERRMSG << RES << content << RED << " is not an accepted command" << RES << std::endl;
				return;
			}
		}
		// else if (token.size() != 0 && content.size() == 0) {

		//ca ne rentre pas dedant. Faire une fonction par commande et comparer a chaque fois comme pour le CAP LS ? @Verena
		else if (token.size() != 0 && content.size() == 0) {
			std::cout << "I am in" << std::endl;
			if (token == "JOIN")
				std::cout << "join us on : " << content << std::endl;
			else if (token == "NICK")
				std::cout << "nickname : " << std::endl;
			else if (token == "USER")
				std::cout << "user name : " << std::endl;
			else if (token == "PART")
				std::cout << "part + arg ? : " << std::endl;
			else if (token == "PRIVMSG")
				std::cout << "private msg : " << std::endl;
			else if (token == "NOTICE")
				std::cout << "notice (private msg also) : " << std::endl;
			else if (token == "KICK")
				std::cout << "kick : " << std::endl;
			else if (token == "INVITE")
				std::cout << "invite : " << std::endl;
			else if (token == "TOPIC")
				std::cout << "topic : " << std::endl;
			else if (token == "MODE")
				std::cout << "mode (+ i, t, k, o or l) : " << std::endl;
			// else if (token.compare("PONG")) {
			// 	std::cout << "pong ? : " << std::endl;
			// 	return ;
			// }
			// else if (token.compare("PING")) {
			// // recuperer ce qui a apres le ping et le renvoyer apres le pong parsePing
			// std::cout << "PONG : " << parsePing(token, ) << std::endl;
			// return ;
			}
			else
				std::cout << "I don't understand this command." << std::endl;
		}
		else if (token.size() != 0 && content.size() != 0)
			std::cout << "I need a working content to handle" << std::endl;
		}
	// else
	// 	throw Server::ServException(ERRMSG"pas content");

	// commands	cmd;
	// cmd = JOIN;

	// for (int i = 0; i < NUM_COMMANDS; i++)
	// {
	// 	if (token == "JOIN")
	// 		std::cout << "join us..."<< std::endl;
	// 	std::cout << "fooooor"<< std::endl;
	// }

	// std::cout << cmd << std::endl;
	// switch(cmd)
	// {
	// 	case join	: std::cout << "join us..."<< std::endl;	break;
	// 	case nick	: std::cout << "nice nickname..." << std::endl;	break;
	// 	default		: std::cout << "I don't understand you." << std::endl;	break;
	// }

// }

