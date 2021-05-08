#include "main.h"

BSDATAC(itemground, 1024)

void itemground::add(indext i, const item& it) {
	itemground* p = 0;
	for(auto& e : bsdata<itemground>()) {
		if(!e)
			continue;
		p = &e;
		break;
	}
	if(!p)
		p = bsdata<itemground>::add();
	*static_cast<item*>(p) = it;
	p->position = i;
}
