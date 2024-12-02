#pragma once

class Channel;

# include "types/Client.hpp"

/* ***************** FORMAT_BASE ****************** */

# define W_RPL(s, c) \
	s + " " + c + " "
# define W_ERC(client, s, c) \
	W_RPL(s, c) + client.get_nickname() + " "
# define W_ERR(client, cmd, s, c) \
	W_RPL(s, c) + client.get_nickname() + " " + cmd + " "

/* **************** FORMAT_SUCCESS **************** */

# define W_RPL_CHANNELMODEIS_EMPTY(client, channel, s) \
	W_RPL(s, "324") + client.get_nickname() + " " + channel
# define W_RPL_CHANNELMODEIS(client, channel, md, s) \
	W_RPL(s, "324") + client.get_nickname() + " " + channel + " " + md
# define W_RPL_CHANNELMODEIS_ARG(client, channel, md, arg, s) \
	W_RPL(s, "324") + client.get_nickname() + " " + channel + " " + md + " " + arg
# define W_RPL_NOTOPIC(channel, client, s) \
	W_RPL(s, "331") + client.get_nickname() + " " + channel + " :No topic is set"
# define W_RPL_TOPIC(channel, client, topic, s) \
	W_RPL(s, "332") + client.get_nickname() + " " + channel + " :" + topic
# define W_RPL_TOPIC_T(channel, client, topic, s) \
	W_RPL(s, "332") + "oui" + " " + channel + " :" + topic
# define W_RPL_TOPICWHOTIME(channel, client, date, s) \
	W_RPL(s, "333") + client.get_nickname() + " " + channel + " " + client.get_nickname() + " " + date
# define W_RPL_INVITING(client, l_client, channel, s) \
	W_RPL(s, "341") + client.get_nickname() + " " + l_client.get_nickname() + " " + channel
# define W_RPL_NAMREPLY(channel, client, l_client, s) \
	W_RPL(s, "353") + client.get_nickname() + " = " + channel + " :" + l_client
# define W_RPL_ENDOFNAMES(channel, client, s) \
	W_RPL(s, "366") + client.get_nickname() + " " + channel + " :End of /NAMES list"

/* ***************** FORMAT_FAIL ****************** */

# define W_ERR_NOSUCHNICK(client, nick, s) \
	W_ERC(client, s, "432") + nick + " " + ":No such nick/channel"
# define W_ERR_NOSUCHCHANNEL(client, cmd, s) \
	W_ERR(client, cmd, s, "403") + ":No such channel"
# define W_ERR_NONICKNAMEGIVEN(client, s) \
	W_ERC(client, s, "431") + ":No nickname given"
# define W_ERR_ERRONEUSNICKNAME(client, s) \
	W_ERC(client, s, "432") + client.get_nickname() + " " + ":Erroneus nickname"
# define W_ERR_NICKNAMEINUSE(client, s) \
	W_ERC(client, s, "433") + client.get_nickname() + " " + ":Nickname is already in use"
# define W_ERR_USERNOTINCHANNEL(channel, op, client, s) \
	W_ERC(client, s, "441") + op.get_nickname() + " " + client.get_nickname() + " " + channel + " :They aren't on that channel"
# define W_ERR_NOTONCHANNEL(client, cmd, s) \
	W_ERR(client, cmd, s, "442") + ":You're not on that channel"
# define ERR_USERONCHANNEL(client, cmd, s) \
	W_ERR(client, cmd, s, "443") + ":is already on channel"
# define W_ERR_NEEDMOREPARAMS(client, cmd, s) \
	W_ERR(client, cmd, s, "461") + ":Not enough parameters"
# define W_ERR_ALREADYREGISTERED(client, s) \
	W_ERC(client, s, "462") + ":You may not reregister"
# define W_ERR_PASSWDMISMATCH(client, s) \
	W_ERC(client, s, "462") + ":Password incorrect"
# define W_ERR_CHANNELISFULL(client, cmd, s) \
	W_ERR(client, cmd, s, "471") + ":Cannot join channel (+l)"
# define W_ERR_INVITEONLYCHAN(client, cmd, s) \
	W_ERR(client, cmd, s, "473") + ":Cannot join channel (+i)"
# define W_ERR_BADCHANNELKEY(client, cmd, s) \
	W_ERR(client, cmd, s, "475") + ":Cannot join channel (+k)"
# define W_ERR_CHANOPRIVSNEEDED(client, cmd, s) \
	W_ERR(client, cmd, s, "482") + ":You're not channel operator"

/* ****************** FORMAT_CMD ****************** */

# define JOIN_MSG(channel, client) \
	client.get_nickname() + " JOIN " + channel
# define INVI_MSG(channel, op, client) \
	op.get_nickname() + " INVITE " + client.get_nickname() + " " + channel
# define KICK_MSG_MSG(channel, op, client, msg) \
	op.get_nickname() + " KICK " + channel + " " + client.get_nickname() + " " + msg
# define KICK_MSG(channel, op, client) \
	op.get_nickname() + " KICK " + channel + " " + client.get_nickname()
# define PART_MSG_MSG(channel, client, msg) \
	client.get_nickname() + " PART " + channel + " " + client.get_nickname() + " " + msg
# define PART_MSG(channel, client) \
	client.get_nickname() + " PART " + channel + " " + client.get_nickname()
# define MODE_MSG(channel, op, md, s) \
	op.get_nickname() + " MODE " + channel + " " + md
# define MODE_MSG_ARG(channel, op, md, arg, s) \
	op.get_nickname() + " MODE " + channel + " " + md + " " + arg
# define PRIV_MSG(client, target, str) \
	client.get_nickname() + " PRIVMSG " + target + " " + str
# define PONG_MSG(token) \
	"PONG " + token
# define QUIT_MSG(str) \
	"QUIT " + str

# define _DEFAULT "You have been kick form this server."

typedef std::vector<std::string>	w_vect_invite;
