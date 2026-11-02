#ifndef CONFIG_H
#define CONFIG_H

#define HEIGHT  40
#define WIDTH   80

#define ORIGINX (WIDTH / 2)
#define ORIGINY (HEIGHT / 2)

#define START_SHIELD    10000
#define FILL            '\1'
#define NUM_ENDS        3
#define END_POINT       'E'
#define NUM_DANGER      100
#define DANGER_POINT    'X'

#define START_FACE      DIR_NORTH

//Default for configuring Auto
#define SHOW_STATUS_AFTER_ANY_ACTION    true   // OPT1
#define SHOW_STATUS_AFTER_SCAN          true   // OPT2
#define SHOW_STATUS_AFTER_MOVE          true   // OPT3
#define SHOW_STATUS_AFTER_SHIELD        true   // OPT4
#define SHOW_MAP_AFTER_SCAN             true   // OPT5
#define SHOW_MAP_AFTER_MOVE             true   // OPT6

#define DELAY_AFTER_STATUS_DISPLAY            0      // OPT7  (t1)
#define DELAY_AFTER_SCAN_MAP                  0      // OPT8  (t2)
#define DELAY_AFTER_TRAVEL_MAP                0      // OPT9  (t3)
#define DELAY_AFTER_ANY_AUTOPILOT_ACTION      0      // OPT10 (t4)
#define DELAY_AFTER_AUTOPILOT_SCAN            0      // OPT11 (t5)
#define DELAY_AFTER_AUTOPILOT_MOVE            0      // OPT12 (t6)
#define DELAY_AFTER_AUTOPILOT_SHIELD          0      // OPT13 (t7)

#define ENERGY_LIMIT    77777   // OPT14 (n1)
#define SHIELD_LIMIT    7000    // OPT15 (n2)
#define MOVE_LIMIT      200     // OPT16 (n3)
#define SCAN_LIMIT      500     // OPT17 (n4)

#endif