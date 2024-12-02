#include "Command.hpp"

Command::Command(int fd, const std::string& buff, Server *serv): _fd(fd), _buff(buff), _serv(serv) {
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
	_cmd["PART"] = &Command::parse_part;
	_cmd["PASS"] = &Command::parse_pass;
	_cmd["NICK"] = &Command::parse_nick;
	_cmd["USER"] = &Command::parse_user;
}

void	Command::treatement(){
	
		std::stringstream ss(_buff);
		std::string	data;

		getline(ss, data, ' ');
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
	std::string nickname, channel, msg = "";
	std::string buff = next(' ');
 	if(buff.empty() || counter(',', buff) || !counter('#', buff)){
		_serv->kick(_fd, "", "", "");
		return ;
	}
	channel = buff;
	erase(0, buff.size() + 1);
	buff = next(' ');
	if(buff.empty() || counter(',', buff) || counter('#', buff)){
		_serv->kick(_fd, channel, "", "");
		return ;
	}
	nickname = buff;
	erase(0, buff.size() + 1);
	buff = next('\r');
	if(!buff.empty())
		msg = buff;
	std::cout << _fd << "(): " << channel << " " << nickname << " " << msg << std::endl;
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

std::string	clean_double(std::string data){
	std::string ref = "itkol";
	std::string result;
	result += data[0];
	int count = 0;
	for(size_t i = 0; i < ref.size(); i++){
		count = 0;
		for(size_t j = 0; j < data.size(); j++){
			if(data[j] == ref[i])
				count++;
		}
		if(count)
			result += ref[i];
	}
	return(result);
}

void	Command::parse_mode(){
	std::string sign, channel, data = "itkol";
	std::string buff = next(' ');
	if(!counter('#', buff))
		_serv->mode(_fd, "", "", "");
	channel = buff;	
	erase(0, buff.size() + 1);
	buff = next(' ');
	int save = buff.size() + 1;
	buff = clean_double(buff);
	int s = 0, alpha = 0;
	for(size_t i = 0; i < buff.size(); i++){
		if(buff[i] == '-' || buff[i] == '+'){
			sign = buff[i];
			s++;
		}
		if(isalpha(buff[i]))
			alpha++;
	}
	if(!s || !alpha || s > 1){
		_serv->mode(_fd, channel, "", "");
		return ;	
	}
	std::string tab, arg;
	buff = trim(buff, sign[0]);
	for(size_t i = 0; i < buff.size(); i++){
		if(data.find(buff[i]) != std::string::npos)
			tab[i] = buff[i];
		else
			return ;
	}
	erase(0, save);
	for(int i = 0; i < alpha; i++){
		if(tab[i] == 'l' || tab[i] == 'k' || tab[i] == 'o'){
			arg = next(' ');
			save = arg.size() + 1;
			if(arg.empty()){
				arg = next('\n');
				if(arg.empty()){
					_serv->mode(_fd, channel, sign + tab[i], "");
					return ;
				}
				save = arg.size() + 1;
				arg = trim(buff, ' ');
			}
			erase(0, save);
		}
		_serv->mode(_fd, channel, sign + tab[i], arg);
	}
}

//////////////////////////////////////////////////////////////////// PART ///////////////////////////////////////////////////////////////////////////////////
//bool	Server::leave_channel(const Client& client, const std::string& channel) {

void	Command::parse_part(){
	std::string buff = next(' ');
	Client client = _serv->get_client(_fd);
	_serv->leave_channel(client, buff);
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

void		Command::parse_pass(){
	_serv->new_client_pass(_fd, _buff);
}

void		Command::parse_nick(){
	_serv->new_client_nick(_fd, _buff);
}

void		Command::parse_user(){
	std::stringstream ss(_buff);
	std::string	data;

	getline(ss, data, ' ');
	std::cout << data << std::endl;
	_serv->new_client_name(_fd, data);
}
