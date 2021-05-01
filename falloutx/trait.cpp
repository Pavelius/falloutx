#include "main.h"

BSDATA(traiti) = {
	{{"FastMetabolism"}, 55, {Num2, HealingRate}},
	{{"Bruiser"}, 56, {Num2, Strenght, Minus, AP}},
	{{"SmallFrame"}, 57, {Agility}},
	{{"OneHanded"}, 58},
	{{"Finesse"}, 59},
	{{"Kamikaze"}, 60, {Num5, Sequence}},
	{{"HeavyHanded"}, 61, {Num4, DamageMelee, Minus, Num30, CriticalHitTable}},
	{{"FastShoot"}, 62},
	{{"BloodyMess"}, 63},
	{{"Jinxed"}, 64},
	{{"GoodNatured"}, 65,	{Num15, FirstAid, Doctor, Speech, Barter, Minus, Num10, SmallGuns, BigGuns, EnergyWeapon, Unarmed, MeleeWeapon, Throwing}},
	{{"ChemReliant"}, 66},
	{{"ChemResistant"}, 67},
	{{"SexAppeal"}, 68},
	{{"Skilled"}, 69, {Num5, SkillRate, Num1, PerkRate}},
	{{"Gifted"}, 70, {Strenght, Perception, Endurance, Charisma, Intellegence, Agility, Luck, Minus, Num10, SmallGuns, BigGuns, EnergyWeapon, Unarmed, MeleeWeapon, Throwing, FirstAid, Doctor, Sneak, Lockpick, Steal, Traps, Science, Repair, Speech, Barter, Gambling, Outdoorsman, Num5, SkillRate}},
};
assert_enum(traiti, Gifted)