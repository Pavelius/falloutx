#include "io.h"
#include "main.h"
#include "logfile.h"

namespace {
struct indexa {
	unsigned		start = 0, count = 0;
	variant*		begin() const;
	variant*		end() const;
	static indexa	add(variant* p, unsigned count);
};
struct dialogi {
	char			id[16];
	flagable<8>		visited;
	bool			isvisited(int v) const { return visited.is(v); }
	void			setvisited(int v) { visited.set(v); }
};
struct chati {
	int				parent;
	int				index, next_index;
	const char*		format;
	indexa			commands;
	bool			isanswer() const { return ((unsigned)next_index) != 0xFFFFFFFF; }
	bool			isnext() const { return next_index != 0 && ((unsigned)next_index) < 0xFFFFFF00; }
};
}
static vector<chati> chats;
static vector<dialogi> dialogs;
static vector<variant> variants;
static int basic_reward[] = {10, 20, 50, 100};
static int basic_stats[] = {3, 5, 7, 9};

variant* find_existing(variant* ps, unsigned count) {
	auto pe = variants.end() - count;
	for(auto p = variants.begin(); p < pe; p++) {
		if(*p != *ps)
			continue;
		if(memcmp(ps, p, count * sizeof(variant)) != 0)
			continue;
		return ps;
	}
	return 0;
}

indexa indexa::add(variant* ps, unsigned count) {
	if(!count)
		return indexa();
	if(!variants)
		variants.reserve(count);
	variant* p = 0;
	if(variants.count >= count)
		p = find_existing((variant*)variants.data, count);
	indexa result;
	if(p) {
		result.start = variants.indexof(p);
		result.count = count;
	} else {
		result.start = variants.count;
		result.count = count;
		variants.reserve(variants.count + count);
		memcpy(variants.ptr(result.start), ps, sizeof(variant) * result.count);
		variants.count += count;
	}
	return result;
}

variant* indexa::begin() const {
	return variants.ptr(start);
}

variant* indexa::end() const {
	return variants.ptr(start + count);
}

static dialogi* find_dialog(const char* id) {
	for(auto& e : dialogs) {
		if(strcmp(e.id, id) == 0)
			return &e;
	}
	return 0;
}

static const char* read_string(const char* p, char* ps, const char* pe) {
	auto pb = ps;
	while(*p) {
		if((p[0] == 13 || p[0] == 10) && (p[1] == '-' || p[1] == '#')) {
			p = skipspcr(p);
			break;
		}
		if(*p == '\r') {
			p++;
			continue;
		}
		if(ps < pe)
			*ps++ = *p;
		p++;
	}
	while(ps > pb && ps[-1] == '\n')
		ps--;
	*ps = 0;
	return p;
}

static const char* read_identifier(const char* p, int& id, indexa& commands) {
	char name[32];
	if(isnum(*p))
		p = stringbuilder::read(p, id);
	else {
		p = stringbuilder::read(p, name, name + sizeof(name));
		id = (int)szdup(name);
	}
	p = skipsp(p);
	adat<variant, 128> source;
	while(p[0] == ',') {
		p = skipsp(p + 1);
		p = stringbuilder::read(p, name, name + sizeof(name));
		p = skipsp(p);
		auto result = variant::find(name);
		source.add(result);
	}
	commands = indexa::add(source.data, source.count);
	return p;
}

static bool read_dialog(const char* url) {
	char value[8192]; szfnamewe(value, url);
	auto p_alloc = (const char*)loadt(url);
	if(!p_alloc)
		return false;
	auto pdg = find_dialog(value);
	if(!pdg) {
		pdg = dialogs.add();
		memset(pdg, 0, sizeof(*pdg));
		stringbuilder sb(pdg->id);
		sb.add(value);
	}
	auto pid = dialogs.indexof(pdg);
	auto p = p_alloc;
	auto records_read = 0;
	chati* pc_base = 0;
	while(*p) {
		auto pc = chats.add();
		memset(pc, 0, sizeof(*pc));
		pc->parent = pid;
		if(p[0] == '#') {
			pc_base = pc;
			pc->next_index = 0xFFFFFFFF;
			p = read_identifier(p + 1, pc->index, pc->commands);
		} else if(p[0] == '-') {
			if(pc_base)
				pc->index = pc_base->index;
			p = read_identifier(p + 1, pc->next_index, pc->commands);
		}
		if(*p == ':')
			p = skipspcr(p + 1);
		else
			return false;
		p = read_string(p, value, value + sizeof(value) - 1);
		pc->format = szdup(value);
		records_read++;
	}
	delete p_alloc;
	return true;
}

