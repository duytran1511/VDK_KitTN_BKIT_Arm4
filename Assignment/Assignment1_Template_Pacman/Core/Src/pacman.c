/*
 * pacman.c
 */

/* Includes ------------------------------------------------------------------*/
#include "pacman.h"
#include "button.h"
#include "lcd.h"
#include "led_7seg.h"

#include <stdlib.h>
#include <time.h>

/* Enums ---------------------------------------------------------------------*/

typedef enum DIRECTION {
	UP, DOWN, LEFT, RIGHT, STOP
} E_DIRECTION;

/* Struct --------------------------------------------------------------------*/
typedef struct CELL {
	uint8_t is_pac_dot;
} S_CELL;

typedef struct MAZE {
	S_CELL cells[MAZE_COLUMN_N][MAZE_ROW_N];
} S_MAZE;

typedef struct GHOST {
	uint8_t i, j;
	uint8_t i_pre, j_pre;
	E_DIRECTION direction;
} S_GHOST;

typedef struct PACMAN {
	uint8_t i, j;
	uint8_t i_pre, j_pre;
	E_DIRECTION direction;
	int score;
} S_PACMAN;

/* Private Objects -----------------------------------------------------------*/
// Pac-Man object
S_PACMAN pacman;
void pacman_draw(uint8_t i, uint8_t j, uint16_t color);
void pacman_direction_process(void);
void pacman_moving_process(void);

// Ghost object
S_GHOST ghost;
void ghost_draw(uint8_t i, uint8_t j, uint16_t color);
void ghost_direction_process(void);
void ghost_moving_process(void);

// Maze object
S_MAZE maze;
void pac_dot_draw(uint8_t i, uint8_t j, uint16_t color);

// Game object
void game_draw(void);
void game_handler(void);

/* Declare Private Support Functions -----------------------------------------*/
uint8_t is_button_up(void);
uint8_t is_button_down(void);
uint8_t is_button_left(void);
uint8_t is_button_right(void);

/* Public Functions ----------------------------------------------------------*/
/**
 * @brief  	Init Pac-Man game
 * @param  	None
 * @note  	Call when you want to init game
 * @retval 	None
 */
void game_init(void) {
	/*
	 * DONE (can be modified)
	 *
	 * 1. Draw a frame for the maze
	 */
	lcd_clear(BACKGROUND_COLOR);
	lcd_draw_rectangle(MAZE_TOP_BORDER, MAZE_LEFT_BORDER, MAZE_BOTTOM_BORDER,
	MAZE_RIGHT_BORDER, BLACK);
	lcd_show_string(20, 230, "Extremely simple PAC-MAN", BLACK,
	BACKGROUND_COLOR, 16, 0);
	lcd_show_string(20, 250, "Score: ", BLACK, BACKGROUND_COLOR, 16, 0);

	/*
	 * TO DO
	 *
	 * 2. When the game starts, all tiles in the 10x10 maze will have one pac dot available, except Pac-man's tile.
	 * - Firstly, you have to assign suitable values to maze.cells[][].is_pac_dot.
	 * - Then, draw all pac dots on the maze.
	 */

	/*
	 * TO DO
	 *
	 * 3. Init Pac-Man object.
	 * - Firstly, you have to initialize default values for the pacman object.
	 * - Then, draw Pac-Man in the first position.
	 * - Remember that reset maze.cells[][] at pacman's location.
	 */

	/*
	 * TO DO
	 *
	 * 4. Init Ghost object.
	 * - Firstly, you have to initialize default values for the ghost object.
	 * - Then, draw ghost in the first position.
	 */

}

/**
 * @brief  	Process game
 * @param  	None
 * @note  	Call in loop (main)
 * @retval 	None
 */
void game_process(void) {
	static uint8_t counter_game = 0;
	counter_game = (counter_game + 1) % 5;

	pacman_direction_process();

	if (counter_game == 0) {
		pacman_moving_process();
		ghost_moving_process();

		game_handler();

		game_draw();
	}
}

/* Private Functions ---------------------------------------------------------*/
void game_draw(void) {
	/*
	 * TO DO
	 *
	 * 1. First of all, you should delete the image at the character's old location (Pac-Man and Ghost).
	 * 2. Because ghosts cannot eat pac dot, so you should redraw the pac dot at the location the ghost just passed.
	 * 3. Then draw Pac-Man and Ghost at the new location.
	 * 4. Finally, update the new score.
	 */

}

void game_handler(void) {
	/*
	 * TO DO
	 *
	 * 1. Check the loss condition, show something, and restart the game.
	 * 2. Check the win condition, show something, and restart the game.
	 * 3. Check if Pac-Man has won any dots or not, and update the score.
	 */

}

void pacman_direction_process(void) {
	/*
	 * TO DO
	 *
	 * Let's user use button to control Pac-Man.
	 */

}

void pacman_moving_process(void) {
	/*
	 * TO DO
	 *
	 * Update Pac-Man's current and previous position based on current direction.
	 */
	switch (pacman.direction) {
	case UP:

		break;

	case DOWN:

		break;

	case LEFT:

		break;

	case RIGHT:

		break;

	case STOP:

		break;

	default:
		pacman.direction = STOP;
		break;
	}
}

void ghost_direction_process(void) {
	/*
	 * TO DO
	 *
	 * Make Ghost will move randomly (change direction randomly).
	 */
	static E_DIRECTION direction;

	switch (ghost.direction) {
	case UP:

		break;

	case DOWN:

		break;

	case LEFT:

		break;

	case RIGHT:

		break;

	default:
		break;
	}

	ghost.direction = direction;
}

void ghost_moving_process(void) {
	ghost_direction_process();

	/*
	 * TO DO
	 *
	 * Update Pac-Man's current and previous position based on current direction.
	 */

	switch (ghost.direction) {
	case UP:

		break;

	case DOWN:

		break;

	case LEFT:

		break;

	case RIGHT:

		break;

	default:
		ghost.direction = UP;
		break;
	}
}

void pac_dot_draw(uint8_t i, uint8_t j, uint16_t color) {
	/*
	 * TO DO
	 *
	 * Draw whatever you like
	 */

}

void pacman_draw(uint8_t i, uint8_t j, uint16_t color) {
	/*
	 * TO DO
	 *
	 * Draw whatever you like
	 */

}

void ghost_draw(uint8_t i, uint8_t j, uint16_t color) {
	/*
	 * TO DO
	 *
	 * Draw whatever you like
	 */

}

uint8_t is_button_up(void) {
	/*
	 * TO DO
	 */

}

uint8_t is_button_down(void) {
	/*
	 * TO DO
	 */

}

uint8_t is_button_left(void) {
	/*
	 * TO DO
	 */

}

uint8_t is_button_right(void) {
	/*
	 * TO DO
	 */

}
