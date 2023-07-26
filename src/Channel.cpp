#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
// #include "../inc/Messages.hpp"
#include "../inc/Channel.hpp"

Channel::Channel(){
	_channelName = "";
	_topic = "";
}

Channel::Channel(std::string name): _channelName(name) {

}

Channel::Channel(Channel const &cpy){
	*this = cpy;
}

Channel &Channel::operator=(Channel const &rhs){
	_channelName = rhs._channelName;
	_members = rhs._members;
	_operators = rhs._operators;
	_banned = rhs._banned;
	_topic = rhs._topic;

	return (*this);
}

Channel::~Channel(){

}

std::string	Channel::getChannelName()
{
	return (_channelName);
}

std::string	Channel::getTopic()
{
	return (_topic);
}

void	Channel::setTopic(std::string topic)
{
	_topic = topic;
}

void Channel::addMember(Client *client)
{
	// if (client->isAuthenticated())
		_members.push_back(client);
		// std::cout << "new member" << std::endl;
	// else {

		return;
	// }
}

void	Channel::removeMember(Client *client, int fd)
{
	for (std::vector<Client*>::iterator it=_members.begin(); it != _members.end(); it++)
	{
		if (client->getFd() == fd)
		{
			_members.erase(it);
			return;
		}
	}
}


std::string	Channel::getAllMembers()
{
	std::string	allMembers;

	for (std::vector<Client*>::iterator it=_members.begin(); it != _members.end(); it++)
	{
		allMembers += (*it)->getNick() + "@" + (*it)->getHostname() + " ";
	}
	// std::cout << "ALLMEMBERS = [" + allMembers + "]" << std::endl;
	return allMembers;
}

std::vector<Client*>		Channel::getMember()
{
	return _members;
}


// void	Channel::sendToAllMembers(std::string msg)
// {
// 	for (std::vector<Client>::iterator it=_members.begin(); it != _members.end(); it++)
// 	{
// 		sendMsg(msg, (*it).getFd());
// 	}
// }


// Channel* Channel::addChannel(std::string name)
// {
// 	Channel* channel = new Channel(name); // Allouer dynamiquement un nouvel objet Channel
// 	_channels.push_back(*channel);
// 	return channel;
// }