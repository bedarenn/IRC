#pragma once

class FD;

# define BUF_SIZE 4096

class FD {
public:
	FD();
	~FD();
	FD(const FD& other);

	FD&	operator=(const FD& other);

	void	read();
	void	write();

private:
	friend	class Server;
	int		_type;
	char	buf_read[BUF_SIZE + 1];
	char	buf_write[BUF_SIZE + 1];
};