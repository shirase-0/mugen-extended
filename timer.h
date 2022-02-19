#ifndef __MUTIMER__
#define __MUTIMER__

void mu_reset_timer(MU_Timer *timer);
void mu_update_timer(MU_Timer *timer);
uint32_t get_game_time(MU_Timer *timer);
void mu_pause(MU_Timer *timer);
void mu_resume(MU_Timer *timer);

#endif