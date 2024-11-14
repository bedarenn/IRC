#pragma once

#include <string>

class Command{
	
	public:
		Command();
		~Command();
		void	choose(std::string cmd);

	private:
	void	join(std::string data); 
	void	privatMsg(std::string data); 
	void	kick(std::string data); 
	void	invite(std::string data); 
	void	topic(std::string data); 
	void	mode(std::string data);	
};