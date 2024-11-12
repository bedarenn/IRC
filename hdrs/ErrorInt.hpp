#pragma once

# include <exception>

# include <string>

# define ERR_INT(f, m, s)		ErrorInt::error_int(f, m, s, __FILE__, __LINE__)

# define ERR_INT_MSG(s, f, l)	s << " error: <" << f << "::l" << l << "> : " << strerror(errno)

class	ErrorInt : public std::exception {
public:
	ErrorInt(const std::string& str, const std::string& file, const int& line);
	~ErrorInt() throw();

	const char	*what() const throw();

	static void	error_int
		(const int& f, const int& max,
		const std::string& str, const std::string& file, const int& line);
private:
	std::string	msg;
};
