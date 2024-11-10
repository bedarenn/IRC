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

void		Server::add_new(int socket){
	pollfd fd;
	fd.fd = socket;
	fd.events = POLLIN;
	fd.revents = 0;
	_fds.push_back(fd);
}

void	Server::connect(){
	while(1){
		if(poll(_fds.data(), _fds.size(), 0) < 0);
			break;
	}
	std::vector<pollfd>::iterator it;
	for(size_t i = 0; i < _fds.size(); i++){
            if(_fds[i].revents & POLLIN){
                if (_fds[i].fd == ) {
                    int client_fd = accept(server_fd, nullptr, nullptr);
                    if (client_fd < 0) {
                        perror("Erreur accept");
                        continue;
                    }
	}
}