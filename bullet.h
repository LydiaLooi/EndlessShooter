/** @file bullet.h
 *  @authors Lewis Thorp, Lydia Looi
 *  @date 17 Oct 19
 *  @brief This stores information about the bullet. Such as, all the bullets locations and their direction of travel.
 *         It also allows bullets to be sent and recived between the two funkits.
 */


#ifndef BULLET_H
#define BULLET_H


#include "system.h"
#include "boing.h"


#define MAX_BULLET_COUNT 10


typedef enum col_num {COL1 = 'A', COL2 = 'B', COL3 = 'C', COL4 = 'D', COL5 = 'E', COL6 = 'F', COL7 = 'G'} col_num_t;


typedef enum dir_num {DIR_LEFT = 10, DIR_RIGHT = 20} dir_num_t;


typedef enum deleted_bullet {ACTIVE, DELETED} del_bul_t;


typedef enum collided {NO_COLLISION, COLLISION} collided_t;


typedef struct bullet_s Bullet;


struct bullet_s
{
    boing_state_t bullet_data;
    uint8_t deleted;
};


/**
 * Sends bullet information to other player when bullet leaves LED mat.
 * Information sent consists of position and directon of travel.
 * @param bullet - The bullet to be 'sent' across to the enemy funkit
 */
void send_bullet (Bullet bullet);


/**
 * Updates all the bullets as they travel across the LED mat if they are active.
 * Deletes bullets that reach the bottom or top of the LED matrix.
 * @param bullets - The array of bullets that are in the game
 */
void update_bullets(Bullet* bullets);


/**
 * Creates a bullet ath position x and y with a direction.
 * @param x - The x direction for the bullet to appear in
 * @param y - The y direction for the bullet to appear in
 * @param bullets - A pointer to the array of bullets that are in the game
 * @param bullet_index - A pointer to the integer of the current bullet_index
 */
void create_bullet (uint8_t x, uint8_t y, boing_dir_t direction, Bullet* bullets, uint8_t* bullet_index);


/**
 * Checks each bullet in the game to see if they have collided
 * with the ship
 * @param ship_pos - The current position of the ship
 * @param bullets - The array of bullets tha are in the game
 * @return COLLISION if there has been a collision. NO_COLLISION otherwise
 */
uint8_t collision_check (tinygl_point_t ship_pos, Bullet* bullets);


#endif
