#include "tcpSocket.hpp"

tcpSocket::tcpSocket()
{
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == -1)
		std::cout << "ERREUR DE SOCKET !" << std::endl;
	std::cout << "NOUVELLE SOCKET !" << std::endl;
	std::cout << _socket << std::endl;

}

tcpSocket::~tcpSocket()
{
	close(_socket);
}

bool	tcpSocket::tcpConnect(const std::string &ipAddress, unsigned short port)
{
	(void)ipAddress;
	(void)port;
	return (true);
}
