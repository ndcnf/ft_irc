#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
// #include "../inc/Messages.hpp"
#include "../inc/Channel.hpp"

Channel::Channel(){
	_nameChannel = "";
}

Channel::Channel(std::string name): _nameChannel(name) {

}

Channel::Channel(Channel const &cpy){
	*this = cpy;
}

Channel &Channel::operator=(Channel const &rhs){
	_nameChannel = rhs._nameChannel;

	return (*this);
}

Channel::~Channel(){

}

std::string	Channel::getNameChannel()
{
	return (_nameChannel);
}

 void Channel::addMembers(Client *client)
{
	if (client->isAuthenticated())
		_members.push_back(client->getNick());
		// std::cout << "new member" << std::endl;
	else {
		
		return;
	}
}

Channel* Channel::addChannel(std::string name)
{
	Channel* channel = new Channel(name); // Allouer dynamiquement un nouvel objet Channel
	_channels.push_back(*channel);
	return channel;
}