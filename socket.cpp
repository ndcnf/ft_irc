#include "socket.hpp"

socket::socket()
{
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == ERROR)
		std::cout << "ERREUR DE SOCKET !" << std::endl;
	std::cout << "NOUVELLE SOCKET !" << std::endl;
	std::cout << _socket << std::endl;

}

socket::~socket()
{
	close(_socket);
}

bool	socket::connect(const std::string &ipAddress, unsigned short port)
{
	(void)ipAddress;
	(void)port;
	return (true);
}
