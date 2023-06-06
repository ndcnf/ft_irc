#include "../inc/Client.hpp"
// #include "Server.hpp"

Client::Client():	_fd(0),
					_connected(false)
{

}

Client::Client(int fd):	_fd(fd),
						_connected(false)
{

}

Client::Client(Client const &src)
{
	*this = src;
}

Client	&Client::operator=(Client const &rhs)
{
	_fd = rhs._fd;
	_nick = rhs._nick;
	_username = rhs._username;
	_channel = rhs._channel;
	_connected = rhs._connected;

	return (*this);
}

Client::~Client()
{

}

int		Client::getFd()
{
	return (_fd);
}

std::string	Client::getNick()
{
	return _nick;
}

// void	Client::setNick(std::string nick) const
// {
// 	std::string commande = std::cin
// 	if commmande.compare("/nick");
// 	std::cout << "You're now known as " << nick << std::endl;
// 	_username = nick;

// 	_nick = nick;
// }
