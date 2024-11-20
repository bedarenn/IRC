#pragma once

class Channel;

# include "types/Client.hpp"

# define W_RPL(s, c) \
	s + " " + c + " "

# define W_RPL_NOTOPIC(channel, client, s) \
	W_RPL(s, "331") + client.get_nickname() + " " + channel + " :No topic is set"
# define W_RPL_TOPIC(channel, client, topic, s) \
	W_RPL(s, "332") + client.get_nickname() + " " + channel + " :" + topic
// RPL_TOPICWHOTIME
# define W_RPL_INVITING(client, l_client, channel, s) \
	W_RPL(s, "341") + client.get_nickname() + " " + l_client.get_nickname() + " " + channel
# define W_RPL_NAMREPLY(channel, client, l_client, s) \
	W_RPL(s, "353") + client.get_nickname() + " = " + channel + " :" + l_client.get_nickname()
# define W_RPL_ENDOFNAMES(channel, client, s) \
	W_RPL(s, "366") + client.get_nickname() + " " + channel + " :End of /NAMES list"

# define JOIN_MSG(channel, client) \
	":" + client.get_nickname() + " JOIN " + channel + " " + client.get_nickname() + " :realname"
# define INVI_MSG(channel, op, client) \
	":" + op.get_nickname() + " INVITE " + client.get_nickname() + " " + channel
# define KICK_MSG(channel, op, client, msg) \
	":" + op.get_nickname() + " KICK " + channel + " " + client.get_nickname() + " " + msg

# define KICK_MSG_DEFAULT "You have been kick form this server."