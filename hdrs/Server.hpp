#pragma once

# include "types/Server.hpp"

# include <string>

# include "FD.hpp"

class Server {
public:
	Server(const char *port);
	~Server();
	Server(const Server& other);

	Server&	operator=(const Server& other);

	class ErrorInt;

private:
	FD			*_fds;
	w_port		_port;
	w_size_fd	_maxfd;
	int			max;
	int			r;
	fd_set		fd_read;
	fd_set		fd_write;

	Server();
};

# include "exceptions/Server.tpp"
