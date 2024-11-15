#pragma once

# include "types/Channel.hpp"

# include "Client.hpp"

class Channel {
public:
	Channel(const std::string& name, const Client& client);
	~Channel();
	Channel(const Channel& cpy);

	Channel&	operator=(const Channel& cpy);

	void	join(const Client& client);
	void	invite(const Client& op, const Client& client);
	void	kick(const Client& op, const Client& client);
	void	topic(const Client& op, const std::string& value);
	void	mode(const Client& op, const std::string& value);

	void	add_client(const Client& client);
	void	rm__client(const Client& client);

	const std::string&	get_topic() const;
	void	set_topic(const std::string& value);

	bool	empty();

private:
	w_map_Client	_client;
	w_map_Client	_op;

	std::string		_name;
	std::string		_topic;

	bool		_inv_only;
	bool		_topic_right;
	std::string	_pass;
	bool		_op_right;
	size_t		_limit;

	Channel();
};
