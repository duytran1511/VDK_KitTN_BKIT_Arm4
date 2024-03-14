/*
 * pacman.h
 */

#ifndef INC_PACMAN_H_
#define INC_PACMAN_H_

/* Includes */
#include <stdint.h>

/* Constants */
#define MAZE_COLUMN_N      		10
#define MAZE_ROW_N         		10
#define MAZE_CELL_WIDTH    		20

#define MAZE_TOP_BORDER			20
#define MAZE_BOTTOM_BORDER 		220
#define MAZE_LEFT_BORDER		20
#define MAZE_RIGHT_BORDER		220

#define BACKGROUND_COLOR		WHITE
#define PACMAN_COLOR			YELLOW
#define GHOST_COLOR				RED
#define PAC_DOTS_COLOR			BROWN

/* Functions */
void game_init(void);
void game_process(void);

#endif /* INC_BUTTON_H_ */
