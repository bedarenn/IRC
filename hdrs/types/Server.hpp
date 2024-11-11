#pragma once

class Server;

#include <string>

# include <sys/resource.h>
# include <sys/select.h>

typedef int				w_fd;

typedef int				w_port;
typedef std::string		w_pass;

typedef struct rlimit	w_rlimit;
typedef fd_set			w_fd_set;

typedef int				w_size_fd;
