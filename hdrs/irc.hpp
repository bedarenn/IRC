#pragma once

# include <string>
# include <iostream>

# include "ErrorInt.hpp"

typedef int	w_fd;

typedef int			w_port;
typedef std::string	w_pass;

typedef std::ostream	w_ostr;
typedef std::istream	w_istr;

extern int g_loop;

# define ERROR		"error: <" << __FILE__ << "::l" << __LINE__ << ">"
# define ERROR_X(x)	"error: <" << __FILE__ << "::l" << __LINE__ << "> : " << x
# define ERROR_XY(x, y)	y << " error: <" << __FILE__ << "::l" << __LINE__ << "> : " << x
