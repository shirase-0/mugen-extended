#ifndef __MUENGINE__
#define __MUENGINE__

MU_Engine *mu_engine_init(MU_Allocator *mem_manager, MU_Graphics_Manager *graphics_manager, MU_Timer *timer);
//void mu_reset_engine(); // TODO
void mu_run_engine(MU_Engine *engine);

#endif