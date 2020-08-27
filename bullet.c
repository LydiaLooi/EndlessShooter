/** @file bullet.c
 *  @authors Lewis Thorp, Lydia Looi
 *  @date 17 Oct 19
 *  @brief This module handles the sending of bullets, moving all current
 * active bullets in the game as well as checking if any bullet has made
 * contact with the ship.
 */


#include "system.h"
#include "tinygl.h"
#include "ir_uart.h"
#include "bullet.h"


#define BOT_OF_MATRIX 4


/**
 * Sends bullet information to other player when bullet leaves LED mat.
 * Information sent consists of position and directon of travel.
 * @param bullet - The bullet to be 'sent' across to the enemy funkit
 */
void send_bullet (Bullet bullet)
{
    uint8_t bullet_info = bullet.bullet_data.pos.y + COL1;
    boing_dir_t bul_dir = bullet.bullet_data.dir;

    if (bul_dir == DIR_NW) {
        bullet_info = bullet_info + DIR_LEFT;

    } else if (bul_dir == DIR_SW) {
        bullet_info = bullet_info + DIR_RIGHT;
    }

    ir_uart_putc(bullet_info);
}


/**
 * Updates all the bullets as they travel across the LED mat if they are active.
 * Deletes bullets that reach the bottom or top of the LED matrix.
 * @param bullets - The array of bullets that are in the game
 */
void update_bullets (Bullet* bullets)
{
    int i;

    for (i = 0; i < MAX_BULLET_COUNT; i++) {
        if (bullets[i].deleted == ACTIVE ) {
            tinygl_draw_point (bullets[i].bullet_data.pos, 0);
            if (bullets[i].bullet_data.pos.x == BOT_OF_MATRIX) {
                bullets[i].deleted = 1;

            } else if ((bullets[i].bullet_data.pos.x > 0 ||
                        bullets[i].bullet_data.dir == DIR_E ||
                        bullets[i].bullet_data.dir == DIR_NE ||
                        bullets[i].bullet_data.dir == DIR_SE) &&
                        bullets[i].deleted == 0) {

                bullets[i].bullet_data = boing_update (bullets[i].bullet_data);
                tinygl_draw_point (bullets[i].bullet_data.pos, 1);

            } else {
                bullets[i].bullet_data.pos.x = -1; // Sets the bullet off screen

                if (bullets[i].deleted == ACTIVE) {
                    send_bullet(bullets[i]);
                    bullets[i].deleted = DELETED;
                }
            }
        }
    }
}


/**
 * Creates a bullet ath position x and y with a direction.
 * @param x - The x direction for the bullet to appear in
 * @param y - The y direction for the bullet to appear in
 * @param bullets - A pointer to the array of bullets that are in the game
 * @param bullet_index - A pointer to the integer of the current bullet_index
 */
void create_bullet (uint8_t x, uint8_t y, boing_dir_t direction, Bullet* bullets, uint8_t* bullet_index)
{
    boing_state_t bullet_position = boing_init (x, y, direction);

    Bullet bullet;
    bullet.bullet_data = bullet_position;
    bullet.deleted = 0;
    bullets[*bullet_index] = bullet;

    tinygl_draw_point (bullets[*bullet_index].bullet_data.pos, 1);

    *bullet_index = *bullet_index + 1;

    if (*bullet_index >= MAX_BULLET_COUNT) {
        *bullet_index = 0;
    }
}


/**
 * Checks each bullet in the game to see if they have collided
 * with the ship
 * @param ship_pos - The current position of the ship
 * @param bullets - The array of bullets tha are in the game
 * @return COLLISION if there has been a collision. NO_COLLISION otherwise
 */
uint8_t collision_check (tinygl_point_t ship_pos, Bullet* bullets)
{
    for (int i = 0 ; i < MAX_BULLET_COUNT ; i++) {
        if (bullets[i].deleted == ACTIVE && bullets[i].bullet_data.pos.x == BOT_OF_MATRIX) {
            if (bullets[i].bullet_data.pos.y == ship_pos.y) {
                return COLLISION;
            }
        }
    }

    return NO_COLLISION;
}












