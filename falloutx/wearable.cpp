#include "main.h"

void wearable::getitems(itema& result) {
	auto ps = result.begin();
	auto pe = result.endof();
	for(auto& e : gears) {
		if(!e)
			continue;
		if(ps < pe)
			*ps++ = &e;
	}
	result.count = ps - result.begin();
}

bool wearable::isgear(const item* p) const {
	return p >= gears && p <= gears + sizeof(gears) / sizeof(gears[0]);
}