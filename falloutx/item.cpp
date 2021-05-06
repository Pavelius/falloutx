#include "main.h"

static_assert(sizeof(item) == sizeof(int), "Item size must be sizeof(int)");

using namespace draw;

BSDATA(itemi) = {
	{{"NoItem"}},
	{{"Cookie"}, 1, 0, 0, {247, 41}, Leather, {}, {}, {}, Throwing, "00000378"},
	{{"LeatherArmorMarkII"}, 10, 10, 1000, {250, 29}, Leather, {}, {20, HMLTHR, HFLTHR, {3, 1, 1, 1, 1, 0, 1}, {25, 20, 25, 10, 40, 0, 25}}, {}, Throwing, "00000379"},
	{{"MetalArmor"}, 10, 35, 1100, {1, 33}, Metal, {}, {10, HMMETL, HFMETL, {4, 6, 4, 4, 0, 0, 4}, {30, 75, 10, 20, 0, 0, 25}}, {}, Throwing, "00000001"},
	{{"MetalArmorMarkII"}, 10, 35, 1900, {248, 33}, Metal, {}, {15, HMMETL, HFMETL, {4, 7, 4, 4, 1, 0, 4}, {35, 80, 15, 25, 10, 0, 30}}, {}, Throwing, "00000380"},
	{{"AdvancedPowerArmor"}, 15, 90, 20000, {227, 152}, Metal, {}, {30, HANPWR, HANPWR, {15, 19, 16, 15, 15, 0, 20}, {55, 90, 70, 60, 60, 0, 65}}, {}, Throwing, "00000348"},
	{{"AdvPowerArmorMKII"}, 15, 100, 25000, {227, 152}, Metal, {}, {35, HANPWR, HANPWR, {18, 19, 16, 18, 15, 0, 20}, {60, 90, 70, 60, 65, 0, 70}}, {}, Throwing, "00000349"},
	{{"Knife"}, 1, 1, 40, {96, 28, 1}, Metal, {1, 6, 2, Phisycal, 3, 1, NoItem, 0, 0, 1}, {}, {}, MeleeWeapon, "00000012"},
	{{"Spear"}, 4, 4, 80, {13, 42, 4}, Wood, {3, 10, 4, Phisycal, 4, 2, NoItem, 0, 0, 1}, {}, {}, MeleeWeapon, "00000013"},
	{{"X10mmPistol"}, 2, 3, 250, {4, 34, 5}, Metal, {5, 12, 3, Phisycal, 5, 25, X10mmAP, 12, 1, 2}, {}, {}, SmallGuns, "00000004"},
	{{"HuntingRifle"}, 4, 9, 1000, {23, 36, 7}, Metal, {8, 20, 5, Phisycal, 5, 40, X223FMJ, 10, 0, 2}, {}, {}, SmallGuns, "00000011"},
	{{"X10mmSMG"}, 3, 5, 1000, {2, 44, 6}, Metal, {5, 12, 4, Phisycal, 5, 25, X10mmAP, 30, 10, 2}, {}, {}, SmallGuns, "00000002"},
	{{"X10mmAP"}, 0, 1, 100, {30, 1}, Metal, {}, {}, {X10mmAP, 24, 0, -25, 50}, Throwing, "00000030"},
	{{"X10mmJHP"}, 0, 1, 75, {31, 1}, Metal, {}, {}, {X10mmAP, 24, 0, 25, 200}, Throwing, "00000029"},
	{{"X12GaShotgunShells"}, 0, 1, 225, {91, 1}, Metal, {}, {}, {X12GaShotgunShells, 20, 10, 0}, Throwing, "00000095"},
	{{"X14mmAP"}, 0, 1, 150, {32, 1}, Metal, {}, {}, {X14mmAP, 30, 0, -50, 50}, Throwing, "00000033"},
	{{"X223FMJ"}, 0, 2, 200, {35, 1}, Metal, {}, {}, {X223FMJ, 50, 20, -20}, Throwing, "00000034"},
	{{"X2mmEC"}, 0, 1, 400, {235, 1}, Metal, {}, {}, {X2mmEC, 50, 30, -20, 150}, Throwing, "00000358"},
	{{"X44MagnumFMJ"}, 0, 2, 50, {33, 1}, Metal, {}, {}, {X44MagnumFMJ, 20, 0, -20}, Throwing, "00000111"},
	{{"X44MagnumJHP"}, 0, 1, 50, {34, 1}, Metal, {}, {}, {X44MagnumFMJ, 20, 0, 20, 200}, Throwing, "00000031"},
	{{"X45Caliber"}, 0, 1, 75, {236, 1}, Metal, {}, {}, {X45Caliber, 10, 0, 0}, Throwing, "00000357"},
	{{"X47mmCaseless"}, 0, 1, 600, {237, 1}, Metal, {}, {}, {X47mmCaseless, 50, 5, -10, 150}, Throwing, "00000359"},
	{{"X5mmAP"}, 0, 1, 120, {38, 1}, Metal, {}, {}, {X5mmAP, 50, 0, -35, 50}, Throwing, "00000036"},
	{{"X5mmJHP"}, 0, 1, 100, {39, 1}, Metal, {}, {}, {X5mmAP, 50, 0, 35, 200}, Throwing, "00000035"},
	{{"X762mm"}, 0, 1, 150, {241, 1}, Metal, {}, {}, {X762mm, 20, 5, -10}, Throwing, "00000363"},
	{{"X9mm"}, 0, 1, 100, {238, 1}, Metal, {}, {}, {X9mm, 20, 0, 10, 50}, Throwing, "00000360"},
	{{"BBS"}, 1, 2, 20, {116, 1}, Metal, {}, {}, {NoItem, 20, 0, 0}, Throwing, "00000163"},
	{{"SmallEnergyCell"}, 0, 3, 400, {36, 4}, Metal, {}, {}, {SmallEnergyCell, 40, 0, 0}, Throwing, "00000038"},
	{{"MicroFusionCell"}, 0, 5, 1000, {37, 4}, Metal, {}, {}, {MicroFusionCell, 50, 0, 0}, Throwing, "00000039"},
	{{"FlamethrowerFuel"}, 0, 10, 250, {74, 43}, Metal, {}, {}, {FlamethrowerFuel, 10, 20, 25}, Throwing, "00000032"},
	{{"ExplosiveRocket"}, 0, 3, 200, {28, 38}, Metal, {}, {}, {ExplosiveRocket, 1, 0, -25}, Throwing, "00000008"},
};
assert_enum(itemi, ExplosiveRocket)

