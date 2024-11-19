#include "Command.hpp"

Command::Command(int fd, char *buff, Server *serv): _fd(fd), _buff(buff), _serv(serv) {
	init_cmd();
	treatement();
}

Command::Command(Command const &cpy){ *this = cpy;}
Command::~Command(){}

Command const &Command::operator=(Command const &rhs){ 
	if(this != &rhs )
		return (*this);
	return(*this);
}

void	Command::init_cmd(){
	_cmd["JOIN"] = &Command::parse_join;
	_cmd["KICK"] = &Command::parse_kick;
	_cmd["INVITE"] = &Command::parse_invite;
	_cmd["TOPIC"] = &Command::parse_topic;
	_cmd["MODE"] = &Command::parse_mode;
}

void	Command::treatement(){
	
	std::stringstream ss(_buff);
	std::string	data;
	
	getline(ss, data, ' ');
	if(data == "CAP"){
		treatement_client();
		return ;
	}
	std::map<std::string, void(Command::*)(void)>::iterator it;
	for(it = _cmd.begin(); it != _cmd.end(); it++){
		if(it->first == data){
			erase(0, _buff.find(' ') + 1);
			(this->*(it->second))();
		}
	}
}

void	Command::erase(int pos, int size){
	_buff.erase(pos, size);
}

std::string	Command::next(char find){
	std::stringstream ss(_buff);
	std::string buff;
	getline(ss, buff, find);
	return(buff);
}

int Command::counter(char c, std::string str){
	
	int count = 0;
	
	for(size_t i = 0; i < str.size(); i++){
		if(str[i] == c)
			count++;
	}
	return(count);
}

void	Command::treatement_client(){
	std::cout << _fd << _buff << std::endl;
}

void	Command::parse_join(){
	

	std::string channel, pass;
	std::string buff = next(' ');
	int chan = counter('#', buff);
	if(counter(',', buff)){
		std::cout << "format: /join #<channel> <password>" << std::endl;
		return ;
	}
	if(chan)
		channel = buff;
	buff = next(' ');
	if(!buff.empty()){
		if(counter(',', buff) || counter('#', buff))
			std::cout << "format: /join #<channel> <password>" << std::endl;
		pass = buff;
	}
	_serv->join(_fd, channel, pass);
}

void	Command::parse_invite(){} 

void	Command::parse_kick(){}

void	Command::parse_topic(){}

void	Command::parse_mode(){}

/*
join(const w_fd& client, std::vector<std::string>); si pass, premier
invite(const w_fd& op, const std::string& channel, const std::string& value); 
kick(const w_fd& op, const std::string& channel, const std::string& value);
topic(const w_fd& op, const std::string& channel, const std::string& value);
mode(const w_fd& op, std::vector<std::string> strs); si un noflag ne rien faire, 
*/ 