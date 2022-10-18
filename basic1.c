#include "ev3api.h"
#include "app.h"

#ifdef BUILD_MODULE
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

// deca secs
#define sleep(decs) tslp_tsk(decs * 100)

#define mport_m EV3_PORT_A
#define mport_l EV3_PORT_B
#define mport_r EV3_PORT_C

#define mmotor(p) ev3_motor_set_power(mport_m, p)
#define lmotor(p) ev3_motor_set_power(mport_l, p)
#define rmotor(p) ev3_motor_set_power(mport_r, p)
#define steer(p, r) ev3_motor_steer(mport_l, mport_r, p, r)

void stop() {
  steer(10, 0);
  sleep(1);
  steer(0, 0);
}

void turn(int decs) {
  lmotor(20);
  rmotor(-20);
  sleep(decs);
}

void pen_down() {
  mmotor(8);
  sleep(3);
  mmotor(0);
}

void pen_up() {
  mmotor(-8);
  sleep(3);
  mmotor(0);
  sleep(2);
}

void main_task(intptr_t _) {
  // init
  ev3_motor_config(mport_m, MEDIUM_MOTOR);
  ev3_motor_config(mport_l, LARGE_MOTOR);
  ev3_motor_config(mport_r, LARGE_MOTOR);
  ev3_motor_reset_counts(mport_m);
  ev3_motor_reset_counts(mport_l);
  ev3_motor_reset_counts(mport_r);

  act_tsk(RUN_TASK);
}

void run_task(intptr_t _) {

  //
  pen_down();
  lmotor(30);
  rmotor(-30);
  sleep(20);
  stop();
  pen_up();

  //
  steer(50, 0);
  sleep(4);
  stop();
  turn(8);

  //
  stop();
  pen_down();
  lmotor(50);
  rmotor(20);
  sleep(45);
  stop();
  pen_up();
}
