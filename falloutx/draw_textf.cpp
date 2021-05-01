#include "crt.h"
#include "draw.h"

using namespace draw;

static bool match(const char** string, const char* name) {
	int n = zlen(name);
	if(memcmp(*string, name, n) != 0)
		return false;
	(*string) += n;
	return true;
}

static const char* glink(const char* p, char* result, unsigned result_maximum) {
	result[0] = 0;
	if(*p == '\"') {
		auto sym = *p++;
		p = psstr(p, result, sym);
	} else if(*p == '(') {
		auto ps = result;
		auto pe = ps + result_maximum;
		while(*p && *p != ')') {
			if(ps < pe)
				*ps++ = *p;
			p++;
		}
		*ps++ = 0;
		if(*p == ')')
			p++;
	}
	return p;
}

static const char* textspc(const char* p, int x0, int& x, int tab_width) {
	int tb;
	while(true) {
		switch(p[0]) {
		case ' ':
			p++;
			x += draw::textw(' ');
			continue;
		case '\t':
			p++;
			if(!tab_width)
				tb = draw::textw(' ') * 4;
			else
				tb = tab_width;
			x = x0 + ((x - x0 + tb) / tb) * tb;
			continue;
		}
		break;
	}
	return p;
}

static const char* word(const char* text) {
	while(((unsigned char)*text) > 0x20 && *text != '*' && *text != '[' && *text != ']')
		text++;
	return text;
}

static void bullet(int x, int y) {
	auto dx = texth() / 2;
	auto rd = texth() / 4;
	circlef(x + dx, y + dx - 2, rd, fore);
	circle(x + dx, y + dx - 2, rd, fore);
}

static void paint_icon(int x, int y, const char* temp) {
	if(equal(temp, "b"))
		bullet(x, y);
}

static int textfln(int x0, int y0, int width, const char** string, color c1, int* max_width, int tab_width) {
	char temp[4096];
	int y = y0;
	int x = x0;
	int x2 = x0 + width;
	const char* p = *string;
	unsigned flags = 0;
	draw::fore = c1;
	if(max_width)
		*max_width = 0;
	temp[0] = 0;
	while(true) {
		if(p[0] == '*' && p[1] == '*') {
			p += 2;
			if(flags & TextBold)
				flags &= ~TextBold;
			else
				flags |= TextBold;
			continue;
		} else if(p[0] == '*') {
			p++;
			if(flags & TextItalic)
				flags &= ~TextItalic;
			else {
				if((flags & TextItalic) == 0)
					x += draw::texth() / 3;
				flags |= TextItalic;
			}
			continue;
		} else if(p[0] == '[' && p[1] == '[')
			p++;
		else if(p[0] == ']' && p[1] == ']')
			p++;
		else if(p[0] == '[') {
			p++;
			switch(*p) {
			case '~':
				p++;
				draw::fore = colors::text.mix(colors::window, 64);
				break;
			case '+':
				p++;
				draw::fore = colors::green;
				break;
			case '-':
				p++;
				draw::fore = colors::red;
				break;
			case '!':
				p++;
				draw::fore = colors::yellow;
				break;
			case '#':
				p++;
				flags |= TextUscope;
				draw::fore = colors::special;
				break;
			default:
				draw::fore = colors::special;
				break;
			}
			p = glink(p, temp, sizeof(temp) / sizeof(temp[0]) - 1);
		} else if(p[0] == ']') {
			p++;
			draw::fore = c1;
			temp[0] = 0;
			flags &= ~TextUscope;
		}
		// Обработаем пробелы и табуляцию
		p = textspc(p, x0, x, tab_width);
		int w;
		if(p[0] == ':' && p[1] >= 'a' && p[1] <= 'z') {
			char temp[128];
			p = stringbuilder::read(p + 1, temp, temp + sizeof(temp) - 1);
			if(*p == ':')
				p++;
			w = textw('A') + 2;
			if(x + w > x2) {
				if(max_width)
					*max_width = imax(*max_width, x - x0);
				x = x0;
				y += draw::texth();
			}
			paint_icon(x, y, temp);
		} else {
			const char* p2 = word(p);
			w = draw::textw(p, p2 - p);
			if(x + w > x2) {
				if(max_width)
					*max_width = imax(*max_width, x - x0);
				x = x0;
				y += draw::texth();
			}
			draw::text(x, y, p, p2 - p, flags);
			p = p2;
		}
		int x4 = x;
		x += w;
		p = textspc(p, x0, x, tab_width);
		if(temp[0] || (flags & TextUscope) != 0) {
			int x3 = imin(x2, x);
			int y2 = y + draw::texth();
			if(flags & TextUscope)
				draw::line(x4, y2, x3, y2, draw::fore);
			rect rc = {x4, y, x3, y2};
			if(ishilite(rc)) {
				if(flags & TextUscope) {
					if(temp[0] && hot.key == MouseLeft && !hot.pressed)
						zcpy(draw::link, temp, sizeof(draw::link) - 1);
				} else
					zcpy(draw::link, temp, sizeof(draw::link) - 1);
			}
		}
		// Отметим перевод строки и окончание строки
		if(p[0] == 0 || p[0] == 10 || p[0] == 13) {
			y += draw::texth();
			p = skipcr(p);
			break;
		}
	}
	if(max_width)
		*max_width = imax(*max_width, x - x0);
	*string = p;
	return y - y0;
}

