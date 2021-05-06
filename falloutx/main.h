#include "crt.h"
#include "stringbuilder.h"
#include "point.h"
#include "pushst.h"

#pragma once

const unsigned short Blocked = 0xFFFF;
const int tile_width = 80; // Width of isometric tile
const int tile_height = 36; // Height of isometric tile

class creaturei;
struct sprite;

enum grade_s : unsigned char {
	Terrible, Poor, Mediocre, Average, Fair, Good, Great, Superb, Fantastic, Epic,
};
enum stat_s : unsigned char {
	Strenght, Perception, Endurance, Charisma, Intellegence, Agility, Luck,
	FirstPrimary = Strenght, LastPrimary = Luck,
	HP, HPCur, AP, APCur, AC,
	DamageMelee, DamageMeleeRate, CarryWeight,
	CriticalHit, CriticalHitTable, CriticalMiss, CriticalMissTable,
	Sequence, PerkRate, SkillRate, HealingRate,
	PoisonResistance, RadiationResistance,
	FirstDerived = HP, LastDerived = RadiationResistance,
	Age, Level,
	PrimaryPoints, SkillTagPoints, SkillPoints, PoisonPoints, RadiationPoints,
	LastStat = RadiationPoints,
};
enum formula_s : unsigned char {
	Experience, NextLevelExperience,
};
enum number_s : unsigned char {
	Num0, Num1, Num2, Num4, Num5, Num10, Num15, Num20, Num30,
	Low, Ave, High, VeryHight,
};
enum modifier_s : unsigned char {
	Plus, Minus, Compare, OneTime,
};
enum perk_s : unsigned char {
	Awareness, BonusHtHAttacks, BonusHtHDamage, BonusMove, BonusRangedDamage, BonusROF,
	EarlierSequence, FasterHealing, MoreCriticals, NightVision, Presence, RadResistance,
	Toughness, StrongBack, SharpShooter, SilenRunnng, Survivalist, MasterTrader, Educated,
	Healer, FortuneFinder, BetterCriticals, Empathy, Slayer, Sniper, SilentDeath,
	ActionBoy, MentalBlock, Lifegiver, Dodger, SnakeEater, MrFixit, Medic, MasterTheif,
	Speaker, HeaveHo, FriendlyFoe, PickPocket, Ghost, CultOfPersonality, Scrounger,
	Explorer, FlowerChild, Pathfinder, AnimalFriend, Scout, MysteriousStranger,
	Ranger, QuickPockets, SmoothTalker, SwiftLearner, Tag, Mutate,
	AddictedNukeCola, AddictedBuffout, AddictedMentat, AddictedPsicho, AddictedRadaway,
	WeaponLongRange, WeaponAccuracy, WeaponPenetrating, WeaponKnockback,
	ArmorPowerPerk, ArmorCombatPerk,
	WeaponScopeRange, WeaponFastReload, WeaponNightSight, WeaponFlameboy,
	ArmorAdvance, ArmorAdvanceII,
	AddictedJet, AddictedGambling,
	ArmorCharisma,
	GeckoSkinning,
	DermalImpactArmor, DermalImpactAssaultEnchantments,
	PhoenixArmorImplants, PhoenixAssaultEnchantments, VaultCityInoculation,
	AdrenalineRush, CautiousNature, Comprehension, DemolitionExpert, Gambler,
	GainStrenght, GainPerception, GainEndurance, GainCharisma, GainIntellegence, GainAgility, GainLuck,
	Harmless, HereAndNow, HtHEvade, KamaSutraMaster, KarmaBeacon, LightStep, LivingAnatomy,
	MagneticPersonality, Negotiator, PackRat, Pyromaniac, QuickRecovery, Salesman,
	Stonewall, Thief, WeaponHandling, VaultCityTraining,
	AlcoholRaisedHP, AlcoholRaisedHP2, AlcoholLoweredHP, AlcoholLoweredHP2,
	AutodocRaisedHP, AutodocRaisedHP2, AutodocLoweredHP, AutodocLoweredHP2,
	ExpertExcrementExpeditor,
	WeaponEnchencedKnockout, MegaUnlucky,
	FirstPerk = Awareness, LastPerk = MegaUnlucky,
};
enum kind_s : unsigned char {
	VaultDweller, Mutant, Ghoul,
};
enum trait_s : unsigned char {
	FastMetabolism, Bruiser, SmallFrame, OneHanded, Finesse, Kamikaze, HeavyHanded, FastShoot,
	BloodyMess, Jinxed, GoodNatured, ChemReliant, ChemResistant, SexAppeal, Skilled, Gifted,
	FirstTrait = FastMetabolism, LastTrait = Gifted
};
enum skill_s : unsigned char {
	SmallGuns, BigGuns, EnergyWeapon,
	Unarmed, MeleeWeapon, Throwing,
	FirstAid, Doctor,
	Sneak, Lockpick, Steal, Traps,
	Science, Repair,
	Speech, Barter, Gambling,
	Outdoorsman,
	FirstSkill = SmallGuns, LastSkill = Outdoorsman
};
enum gender_s : unsigned char {
	Male, Female,
};
enum action_s : unsigned char {
	NoAction,
	ThrowPunch, KickLeg,
	Swing, Thrust, Throw,
	FireSingle, FireBurst, Reload,
	Drop, Look, Talk, Turn, Unload,
	Use, UseItem, UseSkill,
};
enum wound_s : unsigned char {
	WoundEye,
	WoundRightHand, WoundLeftHand,
	WoundRightLeg, WoundLeftLeg,
};
enum damage_s : unsigned char {
	Phisycal, Laser, Fire, Plasma, Electrical, EMP, Explosive,
	LastDamage = Explosive
};
enum item_s : unsigned short {
	NoItem, Cookie,
	LeatherArmorMarkII,
	MetalArmor, MetalArmorMarkII,
	AdvancedPowerArmor, AdvPowerArmorMKII,
	Knife, Spear,
	X10mmPistol, HuntingRifle, X10mmSMG,
	X10mmAP, X10mmJHP, X12GaShotgunShells, X14mmAP, X223FMJ, X2mmEC, X44MagnumFMJ, X44MagnumJHP,
	X45Caliber, X47mmCaseless, X5mmAP, X5mmJHP, X762mm, X9mm,
	BBS, SmallEnergyCell, MicroFusionCell, FlamethrowerFuel, ExplosiveRocket,
};
enum material_s : unsigned char {
	Glass, Metal, Plastic, Wood, Dirt, Stone, Cement, Leather
};
enum direction_s : unsigned char {
	LeftUp, Up, RightUp, Right, RightDown, Down, LeftDown, Left,
	Center,
};
enum settlement_s : unsigned char {
	SettlementArojo, SettlementDen, SettlementKlamath,
};
enum itemkind_s : unsigned char { // Order is important!
	Armor, Container, Drug, Weapon, Ammo, Misc, Key
};
enum critical_effect_s : unsigned {
	KnockOut = 0x10000000,
	KnockDown = 0x02000000,
	CrippledLeftLeg = 0x04000000,
	CrippledRightLeg = 0x08000000,
	CrippledLeftArm = 0x10000000,
	CrippledRightArm = 0x20000000,
	Blinded = 0x40000000,
	InstantDeath = 0x80000000,
	FireDeath = 0x00040000,
	BypassArmor = 0x00080000,
	DroppedWeapon = 0x00400000,
	LoseNextTurn = 0x00800000,
	RandomCriticalEffect = 0x00002000
};
enum animate_s : unsigned char {
	AnimateStand, AnimateWalk, AnimatePickup, AnimateUse, AnimateDodge,
	AnimateDamaged, AnimateDamagedRear,
	AnimateUnarmed1, AnimateUnarmed2, AnimateThrown, AnimateRun,
	AnimateKnockOutBack, AnimateKnockOutForward,
	// Kill animation
	AnimateKilledSingle, AnimateKilledBurst, AnimateKilledBurstAuto, AnimateKilledBlowup, AnimateKilledMelt,
	AnimateBloodedBack, AnimateBloodedForward,
	AnimateStandUpBack, AnimateStandUpForward,
	// Dead body (1 frame animation)
	AnimateDeadBackNoBlood, AnimateDeadForwardNoBlood,
	AnimateDeadSingle, AnimateDeadBurst, AnimateDeadBurstAuto, AnimateDeadBlowup, AnimateDeadMelt,
	AnimateDeadBack, AnimateDeadForward,
	// Weapon Block
	FirstWeaponAnimate,
	AnimateWeaponTakeOn = FirstWeaponAnimate, AnimateWeaponStand, AnimateWeaponTakeOff, AnimateWeaponWalk, AnimateWeaponDodge,
	AnimateWeaponThrust, AnimateWeaponSwing,
	AnimateWeaponAim,
	AnimateWeaponSingle, AnimateWeaponBurst, AnimateWeaponFlame,
	AnimateWeaponThrow, AnimateWeaponAimEnd,
	// Weapon Animate
	AnimateClub,
	AnimateHammer = AnimateClub + 13,
	AnimateSpear = AnimateHammer + 13,
	AnimatePistol = AnimateSpear + 13,
	AnimateSMG = AnimatePistol + 13,
	AnimateRifle = AnimateSMG + 13,
	AnimateHeavyGun = AnimateRifle + 13,
	AnimateMachineGun = AnimateHeavyGun + 13,
	AnimateRocketLauncher = AnimateMachineGun + 13,
	LastAnimation = AnimateRocketLauncher + 13
};
enum map_object_s : short unsigned {
	NoObject,
	FirstWall = 1, LastWall = 1633,
	FirstScenery = 2000, LastScenery = FirstScenery + 1861,
};
enum color_s : unsigned char {
	ColorDisable = 0x60, ColorText = 0xD7, ColorCheck = 0x03, ColorInfo = 0xE4, ColorButton = 0x3C,
	ColorState = 0x90
};
enum tile_s : unsigned char {
	TileDirt, TileStep, TileCave, TileRock, TileBorder, TilePlates, TileAcid
};
enum command_s {
	Cancel, Next, Back, Options, Save, Load, Delete, Print, Quit,
	Bonuses, Karma, KilledCreatures,
	InvalidTag,
	InvalidMaximum, InvalidMinimal, InvalidRemove, NotEnoughtScoresToRaise, InvalidStartGame,
	InvalidTrait,
	LastCommand = InvalidTrait
};
enum pregen_s : unsigned char {
	NoPregen, Narg, Mingun, Chitsa,
};
namespace draw {
enum res_s : unsigned short {
	None,
	BACKGRND, SKILLDEX, INVEN, ITEMS, MISC, SCENERY, WALLS, TILES, INTRFACE,
	FONT1, FONT2, FONT3, FONT4,
	// спрайты людей
	HANPWR, HAPOWR, HAROBE,
	HFCMBT, HFJMPS, HFLTHR, HFMAXX, HFMETL, HFPRIM,
	HMBJMP, HMBMET,
	HMCMBT, HMJMPS, HMLTHR, HMMAXX, HMMETL, HMWARR,
	MABOS2, MABOSS, MAMTNT, MAMTN2,
	MAROBE, NABLUE, NABRWN,
	NACHLD,
	NAGHUL, NAGLOW, NAPOWR, NAROBE, NAVGUL, NAWHIT,
	NFASIA,
	NFBRLP, NFLYNN,
	NFMAXX, NFMETL,
	NFNICE, NFPEAS, NFPRIM, NFTRMP, NFVALT, NFVRED,
	NMASIA,
	NMBONC, NMBOXX, NMBPEA, NMBRLP, NMBRSR, NMBSNP,
	NMCOPP, NMDOCC, NMFATT, NMGANG, NMGRCH, NMLABB,
	NMLOSR, NMLTHR, NMMAXX, NMMEXI, NMMYRN, NMNICE, NMOLDD,
	NMPEAS, NMRGNG, NMVALT, NMWARR,
	// спрайты животных и роботов
	MAANTT, MABRAN, MABROM, MACLAW, MACLW2, MADDOG, MADEGG,
	MADETH, MAFIRE, MAGCKO, MAGKO2, MALIEN, MAMANT, MAMRAT,
	MAMURT, MAPLNT, MAQUEN, MASCP2, MASCRP, MASPHN, MASRAT,
	MATHNG, MACYBR, MAFEYE, MAGUN2, MAGUNN, MAHAND, MAROBO,
	MAROBT,
};
}
enum variant_s : unsigned short {
	NoVariant,
	Action, Command, Creature, Damage, Formula, Item,
	Gender, Grade, Number, Modifier, Perk, Pregen, Skill, Stat, Trait, Wound,
};
typedef flagable<1 + LastPerk / 8> perka;
typedef cflags<trait_s> traita;
typedef cflags<skill_s> skilla;
typedef cflags<wound_s, unsigned short> wounda;
typedef item_s geara[8];
typedef std::initializer_list<short unsigned> tilea;
typedef short unsigned indext;
union variant {
	struct {
		variant_s		type;
		unsigned short	value;
	};
	unsigned			u;
	constexpr variant() : type(NoVariant), value(Awareness) {}
	constexpr variant(unsigned short v) : u(v) {}
	constexpr variant(action_s v) : type(Action), value(v) {}
	constexpr variant(command_s v) : type(Command), value(v) {}
	constexpr variant(damage_s v) : type(Damage), value(v) {}
	constexpr variant(formula_s v) : type(Formula), value(v) {}
	constexpr variant(item_s v) : type(Item), value(v) {}
	constexpr variant(gender_s v) : type(Gender), value(v) {}
	constexpr variant(grade_s v) : type(Grade), value(v) {}
	constexpr variant(modifier_s v) : type(Modifier), value(v) {}
	constexpr variant(number_s v) : type(Number), value(v) {}
	constexpr variant(perk_s v) : type(Perk), value(v) {}
	constexpr variant(pregen_s v) : type(Pregen), value(v) {}
	constexpr variant(skill_s v) : type(Skill), value(v) {}
	constexpr variant(stat_s v) : type(Stat), value(v) {}
	constexpr variant(trait_s v) : type(Trait), value(v) {}
	constexpr variant(wound_s v) : type(Wound), value(v) {}
	constexpr variant(variant_s t, int v) : type(t), value(v) {}
	variant(const void* v);
	constexpr bool operator==(const variant& e) const { return u == e.u; }
	constexpr bool operator!=(const variant& e) const { return u != e.u; }
	constexpr explicit operator bool() const { return type != NoVariant; }
	constexpr operator unsigned short() const { return u; }
	operator const char*() const { return getname(); }
	static variant		find(const char* id);
	int					getavatar() const;
	const char*			getdescription() const;
	const char*			getformat() const;
	const char*			getname() const;
	const char*			getnameabr() const;
	const char*			getnameshort() const;
	void*				getpointer() const;
};
typedef std::initializer_list<variant> varianta;
struct term {
	stat_s				id;
	char				multiplier;
	char				divider;
};
struct formula {
	char				start;
	term				t1, t2;
	variant				condition;
	void				add(stringbuilder& sb) const;
};
typedef std::initializer_list<formula> formulaa;
class nameable {
	kind_s				kind;
	gender_s			gender;
	char				name[16];
public:
	void				act(const char* id, ...) const;
	void				actv(stringbuilder& sb, const char* format, const char* format_param) const;
	gender_s			getgender() const { return gender; }
	kind_s				getkind() const { return kind; }
	const char*			getname() const { return name; }
	void				setgender(gender_s v) { gender = v; }
	void				setkind(kind_s v) { kind = v; }
	void				setname(const char* v);
};
struct drawable : public point {
	draw::res_s			rid;
	unsigned short		frame, frame_start, frame_stop;
	unsigned			next_stamp;
	void				add(point v) { x += v.x; y += v.y; }
	bool				getrect(rect& result, point camera);
	static draw::res_s	getrid(item_s armor, gender_s gender);
	static const char*	getnamefn(const void* object, stringbuilder& sb);
	bool				hittest(point mouse) const;
	void				paint() const { paint(x, y, 0); }
	void				paint(int x, int y, unsigned flags = 0) const;
	void				set(draw::res_s v, int cicle);
	void				set(draw::res_s v, int cicle, point offset);
	void				setposition(point v) { x = v.x; y = v.y; }
	bool				update();
};
struct drawablea : adat<drawable*, 512> {
	void				select();
	void				sortz();
};
struct action {
	action_s			type;
	bool				aimed;
	char				ap;
	constexpr explicit operator bool() const { return type != NoAction; }
};
struct actiona : adat<action, 4> {
	void				add(action_s type, char ap, bool aimed);
};
struct texti {
	const char*			id;
	const char*			name;
	const char*			name_short;
	const char*			name_abr;
	const char*			text;
};
struct itemkindi {
	texti				txt;
};
struct modifieri {
	texti				txt;
};
struct itemi {
	struct ammoi {
		item_s			ammo;
		unsigned char	count;
		char			ac, dr;
		short			dam_bonus = 100;
	};
	struct armori {
		char			ac;
		draw::res_s		male, female;
		char			threshold[LastDamage + 1];
		char			resistance[LastDamage + 1];
	};
	struct imagei {
		short			inventory;
		short			ground;
		short			animation;
	};
	struct weaponi {
		unsigned char	min, max;
		unsigned char	min_strenght;
		damage_s		type;
		unsigned char	ap, range;
		item_s			ammo;
		unsigned char	ammo_count;
		unsigned char	burst;
		short unsigned	critical_fail; // Number of critical failure table
	};
	texti				txt;
	int					size, weight, cost;
	imagei				avatar;
	material_s			material;
	weaponi				weapon;
	armori				armor;
	ammoi				ammo;
	skill_s				use;
	const char*			original;
	constexpr bool		isammo() const { return ammo.ammo != NoItem; }
	constexpr bool		isarmor() const { return armor.ac != 0; }
	constexpr bool		isranged() const { return weapon.range > 2; }
	constexpr bool		isweapon() const { return weapon.min != 0; }
	itemkind_s			getkind() const;
};
class item {
	item_s				type;
	union {
		struct {
			unsigned char weapon_count : 4;
			unsigned char weapon_broken : 1;
			unsigned char ammo_index : 3;
			unsigned char ammo_count;
		};
		short unsigned	count;
	};
public:
	constexpr item() : type(NoItem), count(0) {}
	constexpr item(item_s type) : type(type), count(0) {}
	constexpr operator bool() const { return type != NoItem; }
	constexpr operator item_s() const { return type; }
	bool				addcount(int v) { return setcount(getcount() + v); }
	void				addtext(stringbuilder& sb) const;
	void				clear();
	item_s				getclipammo() const;
	int					getclipcount() const;
	int					getcount() const;
	int					getcountdef() const;
	int					getcountmax() const;
	const itemi&		geti() const;
	const char*			getname() const { return geti().txt.name; }
	static const char*	getobjectname(const void* object, stringbuilder& sb);
	bool				isarmor() const { return geti().isarmor(); }
	bool				isranged() const { return geti().isranged(); }
	bool				isweapon() const { return geti().isweapon(); }
	bool				join(item& e);
	bool				setclipcount(int v);
	bool				setcount(int v);
};
struct pregeni {
	texti				txt;
	unsigned char		level;
	short				age;
	gender_s			gender;
	unsigned char		stats[Luck + 1];
	traita				trait;
	skilla				tag;
	geara				gears;
};
struct statable {
	short				stats[LastStat + 1];
	short				skills[Outdoorsman + 1];
	char				threshold[LastDamage + 1], resistance[LastDamage + 1];
	perka				perks;
	traita				trait;
	skilla				tag;
	wounda				wounds;
	int					experience;
	void				add(variant v, int bonus) { set(v, get(v) + bonus); }
	void				apply(const pregeni& source);
	void				apply(varianta source);
	void				apply_derived();
	void				apply_skills();
	void				apply_traits();
	void				clear();
	void				create(const pregeni& ei);
	int					evalute(const formula& v) const;
	int					get(variant v) const;
	int					get(const term& e) const;
	int					get(const formula* p) const;
	int					get(formula_s v) const;
	const formula*		getformula(stat_s v) const;
	int					getprimarypoints() const;
	int					getskillpoints() const;
	int					gettagpoints() const;
	int					gettraitpoints() const;
	int					getresist(damage_s v) const { return resistance[v]; }
	int					getthreshold(damage_s v) const { return threshold[v]; }
	void				set(variant v, int i);
	void				set(formula_s v, int i);
	void				update();
	void				update(const item& it);
};
class actor : public drawable, public nameable {
	animate_s			animate = AnimateDeadForward;
	unsigned char		direction = 0;
	item				armor, hands[2];
public:
	animate_s			getanimate() const { return animate; }
	static animate_s	getanimate(animate_s v, int w);
	const item&			getarmor() const { return armor; }
	item&				getarmor() { return armor; }
	static animate_s	getbaseanimate(animate_s v, int* w = 0);
	static int			getcicle(draw::res_s& rid, animate_s v, gender_s gender, item_s armor, item_s weapon, int dir);
	unsigned char		getdirection() const { return direction; }
	const item&			getweapon(int i) const { return hands[i]; }
	item&				getweapon(int i) { return hands[i]; }
	const item&			getweapon() const { return getweapon(0); }
	static bool			isweaponanimate(animate_s v);
	static void			preview(int x, int y, gender_s gender, item_s armor, item_s weapon, unsigned tick);
	void				reanimate();
	void				setanimate(animate_s v, bool force = false);
	void				setdirection(unsigned char v);
};
class itema : public adat<item*, 512> {
public:
	int					getcost() const;
	void				paint(const rect& rc, int& origin, int mode, point pts);
	void				sort();
};
class wearable {
	item				gears[16];
public:
	bool				additem(item& v);
	bool				additems(wearable& v);
	item&				getitem(unsigned i) { return gears[i % (sizeof(gears) / sizeof(gears[0]))]; }
	void				getitems(itema& result);
	bool				isgear(const item* p) const;
};
class creaturei : public actor, public wearable {
	statable			basic, current;
	unsigned char		action_index;
	bool				choosestats(bool charsheet_mode, const statable* previous);
	void				finish();
	void				statinfo(int x, int y, variant focus) const;
public:
	void				add(variant v, int i);
	void				add(stringbuilder& sb, const item& weapon) const;
	void				addexp(int v) {}
	void				apply(const pregeni& e);
	void				clear();
	void				charsheet();
	void				chat(creaturei& opponent, const char* cid);
	variant				chooseskill() const;
	void				create(pregen_s v);
	static void			deletegch(const char* name);
	bool				generate();
	int					get(stat_s v) const { return current.stats[v]; }
	int					get(skill_s v) const { return current.skills[v]; }
	int					get(variant v) const { return current.get(v); }
	action				getaction() const;
	void				getactions(actiona& result, const item& ei) const;
	int					getactionindex() { return action_index; }
	int					getresist(damage_s v) const { return current.getresist(v); }
	const char*			getspeech(const char* subject, int id) const;
	int					getthreshold(damage_s v) const { return current.getthreshold(v); }
	void				inventory();
	bool				is(perk_s v) const { return current.perks.is(v); }
	bool				is(trait_s v) const { return current.trait.is(v); }
	bool				is(wound_s v) const { return current.wounds.is(v); }
	bool				raise(skill_s v, int i, bool interactive);
	bool				raise(stat_s v, int i, bool interactive);
	bool				toggle(skill_s v, bool interactive);
	bool				toggle(trait_s v, bool interactive);
	bool				read(const char* name);
	void				set(variant v, int i) { basic.set(v, i); }
	void				setactionindex(int v) { action_index = v; }
	void				trade(creaturei& opponent);
	void				tofile(const char* name) const;
	void				update();
	void				updateanm();
	void				write(const char* name) const;
};
struct stati {
	texti				txt;
	int					avatar;
	formulaa			formulas;
};
struct actioni {
	texti				txt;
	int					avatar;
};
struct animationi {
	texti				txt;
	animate_s			next, next_dead;
};
struct materiali {
	texti				txt;
};
struct formulai {
	texti				txt;
	int					avatar;
};
struct gradei {
	texti				txt;
};
struct commandi {
	texti				txt;
	int					avatar = -1;
};
struct damagei {
	texti				txt;
	int					avatar;
};
struct numberi {
	int					value;
};
struct traiti {
	texti				txt;
	int					avatar;
	varianta			effect;
};
struct skilli {
	texti				txt;
	int					avatar;
	formula				expression;
};
struct woundi {
	texti				txt;
	short				avatar;
};
struct genderi {
	texti				txt;
};
struct walli {
	short unsigned		flags;
	material_s			material;
	const char*			original;
};
struct settlement {
	const char*			name;
	point				position;
	char				size;
};
struct groundi : item {
	point				position;
};
class gamei {
	bool				car;
	point				position;
	unsigned			minutes;
public:
	point				camera;
	int					infopage;
	creaturei			players[8];
	static array		playersa;
	static void			add(const char* format, ...) { addv(format, xva_start(format)); }
	static void			addv(const char* format, const char* format_param);
	static void			automap();
	bool				checklocal();
	static bool			chooseage(int x, int y, int& value, int minimal, int maximum);
	static bool			choosegender(int x, int y, gender_s& value);
	static bool			choosename(int x, int y, char* value, unsigned maximum);
	void				clear();
	static void			combat();
	bool				getcar() const { return car; }
	int					getday() const;
	int					gethour() const;
	int					getminute() const;
	int					getmonth() const;
	creaturei&			getplayer() { return players[0]; }
	point				getposition() const { return position; }
	unsigned			getround() const { return minutes; }
	int					getyear() const;
	static void			introducing();
	static void			localization(const char* id, bool write_mode);
	static void			newgame();
	static void			mainmenu();
	void				passtime();
	void				passtime(unsigned minutes);
	static void			pipboy();
	static void			play();
	void				read(const char* url);
	static bool			readchats(const char* locale);
	void				setcamera(point v);
	void				setdate(int year, int month, int day, int hour, int minute);
	void				setposiiton(point v) { position = v; }
	static void			worldmap();
	void				write(const char* url) const;
};
extern gamei			game;
class answers {
	char				buffer[8192];
	stringbuilder		sc;
	struct element {
		int				id;
		const char*		text;
	};
public:
	answers() : sc(buffer) {}
	adat<element, 128>	elements;
	void				add(int id, const char* name, ...) { addv(id, name, xva_start(name)); }
	element*			addv(int id, const char* name, const char* format);
	void				addvm(const char* name, const char* format);
	int					chat(const char* format, int backf, creaturei* opponent) const;
	int					choose() const;
	int					choose(const char* title) const;
	void				clear();
	static int			compare(const void* v1, const void* v2);
	bool				edit(const char* title, char* value, unsigned maximum) const;
	bool				paintlist(int x, int y, int width, int& result) const;
	void				paintlist(int x, int y, int width, int origin, int& current, int perpage, int perline) const;
	int					random() const;
	void				select(const char* url, const char* filter);
	void				sort();
};
struct tilei {
	texti				txt;
	tilea				center;
	tilea				up, right, down, left;
	short unsigned		sides[4];
	short unsigned		corners[4];
	short unsigned		correct(short unsigned t, unsigned char c);
	static tilei*		find(short unsigned t);
	bool				is(short unsigned v) const;
	int					random() const;
};
struct mapobject : drawable {
	indext				index;
	short unsigned		type;
	explicit			operator bool() const { return type!=0; }
	void				reanimate();
};
class areai {
	unsigned short		floor[100 * 100];
	unsigned short		roof[100 * 100];
public:
	static const int	width = 100;
	static const int	height = 100;
	void				clear();
	void				correct();
	void				correct(indext t);
	void				editor();
	static mapobject*	add(indext index, draw::res_s rid, short unsigned type);
	static mapobject*	find(indext index);
	static int			getx(indext i) { return i % width; }
	static int			gety(indext i) { return i / width; }
	static int			gethx(indext i) { return i % (width * 2); }
	static int			gethy(indext i) { return i / (width * 2); }
	static indext		geti(int x, int y) { return y * width + x; }
	static indext		geth(int x, int y);
	unsigned short		getfloor(indext i) const { return floor[i]; }
	unsigned short		getroof(indext i) const { return roof[i]; }
	bool				read(const char* url);
	void				setroof(indext i, int v) { roof[i] = v; }
	void				setfloor(indext i, int v) { floor[i] = v; }
	void				set(indext i, int v, int w, int h);
	void				set(indext i, const tilei& e, int w, int h);
	static indext		tot(indext i, direction_s d);
	bool				write(const char* url) const;
};
extern areai loc;
namespace draw {
typedef void(*fnpaint)(int x, int y, int width, int param);
struct anminfo {
	unsigned short		fps, frame_count, frame_act;
	point				offset[6];
	point				delta[6];
	static const anminfo* get(const sprite* p);
	static const anminfo* get(res_s rid);
};
void					addaction(fntext getname);
void					addaction(action_s a, fnevent proc, void* object, int param);
void					animate(int x, int y, sprite* ps, int cicle, int fps = 6, int frame = -1, int frame_end = -2);
void					background(int cicle);
bool					button(int x, int y, int width, const char* string, unsigned key, rect* return_rc = 0);
bool					buttonf(int x, int y, int cicles_normal, int cicle_pressed, unsigned key, bool checked, bool execute_by_press = false, bool* pressed = 0, bool disabled = false);
bool					buttong(int x, int y, int width, const char* text, unsigned key);
bool					buttonp(int x, int y, int cicles_normal, int cicle_pressed, const char* title, unsigned key, rect* result_rc);
void					closeform();
void					consolev(const char* format, const char* format_param);
void					consoleview(int x, int y);
void					consoleview();
extern drawable			cursor;
void					edit(int x, int y, int width, char* title, unsigned maximum);
bool					focusable(const rect& rc, variant id);
void					gear(int x, int y, int sx, int sy, item& it, bool resize);
variant					getnext(variant id, int key);
fnevent					getnextstage();
variant					getfocus();
sprite*					gres(res_s id);
const char*				getname(res_s id);
unsigned				getuitime();
point					h2s(point v);
void					image(int x, int y, res_s id, int cicle, int flags = 0, unsigned char alpha = 0xFF);
bool					isactionmode();
bool					isdefaultinput();
bool					ispressed(unsigned t = 1000); // Return true if tips needs to be shown
bool					istips(unsigned t = 1000); // Return true if tips needs to be shown
bool					istipsonetime();
bool					label(int x, int& y, int width, const char* format, unsigned key, bool bullet = false);
void					listinput(int& current, int& origin, int perpage, int perline, unsigned maximum);
void					marker(int x, int y);
point					t2s(point v);
void					message(const char* string, ...);
void					messagev(const char* string, bool focused = false);
void					monthname(int x, int y, int value);
void					number(int x, int y, int digits, int value);
void					numberap(int x, int y, int digits, int value);
void					numbersm(int x, int y, int digits, int value, int mode);
void					post(void* p, fnevent ev, bool run);
void					openform();
bool					radio(int x, int y, int cicle, unsigned key);
void					runstage();
point					s2h(point v);
point					s2t(point v); // Convert screen coordinates to tile index
void					scrollup(const rect& rc, int& origin, int maximum);
void					scrollup(const rect& rc, int& origin, int& maximum, const char* format);
bool					setactionmode();
void					setcolor(color_s v);
void					seteditpos(unsigned index);
void					setfocus(variant v);
void					setfont(res_s v);
void					setint();
void					setpallette(int daylight);
void					setshort();
void					setstage(fnevent proc);
void					setnextstage();
void					window(int& x, int& y, int& sx, int& sy, int frame, int dy = -32);
}