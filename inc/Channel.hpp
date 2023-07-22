#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Server.hpp"
// # include "Client.hpp"

class Channel
{
	public:
			Channel();
			Channel(std::string name);
			Channel(Channel const &cpy);
			Channel &operator=(Channel const &rhs);
			~Channel();

			std::string	getNameChannel();
			void	addMembers(Client *client);
			// Channel* addChannel(std::string name);


	private:
			std::string 				_nameMembers;
			std::vector<Client>			_members;
			std::string 				_nameChannel;

			std::vector<int>			_operators;
			std::vector<int>			_banned;
};

#endif
