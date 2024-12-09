#include "Bot.hpp"

Bot::Bot(int port, std::string pass): _port(port), _pass(pass){
	_shifumi[0] = "paper";
	_shifumi[1] = "rock";
	_shifumi[2] = "scissors";
}

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
	bool		_priv;
	char		buff[BUFFER_SIZE];

	while(true){
		memset(&buff, 0, BUFFER_SIZE);
		size = recv(_fd, buff, BUFFER_SIZE, 0);
		if(size == 0)
			break ;
		buff[size] = '\0';
		_cmd += buff;
		if(_cmd.find('\r') != std::string::npos && _cmd.find('\n') != std::string::npos){
			treat_cmd();
			_cmd = "";
		}
	}
}

void	Bot::treat_cmd(){
	std::cout << _cmd << std::endl;
	std::string	data = next(' ');
	data = trim(data, ':');
	_target = data;
	std::cout << "data1: " << data << std::endl;
	_cmd.erase(0, data.size() + 2);
	data = next(' ');
	std::cout << "data2: " << data << std::endl;
	if(data == "PRIVMSG"){
		_cmd.erase(0, data.size() + 1);
		launch_game();
	}
	if(data == "INVITE"){
		_cmd.erase(0, data.size() + 1);
		invite();
	}
}

void	Bot::launch_game(){
	std::string data = next(' ');
	std::cout << "data3: " << data << std::endl;
	if(data.find('#') != std::string::npos)
		_target = data;
	_cmd.erase(0, data.size() + 1);
	data = next(' ');
	std::cout << "data4: " << data << std::endl;
	if(data.find(13) != std::string::npos)
		data = trim(data, 13);
	if(data.find(10) != std::string::npos)
		data = trim(data, 10);
	if(data == "!play")
		shifumi();
}

void	Bot::invite(){
	std::string data = next(' ');
	_cmd.erase(0, data.size() + 1);
	data = next(' ');
	send_msg(JOIN_CHAN(data));
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

void		Bot::shifumi(){
	std::string str = "shi..";
	send_msg(PRIV_MSG(_target, str));
	usleep(10000);
	str = "fu..";
	send_msg(PRIV_MSG(_target, str));
	usleep(10000);
	str = "mi..";
	send_msg(PRIV_MSG(_target, str));
	usleep(10000);
	int rmd = rand() % 3;
	send_msg(PRIV_MSG(_target, _shifumi[rmd]));
}

