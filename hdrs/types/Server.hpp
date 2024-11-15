#pragma once

class Server;

# include "types/Channel.hpp"

# include <poll.h>
# include <netinet/in.h>
# include <sstream>

typedef std::map<std::string, Channel>	w_channel;

typedef pollfd	w_pollfd;
typedef std::vector<w_pollfd>	w_vect_pollfd;

typedef struct sockaddr_in		w_sockaddr_in;
typedef struct sockaddr_out		w_sockaddr_out;
typedef struct sockaddr			w_sockaddr;
typedef socklen_t				w_socklen;

w_pollfd	set_pollfd(int fd, short int event, short int revent);

# define SRV_NEW_CLIENT(fd)		"new client: " << fd
# define SRV_ERROR_ACCEPT(fd)	"error: client connection: " << fd
# define SRV_ERROR_RECV			"error: recv"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif
