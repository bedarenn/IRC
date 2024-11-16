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
		if(it->first == data)
			it->second;
	}
}

void	Command::treatement_client(){
	std::cout << _fd << _buff << std::endl;
}

void	Command::parse_join(){
	std::cout << "string" << std::endl;
	std::string	treat(_buff);
	treat.erase(0, treat.find(' '));
	std::cout << treat << std::endl; 
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