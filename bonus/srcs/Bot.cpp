#include "Bot.hpp"

Bot::Bot(int port, std::string pass): _port(port), _pass(pass){}

Bot::~Bot(){}

void	Bot::send_msg(std::string str){
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
	send_msg(CREATE_CHAN);
}

void	Bot::loop(){
	ssize_t		size;
	char		buff[BUFFER_SIZE];

	while(true){
		memset(&buff, 0, BUFFER_SIZE);
		size = recv(_fd, buff, BUFFER_SIZE, 0);
		if(size == 0)
			break ;
		buff[size] = '\0';
		_cmd += buff;
		if(_cmd.find('\r') != std::string::npos && _cmd.find('\n') != std::string::npos)
			treat_cmd();
	}
}

void	Bot::treat_cmd(){
	std::cout << "CMD: " << _cmd << std::endl;
	std::string	data = next(' ');
	data = trim(data, ':');
	_target = data;
	std::cout << "data: " << data << std::endl;
	_cmd = "";
}

std::string	Bot::next(char find){
	std::stringstream ss(_cmd);
	std::string buff;
	getline(ss, buff, find);
	return(buff);
}

std::string	Bot::trim(std::string buff, char c){
	std::string result;
	result = buff.erase(buff.find(c), 1);
	return (result);
}

void	Bot::init_map(int paper, int rock, int scissors){
	_shifumi[paper] = "paper";
	_shifumi[rock] = "rock";
	_shifumi[scissors] = "scissors";
}

void		Bot::ShiFuMi(){

	std::srand(std::time(0));
	if(_cmd_player == "paper")
		init_map(1, 0, 2);
	if(_cmd_player == "rock")
		init_map(2, 1, 0);
	if(_cmd_player == "scissors")
		init_map(0, 2, 1);
	int rmd = rand() % 3;
	switch (rmd)
	{
		case 0:
			//send message loose;
			break;
		case 1:
			//send message egual;
			break;
		case 2:
			//send message victory;
			break;
	}
}
