#ifndef TCPSOCKET_HPP
# define TCPSOCKET_HPP

# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h> // sockaddr_in, IPPROTO_TCP
# include <arpa/inet.h> // hton*, ntoh*, inet_addr
# include <unistd.h>  // close
# include <cerrno> // errno

class tcpSocket
{
	public:
		tcpSocket();
		~tcpSocket();

		bool	tcpConnect(const std::string &ipAddress, unsigned short port);

	private:
		int		_socket;
};

#endif
