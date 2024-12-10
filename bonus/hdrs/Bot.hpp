#pragma once

# include <iostream>
# include <netinet/in.h>
# include <netdb.h>
# include <cstdlib>
# include <fcntl.h>
# include <cstring>
# include <sstream>
# include <ctime>
# include <map>
# include <vector>
# include <algorithm>
# include <unistd.h>

# define BUFFER_SIZE 1024

extern int g_loop;

class Bot{
	public:
		Bot(int port, std::string pass);
		~Bot();

		void		send_msg(std::string str);
		void		connect_to_serv();
		void		loop();
		void		treat_cmd();
		void		shifumi();
		void		launch_game();
		void		invite();
		
		std::string	trim(std::string buff, char c);
		std::string	next(char find);

	private:
		int							_fd;
		int							_port;
		std::string 				_pass;
		std::string					_cmd;
		std::map<int, std::string>	_shifumi;
		std::string					_cmd_player;
		std::string					_target;
};

# define CONNECT_MSG(_pass) "PASS " + _pass + "\r\nNICK bot\r\nUSER bot1\r\n"
# define CREATE_CHAN "JOIN #bot\r\n"
# define JOIN_CHAN(chan) "JOIN " + chan + "\r\n"
# define PRIV_MSG(target, str) "PRIVMSG " + target + " " + str + "\r\n"
# define PART(chan) "bot PART " + chan + "\r\n"
# define QUIT "QUIT have a good day\r\n"

//# define 