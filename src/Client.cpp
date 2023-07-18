#include "../inc/Client.hpp"
// #include "Server.hpp"

Client::Client():	_fd(0),
					_connected(false)
{
		_username = "";
		_nick = "";
		_channel= "";
		_hostname= "";
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
	_hostname = rhs._hostname;

	return (*this);
}

Client::~Client()
{

}

int		Client::getFd() const
{
	return (_fd);
}

std::string	Client::getNick() const
{
	return _nick;
}

void	Client::setNick(std::string nick) {
	// si le nickname n existe pas deja?
	//_username = nick;// pas sure de ca
	_nick = nick;
}

void		Client::setUser(std::string user) {
			_username = user;
		}

std::string	Client::getUser() {
			return _username;
		}

// std::string	Client::getHostname() {
// 			return _hostname;
// 		}

std::string Client::getHostname() {
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) == 0) {
		return std::string(hostname);
	}
	return "";
}