#include "Client.hpp"

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
