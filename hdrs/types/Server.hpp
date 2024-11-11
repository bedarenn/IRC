#pragma once

class Server;

#include <string>
# include <vector>

# include <poll.h>
# include <sys/socket.h>

# define BUFFSIZE 1000

typedef int	w_fd;

typedef int			w_port;
typedef std::string	w_pass;

typedef pollfd					w_pollfd;
typedef std::vector<w_pollfd>	w_vect_pollfd;

typedef struct sockaddr_in		w_sockaddr_in;
typedef struct sockaddr_out		w_sockaddr_out;
typedef struct sockaddr			w_sockaddr;

typedef socklen_t				w_socklen;

# define SRV_NEW_CLIENT(fd)	"client fd = " << client

# define SRV_ERROR_POLL		"error: <" << __FILE__ << "::l" << __LINE__ << "> : poll"
# define SRV_ERROR_ACCEPT	"error: <" << __FILE__ << "::l" << __LINE__ << "> : accept"
# define SRV_ERROR_RECV		"error: <" << __FILE__ << "::l" << __LINE__ << "> : recv"
# define SRV_ERROR_SOCKET	"error: <" << __FILE__ << "::l" << __LINE__ << "> : socket"
# define SRV_ERROR_BIND		"error: <" << __FILE__ << "::l" << __LINE__ << "> : bind"
# define SRV_ERROR_LISTEN	"error: <" << __FILE__ << "::l" << __LINE__ << "> : listen"
