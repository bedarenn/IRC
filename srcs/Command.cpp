#include "Command.hpp"

////////////////////////////////////////////////////////////////// CONSTRUCT /////////////////////////////////////////////////////////////////////////

Command::Command(int fd, char *buff, Server *serv): _fd(fd), _buff(buff), _serv(serv) {
	std::cout << _buff << std::endl;
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

//////////////////////////////////////////////////////////////// UTILS ///////////////////////////////////////////////////////////////////////////////

void	Command::init_cmd(){
	_cmd["JOIN"] = &Command::parse_join;
	_cmd["KICK"] = &Command::parse_kick;
	_cmd["INVITE"] = &Command::parse_invite;
	_cmd["TOPIC"] = &Command::parse_topic;
	_cmd["MODE"] = &Command::parse_mode;
	_cmd["QUIT"] = &Command::parse_quit;
	_cmd["PING"] = &Command::parse_ping;
	_cmd["PRIVMSG"] = &Command::parse_msg;
	_new = false;
	_nick = false;
	_user = false;
	_password = false;
	_mad = false;
}

void	Command::treatement(){
	
	std::stringstream ss(_buff);
	std::string	data;
	
	getline(ss, data, ' ');
	if(data == "CAP"){
		erase(0, _buff.find('\n') + 1);
		new_client();
		return ;
	}
	w_map_Command::iterator it;
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

std::string	trim(std::string buff, char c){
	std::string result;
	result = buff.erase(buff.find(c), 1);
	return (result);
}

//////////////////////////////////////////////////////////////////// JOIN ////////////////////////////////////////////////////////////////////////////////

void	Command::parse_join(){

	std::string channel, pass;
	std::string buff = next(' ');
	if(buff.empty() || !counter('#', buff)){
		_serv->join(_fd, "", "");
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
			_serv->join(_fd, channel, "");
			return ;
	}
	if(counter(',', buff)){
		buff = trim(buff, ',');
	}
	pass = buff;
	_serv->join(_fd, channel, pass);
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
		_serv->join(_fd, "", "");
		return ;
	}
	std::string *tab_chan = fill_tab(chan, buff);
	erase(0, save);
	buff = next(' ');
	if(counter('#', buff)){
		delete[] tab_chan;
		_serv->join(_fd, "", "");
		return ;
	}
	std::string *tab_pass = fill_tab(chan, buff);
	for(int i = 0; i < chan; i++)
		_serv->join(_fd, tab_chan[i], tab_pass[i]);
	delete[] tab_chan;
	delete[] tab_pass;
}

//////////////////////////////////////////////////////////////////// INVITE ///////////////////////////////////////////////////////////////////////////////

void	Command::parse_invite(){
	std::string nickname, channel;
	std::string buff = next(' ');
	if(buff.empty() || counter(',', buff) || counter('#', buff)){
		_serv->invite(_fd, "", "");
		return ;
	}
	nickname = buff;
	erase(0, buff.size() + 1);
	buff = next(' ');
	if(buff.empty() || counter(',', buff) || !counter('#', buff)){
		_serv->invite(_fd, "", nickname);
		return ;
	}
	channel = buff;
	_serv->invite(_fd, channel, nickname);
} 

//////////////////////////////////////////////////////////////////// KICK ///////////////////////////////////////////////////////////////////////////////

void	Command::parse_kick(){
	std::string nickname, channel, msg;
	std::string buff = next(' ');
	erase(0, buff.size() + 1);
	buff = next(' ');
	if(buff.empty() || counter(',', buff) || counter('#', buff)){
		_serv->kick(_fd, "", "", "");
		return ;
	}
	nickname = buff;
	erase(0, buff.size() + 1);
	buff = next(' ');
	if(buff.empty() || counter(',', buff) || !counter('#', buff)){
		_serv->kick(_fd, "", nickname, "");
		return ;
	}
	channel = trim(buff, ':');
	erase(0, buff.size() + 1);
	buff = next('\r');
	msg = buff;
	_serv->kick(_fd, channel, nickname, msg);
}

////////////////////////////////////////////////////////////////////// TOPIC //////////////////////////////////////////////////////////////////////////////

void	Command::parse_topic(){
	std::string topic, channel;
	std::string buff = next(' ');
	if(buff.empty() || counter(',', buff) || !counter('#', buff)){
		_serv->topic(_fd, "", "");
		return ;
	}
	topic = buff;
	erase(0, buff.size() + 1);
	buff = next(' ');
	if(buff.empty() || counter(',', buff) || counter('#', buff)){
		_serv->topic(_fd, "", topic);
	}
	channel = buff;
	_serv->topic(_fd, channel, topic);
}

////////////////////////////////////////////////////////////////////// QUIT /////////////////////////////////////////////////////////////////////////////

void	Command::parse_quit(){
	_serv->quit(_fd, "tchao");
}

////////////////////////////////////////////////////////////////////// MODE /////////////////////////////////////////////////////////////////////////////

void	Command::parse_mode(){
	std::string sign, data = "itkol";
	std::string buff = next(' ');
	int s = 0, alpha = 0;
	for(size_t i = 0; i < buff.size(); i++){
		if(buff[i] == '-' || buff[i] == '+'){
			sign = buff[i];
			s++;
		}
		if(isalpha(buff[i]))
			alpha++;
	}
	if(!s || !alpha){
		_serv->mode(_fd, _chan->get_name(), "", "");
		return ;	
	}
	std::string *tab = new std::string[alpha]();
	buff = trim(buff, sign[0]);
	for(size_t i = 0; i < buff.size(); i++){
		if(data.find(buff[i]) != std::string::npos)
			tab[i] = buff[i];
		else{
			delete[] tab;
			return ;
		}
	}
	erase(0, buff.size() + 1);
	buff = next('\n');
	for(int i = 0; i < alpha; i++){
		_serv->mode(_fd,  _chan->get_name(), sign + tab[i], buff);
	}
	delete[] tab;
}

//////////////////////////////////////////////////////////////////// PING ///////////////////////////////////////////////////////////////////////////////////

void	Command::parse_ping(){
	std::string buff = next(' ');
	_serv->pong(_fd, buff);
}

/////////////////////////////////////////////////////////////////// PRIVMSG /////////////////////////////////////////////////////////////////////////////////

void	Command::parse_msg(){
	std::string buff = next(' '), msg;
	erase(0, buff.size());
	if(counter('#', buff)){
		msg = next('\n');
		msg = trim(msg, ':');
		_serv->send_chan(_fd, buff, msg);
	}
	else{
		msg = next('\n');
		msg = trim(msg, ':');
		_serv->send_priv(_fd, buff, msg); 
	}
}

////////////////////////////////////////////////////////////////// NEW CLIENT ////////////////////////////////////////////////////////////////////////////////

void		Command::comp_pass(std::string &pass){
	std::string key = pass.substr(0, pass.find(' '));
	if(key != "PASS"){
		_mad = true;
		return ;
	}
	pass.erase(0, pass.find(' ') + 1);
	if(pass == _serv->get_pass())
		_password = true;
	else{
		_mad = true;
	}
}

void		Command::set_nick(std::string &nick){
	_nickname = nick;
	_nick = true;
}

void 		Command::set_user(std::string &user){
	user.erase(user.find(' '), user.size());
	_username = user;
	_user = true;	
}

bool	Command::check_new(){
	if(_password && _nick && _user)
		_new = true;
	if(_mad)
		return (true);
	return (false);
}

void		Command::new_client(){
	std::string buff;
	buff = next('\n');
	if(buff.empty())
		new_client();
	buff.erase(buff.size() - 1, 1);
	comp_pass(buff);
	if(_mad == true){
		//std::string err = ERR_PASS(_serv->get_name());
		//send(_fd, err.c_str(), err.size(), 0);
		_buff = '\0';
		close(_fd);
		return ;
	}
	erase(0, _buff.find('\n') + 1);
	buff = next('\n');
	buff.erase(buff.size() - 1, 1);
	buff.erase(0, buff.find(' ') + 1);
	set_nick(buff);
	erase(0, _buff.find('\n') + 1);
	buff = next('\n');
	buff.erase(buff.size() - 1, 1);
	buff.erase(0, buff.find(' ') + 1);
	set_user(buff);
	erase(0, _buff.find('\n') + 1);
	_serv->new_client(_username, _nickname, _fd);
}

