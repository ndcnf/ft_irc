#include "../inc/Server.hpp"
#include "../inc/Client.hpp"


void	Server::cmdSelection(char *buf, Client *client)
{
	std::string	str(buf);
	std::string	splitStr;
	// std::string	command;
	// std::string	token;
	std::string	content;

	if (str.size() < 2)
	{
		std::cout << "ERREUR, pas de commande donnee" << std::endl; // NEEDS IMPROVEMENT
		return ;
	}

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

	std::cout << "COMMAND 1 : " << command << "TOKEN : " << token << "CONTENT : " << content << std::endl; // DEBUG ONLY

	if (token.size() != 0) {
		if (command.size() != 0) { // maybe useless @VERENA
			// if (command.size() > 2) {
			// 	static_cast<std::string>(buf) = content.c_str();
			// 	std::cout << "Commandes avec  token et infos ici" << std::endl;
			// 	sendFromClient(buf, client);
			// }
			// sendFromClient(buf, client);
			std::cout << "Commandes avec command ici" << std::endl;
		}
		else {
			std::cout << "Commandes avec juste token ici" << std::endl;
			sendFromClient(buf, client);
		}
	}
	std::cout << "COMMAND 2 : " << command << "TOKEN : " << token << "CONTENT : " << content << std::endl; // DEBUG ONLY

	// splitStr = str.substr(token.size(), str.find('\n'));
	// if (splitStr.size() != 0) {
	// 	content = str.substr(splitStr.size() + 2);
	// 	 for (unsigned int i = 0; i < content.size(); i++)
	// 	 content[i] = toupper(content[i]);
	// 	 if (token.size() < 4)
	// 	 content = str.substr(splitStr.size() + 1);
	// 	 for (unsigned int i = 0; i < content.size(); i++)
	// 	 content[i] = toupper(content[i]);

	// 	std::cout << "CONTENT : [" << content << "]" << std::endl; // DEBUG ONLY

	// std::cout << "COMMAND 3 : " << command << "TOKEN : " << token << "CONTENT : " << content << std::endl; // DEBUG ONLY

	// 	//FORET de IF attention a modifier le try and catch risque d arreet du prog
	// 	if (token.size() != 0 && content.size() != 0) {
	// 		if ((token.compare("CAP") == 0) && (content.compare("LS") == 0)) {
	// 			std::cout << "CAP LS start" << std::endl;
	// 		}
	// 		else if ((token.compare("CAP") == 0) && (content.compare("LS") != 0)) {
	// 			std::cerr << ERRMSG << RES << content << RED << " is not an accepted command" << RES << std::endl;
	// 			return;
	// 		}
	// 	}
	// }
}

// void	Server::cmdSelection(char *buf)
// {
// 	std::string	str(buf);
// 	std::string	splitStr;
// 	std::string	command;
// 	std::string	token;
// 	std::string	content;

// 	if (str.size() < 2)
// 	{
// 		std::cout << "ERREUR, pas de commande donnee" << std::endl; // NEEDS IMPROVEMENT
// 		return ;
// 	}

// 	// if (strstr(buf, "USER :\r\n") != 0) {
// 	// 	sendMsg(" 001 Verena Hi ! Welcome to this awesome IRC server !, @ Verena\r\n", Server::getSocket());
// 	// }
// 	// enum commands
// 	// {
// 	// 	JOIN,
// 	// 	PASS,
// 	// 	NICK,
// 	// 	USER,
// 	// 	// OPER,
// 	// 	// QUIT,
// 	// 	PART,
// 	// 	PRIVMSG,
// 	// 	NOTICE,

// 	// 	//commandes operators listes dans le sujet
// 	// 	KICK,
// 	// 	INVITE,
// 	// 	TOPIC,
// 	// 	MODE,

// 	// 	//commandes donnees par Nicole
// 	// 	PONG,
// 	// 	WHO,
// 	// 	NUM_COMMANDS
// 	// };

// 	str = str.substr(1, str.size() - 2);
// 	token = str.substr(0, str.find(' '));
// 	//  token = strtok(splitStr, " ");

// 		for (unsigned int i = 0; i < token.size(); i++)
// 		token[i] = toupper(token[i]);

// 	std::cout << "TOKEN : [" << token << "]" << std::endl; // DEBUG ONLY

// 	if (str.size() == (token.size() + 1) && (str.size() < token.size() + 2))
// 	{
// 		std::cout << token.size() << " | ";
// 		std::cout << "ERREUR Your command is empty." << std::endl; // ERROR
// 		return ;
// 	}

// 	splitStr = str.substr(token.size(), str.find('\n'));
// 	if (splitStr.size() != 0) {
// 		content = str.substr(splitStr.size() + 2);
// 		 for (unsigned int i = 0; i < content.size(); i++)
// 		 content[i] = toupper(content[i]);
// 		 if (token.size() < 4)
// 		 content = str.substr(splitStr.size() + 1);
// 		 for (unsigned int i = 0; i < content.size(); i++)
// 		 content[i] = toupper(content[i]);

// 		std::cout << "CONTENT : [" << content << "]" << std::endl; // DEBUG ONLY

