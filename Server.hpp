#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h> // sockaddr_in, IPPROTO_TCP
# include <arpa/inet.h> // hton*, ntoh*, inet_addr
# include <unistd.h>  // close
# include <cerrno> // errors
# include <stdexcept>
// # include <queue>
# include <vector>
# include <map>
# include <fcntl.h>

# include <sys/poll.h>


// # include "Socket.hpp"

# define ERROR -1						// Everywhere when -1 means error
# define BACKLOG 32						// Number of connections allowed on the incoming queue for listen()
# define TIMEOUT_NO_P -1				// Specifying a negative value in timeout means an infinite timeout (for poll())
# define TIMEOUT_NO_S NULL				// Specifying a negative value in timeout means an infinite timeout (for select())
# define TIMEOUT_YES (3 * 60 * 1000)	// 3 minutes
# define MAX_FD 200						// Number of maximum fds / may be replaced by a vector
# define ERRMSG "Error: "

class Server
{
	public:
		Server();
		Server(Server const &src);
		Server	&operator=(Server const &rhs);
		~Server();

		bool	createSocket();
		bool	connection();
		void	inputClient(char *buf);
		void	cmdSelection(char *buf);
		// bool	selectConnection();
		void	allSockets();					// useless at the moment
		// void	errorminator();					// TBD

		int		getPort();
		int		getSocket();
		void	setPort(int port);

	private:
		int							_socket;
		int							_validity;
		int							_port;
		struct sockaddr_in			_addr;
		std::vector<int>			_sockets;	// maybe useless
		std::vector<pollfd>			_pfds;
		std::map<int, sockaddr_in>	_clients;	// useless at the moment
		// bool						_quit;		// useless at the moment
};

#endif
