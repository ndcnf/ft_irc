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
# include <fcntl.h>

# include <sys/poll.h>


// # include "Socket.hpp"

# define ERROR -1						// Everywhere when -1 means error
# define BACKLOG 32						// Number of connections allowed on the incoming queue for listen()
# define TIMEOUT_NO -1					// Specifying a negative value in timeout means an infinite timeout
# define TIMEOUT_YES (3 * 60 * 1000)	// 3 minutes
# define MAX_FD 200						// Number of maximum fds / may be replaced by a vector


class Server
{
	public:
		Server();
		Server(Server const &src);
		Server	&operator=(Server const &rhs);
		~Server();

		bool	createSocket();
		bool	pollConnection();
		void	allSockets();



		int		getPort();
		int		getSocket();
		// int		getValidity();
		void	setPort(int port);

	private:
		int					_socket;
		int					_validity;
		int					_port;
		std::vector<int>	_sockets;
		// struct pollfd		_fds[MAX_FD]; // number of fd may be more appropriate in a vector
		std::vector<pollfd>	_cliFds;
		// int					_fdsNum;
		struct sockaddr_in	_addr;


};

#endif
