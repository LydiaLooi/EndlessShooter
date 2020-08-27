/** @file game_data.c
 *  @authors Lewis Thorp, Lydia Looi
 *  @date 17 Oct 19
 *  @brief This stores the generic game data and handles the setup as
 *  well as functions that control the general game.
 */


#include "system.h"
#include "tinygl.h"
#include "game_data.h"
#include <string.h>
#include <stdio.h>


#define START_POS {4,3}
#define NUM_COLUMNS 7


/**
 * Called to setup a fresh game.
 * @param game_data - The game data that is to be updated with the new start
 */
void setup_game (Game_Data* game_data)
{
    tinygl_point_t start_pos = START_POS;
    game_data->ship.ship_pos = start_pos;
    tinygl_point_t loaded_bullet_pos = {game_data->ship.ship_pos.x-1, game_data->ship.ship_pos.y};
    game_data->ship.loaded_bullet_pos = loaded_bullet_pos;
    game_data->ready = READY;
    game_data->ship.aim = DIRECT;
    game_data->own_score = 0;
    game_data->enemy_score = 0;
    game_data->state = STATE_PLAYING;
    show_ship(&game_data->ship, game_data->ready);
}


/**
 * Returns the x position of the ship
 * @param game_data - The game data with ship info
 * @return The x position of the ship as an int
 */
uint8_t get_ship_x (Game_Data game_data)
{
    return game_data.ship.ship_pos.x;
}


/**
 * Returns the y position of the ship
 * @param game_data - The game data with ship info
 * @return The y position of the ship as an int
 */
uint8_t get_ship_y (Game_Data game_data)
{
    return game_data.ship.ship_pos.y;
}


/**
 * Updates whether or not the ship can fire or the cooldown counter
 * @param game_data - The game data with the ready and cooldown info
 */
void update_ready (Game_Data* game_data)
{
    if (game_data->ready == NOT_READY) {

        if (game_data->cooldown_count == SHOOT_COOLDOWN) {
            game_data->cooldown_count = 0;
            game_data->ready = READY;
            show_ship(&game_data->ship, READY);

        } else {
            game_data->cooldown_count = game_data->cooldown_count + 1;
        }
    }
}


/**
 * Checks for incoming bullets. Returns a character indicating the column
 * and the direction the bullet is traveling.
 */
char check_incoming_signal (void)
{
    char received_sig = '\0';

    if (ir_uart_read_ready_p()) {
        received_sig = ir_uart_getc();
    }

    return received_sig;
}


/**
 * Processes the received signal.
 * @param game_data - The game data with the scores and bullet data
 * @param received_sig - The received character
 */
void process_signal (Game_Data* game_data, char received_sig)
{
    /* If the received signal is that the enemy has been hit*/
    if (received_sig == BEEN_HIT) {
        hide_ship(&game_data->ship);
        game_data->own_score++;
        show_score_screen(*game_data);
        game_data->state = STATE_SCORE;
    }

    /* If the received signal is to start a new round */
    if (received_sig == START_ROUND) {
        game_data->state = STATE_PLAYING;
        tinygl_clear();
        show_ship(&game_data->ship, game_data->ready);
        pio_output_low(LED1_PIO);
    }

    /* If the received signal is a valid bullet direction */
    if (received_sig >= COL1 && received_sig <= (COL7 + DIR_RIGHT)) {
        char direction = (received_sig - COL1) / 10;
        char column = ((received_sig - COL1) - (10 * direction));
        column = (-1) * (column - (NUM_COLUMNS - 1)); // inverses the columns so that bullets across the funkits line up

        boing_dir_t bul_dir = DIR_E;
        switch (direction) {
            case 0:
                bul_dir = DIR_E;
                break;
            case 1:
                bul_dir = DIR_SE;
                break;
            case 2:
                bul_dir = DIR_NE;
                break;
        }

        create_bullet(0, column, bul_dir, game_data->bullets, &game_data->bullet_index);
    }
}


/**
 * Shoots a bullet based on the current ship position and aim position
 * @param game_data - The game data with the ship information
 */
void shoot_bullet (Game_Data* game_data)
{
    boing_dir_t direction;
    uint8_t bullet_y = get_ship_y(*game_data);

    if (game_data->ship.aim == RIGHT) {
        direction = DIR_NW;
        bullet_y++;

    } else if (game_data->ship.aim == LEFT) {
        direction = DIR_SW;
        bullet_y--;

    } else {
        direction = DIR_W;
    }

    create_bullet(get_ship_x(*game_data) - 1, bullet_y, direction, game_data->bullets, &game_data->bullet_index);
    game_data->ready = NOT_READY;
}


/**
 * Shows the score screen with both players' scores in the format
 * "Score own | enemy"
 * @param game_data - The game data with the scores
 */
void show_score_screen (Game_Data game_data)
{
    char score_string[20];
    char own_score[4];
    char enemy_score[4];

    strncpy(score_string, "Score ", 8);

    sprintf(own_score, "%d", game_data.own_score);
    sprintf(enemy_score, "|%d", game_data.enemy_score);

    strncat(score_string, own_score, 10);
    strncat(score_string, enemy_score, 10);

    tinygl_text(score_string);
}

/**
 * Called when own ship has been hit.
 * Increases enemy score, shows the score screen and transmits collision event
 * @param game_data - The game data with the scores
 */
void own_ship_hit(Game_Data* game_data)
{
        hide_ship(&game_data->ship);
        game_data->enemy_score++;
        game_data->state = STATE_SCORE;
        show_score_screen(*game_data);
        pio_output_high (LED1_PIO);
        ir_uart_putc(BEEN_HIT);
}

