#ifndef CONFIG_H
#define CONFIG_H

#define HEIGHT  40
#define WIDTH   80

#define ORIGINX (WIDTH / 2)
#define ORIGINY (HEIGHT / 2)

#define NORTH     ((PosType){.x =  0, .y = -1})
#define SOUTH     ((PosType){.x =  0, .y =  1})
#define EAST      ((PosType){.x =  1, .y =  0})
#define WEST      ((PosType){.x = -1, .y =  0})

#define NORTHWEST ((PosType){.x = -1, .y = -1})
#define NORTHEAST ((PosType){.x =  1, .y = -1})
#define SOUTHWEST ((PosType){.x = -1, .y =  1})
#define SOUTHEAST ((PosType){.x =  1, .y =  1})

#define ALLAXIS ((PosType[8]){NORTH, NORTHEAST, EAST, SOUTHEAST,SOUTH, SOUTHWEST, WEST, NORTHWEST})
#endif