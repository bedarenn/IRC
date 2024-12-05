#include <signal.h>

#include "Bot.hpp"

void	sig_quit(int code) {
	g_loop = 0;
	if (code == SIGINT)
		std::cout << std::endl;
}

void	set_sig(void) {
	signal(SIGINT, sig_quit);
}