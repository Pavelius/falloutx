#include "main.h"

void answers::clear() {
	sc.clear();
	elements.clear();
}

int answers::compare(const void* v1, const void* v2) {
	return strcmp(((answers::element*)v1)->text, ((answers::element*)v2)->text);
}

answers::element* answers::addv(int id, const char* text, const char* format) {
	auto p = elements.add();
	p->id = id;
	p->text = sc.get();
	sc.addv(text, format);
	sc.addsz();
	return p;
}

void answers::addvm(const char* name, const char* format) {
	auto p = addv(0, name, format);
	p->id = (int)p->text;
}

void answers::sort() {
	qsort(elements.data, elements.count, sizeof(elements.data[0]), compare);
}

int	answers::random() const {
	if(!elements.count)
		return 0;
	return elements.data[rand() % elements.count].id;
}