int draw::textf(int x, int y, int width, const char* string, int* max_width,
	int min_height, int* cashe_height, const char** cashe_string, int tab_width) {
	state push;
	auto old_fore = fore;
	const char* p = string;
	int y0 = y;
	if(cashe_height) {
		*cashe_string = p;
		*cashe_height = 0;
	}
	if(max_width)
		*max_width = 0;
	auto bas_font = font;
	while(p[0]) {
		int mw2 = 0;
		if(cashe_height && (y - y0) <= min_height) {
			*cashe_string = p;
			*cashe_height = y - y0;
		}
		if(match(&p, "###")) { // Header 3
			p = skipsp(p);
			font = metrics::h3;
			y += textfln(x, y, width, &p, colors::h3, &mw2, tab_width);
		} else if(match(&p, "##")) { // Header 2
			p = skipsp(p);
			font = metrics::h2;
			y += textfln(x, y, width, &p, colors::h2, &mw2, tab_width);
		} else if(match(&p, "#")) { // Header 1
			p = skipsp(p);
			font = metrics::h1;
			y += textfln(x, y, width, &p, colors::h1, &mw2, tab_width);
		} else if(match(&p, "---")) { // Line
			p = skipcr(skipsp(p));
			auto dy = texth() / 2 - 2;
			line(x, y + dy, x + width, y + dy);
			y += texth();
		} else if(match(&p, "...")) { // Без форматирования
			p = skipcr(p);
			font = metrics::font;
			color c1 = colors::window.mix(colors::edit, 256 - 32);
			y += texth() / 2;
			while(p[0]) {
				int c = textbc(p, width);
				if(!c)
					break;
				rectf({x, y, x + width, y + texth()}, c1);
				text(x, y, p, c);
				y += texth();
				p += c;
				if(match(&p, "...")) {
					p = skipcr(p);
					y += texth() / 2;
					break;
				}
			}
		} else if(match(&p, "* ")) {
			bullet(x, y);
			y += textfln(x + texth() + 4, y, width - texth() - 4, &p, old_fore, &mw2, tab_width);
		} else
			y += textfln(x, y, width, &p, old_fore, &mw2, tab_width);
		// Возвратим стандартные настройки блока
		font = bas_font;
		fore = old_fore;
		if(max_width) {
			if(*max_width < mw2)
				*max_width = mw2;
		}
	}
	return y - y0;
}

int draw::textf(rect& rc, const char* string, int tab_width) {
	state push;
	clipping.clear();
	rc.y2 = rc.y1 + draw::textf(0, 0, rc.width(), string, &rc.x2, 0, 0, 0, tab_width);
	rc.x2 += rc.x1;
	return rc.height();
}