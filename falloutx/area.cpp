#include "main.h"

areai loc;
BSDATAC(mapobject, 256 * 16)

indext areai::geth(int x, int y) {
	if((unsigned)x >= width * 2 || (unsigned)y >= height * 2)
		return Blocked;
	return y * width * 2 + x;
}

void areai::clear() {
	memset(this, 0, sizeof(0));
	bsdata<mapobject>::source.clear();
}

void areai::set(indext i, int v, int w, int h) {
	auto x0 = getx(i), y0 = gety(i);
	auto x1 = x0 + w, y1 = y0 + h;
	for(auto y = y0; y < y1; y++) {
		for(auto x = x0; x < x1; x++)
			setfloor(geti(x, y), v);
	}
}

void areai::set(indext i, const tilei& e, int w, int h) {
	auto x0 = getx(i), y0 = gety(i);
	auto x1 = x0 + w, y1 = y0 + h;
	for(auto y = y0; y < y1; y++) {
		for(auto x = x0; x < x1; x++)
			setfloor(geti(x, y), e.random());
	}
}

mapobject* areai::add(indext index, draw::res_s rid, short unsigned type) {
	auto p = find(index);
	if(!p) {
		p = bsdata<mapobject>::add();
		memset(p, 0, sizeof(*p));
		p->index = index;
	}
	p->rid = rid;
	p->type = type;
	p->reanimate();
	return p;
}

mapobject* areai::find(indext index) {
	for(auto& e : bsdata<mapobject>()) {
		if(e.index == index)
			return &e;
	}
	return 0;
}