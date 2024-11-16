#include "Command.hpp"

Command::Command(){}
Command::Command(Command const &cpy){ *this = cpy}
Command::~Command(){}

Command const &Command::operator=(Command const &rhs){ 
	if(this != &rhs )
		return (*this);
	return(*this);
}

void	Command::treatement(int fd, char *buff, Server &server){

}

// void	treatement_client(int fd, char *buff, Server &server);
/*
PASS fd <password>
JOIN fd vector<pair<string, string>>
PRIVMSG fd <nickname> <message>
KICK fd <channel> <user>
INVITE fd  <channel> <nickname> 
MODE fd <channel> {[+|-]|o|p|s|i|t|n|b|v}
TOPIC fd vector<pair<string, string>>
*/ 