/* K1d0x Productions - Chat Color functions
*
* (c) Copyright 2010-2012, K1d0x -> http://pss-live.ro/forum/member.php/859-K1d0x
*
* This file is provided as is (no warranties).
*/

#if defined _K1d0x_C_Color_included
  #endinput
#endif
#define _K1d0x_C_Color_included

stock K1d0x_C_Color(const id, const intrare[], any:...) {
     new numar = 1, jucatori[32]

     static mesaj[191]
     vformat(mesaj, 190, intrare, 3)
	
     replace_all(mesaj, 190, "!v", "^4")     /* culoarea verde                                */
     replace_all(mesaj, 190, "!c", "^1")    /* culoarea chatului                             */
     replace_all(mesaj, 190, "!e", "^3")   /* culoarea echipei ct = albastru | tero = rosie */
     replace_all(mesaj, 190, "!e2", "^0") /* culoarea normala a echipei                    */
	
     if(id)
          jucatori[0] = id
     else

     get_players(jucatori, numar, "ch")
     {
     
          for(new i = 0; i < numar; i++) {
               if(is_user_connected(jucatori[i])) {
                    message_begin(MSG_ONE_UNRELIABLE, get_user_msgid("SayText"), _, jucatori[i])
                    write_byte(jucatori[i])
                    write_string(mesaj)
                    message_end()
               }
          }
     }
}