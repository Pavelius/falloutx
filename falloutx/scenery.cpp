#include "main.h"

using namespace draw;

BSDATAC(scenery, 256*16)

scenery* scenery::add(indext index, short unsigned type) {
	auto p = find(index);
	if(!p) {
		p = bsdata<scenery>::add();
		memset(p, 0, sizeof(*p));
		p->index = index;
	}
	p->type = type;
	p->flags = 0;
	p->reanimate();
	return p;
}

scenery* scenery::find(indext index) {
	for(auto& e : bsdata<scenery>()) {
		if(e.index == index)
			return &e;
	}
	return 0;
}