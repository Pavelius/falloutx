#include "draw.h"
#include "main.h"
#include "screenshoot.h"

using namespace draw;

namespace {
struct number_widget {
	point				position;
	int					value;
	int					frame;
	unsigned			start;
};
struct action_widget {
	action_s			action;
	fnevent				proc;
	void*				object;
	int					param;
	fntext				getname;
	void				execute();
};
}

static bool				break_modal;
static int				break_result;
static point			camera;
static fnevent			mainproc;
fnevent					draw::domodal;
extern rect				sys_static_area;
extern unsigned char	color_pallette[256 * 3];
static color			color_values[256];
static fnevent			current_stage;
static surface			original_surface(640, 480, 32);
static unsigned			tick;
static unsigned			caret_position;
static unsigned			stamp_position;
static point			mouse_position;
static unsigned			stamp_pressed;
static bool				action_mode;
drawable				draw::cursor;
static adat<action_widget, 24> contextmenu;
static adat<number_widget, 24> number_widgets;

unsigned getunsigedtick();
void apply_pallette_cicle(unsigned char* pal, unsigned dwCurrentTime);

static bool checkpressed(const rect& rc, bool& run, unsigned key, bool execute_by_press = false, bool disabled = false) {
	static rect	button_rect;
	if(disabled)
		return false;
	auto pressed = false;
	if(ishilite(rc)) {
		if(hot.pressed)
			pressed = true;
		if(hot.pressed == execute_by_press && hot.key == MouseLeft)
			run = true;
	}
	if(key && hot.key == key)
		button_rect = rc;
	if(hot.key == InputKeyUp) {
		if(button_rect == rc) {
			run = true;
			button_rect.clear();
		}
	}
	if(button_rect == rc)
		pressed = true;
	return pressed;
}

static number_widget* find_number_widget(point pt) {
	for(auto& e : number_widgets) {
		if(e.position == pt)
			return &e;
	}
	return 0;
}

static number_widget* get_number_widget(point pt, int value) {
	auto p = find_number_widget(pt);
	if(!p) {
		p = number_widgets.add();
		p->position = pt;
		p->value = value;
		p->frame = 0;
		p->start = 0;
	}
	return p;
}

void action_widget::execute() {
	if(!proc)
		return;
	hot.object = object;
	hot.param = param;
	proc();
}

unsigned draw::getuitime() {
	if(!tick)
		tick = getunsigedtick();
	return tick;
}

void draw::openform() {
	number_widgets.clear();
	caret_position = 0xFFFFFFFF;
}

void draw::closeform() {
}

void draw::setpallette(int daylight) {
	for(int i = 0; i < 256; i++) {
		color_values[i].r = color_pallette[i * 3 + 0] * daylight;
		color_values[i].g = color_pallette[i * 3 + 1] * daylight;
		color_values[i].b = color_pallette[i * 3 + 2] * daylight;
	}
	draw::palt = color_values;
}

void draw::addaction(action_s a, fnevent proc, void* object, int param, fntext getname) {
	auto p = contextmenu.add();
	p->action = a;
	p->object = object;
	p->proc = proc;
	p->param = param;
	p->getname = getname;
}

bool draw::radio(int x, int y, int cicle, unsigned key) {
	static rect	button_rect;
	auto ps = gres(INTRFACE);
	int frame = ps->ganim(cicle, 0);
	rect rc = {x, y,
		x + ps->get(frame).sx - 1,
		y + ps->get(frame).sy - 1};
	auto r = false;
	if(checkpressed(rc, r, key))
		frame++;
	image(x, y, ps, frame, ImageNoOffset);
	return r;
}

