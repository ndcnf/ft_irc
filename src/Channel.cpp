#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
// #include "../inc/Messages.hpp"
#include "../inc/Channel.hpp"

Channel::Channel(){
	_channelName = "";
	_topic = "";
	_topicOperatorsOnly = false;
}

Channel::Channel(std::string name): _channelName(name) {
	_topic = "";
	_topicOperatorsOnly = false;
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
	_topicOperatorsOnly = rhs._topicOperatorsOnly;

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

void	Channel::setTopic(std::string topic, Client *client)
{
	if (_topicOperatorsOnly && (!isOperator(client)))
	{
		//Erreur t'as pas les droits
		return ;
	}
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
	(void)client;
	for (std::vector<Client*>::iterator it=_members.begin(); it != _members.end(); it++)
	{
		if ((*it)->getFd() == fd)
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

void						Channel::setTopicMode(bool mode)
{
	_topicOperatorsOnly = mode;
}

bool						Channel::getTopicMode()
{
	return _topicOperatorsOnly;
}

bool						Channel::setOperator(bool mode, std::string username)
{
	for(std::vector<Client*>::iterator it=_members.begin(); it != _members.end(); it++)
	{
		if (mode)
		{
			if ((*it)->getNick() == username)
				return (addOperator(*it));
		}
		else
		{
			if ((*it)->getNick() == username)
				return (removeOperator(*it));
		}
	}
	return (false);
}

bool				Channel::addOperator(Client *client)
{
	bool		isMember = false;
	std::string	msg;

	for (std::vector<Client*>::iterator it = _members.begin(); it != _members.end(); it++)
	{
		if (client->getFd() == (*it)->getFd())
		{
			isMember = true;
			break ;
		}
	}

	if (isMember)
	{
		for (std::vector<Client*>::iterator itc = _operators.begin(); itc != _operators.end(); itc++)
		{
			if (client->getFd() == (*itc)->getFd())
			{
				std::cout << "already operator :" + (*itc)->getNick() << std::endl; // erreur existe deja ou osef
				return (false);
			}
		}
		_operators.push_back(client);
		return (true);
	}
	// pas un membre donc erreur
	std::cout << "Nice try, not a member : " + client->getNick() << std::endl; // erreur existe deja ou osef
	return (false);

}

bool				Channel::removeOperator(Client *client)
{
	bool		isMember = false;
	std::string	msg;

	for (std::vector<Client*>::iterator it = _members.begin(); it != _members.end(); it++)
	{
		if (client->getFd() == (*it)->getFd())
		{
			isMember = true;
			break ;
		}
	}

	if (isMember)
	{
		for (std::vector<Client*>::iterator itc = _operators.begin(); itc != _operators.end(); itc++)
		{
			if (client->getFd() == (*itc)->getFd())
			{
				if (_operators.size() == 1)
				{
					//dernier operator dans le channel, on refuse
					std::cout << "il faut un admin ici, tu restes." << std::endl;
					return (false);
				}
				std::cout << "operator removed :" + (*itc)->getNick() << std::endl; // erreur existe deja ou osef
				_operators.erase(itc);
				return (true);
			}
		}
	}
	// pas un membre donc erreur
	std::cout << "Nice try, not a member : " + client->getNick() << std::endl; // erreur existe deja ou osef
	return (false);

}


bool		Channel::isOperator(Client *client)
{
	for (std::vector<Client*>::iterator it = _operators.begin(); it != _operators.end(); it++)
	{
		if ((*it)->getFd() == client->getFd())
			return true;
	}
	return false;
}

bool		Channel::isMembre(Client *client)
{
	for (std::vector<Client*>::iterator it = _members.begin(); it != _members.end(); it++)
	{
		if ((*it)->getFd() == client->getFd())
			return true;
	}
	return false;
}

bool		Channel::isNickMembre(std::string nickname)
{
	for (std::vector<Client*>::iterator it = _members.begin(); it != _members.end(); it++)
	{
		if ((*it)->getNick() == nickname)
			return true;
	}
	return false;
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
