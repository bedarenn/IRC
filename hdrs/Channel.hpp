#pragma once

# include "types/Channel.hpp"

# include "Client.hpp"

class Channel {
public:
	Channel(const Client& client, const std::string& name, const std::string& server);
	~Channel();
	Channel(const Channel& cpy);

	Channel&	operator=(const Channel& cpy);

	bool	join(const Client& client, const std::string& pass);
	bool	invite(const Client& op, const Client& client);
	bool	kick(const Client& op, const Client& client, const std::string& msg);
	bool	topic(const Client& op, const std::string& value);
	bool	mode(const Client& op, const std::string& value);

	bool	join_pass(const Client& client);
	bool	invite_pass(const Client& op, const Client& client);
	bool	kick_pass(const Client& op, const Client& client, std::string msg);
	bool	topic_pass(const Client& op, const std::string& value);

	bool	add_client(const Client& client);
	bool	rm__client(const Client& client);

	void	cast_send(const std::string& str);
	void	cast_send(const std::string& str, const Client& client);
	void	send_list(const Client& client);

	const std::string&	get_name() const;
	void	set_name(const std::string& value);
	const std::string&	get_topic() const;
	void	set_topic(const std::string& value);

	bool	empty();

private:
	w_map_Client	_client;
	w_map_Client	_invite;
	w_map_Client	_op;

	std::string		_name;
	std::string		_pass;
	std::string		_topic;
	size_t			_limit;

	bool		_inv_only;
	bool		_r_topic;
	bool		_r_op;
	bool		_r_pass;
	bool		_r_limit;

	const std::string&	_server;

	Channel();
};
