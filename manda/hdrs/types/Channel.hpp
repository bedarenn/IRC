#pragma once

class Channel;

# include "types/Client.hpp"

/* ***************** FORMAT_BASE ****************** */

# define W_RPL(s, c) \
	s + " " + c + " "
# define W_RPL_CLT(clt, s, c) \
	W_RPL(s, c) + clt->get_nickname() + " "
# define W_RPL_CHN(clt, chn, s, c) \
	W_RPL_CLT(clt, s, c) + chn + " "

/* **************** FORMAT_SUCCESS **************** */

# define W_RPL_CHANNELMODEIS_EMPTY(clt, chn, s) \
	W_RPL_CLT(clt, s, "324") + chn
# define W_RPL_CHANNELMODEIS(clt, chn, md, s) \
	W_RPL_CHANNELMODEIS_EMPTY(clt, chn, s) + " " + md
# define W_RPL_CHANNELMODEIS_ARG(clt, chn, md, arg, s) \
	W_RPL_CHANNELMODEIS(clt, chn, md, s) + " " + arg
# define W_RPL_NOTOPIC(clt, chn, s) \
	W_RPL_CHN(clt, chn, s, "331") + ":No topic is set"
# define W_RPL_TOPIC(clt, chn, tpc, s) \
	W_RPL_CHN(clt, chn, s, "332") + ":" + tpc
# define W_RPL_INVITING(clt, nck, chn, s) \
	W_RPL_CLT(clt, s, "341") + nck + " " + chn
# define W_RPL_NAMREPLY(clt, chn, nck, s) \
	W_RPL_CLT(clt, s, "353") + "= " + chn + " :" + nck
# define W_RPL_ENDOFNAMES(clt, chn, s) \
	W_RPL_CHN(clt, chn, s, "366") + ":End of /NAMES list"

/* ***************** FORMAT_FAIL ****************** */

# define W_ERR_NOSUCHNICK(clt, nck, s) \
	W_RPL_CHN(clt, nck, s, "401") + ":No such nick/channel"
# define W_ERR_NOSUCHCHANNEL(clt, chn, s) \
	W_RPL_CHN(clt, chn, s, "403") + ":No such channel"
# define W_ERR_NONICKNAMEGIVEN(clt, s) \
	W_RPL_CLT(clt, s, "431") + ":No nickname given"
# define W_ERR_ERRONEUSNICKNAME(clt, nck, s) \
	W_RPL_CHN(clt, nck, s, "432") + ":Erroneus nickname"
# define W_ERR_NICKNAMEINUSE(clt, nck, s) \
	W_RPL_CHN(clt, nck, s, "433") + ":Nickname is already in use"
# define W_ERR_USERNOTINCHANNEL(clt, chn, nick, s) \
	W_RPL_CLT(clt, s, "441") + nick + " " + chn + " :They aren't on that channel"
# define W_ERR_NOTONCHANNEL(clt, chn, s) \
	W_RPL_CHN(clt, chn, s, "442") + ":You're not on that channel"
# define W_ERR_USERONCHANNEL(clt, nck, chn, s) \
	W_RPL_CLT(clt, s, "443") + nck + " " + chn + " :is already on channel"
# define W_ERR_NEEDMOREPARAMS(clt, cmd, s) \
	W_RPL_CHN(clt, cmd, s, "461") + ":Not enough parameters"
# define W_ERR_ALREADYREGISTERED(clt, s) \
	W_RPL_CLT(clt, s, "462") + ":You may not reregister"
# define W_ERR_PASSWDMISMATCH(clt, s) \
	W_RPL_CLT(clt, s, "464") + ":Password incorrect"
# define W_ERR_CHANNELISFULL(clt, chn, s) \
	W_RPL_CHN(clt, chn, s, "471") + ":Cannot join channel (+l)"
# define W_ERR_INVITEONLYCHAN(clt, chn, s) \
	W_RPL_CHN(clt, chn, s, "473") + ":Cannot join channel (+i)"
# define W_ERR_BADCHANNELKEY(clt, chn, s) \
	W_RPL_CHN(clt, chn, s, "475") + ":Cannot join channel (+k)"
# define W_ERR_BADCHANMASK(chn, s) \
	W_RPL(s, "476") + chn + " :Bad Channel Mask"
# define W_ERR_CHANOPRIVSNEEDED(clt, chn, s) \
	W_RPL_CHN(clt, chn, s, "482") + ":You're not channel operator"
# define ERR_INVALIDMODEPARAM(clt, chn, mod, arg, des, s) \
	W_RPL_CHN(clt, chn, s, "696") + mod + " " + arg + " :" + des

/* ****************** FORMAT_CMD ****************** */

# define JOIN_MSG(channel, client) \
	client->get_nickname() + " JOIN " + channel
# define INVI_MSG(channel, op, client) \
	op->get_nickname() + " INVITE " + client->get_nickname() + " " + channel
# define KICK_MSG_MSG(channel, op, client, msg) \
	op->get_nickname() + " KICK " + channel + " " + client->get_nickname() + " " + msg
# define KICK_MSG(channel, op, client) \
	op->get_nickname() + " KICK " + channel + " " + client->get_nickname()
# define PART_MSG_MSG(channel, client, msg) \
	client->get_nickname() + " PART " + channel + " " + client->get_nickname() + " " + msg
# define PART_MSG(channel, client) \
	client->get_nickname() + " PART " + channel + " " + client->get_nickname()
# define MODE_MSG(channel, op, md, s) \
	op->get_nickname() + " MODE " + channel + " " + md
# define MODE_MSG_ARG(channel, op, md, arg, s) \
	op->get_nickname() + " MODE " + channel + " " + md + " " + arg
# define PRIV_MSG(client, target, str) \
	client->get_nickname() + " PRIVMSG " + target + " " + str
# define PONG_MSG(token) \
	"PONG " + token
# define QUIT_MSG(client) \
	client->get_nickname() + " QUIT"
# define QUIT_MSG_MSG(client, str) \
	client->get_nickname() + " QUIT :" + str

# define _DEFAULT "You have been kick form this server."

typedef std::vector<std::string>	w_vect_invite;
