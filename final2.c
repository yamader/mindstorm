#include "ev3api.h"
#include "app.h"

#ifdef BUILD_MODULE
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

// milli seconds
#define sleep(decs) tslp_tsk(decs)

#define mport_m EV3_PORT_A
#define mport_l EV3_PORT_B
#define mport_r EV3_PORT_C
#define sport_c EV3_PORT_1
#define sport_s EV3_PORT_4

#define c_h 8
#define lines (128 / c_h)
#define cline "                              "

#define mmotor(p) ev3_motor_set_power(mport_m, p)
#define lmotor(p) ev3_motor_set_power(mport_l, p)
#define rmotor(p) ev3_motor_set_power(mport_r, p)
#define lr(l, r) { lmotor(l); rmotor(r); }
#define both(p) lr(p, p)

#define sonic() ev3_ultrasonic_sensor_get_distance(sport_s)
#define color() ev3_color_sensor_get_color(sport_c)
#define reflect() ev3_color_sensor_get_reflect(sport_c)

size_t cursor = 0;

void stop() {
  both(10);
  sleep(100);
  both(0);
  sleep(250);
}

void pen_down() {
  mmotor(8);
  sleep(300);
  mmotor(0);
}
void pen_up() {
  mmotor(-8);
  sleep(300);
  mmotor(0);
  sleep(200);
}

void lcd_line(const char* s, size_t y) {
  ev3_lcd_draw_string(cline, 0, y);
  ev3_lcd_draw_string(s, 0, y);
}

#define print(s) lcd_line(s, c_h * cursor)
void println(const char* s) {
  print(s);
  if(cursor < lines - 1) cursor++;
}
void printi(int i) {
  static char buf[31];
  sprintf(buf, "%d", i);
  print(buf);
}
void printiln(int i) {
  printi(i);
  if(cursor < lines - 1) cursor++;
}

//
void draw_cir() {
  pen_down();
  lr(50, -50);
  sleep(1250);
  pen_up();
  both(0);
}

void draw_trigon() {
  for(size_t i=0; i<3; i++) {
    pen_down();
    both(50);
    sleep(500);
    pen_up();

    lr(50, 0);
    sleep(500);

    lr(-10, -50);
    sleep(500);

    both(0);
  }
}

void draw_tetragon() {
  for(size_t i=0;; i++) {
    pen_down();
    both(50);
    sleep(400);
    pen_up();
    stop();

    if(i==3) break;

    lr(50, 0)
    sleep(300);

    lr(0, -50);
    sleep(330);

    lr(-50, -50);
    sleep(250);

    both(0);
  }
}

void draw_star() {
  for(size_t i=0;; i++) {
    pen_down();
    both(50);
    sleep(650);
    pen_up();

    if(i==4) {
      sleep(200);
      both(0);
      return;
    }

    lr(55, 10);
    sleep(500);

    lr(10, 0);
    sleep(50);

    lr(-10, -10);
    sleep(50);
    lr(-10, -60);
    sleep(550);
    // lr(-10, -55);
    // sleep(600);

    both(0);
  }
}

void main_task(intptr_t _) {
  // init
  println("initializing motors...");
  ev3_motor_config(mport_m, MEDIUM_MOTOR);
  ev3_motor_config(mport_l, LARGE_MOTOR);
  ev3_motor_config(mport_r, LARGE_MOTOR);
  ev3_motor_reset_counts(mport_m);
  ev3_motor_reset_counts(mport_l);
  ev3_motor_reset_counts(mport_r);
  ev3_sensor_config(sport_c, COLOR_SENSOR);
  ev3_sensor_config(sport_s, ULTRASONIC_SENSOR);
  println("OK");
  println("");

  // wait
  println("initializing sensors...");
  sonic();
  color();
  reflect();
  println("OK");
  println("");

  println("running...");
  println("");
  act_tsk(RUN_TASK);
}

void run_task(intptr_t _) {
  both(25);

  while(color() == 6)
    sleep(5);
  sleep(30);

 color:
  switch(color()) {
    case COLOR_RED:
      println("color: RED");
      println("shape: CIRCLE");
      stop();
      lr(-50, -50);
      sleep(45);
      lr(0, 0);
      draw_cir();
      break;

    case COLOR_GREEN:
      println("color: GREEN");
      println("shape: TRIGON");
      stop();
      lr(-50, -50);
      sleep(400);
      lr(-50, -35);
      sleep(50);
      draw_trigon();
      break;

    case COLOR_YELLOW:
      println("color: YELLOW");
      println("shape: TETRAGON");
      stop();
      lr(-50, -50);
      sleep(400);
      lr(-50, -35);
      sleep(50);
      draw_tetragon();
      break;

    case COLOR_BLUE:
      println("color: BLUE");
      println("shape: STAR");
      stop();
      lr(-50, -47);
      sleep(380);
      draw_star();
      break;

    default:
      sleep(10);
      goto color;
  }

  both(0);
}
