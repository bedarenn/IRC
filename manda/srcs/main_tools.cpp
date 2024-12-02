#include <utility>
#include <cstdlib>
#include <signal.h>

#include "irc.hpp"

bool	test_str(const std::string& str) {
	if (str.empty())
		return (false);
	for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
		if (std::isspace(*it))
			return (false);
	}
	return(true);
}

bool	parse(char **av) {
	w_port		port = 0;

	if (!test_str(av[1]) || !test_str(av[2])) {
		std::cerr << "error: argument empty" << std::endl;
		return(false);
	}
	port = atoi(av[1]);
	if(port < 0 || port > 65535){
		std::cerr << "error: port not allowed" << std::endl;
		return (0);
	}
	return(1);
}

void	sig_quit(int code) {
	g_loop = 0;
	if (code == SIGINT || code == SIGQUIT)
		std::cout << std::endl;
}

void	set_sig(void) {
	signal(SIGINT, sig_quit);
	signal(SIGQUIT, sig_quit);
}