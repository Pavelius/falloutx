#include "archive.h"
#include "main.h"

static bool serial(creaturei& e, const char* name, bool write_mode) {
	char temp[260]; stringbuilder sb(temp);
	sb.add("characters/%1.gch", name);
	io::file file(temp, write_mode ? StreamWrite : StreamRead);
	if(!file)
		return false;
	archive ar(file, write_mode);
	if(!ar.signature("CHR"))
		return false;
	ar.set(e);
	return true;
}

void creaturei::write(const char* name) const {
	serial(*const_cast<creaturei*>(this), name, true);
}

bool creaturei::read(const char* name) {
	return serial(*const_cast<creaturei*>(this), name, false);
}

void creaturei::deletegch(const char* name) {
	char temp[260]; stringbuilder sb(temp);
	sb.add("characters/%1.gch", name);
	io::file::remove(temp);
}

void answers::select(const char* url, const char* filter) {
	char temp[260]; stringbuilder sb(temp);
	for(io::file::find e(url); e; e.next()) {
		auto pn = e.name();
		if(!szpmatch(pn, filter))
			continue;
		sb.clear(); sb.add(e.name());
		auto p = (char*)szext(temp);
		if(p)
			p[-1] = 0;
		addvm(temp, 0);
	}
}

void creaturei::tofile(const char* name) const {
	const char* eol = "\r\n";
	char temp[260]; stringbuilder sb(temp);
	sb.add("characters/%1.txt", name);
	io::file file(temp, StreamWrite);
	if(!file)
		return;
	file << getname() << eol;
	for(variant v = FirstPrimary; v.value <= LastPrimary; v.value++)
		file << v.getnameabr() << ": " << get(v) << " ";
	file << eol;
}

static bool serial(areai& e, const char* name, bool write_mode) {
	char temp[260]; stringbuilder sb(temp);
	sb.add("maps/%1.gmp", name);
	io::file file(temp, write_mode ? StreamWrite : StreamRead);
	if(!file)
		return false;
	archive ar(file, write_mode);
	if(!ar.signature("MAP"))
		return false;
	ar.set(e);
	ar.set(bsdata<mapobject>::source);
	return true;
}

bool areai::write(const char* url) const {
	return serial(*const_cast<areai*>(this), url, true);
}

bool areai::read(const char* url) {
	return serial(*const_cast<areai*>(this), url, false);
}