bool draw::button(int x, int y, int width, const char* string, unsigned key, rect* return_rc) {
	auto old_font = font;
	auto old_fore = fore;
	setcolor(ColorButton);
	setfont(FONT3);
	auto ps = gres(INTRFACE);
	auto frame = ps->ganim(8, 0);
	int sx = ps->get(frame).sx;
	int sy = draw::texth();
	int w1 = width - sx;
	text(x + sx + (w1 - textw(string)) / 2, y, string);
	rect rc = {x, y, x + width, y + sy};
	auto a = ishilite(rc);
	auto result = false;
	if(checkpressed(rc, result, key))
		frame++;
	image(x, y + 1, ps, frame, ImageNoOffset);
	//rectb(rc, colors::red);
	if(return_rc)
		*return_rc = rc;
	font = old_font;
	fore = old_fore;
	return result;
}

bool draw::buttonf(int x, int y, int cicles_normal, int cicle_pressed, unsigned key, bool checked, bool execute_by_press, bool* result_pressed, bool disabled) {
	auto ps = gres(INTRFACE);
	auto normal = ps->ganim(cicles_normal, 0);
	auto pressed = ps->ganim(cicle_pressed, 0);
	auto result = false;
	rect rc = {x, y,
		x + ps->get(normal).sx - 1,
		y + ps->get(normal).sy - 1};
	auto a = checkpressed(rc, result, key, execute_by_press, disabled);
	if(checked)
		a = true;
	if(result_pressed)
		*result_pressed = a;
	image(x, y, ps, a ? pressed : normal, ImageNoOffset);
	return result;
}

bool draw::buttong(int x, int y, int width, const char* title, unsigned key) {
	auto old_fore = fore;
	setcolor(ColorButton);
	auto r = radio(x, y, 299, key);
	text(x + width - textw(title) / 2, y, title);
	fore = old_fore;
	return r;
}

bool draw::buttonp(int x, int y, int cicles_normal, int cicle_pressed, const char* title, unsigned key, rect* return_rc) {
	auto old_font = font;
	auto old_fore = fore;
	setcolor(ColorButton);
	setfont(FONT3);
	auto ps = gres(INTRFACE);
	auto frame_normal = ps->ganim(cicles_normal, 0);
	auto frame_pressed = ps->ganim(cicle_pressed, 0);
	auto sx = ps->get(frame_normal).sx;
	auto sy = ps->get(frame_normal).sy;
	rect rc = {x, y, x + sx, y + sy};
	auto a = ishilite(rc);
	auto result = false;
	auto frame = frame_normal;
	if(checkpressed(rc, result, key))
		frame = frame_pressed;
	image(x, y + 1, ps, frame, ImageNoOffset);
	if(frame == frame_pressed)
		x++;
	text(x + 1 + (sx - textw(title)) / 2, y + 2 + (sy - texth()) / 2, title);
	//rectb(rc, colors::red);
	if(return_rc)
		*return_rc = rc;
	font = old_font;
	fore = old_fore;
	return result;
}

bool draw::label(int x, int& y, int width, const char* format, unsigned key, bool bullet) {
	y += 2;
	rect rc = {x, y, x + width, y}; textf(rc, format);
	auto run = false;
	auto old_fore = fore;
	auto need_pressed = false;
	if(ishilite(rc)) {
		fore = fore.hilight();
		if(hot.key == MouseLeft) {
			if(!hot.pressed)
				run = true;
		}
		if(hot.pressed)
			need_pressed = true;
	}
	if(checkpressed(rc, run, key))
		fore = fore.darken();
	if(bullet)
		text(x - textw('A') - 1, y, "~");
	y += textf(x, y, width, format);
	y += 2;
	fore = old_fore;
	return run;
}

void draw::post(void* p, fnevent ev, bool run) {
	if(run)
		execute(ev, 0, 0, p);
}

