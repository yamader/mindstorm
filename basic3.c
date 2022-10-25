#include "ev3api.h"
#include "app.h"

#ifdef BUILD_MODULE
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

// deca secs
#define sleep(decs) tslp_tsk(decs * 100)

//
// constants
//

#define mport_m EV3_PORT_A
#define mport_l EV3_PORT_B
#define mport_r EV3_PORT_C

//
// tools
//

#define mmotor(p) ev3_motor_set_power(mport_m, p)
#define lmotor(p) ev3_motor_set_power(mport_l, p)
#define rmotor(p) ev3_motor_set_power(mport_r, p)
#define steer(p, r) ev3_motor_steer(mport_l, mport_r, p, r)

void stop() {
  steer(10, 0);
  sleep(1);

  steer(0, 0);
  sleep(1);
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

//
// utils
//

void turn_corner(int inv) {
  int r, l;
  if(!inv) {
    // :(
    steer(14, 0);
    sleep(2);
  }

  r = 30, l = 47;
  rmotor(inv ? l : r);
  lmotor(inv ? r : l);
  sleep(9);
  stop();

  r = -50, l = -22;
  rmotor(inv ? l : r);
  lmotor(inv ? r : l);
  sleep(9);

  lmotor(0);
  rmotor(0);
  sleep(3);
}

void trigon_corner() {
  stop();
  pen_up();
  turn_corner(0);
  pen_down();
}

void trigon_corner_rev() {
  stop();
  pen_up();
  turn_corner(1);
  pen_down();
}

//
// main
//

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
  pen_down();
  for(int i=0; i<28; i++) {
    lmotor(30);
    rmotor(-40);
    sleep(5);

    lmotor(-40);
    rmotor(30);
    sleep(5);
  }

  pen_up();
  rmotor(0);
  lmotor(0);
}
