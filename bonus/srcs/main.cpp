#include "Bot.hpp"

int g_loop;

bool	parse(char **av);
void	set_sig(void);

int	main(int ac, char **av){

	if(ac == 3){
		Bot bot(atoi(av[1]), av[2]);
		bot.connect_to_serv();
		bot.loop();
		return (0);
	}
	else
		std::cout << "sorry, i need 2 arguments" << std::endl;
}