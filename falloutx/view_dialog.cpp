#include "main.h"
#include "draw.h"
#include "screenshoot.h"

using namespace draw;

static void setuchar() {
	auto p = (unsigned char*)hot.object;
	*p = hot.param;
}

void draw::setint() {
	auto p = (int*)hot.object;
	*p = hot.param;
}

void draw::setshort() {
	auto p = (short*)hot.object;
	*p = hot.param;
}

void draw::listinput(int& current, int& origin, int perpage, int perline, unsigned maximum) {
	if(current >= (int)maximum)
		current = maximum - 1;
	if(current < 0)
		current = 0;
	if(current < origin)
		origin = current;
	if(origin + perpage < current)
		origin = current - perpage;
	switch(hot.key) {
	case KeyUp:
	case MouseWheelUp:
		if(current)
			execute(setint, current - 1, 0, &current);
		break;
	case MouseWheelDown:
	case KeyDown:
		if((unsigned)current < maximum - 1)
			execute(setint, current + 1, 0, &current);
		break;
	case KeyHome:
		if(current)
			execute(setint, 0, 0, &current);
		break;
	case KeyEnd:
		if(current != maximum - 1)
			execute(setint, maximum - 1, 0, &current);
		break;
	case KeyPageUp:
		if(current)
			execute(setint, current - perpage, 0, &current);
		break;
	case KeyPageDown:
		execute(setint, current + perpage, 0, &current);
		break;
	}
}

void draw::messagev(const char* body, bool focused) {
	screenshoot screen(focused);
	openform();
	while(ismodal()) {
		screen.restore();
		int x, y, sx, sy;
		window(x, y, sx, sy, 217);
		auto old_fore = fore;
		setcolor(ColorState);
		rect rc = {x + 32, y + 32, x + sx - 24, y + sy - 48};
		text(rc, body, AlignCenterCenter);
		image(x + 94, y + sy - 29, INTRFACE, 209, ImageNoOffset);
		if(button(x + 104, y + 101, 90, variant(Next), KeyEscape))
			execute(buttonok);
		fore = old_fore;
		domodal();
	}
	closeform();
}

bool gamei::choosegender(int x, int y, gender_s& value) {
	screenshoot screen;
	openform();
	while(ismodal()) {
		screen.restore();
		image(x, y, INTRFACE, 208, ImageNoOffset);
		if(buttonf(x + 24, y + 2, 212, 213, '1', (value == Male), true))
			execute(setuchar, Male, 0, &value);
		if(buttonf(x + 74, y + 2, 210, 211, '2', (value == Female), true))
			execute(setuchar, Female, 0, &value);
		image(x + 15, y + 42, INTRFACE, 209, ImageNoOffset);
		if(button(x + 25, y + 45, 90, variant(Next), KeyEnter))
			execute(buttonok);
		if(hot.key == KeyEscape)
			execute(buttoncancel);
		domodal();
	}
	closeform();
	return getresult() != 0;
}

bool gamei::chooseage(int x, int y, int& value, int minimal, int maximum) {
	screenshoot screen;
	openform();
	while(ismodal()) {
		screen.restore();
		image(x, y, INTRFACE, 208, ImageNoOffset);
		image(x + 8, y + 8, INTRFACE, 205, ImageNoOffset);
		number(x + 56, y + 10, 2, value);
		if(buttonf(x + 18, y + 13, 122, 123, '-', false))
			execute(setint, value - 1, 0, &value);
		if(buttonf(x + 102, y + 13, 124, 125, '+', false))
			execute(setint, value + 1, 0, &value);
		image(x + 15, y + 42, INTRFACE, 209, ImageNoOffset);
		if(button(x + 25, y + 45, 90, variant(Next), KeyEnter))
			execute(buttonok);
		if(hot.key == KeyEscape)
			execute(buttoncancel);
		domodal();
		if(value < minimal)
			value = minimal;
		if(value > maximum)
			value = maximum;
	}
	closeform();
	return getresult() != 0;
}

