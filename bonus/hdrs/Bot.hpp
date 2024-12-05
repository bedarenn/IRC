#pragma once

# include "iostream"
# include <netinet/in.h>
# include <netdb.h>
# include <cstdlib>
# include <fcntl.h>
# include <cstring>

# define BUFFER_SIZE 1024

extern int g_loop;

class Bot{
	public:
		Bot(int port, std::string pass);
		~Bot();
		void	send_msg(std::string str);
		void	connect_to_serv();
		void	loop();
		void	treat_cmd();

	private:
		int			_fd;
		int			_port;
		std::string _pass;
		std::string	_cmd;
};

# define CONNECT_MSG(_pass) "PASS " + _pass + "\r\nNICK bot\r\nUSER bot1\r\n"