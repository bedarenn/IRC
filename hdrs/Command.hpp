#pragma once

#include "types/Command.hpp"
#include "types/Channel.hpp"
#include "Server.hpp"
#include "Channel.hpp"


class Server;
class Channel;

class Command{
	public:
		Command(int fd, const std::string& buff, Server *serv);
		Command(int fd, char *buff, Server *serv, Channel *chan);
		Command(Command const &cpy);
		~Command();
		Command const &operator=(Command const &rhs);

		void		init_cmd();
		void		treatement();

		void		erase(int pos, int size);
		std::string	next(char find);

		void		parse_join();
		void		multi_join(std::string buff);
		void		parse_invite(); 
		void		parse_kick();
		void		parse_topic();
		void		parse_mode();
		void		parse_quit();
		void		parse_ping();
		void		parse_msg();

		bool		check_new();
		void		new_client();
		void		comp_pass(std::string &pass);
		void		set_nick(std::string &nick);
		void 		set_user(std::string &user);


	private:
		int				_fd;
		std::string		_buff;
		Server			*_serv;
		Channel			*_chan;
		w_map_Command	_cmd;
		std::string		_nickname;
		std::string		_username;
		bool			_new;
		bool			_nick;
		bool			_user;
		bool			_password;
		bool			_mad;
};

std::string *fill_tab(int size, std::string data);
int 		counter(char c, std::string str);
std::string	trim(std::string buff, char c);