bool gamei::choosename(int x, int y, char* value, unsigned maximum) {
	screenshoot screen;
	openform();
	while(ismodal()) {
		screen.restore();
		image(x, y, INTRFACE, 208, ImageNoOffset);
		image(x + 14, y + 10, INTRFACE, 214, ImageNoOffset);
		edit(x + 20, y + 16, 100, value, maximum);
		image(x + 15, y + 42, INTRFACE, 209, ImageNoOffset);
		if(button(x + 25, y + 45, 90, variant(Next), KeyEnter))
			execute(buttonok);
		if(hot.key == KeyEscape)
			execute(buttoncancel);
		domodal();
	}
	closeform();
	return getresult() != 0;
}

static void answer_button(int x, int& y, const char* title, int id, unsigned key) {
	rect rc;
	if(buttonp(x, y, 221, 222, title, key, &rc))
		execute(buttonparam, id);
	y += rc.height() + 2;
}

int answers::choose() const {
	screenshoot screen;
	openform();
	while(ismodal()) {
		screen.restore();
		int x, y, sx, sy;
		window(x, y, sx, sy, 220);
		x += 14; y += 16;
		auto index = 0;
		for(auto& e : elements) {
			if(index > 5)
				break;
			answer_button(x, y, e.text, e.id, '1' + index);
			index++;
		}
		if(index < 5)
			answer_button(x, y, variant(Cancel), 0, KeyEscape);
		domodal();
	}
	closeform();
	return getresult();
}

void answers::paintlist(int x, int y, int width, int origin, int& current, int perpage, int perline) const {
	auto maximum = elements.getcount();
	if(maximum > origin + perpage)
		maximum = origin + perpage;
	for(auto i = origin; i < maximum; i++) {
		auto old_fore = fore;
		if(i == current)
			fore = fore.hilight();
		rect rc = {x, y, x + width, y + perpage};
		if(ishilite(rc) && hot.key == MouseLeft && hot.pressed)
			execute(setint, i, 0, &current);
		text(x, y, elements[i].text);
		fore = old_fore;
		y += perline;
	}
}

bool answers::paintlist(int x, int y, int width, int& result) const {
	auto run = false;
	result = -1;
	for(auto& e : elements) {
		rect rc = {x, y, x + width, y + texth()};
		if(ishilite(rc) && hot.key == MouseLeft && hot.pressed) {
			result = e.id;
			run = true;
		}
		textf(x, y, width, e.text);
		y += texth() + 4;
	}
	return run;
}

int answers::choose(const char* title) const {
	screenshoot screen;
	openform();
	int origin = 0, current = 0, perline = texth() + 2;
	const int perpage = 10;
	const int width = 200;
	while(ismodal()) {
		listinput(current, origin, perpage, perline, elements.getcount());
		screen.restore();
		int x, y, sx, sy;
		window(x, y, sx, sy, 224);
		auto old_font = font;
		auto old_fore = fore;
		setcolor(ColorButton);
		setfont(FONT3);
		text(x + 48, y + 16, title);
		fore = old_fore;
		font = old_font;
		paintlist(x + 56, y + 48, width, origin, current, perpage, perline);
		if(button(x + 58, y + 187, 80, variant(Next), KeyEnter))
			execute(buttonparam, elements[current].id);
		if(button(x + 160, y + 187, 90, variant(Cancel), KeyEscape))
			execute(buttoncancel);
		domodal();
	}
	closeform();
	return getresult();
}

static void trade_creature() {
	auto opponent = (creaturei*)hot.object;
	game.getplayer().trade(*opponent);
}

static void showhistory() {
	static int origin;
	showtext(origin, "Некоторый текст, который очень длинный");
}

