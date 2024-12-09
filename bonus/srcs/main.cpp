#include "Bot.hpp"

int g_loop;
void	set_sig(void);

bool	test_str(const std::string& str){
	if (str.empty())
		return (false);
	for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
		if (std::isspace(*it))
			return (false);
	}
	return(true);
}

bool	parse(char **av){

	int	port;

	if (!test_str(av[1]) || !test_str(av[2])) {
		std::cerr << "error: argument empty" << std::endl;
		return(false);
	}
	port = atoi(av[1]);
	if(port < 0 || port > 65535){
		std::cerr << "error: port not allowed" << std::endl;
		return (false);
	}
	return(true);
}

int	main(int ac, char **av){

	if(ac == 3){
		if(!parse(av))
			return (0);
		Bot bot(atoi(av[1]), av[2]);
		bot.connect_to_serv();
		bot.loop();
		return (0);
	}
	else
		std::cout << "sorry, i need 2 arguments" << std::endl;
}