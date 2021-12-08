#ifndef __MUENGINE__
#define __MUENGINE__

MU_Engine *mu_engine_init(MU_Allocator *mem_manager, MU_Video_System *video_system, MU_Timer *timer);
void mu_reset_engine(); // Not defined in original code
void mu_run_engine(MU_Engine *engine);

#endif