int answers::chat(const char* format, int backf, creaturei* opponent) const {
	char temp[260]; stringbuilder sb(temp);
	openform();
	screenshoot push;
	int origin = 0, maximum = -1;
	int origin_aw = 0, maximum_aw = -1;
	while(ismodal()) {
		push.restore();
		if(backf)
			image(126, 14, BACKGRND, backf, ImageNoOffset);
		background(103);
		auto x = 0, y = 290;
		image(x, y, INTRFACE, 99, ImageNoOffset);
		rect rc = {140, 232, 140 + 370, 232 + 45};
		scrollup(rc, origin, maximum, format);
		rect r1 = {120, 340, 140 + 370, 340 + 108};
		if(true) {
			scrollup(r1, origin_aw, maximum_aw);
			state push; setclip(r1);
			auto x = 145, y = 340 - origin_aw, y0 = y, w = 363, i = 0;
			for(auto& e : elements) {
				if(label(x, y, w, e.text, '1' + i, true))
					execute(buttonparam, e.id);
				i++;
			}
			maximum_aw = y - y0;
		}
		sb.clear(); sb.add("$%1i", 31201);
		text(38 - textw(temp) / 2, 324, temp);
		if(buttonf(13, 444, 97, 98, 'V', false))
			execute(showhistory);
		if(buttonf(593, 331, 96, 95, 'T', false))
			execute(trade_creature, 0, 0, opponent);
		domodal();
	}
	push.restore();
	closeform();
	return getresult();
}

static void settext() {
	auto pd = (char*)hot.object;
	auto ps = (char*)hot.param;
	stringbuilder sb(pd, pd + hot.param2);
	sb.add(ps);
	seteditpos(0xFFFFFFFF);
}

void draw::showtext(int& origin, const char* text) {
	while(ismodal()) {
		background(102);
		if(origin < 0)
			origin = 0;
		if(true) {
			state push;
			setclip({95, 95, 95 + 320, 95 + 310});
			textf(95, 95 - origin, 320, text);
		}
		if(buttonf(476, 192, 87, 88, KeyDown, false))
			execute(setint, origin + texth(), 0, &origin);
		if(buttonf(476, 154, 89, 90, KeyUp, false))
			execute(setint, origin - texth(), 0, &origin);
		if(buttonf(500, 398, 91, 92, KeyEscape, false))
			execute(buttoncancel);
		domodal();
	}
	closeform();
}

bool answers::edit(const char* title, char* value, unsigned maximum) const {
	screenshoot screen;
	openform();
	const int perpage = 10;
	const int width = 200;
	int origin = 0, current = 0, perline = texth() + 2;
	auto current_edit = current;
	while(ismodal()) {
		listinput(current, origin, perpage, perline, elements.getcount());
		if(isdefaultinput() && current_edit != current) {
			current_edit = current;
			execute(settext, (int)elements[current].text, maximum, value);
		}
		screen.restore();
		int x, y, sx, sy;
		window(x, y, sx, sy, 225);
		auto old_font = font;
		auto old_fore = fore;
		setcolor(ColorButton);
		setfont(FONT3);
		text(x + 48, y + 16, title);
		fore = old_fore;
		font = old_font;
		paintlist(x + 56, y + 48, 160, origin, current, perpage, perline);
		draw::edit(x + 56, y + 190, 184, value, maximum);
		if(button(x + 58, y + 213, 76, variant(Next), KeyEnter))
			execute(buttonok);
		if(button(x + 160, y + 213, 90, variant(Cancel), KeyEscape))
			execute(buttoncancel);
		domodal();
	}
	closeform();
	return getresult() != 0;
}

variant creaturei::chooseskill() const {
	static skill_s source[] = {Sneak, Lockpick, Steal, Traps, FirstAid, Doctor, Science, Repair};
	openform();
	screenshoot screen;
	while(ismodal()) {
		screen.restore();
		auto x = 452, y = 5;
		image(x, y, INTRFACE, 121, ImageNoOffset);
		auto old_font = font;
		auto old_fore = fore;
		setfont(FONT3);
		setcolor(ColorButton);
		text(x + 55, y + 14, "Умения");
		fore = old_fore;
		font = old_font;
		auto x1 = x + 14, y1 = y + 44;
		auto index = 0;
		for(auto e : source) {
			rect rc;
			if(buttonp(x1, y1, 120, 119, variant(e).getnameshort(), '1' + index, &rc))
				execute(buttonparam, variant(e));
			number(x1 + 97, y1 + 4, 3, get(e));
			index++;
			y1 += rc.height() + 3;
		}
		if(button(x + 48, y + 338, 90, variant(Back), KeyEscape))
			execute(buttoncancel);
		domodal();
	}
	closeform();
	return getresult();
}

void draw::message(const char* format, ...) {
	char temp[512]; stringbuilder sb(temp);
	sb.addv(format, xva_start(format));
	messagev(temp);
}