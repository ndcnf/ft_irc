#ifndef MESSAGES_HPP
# define MESSAGES_HPP

# include "Server.hpp"
# include "Client.hpp"

# define RPL_WELCOME  "001 " + client->getNick() "\033[34m*: Welcome on " +  SERVNAME + "!~" + client->getNick() + "@" + client->gethostname() + END_SEQUENCE + RES

# define ERR_NONICKNAMEGIVEN(source)		                "431 " + source + " :No nickname given"
# define ERR_NICKNAMEINUSE(source, new_nick)		        "433 " + source + " " + new_nick + " :Nickname is already in use"
# define ERR_ALREADYREGISTERED(source)		                "462 " + source + " :You may not reregister"
# define ERR_PASSWDMISMATCH(source)		                    "464 " + source + " :Password is incorrect"

# define ERR_NEEDMOREPARAMS(source, command)	            "461 " + source + " " + command + " :Not enough parameters"

# define ERR_NOSUCHCHANNEL(client, channel)	                "403 " + client + " " + channel + " :No such channel"
# define ERR_NOTONCHANNEL(client, channel)	                "442 " + client + " " + channel + " :You're not on that channel"
# define ERR_USERNOTINCHANNEL(client, cgetNick(), channel)  "441 " + client + " " + nick + " " + channel + " :They aren't on that channel"
# define ERR_BADCHANNELKEY(client, channel)                 "475 " + client + " " + channel + " :Cannot join channel (+k)"
# define ERR_INVITEONLYCHAN(client, channel)	            "473 " + client + " " + channel + " :Cannot join channel (+i)"
# define ERR_CHANNELISFULL(client, channel)	                "471 " + client + " " + channel + " :Cannot join channel (+l)"

# define ERR_NOSUCHNICK(client, channel)		            "401 " + client + " " + channel + " :No such nick/channel"
# define ERR_NORECIPIENT(client, command)	                "411 " + client + " :No recipient given " + command
# define ERR_TOOMANYTARGETS(client, command)	            "407 " + client + " :Too many targets " + command
# define ERR_NOTEXTTOSEND(client)			                "412 " + client + " :No text to send"
# define ERR_CHANOPRIVSNEEDED(client, channel)	            "482 " + client + " " + channel + " :You're not channel operator"

# define ERR_NOTREGISTERED(client)			                "451 " + client + " :You have not registered"
# define ERR_USERONCHANNEL(client, nick, channel)	        "443 " + client + " " + nick + " #" + cha


#endif