#include "main.h"

animate_s actor::getbaseanimate(animate_s v, int* w) {
	if(isweaponanimate(v)) {
		if(w)
			*w = (v - FirstWeaponAnimate) / 13;
		return animate_s(FirstWeaponAnimate + (v - FirstWeaponAnimate) % 13);
	}
	return v;
}

bool actor::isweaponanimate(animate_s v) {
	return v >= FirstWeaponAnimate;
}

animate_s actor::getanimate(animate_s v, int w) {
	if(w != -1 && isweaponanimate(v))
		return animate_s(v + w * 13);
	return v;
}

int	actor::getcicle(draw::res_s& rid, animate_s v, gender_s gender, item_s armor, item_s weapon, int dir) {
	rid = (gender == Male) ? bsdata<itemi>::elements[armor].armor.male : bsdata<itemi>::elements[armor].armor.female;
	if(!rid)
		rid = (gender == Male) ? draw::HMJMPS : draw::HFJMPS;
	auto wp = bsdata<itemi>::elements[weapon].avatar.animation;
	if(wp > 0) {
		switch(v) {
		case AnimateStand: v = AnimateWeaponStand; break;
		case AnimateWalk: v = AnimateWeaponWalk; break;
		case AnimateDodge: v = AnimateWeaponDodge; break;
		case AnimateThrown: v = AnimateWeaponThrow; break;
		case AnimateUnarmed1: v = AnimateWeaponSwing; break;
		case AnimateUnarmed2: v = AnimateWeaponThrust; break;
		}
		return (v + (wp - 1) * 13) * 6 + dir;
	}
	return v * 6 + dir;
}

void actor::reanimate() {
	draw::res_s rid;
	auto cicle = getcicle(rid, animate, getgender(), getarmor(), getweapon(), direction);
	set(rid, cicle);
	next_stamp = game.getaitime();
}

void actor::setdirection(unsigned char v) {
	direction = v % 6;
	set(rid, animate * 6 + direction);
}

void actor::setanimate(animate_s v, bool force) {
	if(!force && animate == v)
		return;
	animate = v;
	reanimate();
}

bool wearable::additem(item& ei) {
	for(auto& e : gears) {
		if(!e)
			continue;
		if(e.join(ei))
			return true;
	}
	for(auto& e : gears) {
		if(e)
			continue;
		e = ei;
		ei.clear();
		return true;
	}
	return false;
}

bool wearable::additems(wearable& source) {
	for(auto& e : source.gears) {
		if(!e)
			continue;
		if(!additem(e))
			return false;
	}
	return true;
}