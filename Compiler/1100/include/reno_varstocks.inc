#if defined _reno_varstocks_included
    #endinput
#endif

#define _reno_varstocks_included

stock const RENO_TID_DATA_CHECK     = 100900701;
stock const RENO_TID_FREEVIP_EVENT  = 100900703;

stock const MENU_KEYS = MENU_KEY_1|MENU_KEY_2|MENU_KEY_3|MENU_KEY_4|MENU_KEY_5|MENU_KEY_6|MENU_KEY_7|MENU_KEY_8|MENU_KEY_9|MENU_KEY_0;

stock const REGEX_PATTERN_IP[] = "(?:\d{1,3}([[:punct:]]|\s|\s\s|\s\s\s)){2,3}(?:\d{1,5})"
stock const REGEX_PATTERN_URL[] = "((?:https?|ftp|file):\/\/)?((?:[а-яА-Я\w](?:[а-яА-Я\w-]{0,61}[а-яА-Я\w])?\s*\.)+[а-яА-Я\w][а-яА-Я\w-]{0,61}[а-яА-Я\w][^^\s^^%@]*$)"