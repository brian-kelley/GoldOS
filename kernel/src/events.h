#ifndef EVENTS_H
#define EVENTS_H

#define EVENT_QUEUE_MAX 64

#include "globalDefines.h"

//Obsidian event system

enum
{
  KEY_EVENT,        //Key press or release
  MOTION_EVENT,     //Mouse motion
  BUTTON_EVENT      //Mouse button press or release
};

typedef struct
{
  byte scancode;
  bool pressed;
} KeyEvent;

typedef struct
{
  short dx;
  short dy;
} MotionEvent;

typedef struct
{
  short button;
  bool pressed;
} ButtonEvent;

typedef struct {} ClockEvent;

typedef struct 
{
  int type;
  union
  {
    KeyEvent key;
    MotionEvent motion;
    ButtonEvent button;
    ClockEvent clock;
  } e;
} Event;

void initEvents();
void addEvent(Event e);
Event getNextEvent();

#endif

