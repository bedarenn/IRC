#pragma once

#include "types/Command.hpp"
#include "Server.hpp"


class Server;

class Command{
	public:
		Command(int fd, char *buff, Server *serv);
		Command(int fd, Server *serv);
		Command(Command const &cpy);
		~Command();
		Command const &operator=(Command const &rhs);

		void		init_cmd();
		void		treatement();
		void		treatement_client();

		void		erase(int pos, int size);
		std::string	next(char find);

		void		parse_join();
		void		multi_join(std::string buff);
		void		parse_invite(); 
		void		parse_kick();
		void		parse_topic();
		void		parse_mode();

		void		comp_pass();
		void		set_nick();
		void 		set_user();
		void		new_client();

	private:
		int												_fd;
		std::string										_buff;
		Server											*_serv;
		std::map<std::string, void(Command::*)(void)> 	_cmd;
		std::string										_nickname;
		std::string										_username;
		bool											_new;
		bool											_nick;
		bool											_user;
		bool											_password;
		bool											_mad;
};

std::string *fill_tab(int size, std::string data);
int 		counter(char c, std::string str);