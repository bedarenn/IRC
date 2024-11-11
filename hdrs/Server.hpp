#pragma once

#include <string>
#include <poll.h>
#include <vector>
#include <iostream>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <cstdlib>

# define BUFFSIZE 1000 

class Server{

	public:
		Server();
		Server(Server const &cpy);
		~Server();
		Server const &operator=(Server const &rhs);

		double		getPort() const;
		std::string getPass() const;
		int			get_fd_serv() const;
		void		setPort(double port);
		void		setPass(std::string pass);
		void		init_server();
		void		run();
		void		add_new(int socket);
		void		connect();
		//void		received_data(size_t fd);
		void		clear_fd();
		
	private:
		double					_port;
		std::string				_pass;
		int						_server_fd;
		std::vector<pollfd>		_fds;
};