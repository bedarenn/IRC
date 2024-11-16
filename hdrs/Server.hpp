#pragma once

# include "types/Server.hpp"

# include "Channel.hpp"
# include "Command.hpp"

class Server {
public:
	Server(w_port port, w_pass pass);
	~Server();
	Server(const Server& cpy);

	Server&	operator=(const Server& cpy);

	void	init_server();

	void	run();
	void	event();

	void	connect();
	void	read(w_vect_pollfd::iterator& poll);

	void	join(const w_fd& client, std::vector< std::pair<std::string, std::string> >);
	void	invite(const w_fd& op, const std::string& channel, const std::string& value);
	void	kick(const w_fd& op, const std::string& channel, const std::string& value);
	void	topic(const w_fd& op, const std::string& channel, const std::string& value);
	void	mode(const w_fd& op, std::vector<std::string> strs);

	void	new_fd(const w_fd& socket);
	void	new_client(const std::string& name, const std::string& nickname, const w_fd& fd);
	void	rm__client(const Client& client);

	bool	join__channel(const Client& client, const std::string& channel);
	bool	leave_channel(const Client& client, const std::string& channel);

	void	new_map_Channel(const std::string& channel, const Client& client);

	w_port		get_port() const;
	w_pass		get_pass() const;

	const Client&	get_client(w_fd fd) const;

private:
	w_port		_port;
	w_pass		_pass;

	w_vect_pollfd	_fds;
	w_map_Client	_client;
	w_map_Channel	_channel;

	char	buff[BUFFER_SIZE];

	Server();
};
