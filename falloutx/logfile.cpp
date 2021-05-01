#include "logfile.h"
#include "io.h"
#include "stringbuilder.h"

static io::file		file;

logfile::logfile(const char* id) : id(id), errors(0) {
}

logfile::~logfile() {
	close();
}

void logfile::close() {
	file.close();
}

void logfile::open() {
	if(file)
		return;
	char temp[260]; stringbuilder sb(temp);
	sb.add("%1.txt", id);
	file.create(temp, StreamWrite | StreamText);
}

void logfile::add(const char* format, ...) {
	errors++;
	addv(format, xva_start(format), 1);
}

void logfile::addv(const char* format, const char* format_param, int ident) {
	char temp[2048]; stringbuilder sb(temp);
	while(ident-- > 0)
		sb.add(" ");
	sb.addv(format, format_param);
	sb.add("\r\n");
	open();
	if(file)
		file << temp;
}