/** @file game.h
 *  @authors Lewis Thorp, Lydia Looi
 *  @date 17 Oct 19
 *  @brief This stores the location of the shp and allows the ship to move.
 */


#ifndef SHIP_H
#define SHIP_H


#include "system.h"


typedef struct ship_data Ship;


enum aim_dir {LEFT, DIRECT, RIGHT};


struct ship_data {
    tinygl_point_t ship_pos;
    tinygl_point_t loaded_bullet_pos;
    uint8_t aim;
};


/**
 * Sets the postion of loaded bullet.
 * @param ship - A pointer to the ship
 */
void set_loaded_pos (Ship* ship);


/**
 * Draws ship on LED mat.
 * @param ship - A pointer to the ship
 * @param ready - Whether or not the ship can fire or not
 */
void show_ship(Ship* ship, uint8_t ready);


/**
 * Hides the ship
 * @param ship - A pointer to the ship
 */
void hide_ship(Ship* ship);


/**
 * Moves ship one position to the right.
 * @param ship - A pointer to the ship
 * @param ready - Whether or not the ship can fire or not
 */
void ship_move_right(Ship* ship, uint8_t ready);


/**
 * Moves ship one position to the left.
 * @param ship - A pointer to the ship
 * @param ready - Whether or not the ship can fire or not
 */
void ship_move_left(Ship* ship, uint8_t ready);


/**
 * Moves loaded bullet one position to the left.
 * @param ship - A pointer to the ship
 * @param ready - Whether or not the ship can fire or not
 */
void ship_aim_left(Ship* ship, uint8_t ready);


/**
 * Moves loaded bullet one position to the right.
 * @param ship - A pointer to the ship
 * @param ready - Whether or not the ship can fire or not
 */
void ship_aim_right(Ship* ship, uint8_t ready);


#endif
