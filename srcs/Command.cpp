#include "Command.hpp"

Command::Command(int fd, const std::string& buff, Server *serv): _fd(fd), _buff(buff), _serv(serv) {
	//std::cout << _buff << std::endl;
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
	_nick = false;
	_user = false;
	_password = false;
	_mad = false;
}

void	Command::treatement(){
	
	if(_buff.find('\n') != std::string::npos){

		std::stringstream ss(_buff);
		std::string	data;

		getline(ss, data, ' ');
		if(data == "CAP" || data == "PASS"){
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
	channel = buff;
	erase(0, buff.size() + 1);
	buff = next(' ');
	if(buff.empty() || counter(',', buff) || counter('#', buff)){
		_serv->topic(_fd, channel, "");
	}
	topic = buff;
	_serv->topic(_fd, channel, topic);
}

////////////////////////////////////////////////////////////////////// QUIT /////////////////////////////////////////////////////////////////////////////

void	Command::parse_quit(){
	_serv->quit(_fd, "tchao");
}

////////////////////////////////////////////////////////////////////// MODE /////////////////////////////////////////////////////////////////////////////

void	Command::parse_mode(){
	std::string sign, channel, data = "itkol";
	std::string buff = next(' ');
	if(!counter('#', buff))
		_serv->mode(_fd, "", "", "");
	channel = buff;	
	erase(0, buff.size() + 1);
	buff = next(' ');
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
		_serv->mode(_fd, channel, "", "");
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
		_serv->mode(_fd, channel, sign + tab[i], buff);
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
	erase(0, buff.size() + 1);
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

void		Command::comp_pass(std::string pass){
	pass.erase(0, pass.find(' ') + 1);
	pass.erase(pass.size() - 1, 1);
	if(pass == _serv->get_pass())
		_password = true;
	else{
		_mad = true;
	}
}

void		Command::set_nick(std::string nick){
	nick.erase(0 , nick.find(' ') + 1);
	nick.erase(nick.size() - 1, 1);
	_nickname = nick;
	_nick = true;
}

void 		Command::set_user(std::string user){
	user.erase(0 , user.find(' ') + 1);
	user.erase(user.size() - 1, 1);
	user.erase(user.find(' '), user.size());
	_username = user;
	_user = true;
}

void	Command::new_client(){
	std::string buff = next(' ');
	if(buff == "CAP"){
		buff = next('\n');
		erase(0, buff.size() + 1);
	}
	if(_buff.empty())
		return;
	buff = next(' ');
	if (buff == "PASS"){
		buff = next('\n');
		comp_pass(buff);
		if(_mad){
			close(_fd);		
			return ;
		}
		erase(0, buff.size() + 1);
	}
	buff = next(' ');
	if(buff == "NICK"){
		buff = next('\n');
		set_nick(buff);
		erase(0, buff.size() + 1);
	}
	buff = next(' ');
	if(buff == "USER"){
		buff = next('\n');
		set_user(buff);
		erase(0, buff.size() + 1);
	}
	if(_password && _nick && _user)
		_serv->new_client(_nickname, _username, _fd);
}

/*
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAP LS 302^M$
$
PASS pop^M$
NICK tonup1^M$
USER upman1 0 * :realname^M$
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAP LS 302^M$
PASS pop^M$
NICK tonup1^M$
USER upman1 0 * :realname^M$
*/