static void render_number(int x, int y, int digits, int value, sprite* ps, int cicle, int symbol_offset, int symbol_width, bool animate) {
	if(!ps)
		return;
	auto pw = get_number_widget({(short)x, (short)y}, value);
	auto fr = ps->ganim(cicle, 0);
	auto& pf = ps->get(fr);
	auto symbol_height = pf.sy;
	char temp[16]; stringbuilder sb(temp);
	sb.adduint(value, digits, 10);
	state push;
	for(int i = 0; i < digits; i++) {
		clipping.y1 = y;
		clipping.y2 = y + symbol_height;
		clipping.x1 = x + i * symbol_width;
		clipping.x2 = x + (i + 1) * symbol_width;
		int sym = temp[i] - '0';
		if(animate && pw->value != value) {
			char t1[16]; stringbuilder sb(t1);
			sb.adduint(pw->value, digits, 10);
			if(t1[i] != temp[i]) {
				if(!pw->start) {
					pw->start = getuitime();
					if(pw->value > value)
						pw->frame = 1;
					else
						pw->frame = -1;
				} else if(pw->frame == 0) {
					pw->value = value;
					pw->start = 0;
				} else if(pw->frame > 0) {
					sym = 11;
					if((getuitime() - pw->start) / 60) {
						pw->start = getuitime();
						pw->frame = 0;
					}
				} else {
					sym = 10;
					if((getuitime() - pw->start) / 60) {
						pw->start = getuitime();
						pw->frame = 0;
					}
				}
			}
		}
		image(x + (i - sym) * symbol_width - symbol_offset, y, ps, fr, ImageNoOffset);
	}
}

void draw::number(int x, int y, int digits, int value) {
	if(value >= 0)
		render_number(x, y, digits, value, gres(INTRFACE), 170, 0, 14, true);
	else
		render_number(x, y, digits, value, gres(INTRFACE), 170, 12 * 14, 14, true);
}

void draw::numbersm(int x, int y, int digits, int value, int mode) {
	render_number(x, y, digits, value, gres(INTRFACE), 82, 0, 9, true);
}

void draw::numberap(int x, int y, int digits, int value) {
	render_number(x, y, digits, value, gres(INTRFACE), 290, 0, 10, false);
}

void draw::monthname(int x, int y, int value) {
	state push;
	const int dy = 15;
	setclip({x, y, x + 30, y + dy});
	image(x, y - dy * (value - 1), INTRFACE, 129, ImageNoOffset);
}

void draw::gear(int x, int y, int sx, int sy, item& it, bool resize) {
	if(!it)
		return;
	auto cl = it.geti().avatar.inventory;
	if(!cl)
		return;
	auto ps = gres(INVEN);
	if(!ps)
		return;
	auto fr = ps->ganim(cl, 0);
	int isx = ps->get(fr).sx;
	int isy = ps->get(fr).sy;
	if(!resize || (isx <= sx && isy <= sy))
		image(x + (sx - isx) / 2, y + (sy - isy) / 2, ps, fr, ImageNoOffset);
	else {
		int rsx = sx;
		int rsy = sy;
		if(isx > sx)
			rsy = (isy * sx) / isx;
		else
			rsx = (isx * sy) / isy;
		if(rsy > sy) {
			rsy = sy;
			rsx = (isx * sy) / isy;
		}
		surface real(rsx, rsy, 32);
		if(true) {
			surface zoom(isx, isy, 32);
			state push;
			canvas = &zoom; setclip();
			fore = colors::white;
			fore.a = 0xFF;
			rectf({0, 0, isx, isy});
			image(0, 0, ps, fr, ImageNoOffset);
			blit(real, 0, 0, real.width, real.height, 0, zoom, 0, 0, zoom.width, zoom.height);
		}
		blit(*draw::canvas,
			x + (sx - real.width) / 2,
			y + (sy - real.height) / 2,
			real.width, real.height,
			ImageTransparent, real, 0, 0);
	}
}

static void remove_symbol() {
	auto p = (char*)hot.object;
	auto i = hot.param;
	auto m = zlen(p + i);
	if(!m)
		return;
	memmove(p + i, p + i + 1, m);
	caret_position = i;
}

