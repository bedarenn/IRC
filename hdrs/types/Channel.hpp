#pragma once

class Channel;

# include "types/Client.hpp"

# define W_RPL(s, c) \
	s + " " + c + " "

# define W_RPL_NOTOPIC(channel, client, s) \
	W_RPL(s, "331") + client.get_nickname() + " " + channel + " :No topic is set"
# define W_RPL_TOPIC(channel, client, topic, s) \
	W_RPL(s, "332") + client.get_nickname() + " " + channel + " :" + topic
# define W_RPL_TOPIC_T(channel, client, topic, s) \
	W_RPL(s, "332") + "oui" + " " + channel + " :" + topic
# define W_RPL_TOPICWHOTIME(channel, client, date, s) \
	W_RPL(s, "333") + client.get_nickname() + " " + channel + " " + client.get_name() + " " + date
# define W_RPL_INVITING(client, l_client, channel, s) \
	W_RPL(s, "341") + client.get_nickname() + " " + l_client.get_nickname() + " " + channel
# define W_RPL_NAMREPLY(channel, client, l_client, s) \
	W_RPL(s, "353") + client.get_nickname() + " = " + channel + " :" + l_client.get_nickname()
# define W_RPL_ENDOFNAMES(channel, client, s) \
	W_RPL(s, "366") + client.get_nickname() + " " + channel + " :End of /NAMES list"

# define W_ERR(client, cmd, s, c) \
	W_RPL(s, c) + client.get_nickname() + " " + cmd + " "

# define W_ERR_NOSUCHCHANNEL(client, cmd, s) \
	W_ERR(client, cmd, s, "403") + ":No such channel"
# define W_ERR_NOTONCHANNEL(client, cmd, s) \
	W_ERR(client, cmd, s, "442") + ":You're not on that channel"
# define ERR_USERONCHANNEL(client, cmd, s) \
	W_ERR(client, cmd, s, "443") + ":is already on channel"
# define W_ERR_NEEDMOREPARAMS(client, cmd, s) \
	W_ERR(client, cmd, s, "461") + ":Not enough parameters"
# define W_ERR_CHANNELISFULL(client, cmd, s) \
	W_ERR(client, cmd, s, "471") + ":Cannot join channel (+l)"
# define W_ERR_INVITEONLYCHAN(client, cmd, s) \
	W_ERR(client, cmd, s, "473") + ":Cannot join channel (+i)"
# define W_ERR_BADCHANNELKEY(client, cmd, s) \
	W_ERR(client, cmd, s, "475") + ":Cannot join channel (+k)"
# define W_ERR_CHANOPRIVSNEEDED(client, cmd, s) \
	W_ERR(client, cmd, s, "482") + ":You're not channel operator"

# define JOIN_MSG(channel, client) \
	":" + client.get_nickname() + " JOIN " + channel + " " + client.get_nickname() + " :realname"
# define INVI_MSG(channel, op, client) \
	":" + op.get_nickname() + " INVITE " + client.get_nickname() + " " + channel
# define KICK_MSG(channel, op, client, msg) \
	":" + op.get_nickname() + " KICK " + channel + " " + client.get_nickname() + " " + msg

# define KICK_MSG_DEFAULT "You have been kick form this server."

typedef std::vector<std::string>	w_vect_invite;
