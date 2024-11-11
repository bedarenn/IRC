#pragma once

class Server;

# include <sys/resource.h>
# include <sys/select.h>

typedef struct rlimit	w_rlimit;

typedef int				w_port;

typedef fd_set			w_fd_set;
typedef int				w_size_fd;
