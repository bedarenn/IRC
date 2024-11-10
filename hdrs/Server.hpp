#pragma once

class Server;

# include <exception>
# include <string>

# include <sys/select.h>
# include <sys/resource.h>

# include "FD.hpp"

typedef struct rlimit	t_rlimit;

class Server {
public:
	Server(const char *port);
	~Server();
	Server(const Server& other);

	Server&	operator=(const Server& other);

	class ErrorInt;
	class ErrorVoid;

private:
	FD		*_fds;
	int		_port;
	int		_maxfd;
	int		max;
	int		r;
	fd_set	fd_read;
	fd_set	fd_write;

	Server();
};

class	Server::ErrorInt : public std::exception {
public:
	ErrorInt(const std::string& str);
	~ErrorInt() throw();

	const char	*what() const throw();
private:
	std::string	msg;
};

class	Server::ErrorVoid : public std::exception {
public:
	ErrorVoid(const std::string& str);
	~ErrorVoid() throw();

	const char	*what() const throw();
private:
	std::string	msg;
};