static void add_symbol() {
	auto p = (char*)hot.object;
	auto i = hot.param;
	auto s = hot.param2;
	auto m = zlen(p);
	auto c = m - i;
	memmove(p + i + 1, p + i, c);
	p[i + c + 1] = 0;
	p[i] = s;
	caret_position = i + 1;
}

void draw::seteditpos(unsigned index) {
	caret_position = index;
}

void draw::edit(int x, int y, int width, char* title, unsigned maximum) {
	if(!title)
		return;
	unsigned lenght = zlen(title);
	if(caret_position > lenght)
		caret_position = lenght;
	//line(x, y, x + width, y, colors::red);
	text(x, y, title);
	auto frame = (getuitime() / 200) % 3;
	if(frame != 0) {
		auto x1 = x + textw(title, caret_position);
		rectf({x1 - 1, y - 1, x1 + 2, y + texth() - 1}, fore, 192);
	}
	switch(hot.key) {
	case KeyLeft:
		if(caret_position > 0)
			execute(setint, caret_position - 1, 0, &caret_position);
		break;
	case KeyRight:
		if(caret_position < lenght)
			execute(setint, caret_position + 1, 0, &caret_position);
		break;
	case KeyHome:
		execute(setint, 0, 0, &caret_position);
		break;
	case KeyEnd:
		execute(setint, lenght, 0, &caret_position);
		break;
	case KeyBackspace:
		if(caret_position > 0)
			execute(remove_symbol, caret_position - 1, 0, title);
		break;
	case KeyDelete:
		execute(remove_symbol, caret_position, 0, title);
		break;
	case InputSymbol:
		if(lenght < maximum - 1 && hot.param >= ' ')
			execute(add_symbol, caret_position, hot.param, title);
		break;
	}
}

void draw::window(int& x, int& y, int& sx, int& sy, int frame, int dy) {
	auto ps = gres(INTRFACE);
	if(!ps)
		return;
	auto width = draw::getwidth();
	auto height = draw::getheight();
	auto id = ps->ganim(frame, 0);
	sx = ps->get(id).sx;
	sy = ps->get(id).sy;
	x = (width - sx) / 2;
	y = (height - sy) / 2 + dy;
	image(x, y, ps, id, ImageNoOffset);
}

void draw::background(int cicle) {
	auto ps = gres(INTRFACE);
	auto frame = ps->ganim(cicle, 0);
	draw::image(0, 0, ps, frame, ImageNoOffset, 255);
}

void draw::scrollup(const rect& rc, int& origin, int maximum) {
	if(maximum == -1)
		return;
	auto height = rc.height();
	if(origin > maximum - height)
		origin = maximum - height;
	if(origin < 0)
		origin = 0;
	if(ishilite(rc)) {
		rect ru = {rc.x1, rc.y1, rc.x2, rc.y1 + texth() + 2};
		rect rd = {rc.x1, rc.y2 - texth() - 2, rc.x2, rc.y2};
		if(ishilite(ru)) {
			if(origin > 0) {
				cursor.set(INTRFACE, 268);
				if(hot.key == MouseLeft && hot.pressed)
					execute(setint, origin - texth(), 0, &origin);
			}
		} else if(ishilite(rd)) {
			if(origin < maximum - height) {
				cursor.set(INTRFACE, 269);
				if(hot.key == MouseLeft && hot.pressed)
					execute(setint, origin + texth(), 0, &origin);
			}
		}
		switch(hot.key) {
		case MouseWheelUp:
			execute(setint, origin - texth(), 0, &origin);
			break;
		case MouseWheelDown:
			execute(setint, origin + texth(), 0, &origin);
			break;
		}
	}
}

void draw::scrollup(const rect& rc, int& origin, int& maximum, const char* format) {
	scrollup(rc, origin, maximum);
	state push; setclip(rc);
	maximum = textf(rc.x1, rc.y1 - origin, rc.width(), format);
}

