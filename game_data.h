/** @file game_data.h
 *  @authors Lewis Thorp, Lydia Looi
 *  @date 17 Oct 19
 *  @brief This stores the generic game data and handles the setup as
 *  well as functions that control the general game.
 */


#ifndef GAME_DATA_H
#define GAME_DATA_H


#include "system.h"
#include "ir_uart.h"
#include "ship.h"
#include "bullet.h"
#include "pio.h"


#define SHOOT_COOLDOWN 2 // cooldown seconds = SHOOT_COUNTDOWN / COOLDOWN_RATE


typedef struct game_data_s Game_Data;


typedef enum game_state {STATE_PLAYING, STATE_SCORE} state_t;


typedef enum ship_ready {NOT_READY, READY} ready_t;


typedef enum hit_type {ENEMY_HIT = '?', BEEN_HIT = '!', START_ROUND = '?'} hit_t;


struct game_data_s
{
    Ship ship;
    uint8_t cooldown_count; // cooldown counter to shoot
    Bullet bullets[MAX_BULLET_COUNT];
    uint8_t bullet_index;
    uint8_t ready; // ready to shoot
    uint8_t own_score;
    uint8_t enemy_score;
    state_t state;

};


/**
 * Called to setup a fresh game.
 * @param game_data - A pointer to the game data that is to be updated with the new start
 */
void setup_game(Game_Data* game_data);


/**
 * Returns the x position of the ship
 * @param game_data - The game data with ship info
 * @return The x position of the ship as an int
 */
uint8_t get_ship_x (Game_Data game_data);


/**
 * Returns the y position of the ship
 * @param game_data - The game data with ship info
 * @return The y position of the ship as an int
 */
uint8_t get_ship_y (Game_Data game_data);


/**
 * Updates whether or not the ship can fire or the cooldown counter
 * @param game_data - A pointer to the game data with the ready and cooldown info
 */
void update_ready(Game_Data* game_data);


/**
 * Checks for incoming bullets. Returns a character indicating the column
 * and the direction the bullet is traveling.
 */
char check_incoming_signal (void);


/**
 * Processes the received signal.
 * @param game_data - A pointer to the game data with the scores and bullet data
 * @param received_sig - The received character
 */
void process_signal(Game_Data* game_data, char received_sig);


/**
 * Shoots a bullet based on the current ship position and aim position
 * @param game_data - A pointer to he game data with the ship information
 */
void shoot_bullet (Game_Data* game_data);


/**
 * Shows the score screen with both players' scores in the format
 * "Score own | enemy"
 * @param game_data - The game data with the scores
 */
void show_score_screen(Game_Data game_data);


/**
 * Called when own ship has been hit.
 * Increases enemy score, shows the score screen and transmits collision event
 * @param game_data - The game data with the scores
 */
void own_ship_hit(Game_Data* game_data);


#endif
