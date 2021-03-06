#include "crt.h"
#include "io.h"
#include "stringbuilder.h"

const int maximum_strings = 32;

static const char* read_string(const char* p, char* ps, const char* pe) {
	char sym;
	while(*p && *p != '\n' && *p != '\r') {
		if(p[0] == '\\' && p[1] == 'n') {
			sym = '\n';
			p += 2;
		} else {
			sym = *p;
			p++;
		}
		if(ps < pe)
			*ps++ = sym;
	}
	*ps = 0;
	while(*p == '\n' || *p == '\r') {
		p = skipcr(p);
		p = skipsp(p);
	}
	return p;
}

static const char* read_identifier(const char* p, char* ps, const char* pe) {
	while(*p && (ischa(*p) || isnum(*p) || *p == '_' || *p == ' ')) {
		if(ps < pe)
			*ps++ = *p++;
		else
			break;
	}
	*ps = 0;
	return p;
}

static void* apply_value(array& source, const unsigned* fields, const char* id_value, const char** strings, int count) {
	auto pe = source.end();
	for(auto p = source.begin(); p < pe; p += source.getsize()) {
		auto pn = *((const char**)p);
		if(!pn)
			continue;
		if(strcmp(pn, id_value) != 0)
			continue;
		for(auto i = 0; i < count; i++) {
			if(!strings[i])
				continue;
			if(!fields[i])
				continue;
			auto pp = (const char**)((char*)p + fields[i]);
			*pp = szdup(strings[i]);
		}
		return p;
	}
	return 0;
}

bool readl(const char* url, array& source, unsigned* fields, int fields_count, unsigned* special, int special_count) {
	if(!fields || !fields_count || fields_count>32)
		return false;
	unsigned fields_copy[32];
	unsigned fields_copy_count = 0;
	for(auto i = 0; i < fields_count; i++) {
		if(fields[i])
			fields_copy[fields_copy_count++] = fields[i];
	}
	fields_count = fields_copy_count;
	if(!fields_count)
		return false;
	auto p_alloc = (const char*)loadt(url);
	if(!p_alloc)
		return false;
	auto p = p_alloc;
	char name[128], value[8192];
	auto records_read = 0;
	while(*p) {
		p = read_identifier(p, name, name + sizeof(name) - 1);
		if(p[0] != ':')
			break;
		p = skipsp(p + 1);
		p = read_string(p, value, value + sizeof(value) - 1);
		const char* strings[maximum_strings] = {};
		auto pt = value;
		strings[0] = pt;
		auto count = 0;
		while(pt[0]) {
			if(pt[0] == '.' && fields_count > 1) {
				// ????? ????? ????? ???????? ????????, ?????? ?? ?? ?? ????.
				// ????????, ?? ???????, ??? ???? ???????? name, nameof, text.
				// ?? ????? ??????: Red|Red is bright color. Use it to hilite errors.
				// ?? ?????? ?????: name='Red', nameof=0, text='Red is bright color. Use it to hilite errors'
				pt[0] = 0;
				pt = (char*)skipsp(pt + 1);
				strings[fields_count - 1] = pt;
				count = fields_count;
				break;
			} else if(pt[0] == '|') {
				pt[0] = 0;
				pt = (char*)skipsp(pt + 1);
				if(count < maximum_strings)
					count++;
				strings[count] = pt;
				continue;
			}
			pt++;
		}
		if(!count)
			count = 1;
		if(count > fields_count)
			count = fields_count;
		auto ps = apply_value(source, fields, name, strings, count);
		records_read++;
		while(special && special_count && p[0] == '-') {
			int special_index = -1;
			p = stringbuilder::read(p + 1, special_index);
			if(p[0] != ' ')
				break;
			p = skipsp(p + 1);
			p = read_string(p, value, value + sizeof(value) - 1);
			if(ps && special_index < special_count) {
				auto pp = (const char**)((char*)ps + special[special_index]);
				*pp = szdup(value);
			}
		}
	}
	delete p_alloc;
	return true;
}

static int sort_by_id(const void* v1, const void* v2, void* param) {
	auto p1 = *(*((const char***)v1));
	auto p2 = *(*((const char***)v2));
	return strcmp(p1, p2);
}

bool savel(const char* url, array& source, unsigned* fields, int fields_count, unsigned* special, int special_count) {
	io::file file(url, StreamText | StreamWrite);
	if(!file)
		return false;
	int count = source.getcount();
	vector<void*> sorted;
	for(auto i = 0; i < count; i++)
		sorted.add(source.ptr(i));
	sorted.sort(0, sorted.getcount() - 1, sort_by_id, 0);
	for(auto i = 0; i < count; i++) {
		auto p = sorted[i];
		auto id = *((const char**)p);
		if(!id || id[0] == 0)
			continue;
		file << id << ": ";
		auto out_count = 0;
		for(auto j = 0; j < fields_count; j++) {
			if(!fields[j])
				continue;
			auto pr = *((const char**)((char*)p + fields[j]));
			if(j == fields_count - 1)
				file << ". ";
			else if(out_count != 0)
				file << "|";
			if(pr)
				file << pr;
			out_count++;
		}
		file << "\r\n";
		for(auto j = 0; j < special_count; j++) {
			if(!special[j])
				continue;
			auto pr = *((const char**)((char*)p + special[j]));
			if(!pr || !pr[0])
				continue;
			file << "-" << j << " " << pr;
			file << "\r\n";
		}
	}
	return true;
}