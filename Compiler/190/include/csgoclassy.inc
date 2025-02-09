#if defined _csgoclassy_included
	#endinput
#endif

#define _csgoclassy_included

#define LICENSED_IP "192.168.1.2"

// 87.98.130.173 - cs csblackdevil
// 188.212.102.174 - cs playarena
// 188.212.102.31 - cs warfield
// 109.163.232.98 - csgo cspower
// 188.212.100.41 - csgo deadly
// 109.163.232.99 - csgo dual-gaming

// 5.254.20.8 - go16 elders

// 109.163.232.167 - csgo erazer
// 188.212.101.171 - csgo gamelife
// 145.239.116.26 - csgo indungi
// 93.114.82.120 - csgo intrusii
// 188.212.101.80 - csgo westcstrike


// 89.40.105.90 - csgo oldstars
// 89.40.105.197 - global oldstars

// 188.212.100.232 - csgo joinet
// 89.40.105.125 - csgo leaguecs
// 93.114.82.30 - csgo warface
// 93.114.82.156 - csgo xplayers
// 45.11.228.17 - csgo skillone


// 51.195.17.136 - free leaguecs
// 188.212.101.144 - fun westcstrike

// 135.125.140.101 - global darkevils
// 54.37.196.185 - csgo darkevils

// 5.254.116.21 - global creativecs

// 188.212.100.7 - global indungi
// 45.153.88.46 - global laleagane

// 89.40.105.53 - go csglobal
// 164.132.220.103 - go cspower
// 188.212.101.80 - go darkcs
// 188.212.102.27 - go pglstar
// 45.153.88.33 - go zerohp

// 89.40.105.86 - nebunaticii deadly
// 188.212.101.139 - nord leaguecs

// 188.212.101.183 - skynet laleagane
// 5.254.20.12 - remake leaguecs
// 5.254.20.17 - rs elders

// 95.156.230.139 - haxorzone
// 89.34.25.5 - scatter

#define maxranks 40
#define maxlevels 40
#define rank_0 0
#define rank_1 1
#define rank_2 2
#define rank_3 3
#define rank_4 4
#define rank_5 5
#define rank_6 6
#define rank_7 7
#define rank_8 8
#define rank_9 9
#define rank_10 10
#define rank_11 11
#define rank_12 12
#define rank_13 13
#define rank_14 14
#define rank_15 15
#define rank_16 16
#define rank_17 17
#define rank_18 18
#define rank_19 19
#define rank_20 20
#define rank_21 21
#define rank_22 22
#define rank_23 23
#define rank_24 24
#define rank_25 25
#define rank_26 26
#define rank_27 27
#define rank_28 28
#define rank_29 29
#define rank_30 30
#define rank_31 31
#define rank_32 32
#define rank_33 33
#define rank_34 34
#define rank_35 35
#define rank_36 36
#define rank_37 37
#define rank_38 38
#define rank_39 39

native get_user_money(id)
native set_user_money(id, amount)
native get_user_scraps(id)
native set_user_scraps(id, amount)
native get_user_keys(id)
native set_user_keys(id, amount)
native get_user_cases(id)
native set_user_cases(id, amount)
native get_user_rank(id, output[], len)
native set_user_rank(id, rankid);
native get_user_skins(id, skinid)
native set_user_skins(id, skinid, amount)
native is_user_logged(id)