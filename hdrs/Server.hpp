#pragma once

#include <string>

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
		
	private:
		double		_port;
		std::string	_pass;
};