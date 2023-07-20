#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Server.hpp"

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
			Channel* addChannel(std::string name);
			

	private:
			std::string 				_nameMembers;
			std::vector<Channel>		_members;
			std::vector<Channel>		_channels;
			std::string 				_nameChannel;


};

#endif
