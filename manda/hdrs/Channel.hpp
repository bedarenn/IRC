#pragma once

# include "types/Channel.hpp"

# include <ctime>

# include "Client.hpp"

class Channel {
public:
	Channel(Client *client, const std::string& name, const std::string& server);
	~Channel();
	Channel(const Channel& cpy);

	Channel&	operator=(const Channel& cpy);

	bool	join(Client *client, const std::string& pass);
	bool	invite(Client *op, const std::string& client);
	bool	kick(Client *op, Client *client, const std::string& msg);
	bool	topic(Client *op, const std::string& value);
	bool	mode(Client *op, const std::string& md, const std::string& arg);
	bool	part(Client *client, const std::string& str);
	bool	quit(Client *client, const std::string& str);

	bool	join_pass(Client *client);
	bool	invite_pass(Client *op, const std::string& client);
	bool	kick_pass(Client *op, Client *client, const std::string& msg);
	bool	topic_pass(const std::string& value);
	bool	mode_pass(Client *op, const std::string& md, const std::string& arg);

	bool	add_client(Client *client);
	bool	rm__client(Client *client);

	bool	is__invite(const std::string& client) const;
	bool	del_invite(const std::string& client);

	bool	mode_i(Client *op, const std::string& md);
	bool	mode_t(Client *op, const std::string& md);
	bool	mode_k(Client *op, const std::string& md, const std::string& arg);
	bool	mode_o(Client *op, const std::string& md, const std::string& arg);
	bool	mode_l(Client *op, const std::string& md, const std::string& arg);
	bool	mode_empty(Client *op);

	bool	is_on_channel(const w_fd& fd) const;
	bool	is_on_channel(Client *client) const;
	bool	is_on_channel(const std::string& nick) const;

	void	cast_send(const std::string& str) const;
	void	cast_send(const std::string& str, Client *client) const;
	void	cast_f(void (Channel::*f)(Client *));

	void	send_topic(Client *client);

	void	send_list(Client *client);

	const std::string&	get_name() const;
	void	set_name(const std::string& value);
	const std::string&	get_topic() const;
	void	set_topic(const std::string& value);

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
	bool		_r_pass;
	bool		_r_limit;

	std::string	_topic;

	std::string	_server;

	Channel();
};