void draw::image(int x, int y, res_s id, int cicle, int flags, unsigned char alpha) {
	auto p = gres(id);
	if(!p)
		return;
	auto f = p->ganim(cicle, getuitime());
	image(x, y, p, f, flags, alpha);
}

void draw::animate(int x, int y, sprite* ps, int cicle, int fps, int frame, int frame_end) {
	auto pc = ps->gcicle(cicle);
	if(!pc || !pc->count)
		return;
	auto fb = pc->start;
	auto fe = pc->start + pc->count - 1;
	if(frame == -2)
		frame = fe;
	else if(frame == -1)
		frame = fb;
	if(frame_end == -2)
		frame_end = fe;
	else if(frame_end == -1)
		frame_end = fb;
	auto d = (frame > frame_end) ? -1 : 1;
	screenshoot push;
	auto old_push = cursor;
	auto tick_rate = 2500 / fps;
	auto next_tick = getuitime() + tick_rate;
	while(ismodalex()) {
		push.restore();
		cursor.set(INTRFACE, 295);
		image(x, y, ps, frame, 0);
		domodal();
		if(next_tick < getuitime()) {
			if(frame == frame_end)
				break;
			next_tick += tick_rate;
			frame += d;
		}
	}
	push.restore();
}

bool draw::istips(unsigned t) {
	if(!stamp_position)
		return false;
	return (getuitime() - stamp_position) >= t;
}

bool draw::ispressed(unsigned t) {
	if(!stamp_pressed)
		return false;
	return (getuitime() - stamp_pressed) >= t;
}

bool draw::istipsonetime() {
	static point position;
	if(hot.key == MouseLeft || hot.key == MouseRight)
		position = hot.mouse;
	if(position == hot.mouse)
		return false;
	position = hot.mouse;
	return true;
}

void actor::preview(int x, int y, gender_s gender, item_s armor, item_s weapon, unsigned tick) {
	res_s rid;
	int c = getcicle(rid, AnimateStand, gender, armor, weapon, tick % 6);
	auto ps = gres(rid);
	if(!ps)
		return;
	image(x, y, ps, ps->ganim(c, 0), 0);
}

void draw::setcolor(color_s value) {
	fore = color_values[value];
}

void draw::setfont(res_s v) {
	font = gres(v);
}

void draw::breakmodal(int v) {
	hot.key = 0;
	hot.pressed = false; // Чтобы кнопка не нажималась
	break_modal = true;
	break_result = v;
}

void draw::buttoncancel() {
	breakmodal(0);
}

void draw::buttonok() {
	breakmodal(1);
}

void draw::buttonparam() {
	breakmodal(hot.param);
}

void draw::execute(fnevent proc, int value, int value2, void* object) {
	domodal = proc;
	hot.key = 0;
	hot.param = value;
	hot.param2 = value2;
	hot.object = object;
}

int draw::getresult() {
	return break_result;
}

fnevent draw::getnextstage() {
	return current_stage;
}

void draw::setstage(fnevent v) {
	current_stage = v;
}

void draw::setnextstage() {
	setstage((fnevent)hot.object);
}

void draw::runstage() {
	while(current_stage) {
		auto p = current_stage;
		current_stage = 0;
		p();
	}
}

void draw::initialize() {
	create(-1, -1, 640, 480, WFResize | WFMinmax, 32);
	setcaption("Fallout engine 0.1");
	setpallette(4);
	settimer(48);
	syscursor(false);
	setfont(FONT1);
	setcolor(ColorText);
	settimer(100);
	fore_stroke = colors::blue;
	metrics::font = gres(FONT1);
	colors::h1 = color_values[ColorButton];
	metrics::h1 = gres(FONT4);
	colors::h2 = color_values[ColorButton];
	metrics::h2 = gres(FONT3);
	colors::h3 = color_values[ColorText].darken();
	metrics::h3 = gres(FONT2);
	colors::special = color_values[ColorCheck];
}

