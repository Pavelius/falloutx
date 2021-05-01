#include "draw.h"
#include "main.h"

using namespace draw;

static char				console_text[256 * 8];
static stringbuilder	sb(console_text);
static int				console_height;
static int				console_origin;
static const char*		console_string;
static int				console_offset;

const int width = 160;
const int height = 50;

static void try_minimize() {
	if(sb.getlenght() < sizeof(console_text) - 260)
		return;
	auto p = sb.find('\n');
	if(p) {
		auto n = zlen(p + 1);
		if(!n)
			sb.clear();
		else {
			memmove(sb.begin(), p, n + 1);
			sb.set(sb.begin());
		}
	}
}

static void cashing() {
	state push;
	setclip({0, 0, 0, 0});
	if(console_origin > console_height - height)
		console_origin = console_height - height;
	if(console_origin < 0)
		console_origin = 0;
	auto old_height = console_height;
	console_height = textf(0, 0, width, sb.begin(), 0, console_origin, &console_offset, &console_string, 0);
	console_offset = console_origin - console_offset;
	if((old_height < console_height) && console_origin != (console_height - height)) {
		console_origin = console_height - height;
		if(console_origin < 0)
			console_origin = 0;
		textf(0, 0, width, sb.begin(), 0, console_origin, &console_offset, &console_string, 0);
		console_offset = console_origin - console_offset;
	}
}

static void clear_cash() {
	console_string = 0;
}

static void setintcs() {
	auto p = (int*)hot.object;
	*p = hot.param;
	clear_cash();
}

void draw::consoleview(int x, int y) {
	draw::state push;
	rect rc = {x, y, x + width, y + height};
	if(!console_string)
		cashing();
	if(ishilite(rc)) {
		rect ru = {rc.x1, rc.y1, rc.x2, rc.y1 + height / 2};
		if(ishilite(ru)) {
			if(console_origin > 0) {
				cursor.set(INTRFACE, 268);
				if(hot.key == MouseLeft && hot.pressed)
					execute(setintcs, console_origin - texth(), 0, &console_origin);
			}
		} else {
			if(console_origin < console_height - height) {
				cursor.set(INTRFACE, 269);
				if(hot.key == MouseLeft && hot.pressed)
					execute(setintcs, console_origin + texth(), 0, &console_origin);
			}
		}
		switch(hot.key) {
		case MouseWheelUp:
			execute(setintcs, console_origin - texth(), 0, &console_origin);
			break;
		case MouseWheelDown:
			execute(setintcs, console_origin + texth(), 0, &console_origin);
			break;
		}
	}
	//rectb(rc, colors::red);
	setclip(rc);
	textf(x, y - console_offset, width, console_string);
}

void draw::consoleview() {
	state push;
	auto x = 26, y = 480 - 99 + 32;
	setclip({x, y, x + width, y + height});
	image(0, 480 - 99, INTRFACE, 16, ImageNoOffset);
	consoleview(x, y);
}

void gamei::addv(const char* format, const char* format_param) {
	if(!format || format[0] == 0)
		return;
	try_minimize();
	auto p = sb.get();
	if(p > sb.begin() && p[-1] != '\n')
		sb.add("\n:b:");
	sb.addv(format, format_param);
	clear_cash();
}