#include "draw.h"
#include "main.h"

using namespace draw;

BSDATA(resei) = {
	{"NONE"},
	{"BACKGRND"}, {"SKILLDEX"}, {"INVEN"}, {"ITEMS"},
	{"MISC"}, {"SCENERY"}, {"WALLS"}, {"TILES"}, {"INTRFACE"},
	{"FONT1"}, {"FONT2"}, {"FONT3"}, {"FONT4"},
	// humans
	{"HANPWR"}, {"HAPOWR"}, {"HAROBE"},
	{"HFCMBT"}, {"HFJMPS"}, {"HFLTHR"}, {"HFMAXX"}, {"HFMETL"}, {"HFPRIM"},
	{"HMBJMP"}, {"HMBMET"},
	{"HMCMBT"}, {"HMJMPS"}, {"HMLTHR"}, {"HMMAXX"}, {"HMMETL"}, {"HMWARR"},
	{"MABOS2"}, {"MABOSS"}, {"MAMTNT"}, {"MAMTN2"},
	{"MAROBE"}, {"NABLUE"}, {"NABRWN"},
	{"NACHLD"},
	{"NAGHUL"}, {"NAGLOW"}, {"NAPOWR"}, {"NAROBE"}, {"NAVGUL"}, {"NAWHIT"},
	{"NFASIA"},
	{"NFBRLP"}, {"NFLYNN"},
	{"NFMAXX"}, {"NFMETL"},
	{"NFNICE"}, {"NFPEAS"}, {"NFPRIM"}, {"NFTRMP"}, {"NFVALT"}, {"NFVRED"},
	{"NMASIA"},
	{"NMBONC"}, {"NMBOXX"}, {"NMBPEA"}, {"NMBRLP"}, {"NMBRSR"}, {"NMBSNP"},
	{"NMCOPP"}, {"NMDOCC"}, {"NMFATT"}, {"NMGANG"}, {"NMGRCH"}, {"NMLABB"},
	{"NMLOSR"}, {"NMLTHR"}, {"NMMAXX"}, {"NMMEXI"}, {"NMMYRN"}, {"NMNICE"}, {"NMOLDD"},
	{"NMPEAS"}, {"NMRGNG"}, {"NMVALT"}, {"NMWARR"},
	// animals and robots
	{"MAANTT"}, {"MABRAN"}, {"MABROM"}, {"MACLAW"}, {"MACLW2"}, {"MADDOG"}, {"MADEGG"},
	{"MADETH"}, {"MAFIRE"}, {"MAGCKO"}, {"MAGKO2"}, {"MALIEN"}, {"MAMANT"}, {"MAMRAT"},
	{"MAMURT"}, {"MAPLNT"}, {"MAQUEN"}, {"MASCP2"}, {"MASCRP"}, {"MASPHN"}, {"MASRAT"},
	{"MATHNG"}, {"MACYBR"}, {"MAFEYE"}, {"MAGUN2"}, {"MAGUNN"}, {"MAHAND"}, {"MAROBO"},
	{"MAROBT"},
};
assert_enum(resei, MAROBT)

sprite* draw::gres(res_s id) {
	auto& e = bsdata<resei>::elements[id];
	if(e.notfound)
		return 0;
	if(!e.data) {
		if(!id)
			return 0;
		char temp[260]; stringbuilder sb(temp);
		sb.add("art/%1.pma", e.name);
		e.data = (sprite*)loadb(temp);
		e.notfound = !e.data;
	}
	return e.data;
}

const char* draw::getname(res_s id) {
	return bsdata<resei>::elements[id].name;
}