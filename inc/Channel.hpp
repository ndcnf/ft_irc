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

			void						addMember(Client *client);
			void						removeMember(Client *client, int fd);
			void						setTopic(std::string topic);
			void						sendToAllMembers(std::string msg);

			std::string					getChannelName();
			std::string					getTopic();
			std::string					getAllMembers();
			std::vector<Client*>		getMember();

	private:
			// std::string 				_nameMembers; //a quoi sert-il ?
			std::vector<Client*>		_members;
			std::string 				_channelName;
			std::string					_topic;

			std::vector<int>			_operators;
			std::vector<int>			_banned;
};

#endif