static action_widget* context_menu() {
	auto ps = gres(INTRFACE);
	if(!ps)
		return 0;
	screenshoot screen;
	openform();
	point cps; getcursor(cps);
	const auto size = 40;
	auto push_action = action_mode;
	auto push_mouse = hot.mouse;
	auto menu = contextmenu;
	auto height = menu.getcount() * size;
	action_mode = false;
	while(ismodal()) {
		screen.restore();
		cursor.set(None, 0);
		image(push_mouse.x, push_mouse.y, INTRFACE, 250);
		auto x = push_mouse.x + 48;
		auto y = push_mouse.y + 40;
		auto i1 = 0;
		auto i2 = (hot.mouse.y - push_mouse.y) / size;
		if(i2 > menu.getcount() - 1)
			i2 = menu.getcount() - 1;
		if(i2 < 0)
			i2 = 0;
		for(auto& e : menu) {
			auto i = bsdata<actioni>::elements[e.action].avatar;
			if(i1 == i2) {
				switch(hot.key) {
				case MouseLeft:
				case MouseRight:
					execute(buttonparam, (int)&e);
					break;
				}
			} else
				i++;
			image(x, y, ps, ps->ganim(i, 0), 0);
			y += 40; i1++;
		}
		domodal();
	}
	setcursor(cps);
	hot.mouse = push_mouse;
	action_mode = push_action;
	closeform();
	return (action_widget*)getresult();
}

static void standart_domodal() {
	static void* tips_object;
	auto x = hot.mouse.x + cursor.x;
	auto y = hot.mouse.y + cursor.y;
	if(action_mode) {
		auto ps = gres(INTRFACE);
		if(ps) {
			image(x, y, ps, ps->ganim(250, 0), 0);
			if(contextmenu && istips(500)) {
				auto& e = contextmenu[0];
				auto frame = bsdata<actioni>::elements[e.action].avatar;
				image(x + 48, y + 40, ps, ps->ganim(frame + 1, 0), 0);
				if(tips_object != e.object) {
					tips_object = e.object;
					if(e.getname) {
						char temp[260]; stringbuilder sb(temp);
						game.add("Это %1.", e.getname(e.object, sb));
					}
				}
			} else
				tips_object = 0;
		}
	} else {
		image(x, y, gres(cursor.rid),
			cursor.frame + getuitime() % (cursor.frame_stop - cursor.frame_start + 1), 0);
	}
	hot.key = rawinput();
	tick = getunsigedtick();
	apply_pallette_cicle((unsigned char*)color_values, tick);
	switch(hot.key) {
	case 0:
		exit(0);
		break; // If window closed
	case MouseMove:
		if(mouse_position != hot.mouse) {
			mouse_position = hot.mouse;
			stamp_position = getuitime();
		}
		break;
	case MouseLeft:
		if(hot.pressed)
			stamp_pressed = getuitime();
		else {
			stamp_pressed = 0;
			if(action_mode && contextmenu) {
				hot.zero();
				contextmenu[0].execute();
			}
		}
		break;
	case MouseRight:
		if(isactionmode()) {
			if(hot.pressed) {
				setactionmode(false);
				hot.zero();
			}
		}
		break;
	}
	if(action_mode) {
		if(contextmenu && ispressed()) {
			hot.zero();
			auto pa = context_menu();
			hot.zero();
			if(pa)
				pa->execute();
		}
	}
}

bool draw::isactionmode() {
	return action_mode;
}

bool draw::isdefaultinput() {
	return domodal == standart_domodal;
}

void draw::setactionmode(bool v) {
	action_mode = v;
}

bool draw::ismodalex() {
	cursor.set(INTRFACE, 267, {0, 0});
	contextmenu.clear();
	domodal = standart_domodal;
	return true;
}

bool draw::ismodal() {
	ismodalex();
	if(getnextstage()) {
		break_modal = false;
		break_result = 0;
		return false;
	}
	if(!break_modal)
		return true;
	break_modal = false;
	return false;
}