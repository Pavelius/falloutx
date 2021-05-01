#include "main.h"

static int compare_item(const void* v1, const void* v2) {
	auto p1 = *((item**)v1);
	auto p2 = *((item**)v2);
	auto& e1 = p1->geti();
	auto& e2 = p2->geti();
	auto k1 = e1.getkind();
	auto k2 = e2.getkind();
	if(k1 != k2)
		return k1 - k2;
	return strcmp(e1.txt.name, e2.txt.name);
}

void itema::sort() {
	qsort(data, count, sizeof(data[0]), compare_item);
}

int itema::getcost() const {
	auto result = 0;
	for(auto p : *this)
		result += p->geti().cost;
	return result;
}