bool gamei::readchats(const char* locale) {
	char temp[128]; stringbuilder sb(temp);
	sb.add("chats/%1", locale);
	auto result = true;
	for(io::file::find find(temp); find; find.next()) {
		auto pn = find.name();
		if(pn[0] == '.')
			continue;
		if(!szpmatch(pn, "*.txt"))
			continue;
		char fullname[260]; find.fullname(fullname);
		if(!read_dialog(fullname))
			result = false;
	}
	return result;
}

static const char* chat_next(const char* format, creaturei* opponent, int backf) {
	char temp[1024];
	while(format && format[0]) {
		auto pn = zchr(format, '\n');
		if(!pn)
			return format;
		auto n = pn - format;
		if(n > sizeof(temp) - 1)
			n = sizeof(temp) - 1;
		zcpy(temp, format, n);
		temp[n] = 0;
		answers an;
		an.add(1, variant(Next).getname());
		an.chat(temp, backf, opponent);
		format += n;
		format = skipspcr(format);
	}
	return format;
}

static int getreward(indexa& source) {
	auto number_type = Ave;
	auto modifier = Compare;
	unsigned result = 0;
	for(auto v : source) {
		if(v.type == Number) {
			number_type = (number_s)v.value;
			continue;
		} else if(v.type == Modifier) {
			modifier = (modifier_s)v.value;
			if(modifier != Compare && modifier != OneTime)
				break;
			continue;
		}
		unsigned vi = 0;
		if(v.type == Stat || v.type == Skill) {
			if(number_type >= Ave && number_type <= VeryHight)
				vi = basic_reward[number_type - Low];
		}
		if(vi) {
			if(result < vi)
				result = vi;
		}
	}
	return result;
}

static bool condition(const creaturei* player, dialogi* pdg, chati& e) {
	auto number_type = Ave;
	auto modifier = Compare;
	for(auto v : e.commands) {
		if(v.type == Number) {
			number_type = (number_s)v.value;
			continue;
		} else if(v.type == Modifier) {
			if(v.value == OneTime) {
				if(e.isanswer() && e.next_index && pdg->isvisited(e.next_index))
					return false;
				else if(!e.isanswer() && pdg->isvisited(e.index))
					return false;
			} else {
				modifier = (modifier_s)v.value;
				if(modifier != Compare)
					break;
			}
			continue;
		}
		auto iv = bsdata<numberi>::elements[number_type].value;
		auto i = player->get(v);
		if(v.type == Stat && number_type >= Low && number_type <= VeryHight)
			iv = basic_stats[number_type - Low];
		if(i < iv)
			return false;
	}
	return true;
}

static const chati* find_chat(const creaturei* player, unsigned parent, int id) {
	auto pdg = dialogs.ptr(parent);
	for(auto& e : chats) {
		if(e.parent != parent)
			continue;
		if(e.index != id)
			continue;
		if(!condition(player, pdg, e))
			continue;
		if(e.isanswer())
			break;
		return &e;
	}
	return 0;
}

static int chat_stage(creaturei* opponent, unsigned parent, int id, int backf) {
	answers an;
	const char* format = 0;
	auto pdg = dialogs.ptr(parent);
	auto current_id = 0;
	auto& player = game.getplayer();
	for(auto& e : chats) {
		if(e.parent != parent)
			continue;
		if(e.index != id)
			continue;
		if(!condition(&player, pdg, e))
			continue;
		if(current_id && e.index != current_id)
			break;
		if(e.isanswer()) {
			if(!format)
				continue;
			an.add((int)&e, e.format);
		} else if(!current_id) {
			current_id = e.index;
			format = e.format;
		}
	}
	if(!an.elements)
		return 0;
	auto p = dialogs.ptr(parent);
	p->setvisited(id);
	format = chat_next(format, opponent, backf);
	auto pr = (chati*)an.chat(format, backf, opponent);
	if(pr && pr->isnext()) {
		if(!pdg->isvisited(pr->next_index)) {
			// Give reward if need test
			auto reward_value = getreward(pr->commands);
			if(reward_value)
				player.addexp(reward_value);
		}
		return pr->next_index;
	}
	return 0;
}

void creaturei::chat(creaturei& opponent, const char* cid) {
	auto pdg = find_dialog(cid);
	if(!pdg)
		return;
	auto id = 1;
	while(id)
		id = chat_stage(&opponent, dialogs.indexof(pdg), id, 3);
}

const char* creaturei::getspeech(const char* subject, int id) const {
	auto pdg = find_dialog(subject);
	if(!pdg)
		return 0;
	auto p = find_chat(this, dialogs.indexof(pdg), id);
	if(!p)
		return 0;
	return p->format;
}