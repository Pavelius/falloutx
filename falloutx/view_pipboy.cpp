#include "draw.h"
#include "main.h"

using namespace draw;

static void page_status(int x, int y, int width, int param) {
}

static void page_automap(int x, int y, int width, int param) {
}

static void page_archive(int x, int y, int width, int param) {
}

static void wait_time() {
	breakmodal(0);
}

void post(fnevent proc, int param, bool run) {
	if(run)
		execute(proc, param);
}

static void page_clock(int x, int y, int width, int param) {
	y += textf(x, y, width, "###Будильник\n---");
	post(wait_time, 30, label(x, y, width, "Подождать полчаса.", 0));
	post(wait_time, 60, label(x, y, width, "Подождать час.", 0));
}

static void page_intro(int x, int y, int width, int param) {
	image(x + width / 2, 480/2, INTRFACE, 133);
}

void gamei::pipboy() {
	fnpaint tabs = page_intro;
	openform();
	while(ismodal()) {
		background(127);
		numbersm(20, 18, 2, game.getday(), 0);
		monthname(46, 18, game.getmonth());
		numbersm(84, 18, 4, game.getyear(), 0);
		numbersm(156, 18, 4, game.gethour() * 100 + game.getminute(), 0);
		if(buttonf(126, 15, 132, 131, 'W', false))
			execute(setint, (int)page_clock, 0, &tabs);
		if(buttonf(53, 340, 8, 9, 'S', false))
			execute(setint, (int)page_status, 0, &tabs);
		if(buttonf(53, 394, 8, 9, 'M', false))
			execute(setint, (int)page_automap, 0, &tabs);
		if(buttonf(53, 422, 8, 9, 'A', false))
			execute(setint, (int)page_archive, 0, &tabs);
		if(buttonf(53, 448, 8, 9, KeyEscape, false))
			execute(buttoncancel);
		tabs(262, 46, 340, 0);
		domodal();
	}
	closeform();
}