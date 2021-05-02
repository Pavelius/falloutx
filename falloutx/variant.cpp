#include "main.h"
#include "logfile.h"

static_assert(sizeof(variant) == sizeof(int), "Variant must be sizoef(int)");

#define VRNA(E) #E, {}, {}
#define VRAR(E) #E, bsdata<E##i>::source_ptr, {}
#define VRAE(E, A) #E, bsdata<E##i>::source_ptr, A
#define VRNF() {FO(texti, name), FO(texti, text)}, {FO(texti, name_short), FO(texti, name_abr)}
#define VRNX() {FO(texti, name)}

struct varianti {
	const char*		id;
	array*			source;
	array*			source_ex;
	unsigned		fields[2];
	unsigned		special[2];
};

BSDATA(varianti) = {
	{"NoVariant"},
	{VRAR(action), VRNF()},
	{VRAR(command), VRNX()},
	{VRAE(creature, &game.playersa)},
	{VRAR(damage), VRNF()},
	{VRAR(formula), VRNF()},
	{VRAR(item), VRNF()},
	{VRAR(gender), VRNF()},
	{VRAR(grade), VRNF()},
	{VRAR(number)},
	{VRAR(modifier), VRNF()},
	{VRNA(perk)},
	{VRAR(pregen), VRNF()},
	{VRAR(skill), VRNF()},
	{VRAR(stat), VRNF()},
	{VRAR(trait), VRNF()},
	{VRAR(wound), VRNF()},
};
assert_enum(varianti, Wound)

bool readl(const char* url, array& source, unsigned* fields, int fields_count, unsigned* special, int special_count);
bool savel(const char* url, array& source, unsigned* fields, int fields_count, unsigned* special, int special_count);

void gamei::localization(const char* id, bool write_mode) {
	char temp[260]; stringbuilder sb(temp);
	for(auto& e : bsdata<varianti>()) {
		if(!e.source || !e.fields[0])
			continue;
		sb.clear();
		sb.add("localization/%1/%2.txt", id, e.id);
		if(write_mode)
			savel(temp, *e.source, e.fields, sizeof(e.fields) / sizeof(e.fields[0]), e.special, sizeof(e.special) / sizeof(e.special[0]));
		else
			readl(temp, *e.source, e.fields, sizeof(e.fields) / sizeof(e.fields[0]), e.special, sizeof(e.special) / sizeof(e.special[0]));
	}
}

bool gamei::checklocal() {
	logfile errors("errors");
	char temp[260]; stringbuilder sb(temp);
	for(auto& e : bsdata<varianti>()) {
		if(!e.source || !e.fields[0])
			continue;
		sb.clear();
		sb.add("In localization file %1.txt", e.id);
		auto pe = (char*)e.source->end();
		for(auto ps = (char*)e.source->begin(); ps < pe; ps += e.source->getsize()) {
			auto pi = *((const char**)(ps + 0));
			if(!pi)
				continue;
			auto pn = *((const char**)(ps + e.fields[0]));
			if(pn)
				continue;
			if(temp[0]) {
				errors.addv(temp, 0, 0);
				sb.clear();
			}
			errors.add("Not found translation %1", pi);
		}
	}
	return !errors;
}

int	variant::getavatar() const {
	switch(type) {
	case Command: return bsdata<commandi>::elements[value].avatar;
	case Damage: return bsdata<damagei>::elements[value].avatar;
	case Formula: return bsdata<formulai>::elements[value].avatar;
	case Stat: return bsdata<stati>::elements[value].avatar;
	case Skill: return bsdata<skilli>::elements[value].avatar;
	case Trait: return bsdata<traiti>::elements[value].avatar;
	case Wound: return bsdata<woundi>::elements[value].avatar;
	default: return -1;
	}
}

const char* variant::getname() const {
	if(type == Creature) {
		auto p = (creaturei*)getpointer();
		return p->getname();
	} else {
		auto& e = bsdata<varianti>::elements[type];
		auto ps = e.source;
		if(!ps)
			return "array?";
		if(!e.fields[0])
			return "name?";
		auto v = *((const char**)((char*)ps->ptr(value) + e.fields[0]));
		if(!v)
			return "null?";
		return v;
	}
}

const char* variant::getdescription() const {
	auto& e = bsdata<varianti>::elements[type];
	auto ps = e.source;
	if(!ps)
		return 0;
	if(!e.fields[1])
		return 0;
	auto v = *((const char**)((char*)ps->ptr(value) + e.fields[1]));
	if(!v)
		return 0;
	return v;
}

const char* variant::getnameshort() const {
	auto& e = bsdata<varianti>::elements[type];
	auto ps = e.source;
	if(!ps)
		return "array?";
	if(!e.special[0])
		return getname();
	auto v = *((const char**)((char*)ps->ptr(value) + e.special[0]));
	if(!v)
		return getname();
	return v;
}

const char* variant::getnameabr() const {
	auto& e = bsdata<varianti>::elements[type];
	auto ps = e.source;
	if(!ps)
		return 0;
	if(!e.special[1])
		return 0;
	auto v = *((const char**)((char*)ps->ptr(value) + e.special[1]));
	if(!v)
		return 0;
	return v;
}

const char* variant::getformat() const {
	switch(type) {
	case Stat:
		switch(value) {
		case CriticalHit:
		case CriticalMiss:
		case PoisonResistance:
		case RadiationResistance:
			return "%1i%%";
		default: return "%1i";
		}
	case Skill: return "%1i%%";
	case Damage: return "%1i%%";
	default: return "%1i";
	}
}

const unsigned custom_value_base = 240;

variant::variant(const void* v) : type(NoVariant), value(0) {
	for(auto& e : bsdata<varianti>()) {
		if(!e.source)
			continue;
		auto i = e.source->indexof(v);
		if(i==-1 && e.source_ex) {
			i = e.source_ex->indexof(v);
			if(i != -1)
				i += custom_value_base;
		}
		if(i == -1)
			continue;
		type = (variant_s)(&e - bsdata<varianti>::begin());
		value = i;
		break;
	}
}

void* variant::getpointer() const {
	if(value < 240)
		return bsdata<varianti>::elements[type].source->ptr(value);
	return bsdata<varianti>::elements[type].source_ex->ptr(value - custom_value_base);
}

variant	variant::find(const char* id) {
	for(auto& e : bsdata<varianti>()) {
		if(!e.source)
			continue;
		if(!e.fields[0])
			continue;
		auto s = e.source->getsize();
		auto pe = e.source->end();
		for(auto pb = e.source->begin(); pb < pe; pb += s) {
			auto p = *((const char**)pb);
			if(strcmp(p, id) == 0)
				return variant((variant_s)(&e - bsdata<varianti>::begin()), e.source->indexof(pb));
		}
	}
	return variant();
}