#if defined _reno_utils_included
    #endinput
#endif

#define _reno_utils_included

#if !defined _fakemeta_included	
	#include <fakemeta>
#endif

const g_iColorsNum = 6;

enum _:Colors
{
	COLOR_GREEN = 1,
	COLOR_RED,
	COLOR_BLUE,
	COLOR_WHITE,
	COLOR_ORANGE
}

stock RENO_FadeScreen(const iPlayer, const iColor)
{
	// Screen fading
	message_begin(MSG_ONE_UNRELIABLE, get_user_msgid("ScreenFade"), {0,0,0}, .player = iPlayer)
	write_short(1<<10)
	write_short(1<<10)
	write_short(0x0000)
	switch(iColor)
	{
		case COLOR_GREEN: {
			write_byte(0)
			write_byte(200)
			write_byte(0)
		}
		case COLOR_RED: {
			write_byte(200)
			write_byte(0)
			write_byte(0)
		}
		case COLOR_BLUE: {
			write_byte(0)
			write_byte(0)
			write_byte(200)
		}
		case COLOR_WHITE: {
			write_byte(255)
			write_byte(255)
			write_byte(250)
		}
		case COLOR_ORANGE: {
			write_byte(255)
			write_byte(150)
			write_byte(0)
		}
		default: {
			write_byte(random_num(0,255))
			write_byte(random_num(10,255))
			write_byte(random_num(0,255))
		}
	}
	write_byte(75);
	message_end()
	// Screen fading
}

stock RENO_SendUserAudio(const iPlayer, const audio[])
{
	if(!is_user_connected(iPlayer))
		return;

	rg_send_audio(iPlayer, audio, PITCH_NORM);
}