// 		//FORET de IF attention a modifier le try and catch risque d arreet du prog
// 		if (token.size() != 0 && content.size() != 0) {
// 			if ((token.compare("CAP") == 0) && (content.compare("LS") == 0)) {
// 				std::cout << "CAP LS start" << std::endl;
// 				// std::cout << "Capabilities supported: " << std::endl; // envoyer la liste de commandes a imprimer (note de @Verena)
// 				// capOrNOt();
// 				// try {
// 				// 	getCap();
// 				// } catch (const std::exception &e) {
// 				// 		std::cerr << "End of CAP LS negotiation" << e.what() << std::endl;
// 				// }
// 				// return ;
// 			}
// 			else if ((token.compare("CAP") == 0) && (content.compare("LS") != 0)) {
// 				std::cerr << ERRMSG << RES << content << RED << " is not an accepted command" << RES << std::endl;
// 				return;
// 			}
// 		}
// 		// else if (token.size() != 0 && content.size() == 0) {

// 		//ca ne rentre pas dedant ???? @Verena
// 		else if (token.size() != 0 && content.size() == 0) {			
// 			std::cout << "I am in 96" << std::endl;
// 			if (token == "JOIN")
// 				std::cout << "join us on : " << content << std::endl;
// 			else if (token == "NICK")
// 				std::cout << "nickname : " << std::endl;
// 			else if (token == "USER")
// 				std::cout << "user name : " << std::endl;
// 			else if (token == "PART")
// 				std::cout << "part + arg ? : " << std::endl;
// 			else if (token == "PRIVMSG")
// 				std::cout << "private msg : " << std::endl;
// 			else if (token == "NOTICE")
// 				std::cout << "notice (private msg also) : " << std::endl;
// 			else if (token == "KICK")
// 				std::cout << "kick : " << std::endl;
// 			else if (token == "INVITE")
// 				std::cout << "invite : " << std::endl;
// 			else if (token == "TOPIC")
// 				std::cout << "topic : " << std::endl;
// 			else if (token == "MODE")
// 				std::cout << "mode (+ i, t, k, o or l) : " << std::endl;
// 			else if (token.compare("PONG :")) {
// 				std::cout << "pong ? 118: " << std::endl;
// 				return ;
// 			}
// 			else if (token.compare("PING")) {
// 			// recuperer ce qui a apres le ping et le renvoyer apres le pong parsePing
// 			std::cout << "PONG : " << buf << std::endl;
// 			return ;
// 			}
// 			else
// 				std::cout << "I don't understand this command." << std::endl;
// 		}
// 		else if (token.size() != 0 && content.size() != 0)
// 			std::cout << "I need a working content to handle" << std::endl;
// 		}
// 	// else
// 	// 	throw Server::ServException(ERRMSG"pas content");
// }

void	Server::sendFromClient(char *buf, Client *client)
{
	// std::string	token(buf);
	std::string	splitStr;
	// std::string	command;

	if (token.size() <= 0)
	{
		std::cout << "ERREUR, pas de donnee buff = : " << buf << std::endl; // NEEDS IMPROVEMENT
		return ;
	}

	// welcomeMsg(buf, fdClient);


	std::cout << "TOKEN CLIENT: " << token << std::endl; // DEBUG ONLY
	if (token.compare("PONG :")) {
		std::cout << "pong ? : " << std::endl;
		token.clear();
		// sendMsg("PING", fdClient);
	}
	if (strstr(buf, "PROUT") != 0) {
		std::cout << "nickname token commands on peut faire un truc ici: " << std::endl;
		parseNick(buf, client); //ou Server L.411
	}	
	std::cout << "I am in 251" << std::endl;
	if (token == "JOIN")
		std::cout << "join us on : " << std::endl;
	if (token.compare("NICK") || token.compare("nick")) {
		parseNick(buf, client);
		std::cout << "nickname : " << client->getNick() << std::endl;
		// verifier si le nickname existe deja dans le channel si on est dans un channel sinon
		client->getNick() = token;
		std::cout << "nickname : " << client->getNick() << std::endl;
	}

	if (token == "USER")
		std::cout << "user name : " << std::endl;
	if (token == "PART")
		std::cout << "part + arg ? : " << std::endl;
	if (token == "PRIVMSG")
		std::cout << "private msg : " << std::endl;
	if (token == "NOTICE")
		std::cout << "notice (private msg also) : " << std::endl;
	if (token == "KICK")
		std::cout << "kick : " << std::endl;
	if (token == "INVITE")
		std::cout << "invite : " << std::endl;
	if (token == "TOPIC")
		std::cout << "topic : " << std::endl;
	if (token == "MODE")
		std::cout << "mode (+ i, t, k, o or l) : " << std::endl;
	if (token.compare("PING")) {
	// recuperer ce qui a apres le ping et le renvoyer apres le pong parsePing
	std::cout << "PONG : " << buf << std::endl;
	}
	else
		std::cout << "I don't understand this command." << std::endl;
	// else
	// 	throw Server::ServException(ERRMSG"pas content");
}