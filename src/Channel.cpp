#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/Messages.hpp"
#include "../inc/Channel.hpp"

Channel::Channel(){

}

Channel::Channel(Channel const &cpy){

}

Channel &Channel::operator=(Channel const &rhs){

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
		// _members.push_back(client);
        std::cout << "new member" << std::endl;
    else {
		
		return;
	}
}