#pragma once

# include "types/Channel.hpp"

# include <ctime>

# include "Client.hpp"

class Channel {
public:
	Channel(const Client& client, const std::string& name, const std::string& server);
	~Channel();
	Channel(const Channel& cpy);

	Channel&	operator=(const Channel& cpy);

	bool	join(const Client& client, const std::string& pass);
	bool	invite(const Client& op, const std::string& client);
	bool	kick(const Client& op, const Client& client, const std::string& msg);
	bool	topic(const Client& op, const std::string& value);
	bool	mode(const Client& op, const std::string& md, const std::string& arg);
	bool	part(const Client& client, const std::string& str);
	bool	quit(const Client& client, const std::string& str);

	bool	join_pass(const Client& client);
	bool	invite_pass(const Client& op, const std::string& client);
	bool	kick_pass(const Client& op, const Client& client, const std::string& msg);
	bool	topic_pass(const Client& op, const std::string& value);
	bool	mode_pass(const Client& op, const std::string& md, const std::string& arg);

	bool	add_client(const Client& client);
	bool	rm__client(const Client& client);

	bool	is__invite(const std::string& client) const;
	bool	del_invite(const std::string& client);

	bool	mode_i(const Client& op, const std::string& md);
	bool	mode_t(const Client& op, const std::string& md);
	bool	mode_k(const Client& op, const std::string& md, const std::string& arg);
	bool	mode_o(const Client& op, const std::string& md, const std::string& arg);
	bool	mode_l(const Client& op, const std::string& md, const std::string& arg);
	bool	mode_empty(const Client& op);

	bool	is_on_channel(const w_fd& fd) const;
	bool	is_on_channel(const Client& client) const;
	bool	is_on_channel(const std::string& nick) const;

	void	cast_send(const std::string& str) const;
	void	cast_send(const std::string& str, const Client& client) const;
	void	cast_f(void (Channel::*f)(const Client&));

	void	send_topic(const Client& client);

	void	send_list(const Client& client);

	const std::string&	get_name() const;
	void	set_name(const std::string& value);
	const std::string&	get_topic() const;
	void	set_topic(const std::string& value);

	std::string	get_topic_time() const;

	bool	empty();

private:
	w_map_Client	_client;
	w_vect_invite	_invite;
	w_map_Client	_op;

	std::string	_name;
	std::string	_pass;
	size_t		_limit;

	bool		_inv_only;
	bool		_r_topic;
	bool		_r_op;
	bool		_r_pass;
	bool		_r_limit;

	std::string	_topic;
	std::time_t _topic_time;
	Client		_topic_change;

	std::string	_server;

	Channel();
};
