#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h> // sockaddr_in, IPPROTO_TCP
# include <arpa/inet.h> // hton*, ntoh*, inet_addr
# include <unistd.h>  // close
# include <cerrno> // errors
# include <stdexcept>

// # include <stdlib.h>
// # include <sys/types.h>

// # include <string>
// # include <netinet/in.h>

# define ERROR -1



class Socket
{
	public:
		Socket();
		Socket(Socket const &src);
		Socket	&operator=(Socket const &rhs);
		~Socket();


		// bool	connect(const std::string &ipAddress, unsigned short port);
		int	getSocket();
		int	getValidity();
		

	private:
		int	_socket;
		int	_validity;
};

#endif
