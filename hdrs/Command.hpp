#pragma once

#include "types/Command.hpp"

class Server;

class Command{
	public:
		Command(int fd, char *buff, Server *serv);
		Command(Command const &cpy);
		~Command();
		Command const &operator=(Command const &rhs);

		void	init_cmd();
		void	treatement();
		void	treatement_client();

		void	parse_join();
		void	parse_invite(); 
		void	parse_kick();
		void	parse_topic();
		void	parse_mode();

	private:
		int	_fd;
		std::string	_buff;
		Server	*_serv;
		std::map<std::string, void(Command::*)(void)> _cmd;
};