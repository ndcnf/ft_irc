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

# include "Socket.hpp"

class Server
{
	public:
		Server();
		Server(Server const &src);
		Server	&operator=(Server const &rhs);
		~Server();

		int		getPort();
		// int		getSockets();
		void	allSockets();

		void	setPort(int port);


	private:
		int		_port;
		// Socket	_sock();
		// std::queue<Socket*>	_sockets;
		std::vector<Socket>	_sockets;

};


#endif