const itemi& item::geti() const {
	return bsdata<itemi>::elements[type];
}

int item::getclipcount() const {
	if(!geti().weapon.ammo_count)
		return 0;
	return ammo_count;
}

bool item::setclipcount(int v) {
	const auto& ei = geti();
	if(!ei.weapon.ammo_count)
		return false;
	ammo_count = v;
	return true;
}

item_s item::getclipammo() const {
	const auto& ei = geti();
	if(!ei.weapon.ammo_count)
		return NoItem;
	if(!ammo_count)
		return NoItem;
	return (item_s)(ei.weapon.ammo + ammo_index);
}

int item::getcount() const {
	if(type == NoItem)
		return 0;
	if(geti().isweapon())
		return weapon_count + 1;
	return count + 1;
}

bool item::setcount(int v) {
	if(!v) {
		clear();
		return true;
	} else if(type == NoItem)
		return false;
	else {
		auto max = getcountmax();
		if(v > max)
			v = max;
		if(geti().isweapon())
			weapon_count = v - 1;
		else
			count = v - 1;
		return false;
	}
}

int item::getcountmax() const {
	if(geti().isweapon())
		return 4 * 4;
	return 256 * 256;
}

bool item::join(item& e) {
	if(!type) {
		*this = e;
		e.clear();
		return true;
	}
	if(type != e.type)
		return false;
	auto c1 = getcount();
	auto c2 = e.getcount();
	int max = getcountmax();
	if(c1 + c2 > max) {
		c2 = max - c1;
		c1 = max;
	} else {
		c1 = c1 + c2;
		c2 = 0;
	}
	setcount(c1);
	return e.setcount(c2);
}

void item::clear() {
	type = NoItem;
	count = 0;
}

itemkind_s itemi::getkind() const {
	if(isweapon())
		return Weapon;
	else if(isammo())
		return Ammo;
	else if(isarmor())
		return Armor;
	return Misc;
}

void item::addtext(stringbuilder& sb) const {
	auto& ei = geti();
	sb.addn(ei.txt.text);
	if(ei.weapon.min_strenght)
		sb.adds("Ìèí. ÑË: %1i", ei.weapon.min_strenght);
}

const char*	item::getobjectname(const void* object, stringbuilder& sb) {
	return ((item*)object)->getname();
}

void item::create() {
	auto& ei = geti();
	if(ei.weapon.ammo_count)
		setclipcount(ei.weapon.ammo_count);
	else if(ei.ammo.count)
		setcount(ei.ammo.count);
}

creaturei* item::getowner() const {
	return creaturei::ptr(this);
}

bool item::unload() {
	if(getclipcount() == 0)
		return false;
	auto owner = getowner();
	if(!owner)
		return false;
	item it(getclipammo());
	it.setcount(getclipcount());
	if(!owner->additem(it))
		return false;
	setclipcount(0);
	return true;
}