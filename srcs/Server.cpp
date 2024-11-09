#include "Server.hpp"

Server::Server(){}
Server::Server(Server const &cpy){*this = cpy;}
Server::~Server(){}

Server const &Server::operator=(Server const &rhs){
	if(this != &rhs){
		_port = rhs.getPort();
		_pass = rhs.getPass();
	}
	return(*this);
}

double		Server::getPort() const{return(_port);}
std::string Server::getPass() const{return(_pass);}
void		Server::setPort(double port){_port = port;}
void		Server::setPass(std::string pass){_pass = pass;}

