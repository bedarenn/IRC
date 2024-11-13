#pragma once

class Client;

# include "irc.hpp"

# include <vector>

# include <poll.h>

# define BUFFER_SIZE 1024

typedef pollfd	w_pollfd;
w_pollfd		set_pollfd(int fd, short int event, short int revent);

typedef std::vector<w_pollfd>	w_vect_pollfd;

# define SRV_ERROR_RECV			"error: recv"
