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
int			Server::get_fd_serv() const{return(_server_fd);}
void		Server::setPort(double port){_port = port;}
void		Server::setPass(std::string pass){_pass = pass;}

void		Server::init_server(){
		
	struct sockaddr_in adr;
	pollfd	fd;

	fd.fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd.fd < 0){
		std::cerr << "error: socket" << std::endl;
		return ;
	}
	_server_fd = fd.fd;
	std::cout << "serveur fd = " << _server_fd << std::endl;
	fcntl(fd.fd, F_SETFL, O_NONBLOCK);
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = INADDR_ANY;
	adr.sin_port = htons(_port);
	if(bind(fd.fd, (struct sockaddr*)&adr, sizeof(adr)) < 0)
		std::cerr << "error: bind" << std::endl;
	if(listen(fd.fd, 5) < 0)
		std::cerr << "error: listen" << std::endl;
	fd.events = POLLIN;
	fd.revents = 0;
	_fds.push_back(fd);
}

void	Server::run(){
	while(1){
		if(poll(_fds.data(), _fds.size(), 0) < 0)
			break;
		for(size_t i = 0; i < _fds.size(); i++){
			if(_fds[i].revents && _fds[i].fd == _server_fd)
				connect();
		}
	}
}

void	Server::connect(){
	
	struct sockaddr_in adr;
	socklen_t len =  sizeof(adr);
	int	client = accept(_server_fd, (struct sockaddr*)&adr, &len);
	std::cout << "client fd = " << client << std::endl;
	if(client){
		add_new(client);
		std::cout << "new client connected" << std::endl;
	}
	else
		std::cerr << "error: accept" << std::endl; 
}

void		Server::add_new(int socket){
	
	pollfd fd;
	
	fd.fd = socket;
	fd.events = POLLIN;
	fd.revents = 0;
	_fds.push_back(fd);
}
