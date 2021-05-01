#pragma once

class logfile {
	const char*			id;
	int					errors;
	void				open();
public:
	logfile(const char* id);
	~logfile();
	constexpr operator bool() const { return errors > 0; }
	void				add(const char* format, ...);
	void				addv(const char* format, const char* format_param, int ident = 0);
	void				close();
};