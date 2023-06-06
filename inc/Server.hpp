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
# define ERRMSG RED"Error: "


class Server
{
	public:
		Server();
		Server(Server const &src);
		Server	&operator=(Server const &rhs);
		~Server();

		bool	createSocket();
		bool	connection();
		bool	addClient(int fd);
		void	inputClient(char *buf);
		void	cmdSelection(char *buf);
		//@Verena CAP LS
		void		capOrNOt(int clientSocket);
		std::vector<std::string> getCap();
		// bool	selectConnection();
		void	allSockets();					// useless at the moment
		// void	errorminator();					// TBD

		int			getPort();
		int			getSocket();
		void		setPort(int port);
		std::string	getPassword();//@Verena to print the password entered
		void		setPassword(std::string pass) ;

		//COMMNANDS
		std::string	parsePing(std::string token, int clientSocket);

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
		std::vector<Client>			_clients;	// useless at the moment, maybe now?
		// bool						_quit;		// useless at the moment
		std::string					_password; //@Verena to get the password
};

#endif
