#pragma once

# include "types/Channel.hpp"

# include "Client.hpp"

class Channel {
public:
	Channel(const std::string& name, const w_fd& fd);
	~Channel();
	Channel(const Channel& cpy);

	Channel&	operator=(const Channel& cpy);

	bool	join(const Client& client);

private:
	w_map_Client	_client;
	w_map_Client	_op;
	std::string		_topic;

	bool		_inv_only;
	bool		_topic_right;
	std::string	_pass;
	bool		_op_right;
	size_t		_limit;

	Channel();
};
