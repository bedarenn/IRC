#pragma once

#include "types/Command.hpp"


class Command{
	public:
		Command();
		Command(Command const &cpy);
		~Command();
		Command const &operator=(Command const &rhs);

		void	treatement(int fd, char *buff, Server &server);
		void	treatement_client(int fd, char *buff, Server &server);
	
	private:
		std::map<regex_t, void(Server::*)(void)>	_cmd;
};