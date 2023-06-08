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

	private:
			std::string _nameUser;


};

#endif
