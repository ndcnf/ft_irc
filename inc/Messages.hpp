#ifndef MESSAGES_HPP
# define MESSAGES_HPP

# include "Server.hpp"
# include "Client.hpp"
# include "Channel.hpp"

# define RPL_WELCOME  "001 " + client->getNick() "\033[34m*: Welcome on " +  SERVNAME + "!~" + client->getNick() + "@" + client->gethostname() + END_SEQUENCE + RES

# define ERR_NOSUCHNICK "401 :No such nick/channel"
# define ERR_NOSUCHCHANNEL  "403 :No such channel"
# define ERR_NONICKNAMEGIVEN    "431 :No nickname given"
# define ERR_NICKNAMEINUSE  "433 :Nickname is already in use"
# define ERR_ALREADYREGISTERED  "462 :You may not reregister"
# define ERR_PASSWDMISMATCH "464 :Password is incorrect"

# define ERR_NEEDMOREPARAMS "461 :Not enough parameters"

# define ERR_NOTONCHANNEL   "442 :You're not on that channel"
# define ERR_USERNOTINCHANNEL   "441 :They aren't on that channel"
# define ERR_BADCHANNELKEY  "475 :Cannot join channel (+k)"
# define ERR_INVITEONLYCHAN "473 :Cannot join channel (+i)"
# define ERR_CHANNELISFULL  "471 :Cannot join channel (+l)"

# define ERR_NORECIPIENT    "411 :No recipient given"
# define ERR_TOOMANYTARGETS "407 :Too many targets "
# define ERR_NOTEXTTOSEND   "412 :No text to send"
# define ERR_CHANOPRIVSNEEDED   "482 :You're not channel operator"

# define ERR_NOTREGISTERED  "451 :You have not registered"
# define ERR_USERONCHANNEL  "443 "


#endif