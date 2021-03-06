#include "main.h"
#include "datetime.h"

gamei				game;
array				gamei::playersa(game.players, sizeof(game.players[0]), sizeof(game.players)/sizeof(game.players[0]), sizeof(game.players) / sizeof(game.players[0]));
static unsigned		game_tick;

void gamei::clear() {
	memset(this, 0, sizeof(*this));
	game.setdate(2250, 3, 12, 10, 00);
	game.setposiiton({125, 100});
	game.setcamera({0, 0});
}

void gamei::setdate(int year, int month, int day, int hour, int minute) {
	datetime value(year, month, day, hour, minute);
	minutes = value;
}

int	gamei::getday() const {
	return datetime(minutes).day();
}

void gamei::setcamera(point v) {
	camera.x = v.x - 640 / 2;
	camera.y = v.y - (480 - 99) / 2;
}

int	gamei::gethour() const {
	return datetime(minutes).hour();
}

int	gamei::getminute() const {
	return datetime(minutes).minute();
}

int	gamei::getmonth() const {
	return datetime(minutes).month();
}

int	gamei::getyear() const {
	return datetime(minutes).year();
}

void gamei::introducing() {
	draw::setstage(play);
}

void gamei::passtime(unsigned v) {
	minutes += v;
}

unsigned gamei::getaitime() {
	return game_tick;
}

void gamei::passai(unsigned ticks) {
	game_tick += ticks;
}

void gamei::update() {
	static unsigned last_tick;
	auto m = draw::getuitime();
	if(!last_tick || (m - last_tick) >= 2000)
		last_tick = m;
	passai(m - last_tick);
	last_tick = m;
	for(auto& e : bsdata<creaturei>()) {
		if(e)
			e.updateanm();
	}
	for(auto& e : game.players) {
		if(e)
			e.updateanm();
	}
}