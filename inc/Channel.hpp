#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Server.hpp"

class Channel
{
	public:
			Channel();
			Channel(Channel const &cpy);
			Channel &operator=(Channel const &rhs);
			~Channel();

			std::string	getNameChannel();
			void	addMembers(Client *client);
			

	private:
			std::string 				_nameMembers;
			std::vector<std::string>	_members;
			std::string 				_nameChannel;


};

#endif
