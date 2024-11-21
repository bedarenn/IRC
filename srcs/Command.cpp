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
	_cmd["PASS"] = &Command::comp_pass;
	_cmd["NICK"] = &Command::set_nick;
	_cmd["USER"] = &Command::set_user;
	_cmd["CAP"] = &Command::new_client;
}

void	Command::treatement(){
	
	std::stringstream ss(_buff);
	std::string	data;
	
	getline(ss, data, ' ');
	std::map<std::string, void(Command::*)(void)>::iterator it;
	for(it = _cmd.begin(); it != _cmd.end(); it++){
		if(it->first == data){
			erase(0, _buff.find(' ') + 1);
			erase(_buff.find(13), 2);
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

int	counter(char c, std::string str){
	
	if(str.empty())
		return (0);
	int count = 0;
	
	for(size_t i = 0; i < str.size(); i++){
		if(str[i] == c)
			count++;
	}
	return(count);
}

void	Command::parse_join(){

	std::string channel, pass;
	std::string buff = next(' ');
	if(buff.empty() || !counter('#', buff)){
		std::cout << "format: /join #<channel> <password>" << std::endl;
		return ;
	}
	if(counter('#', buff) > 1 && counter(',', buff)){
		multi_join(buff);
		return ;
	}
	channel = buff;
	erase(0, buff.size() + 1);
	buff = next(' ');
	if(counter('#', buff)){
			std::cout << "format: /join #<channel> <password>" << std::endl;
			return ;
	}
	pass = buff;
	//_serv->join(_fd, channel, pass);
}

std::string *fill_tab(int size, std::string data){

	int i = 0;
	int end = 0;
	std::string *tab = new std::string[size]();
	while(i < size){
		std::string tmp = data.substr(0, data.find(','));
		tab[i] = tmp;
		if(end)
			tab[i] = "";
		if(data.find(',') == std::string::npos)
			end++;
		data.erase(0, data.find(',') + 1);
		i++;
	}
	return(tab);
}

void	Command::multi_join(std::string buff){
	
	int chan = counter('#', buff);
	int comma = counter(',', buff);
	int save = buff.size() + 1;
	if(chan - comma != 1){
		std::cout << "format: /join #<channel>[,#channel] <password>[,<password>]" << std::endl;
		return ;
	}
	std::string *tab_chan = fill_tab(chan, buff);
	erase(0, save);
	buff = next(' ');
	if(counter('#', buff)){
		delete[] tab_chan;
		std::cout << "format: /join #<channel>[,#channel] <password>[,<password>]" << std::endl;
		return ;
	}
	std::string *tab_pass = fill_tab(chan, buff);
	for(int i = 0; i < chan; i++)
		//_serv->join(_fd, tab_chan[i], tab_pass[i]);
	delete[] tab_chan;
	delete[] tab_pass;
}

void	Command::parse_invite(){
	
	std::cout << "buff => " << _buff << std::endl;
	std::string nickname, channel;
	std::string buff = next(' ');
	if(buff.empty() || counter(',', buff) || counter('#', buff)){
		std::cout << "format: /invite <nickname> #<channel>" << std::endl;
		return ;
	}
	nickname = buff;
	erase(0, buff.size() + 1);
	buff = next(' ');
	if(buff.empty() || counter(',', buff) || !counter('#', buff)){
			std::cout << "format: /invite <nickname> #<channel>" << std::endl;
	}
	channel = buff;
	std::cout << channel << ' ' << nickname << std::endl;
	//_serv->invite(_fd, channel, nickname);
} 

void	Command::parse_kick(){

	std::cout << "buff => " << _buff << std::endl;
	std::string nickname, channel;
	std::string buff = next(' ');
	if(buff.empty() || counter(',', buff) || counter('#', buff)){
		std::cout << "format: /kick <nickname> #<channel>" << std::endl;
		return ;
	}
	nickname = buff;
	erase(0, buff.size() + 1);
	buff = next(' ');
	if(buff.empty() || counter(',', buff) || !counter('#', buff)){
			std::cout << "format: /kick <nickname> #<channel>" << std::endl;
	}
	channel = buff;
	std::cout << channel << ' ' << nickname << std::endl;
	//_serv->kick(_fd, channel, nickname);
}

void	Command::parse_topic(){
	
	std::cout << "buff => " << _buff << std::endl;
	std::string topic, channel;
	std::string buff = next(' ');
	if(buff.empty() || counter(',', buff) || !counter('#', buff)){
		std::cout << "format: /topic #<channel> <topic>" << std::endl;
		return ;
	}
	topic = buff;
	erase(0, buff.size() + 1);
	buff = next(' ');
	if(buff.empty() || counter(',', buff) || counter('#', buff)){
			std::cout << "format: /topic #<channel> <topic>" << std::endl;
	}
	channel = buff;
	std::cout << channel << ' ' << topic << std::endl;
	//_serv->topic(_fd, channel, topic);
}

void	Command::parse_mode(){}

// new client: 4
// 4CAP LS 302
// PASS pop
// NICK qdev
// USER qdeviann
// void	Server::new_client(const std::string& name, const std::string& nickname, const w_fd& fd)

void		Command::comp_pass(){
}

void		Command::set_nick(){
}

void 		Command::set_user(){
}

void		Command::new_client(){
	std::cout << _buff << std::endl;
}


/*
join(const w_fd& client, std::vector<std::string>); si pass, premier
invite(const w_fd& op, const std::string& channel, const std::string& value); 
kick(const w_fd& op, const std::string& channel, const std::string& value);
topic(const w_fd& op, const std::string& channel, const std::string& value);
mode(const w_fd& op, std::vector<std::string> strs); si un noflag ne rien faire, 
*/ 