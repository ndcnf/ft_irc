#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h> // sockaddr_in, IPPROTO_TCP
# include <arpa/inet.h> // hton*, ntoh*, inet_addr
# include <unistd.h>  // close
# include <cerrno> // errors
# include <stdexcept>
# include <vector>
# include <map>
# include <fcntl.h>

// # include <sys/poll.h>

# define ERROR -1						// Everywhere when -1 means error
// # define BACKLOG 32						// Number of connections allowed on the incoming queue for listen()
// # define TIMEOUT_NO_P -1				// Specifying a negative value in timeout means an infinite timeout (for poll())
// # define TIMEOUT_NO_S NULL				// Specifying a negative value in timeout means an infinite timeout (for select())
// # define TIMEOUT_YES (3 * 60 * 1000)	// 3 minutes
// # define MAX_FD 200						// Number of maximum fds / may be replaced by a vector
// # define ERRMSG "Error: "

class Client
{
	public:
		Client();
		Client(int fd);
		Client(Client const &src);
		Client	&operator=(Client const &rhs);
		~Client();

		int		getFd();
		//autres fonctions a creer probablement @Verena :
		void	logIn();
		void	applyNick();
		void	defineUsername();
		bool	isConnected();

	private:
		int			_fd;
		std::string	_nick;
		std::string	_username;
		bool		_connected; // is the client successfully connected or not?

};

#endif
