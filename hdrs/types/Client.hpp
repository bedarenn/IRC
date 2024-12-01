#pragma once

class Client;

# include "irc.hpp"

# include <vector>
# include <map>

#include <sys/types.h>

typedef std::vector<std::string>	w_map_Channel_list;

# define L_STR_CLT	<w_fd, Client>
typedef std::pair L_STR_CLT	w_pair_Client;
typedef std::map L_STR_CLT	w_map_Client;

# define W_SND(str) \
	":" + str + "\r\n"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 2048
# endif
