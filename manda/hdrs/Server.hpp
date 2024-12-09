#pragma once

# include "types/Server.hpp"

# include "Channel.hpp"
# include "Command.hpp"

class Server {
public:
	Server(const std::string& name, w_port port, w_pass pass);
	~Server();
	Server(const Server& cpy);

	Server&	operator=(const Server& cpy);

	void	init_server();

	void	run();
	void	event();

	void	connect();
	ssize_t	read(const w_fd& fd);
	void	exec(w_fd fd, std::string str, Client *client);

	void	join(const w_fd& fd, const std::string& channel, const std::string& pass);
	void	invite(const w_fd& fd, const std::string& channel, const std::string& client);
	void	kick(const w_fd& fd, const std::string& channel, const std::string& client, const std::string& msg);
	void	topic(const w_fd& fd, const std::string& channel, const std::string& value);
	void	mode(const w_fd& fd, const std::string& channel, const std::string& md, const std::string& arg);
	void	send_chan(const w_fd& fd, const std::string& chan, const std::string& str) const;
	void	send_priv(const w_fd& fd, const std::string& priv, const std::string& str) const;
	void	pong(const w_fd& fd, const std::string& token) const;
	void	part(const w_fd& fd, const std::string& channel, const std::string& str);
	void	quit(const w_fd& fd, const std::string& str);

	void	new_fd(const w_fd& socket);
	void	new_client(const std::string& name, const std::string& nickname, const w_fd& fd);
	void	new_client_pass(const w_fd& fd, const std::string pass);
	void	new_client_name(const w_fd& fd, const std::string name);
	void	new_client_nick(const w_fd& fd, const std::string nick);
	void	rm__client(const w_fd fd);

	bool	join__channel(Client *client, const std::string& channel, const std::string& pass);

	void	new_map_Channel(Client *client, const std::string& channel);

	w_port		get_port() const;
	w_pass		get_pass() const;
	std::string get_name() const;

	Client	*get_client(const w_fd& fd) const;
	const w_map_Client::const_iterator	get_client(const std::string& name) const;

	void	close_fd(const w_fd& fd);

private:
	std::string	_name;
	w_port		_port;
	w_pass		_pass;

	w_vect_pollfd	_fds;
	w_map_Client	_client;
	w_map_Channel	_channel;

	char	buff[BUFFER_SIZE];

	Server();
};
