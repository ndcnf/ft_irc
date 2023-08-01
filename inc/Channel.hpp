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
			void						setTopic(std::string topic, Client *client);
			void						setTopicMode(bool mode);
			void						addOperator(Client *client);
			void						sendToAllMembers(std::string msg);

			std::string					getChannelName();
			std::string					getTopic();
			std::string					getAllMembers();
			std::vector<Client*>		getMember();
//			void						addInvite(std::string *invited);
			bool						isMember(Client *client);
			bool 						isMember(const std::string& nickname);
			bool						getTopicMode();
			bool						isOperator(Client *client);

	private:
			// std::string 				_nameMembers; //a quoi sert-il ?
			std::vector<Client*>		_members;
			std::vector<Client*>		_invite;
			std::string 				_channelName;
			std::string					_topic;

			std::vector<Client*>		_operators;
			std::vector<int>			_banned;
			bool						_topicOperatorsOnly;
};

#endif
