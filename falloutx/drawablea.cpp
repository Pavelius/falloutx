#include "main.h"

void drawablea::select() {
	auto ps = data + count;
	auto pe = endof();
	for(auto& e : bsdata<creaturei>()) {
		if(!e)
			continue;
		if(ps < pe)
			*ps++ = &e;
	}
	for(auto& e : game.players) {
		if(!e)
			continue;
		if(ps < pe)
			*ps++ = &e;
	}
	count = ps - data;
}

static int compare(const void* v1, const void* v2) {
	auto p1 = *((drawable**)v1);
	auto p2 = *((drawable**)v2);
	if(p1->y != p2->y)
		return p1->y - p2->y;
	return p2->x - p1->x;
}

void drawablea::sortz() {
	qsort(data, count, sizeof(data[0]), compare);
}