#include "ev3api.h"
#include "app.h"

#ifdef BUILD_MODULE
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

#define sleep(ms) tslp_tsk(ms)

#define mport_l EV3_PORT_B
#define mport_r EV3_PORT_C
#define steer(p, r) ev3_motor_steer(mport_l, mport_r, p, r)

void main_task(intptr_t _) {
  // init
  ev3_motor_config(mport_l, LARGE_MOTOR);
  ev3_motor_config(mport_r, LARGE_MOTOR);
  ev3_motor_reset_counts(mport_l);
  ev3_motor_reset_counts(mport_r);

  act_tsk(RUN_TASK);
}

void run_task(intptr_t _) {
  steer(10, 0);
  sleep(2000);
  steer(0, 0);
}
