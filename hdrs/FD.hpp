#pragma once

# include "types/FD.hpp"

# include "types/Server.hpp"

class FD {
public:
	FD();
	~FD();
	FD(const FD& other);

	FD&	operator=(const FD& other);

	void	read();
	void	write();

private:
	w_fd		_type;
	w_char	buf_read[BUF_SIZE + 1];
	w_char	buf_write[BUF_SIZE + 1];

	friend	class Server;
};
