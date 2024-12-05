#include "Bot.hpp"

Bot::Bot(int port, std::string pass): _port(port), _pass(pass){}
Bot::~Bot(){}

void	Bot::send_msg(std::string str){
	std::cout << _fd << ": " << str <<std::endl;
	send(_fd, str.c_str(), str.size(), 0);
}

void	Bot::connect_to_serv(){

	struct sockaddr_in 	serv_info;
	
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(_fd, F_SETFL, O_NONBLOCK);
	serv_info.sin_family = AF_INET;
	serv_info.sin_addr.s_addr = INADDR_ANY;
	serv_info.sin_port = htons(_port);
	connect(_fd, (struct sockaddr *)&serv_info, sizeof(serv_info));
	send_msg(CONNECT_MSG(_pass));
}

void	Bot::loop(){
	
	ssize_t	size;
	char		buff[BUFFER_SIZE];

	memset(&buff, 0, BUFFER_SIZE);
	while(true){
		size = recv(_fd, buff, BUFFER_SIZE, 0);
		if(size == 0)
			break ;
		buff[size] = '\0';
		_cmd += buff;
		if(_cmd.find('\r') != std::string::npos && _cmd.find('\n') != std::string::npos){
			treat_cmd();
		}
	}
}

void	Bot::treat_cmd(){
	std::cout << _cmd << std::endl;

}