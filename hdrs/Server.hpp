#pragma once

#include <string>
#include <poll.h>
#include <vector>

class Server{

	public:
		Server();
		Server(Server const &cpy);
		~Server();
		Server const &operator=(Server const &rhs);

		double		getPort() const;
		std::string getPass() const;
		void		setPort(double port);
		void		setPass(std::string pass);
		void		add_new(int socket);
		void		connect();
		
	private:
		double							_port;
		std::string						_pass;
		std::vector<pollfd>				_fds;
};