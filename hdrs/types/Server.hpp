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

# define SRV_NEW_CLIENT(fd)		"new client: " << fd
# define SRV_ERROR_ACCEPT(fd)	"error: client connection: " << fd

# define SRV_ERROR_POLL		"error: poll"
# define SRV_ERROR_RECV		"error: recv"

# define SRV_ERROR		"error: <" << __FILE__ << "::l" << __LINE__ << ">"
# define SRV_ERROR_X(x)	"error: <" << __FILE__ << "::l" << __LINE__ << "> : " << x
# define SRV_ERROR_XY(x, y)	y << " error: <" << __FILE__ << "::l" << __LINE__ << "> : " << x
