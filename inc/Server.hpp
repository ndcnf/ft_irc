#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h> // sockaddr_in, IPPROTO_TCP
# include <arpa/inet.h> // hton*, ntoh*, inet_addr
# include <unistd.h>  // close
# include <cerrno> // errors
# include <stdexcept>
# include <vector>
# include <map>
# include <fcntl.h>
# include <sys/poll.h>
# include <string>
# include <cstring>
# include <sstream>
# include <algorithm>
# include <poll.h>
# include <cstdlib>

# include "Client.hpp"

# define RED "\e[31m"
# define RES "\e[0m"

# define ERROR -1						// Everywhere when -1 means error
# define BACKLOG 32						// Number of connections allowed on the incoming queue for listen()
# define TIMEOUT_NO_P -1				// Specifying a negative value in timeout means an infinite timeout (for poll())
# define TIMEOUT_NO_S NULL				// Specifying a negative value in timeout means an infinite timeout (for select())
# define TIMEOUT_YES (3 * 60 * 1000)	// 3 minutes
# define MAX_FD 200						// Number of maximum fds / may be replaced by a vector
# define ERRMSG RED"Error: " //parce que c est joli
# define END_SEQUENCE "\r\n" //pour temriner les phrases
# define SERVNAME "The MoIRes Connection Server" //nom du serveur
# define CMDNBR 12 //tableau de commandes si besoin de le modifier en terme de nombre

class Server
{
	public:
		Server();
		Server(Server const &src);
		Server	&operator=(Server const &rhs);
		~Server();

		Client						*currentClient;
		std::string					token; //@Verena to have the token
		std::string					command; //@Verena ... to arrete de se faire chier

		bool		createSocket();
		bool		connection();
		Client*		addClient(int fd);
		void		inputClient(char *buf, Client *client);
		void		cmdSelection(char *buf, Client *client);
		//@Verena CAP LS
		// void	capOrNOt(char *buf, int clientSocket);
		void		capOrNOt(Client *client);
		std::vector<std::string> getCap();
		// bool	selectConnection();
		void		allSockets();					// useless at the moment
		// void	errorminator();					// TBD

		int			getPort();
		int			getSocket();
		void		setPort(int port);
		std::string	getPassword();//@Verena to print the password entered
		void		setPassword(std::string pass);
		Client		getClient(Client *client);

		//COMMNANDS
		void		parseNick(char *buf, Client *client);
		void		parseUser(char *buf, Client *client);
		void		parseCommand(char* buf);
		void		sendMsg(std::string message, int fd);
		void		getCapLs(char *buf);
		void		getPing(char *buf, Client *client);
		void		first_message(Client *client);
		void		sendMsgServer(Client *Client); // pas encore ecrite correctement donc ca ne compilera pas avec
		void		sendErrMsgServer(int errorCode, Client *client); // pas encore ecrite correctement donc ca ne compilera pas avec

		//COMMANDS CALL
		void	commands(std::string cmd, Client *client);

		class ServException : public std::exception {
			public:
				ServException(const char* msg) : _msg(msg) {}
				const char* what() const throw() {
					return _msg;
				}

			private:
				const char* _msg;
		};

	private:
		int							_socket;
		int							_validity;
		int							_port;
		struct sockaddr_in			_addr;
		std::vector<int>			_sockets;	// maybe useless
		std::vector<pollfd>			_pfds;
		std::vector<Client>			_clients;	// vecteur clients pour faire le lien entre les classes ? @Verena
		// bool						_quit;		// useless at the moment
		std::string					_password; //@Verena to get the password
		int							_lastPing;

		void	NICK(Client *client);
		void	USER(Client *client);
		void	JOIN(Client *client);
		void	MODE(Client *client);
		void	PRIVMSG(Client *client);
		void	NOTICE(Client *client);
		void	TOPIC(Client *client);
		void	PART(Client *client);
		void	KICK(Client *client);
		void	INVITE(Client *client);
		void	PASS(Client *client);
		void	QUIT(Client *client);

		std::string _cmdArray[CMDNBR];
};

#endif
