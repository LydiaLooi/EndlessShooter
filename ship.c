/** @file game.c
 *  @authors Lewis Thorp, Lydia Looi
 *  @date 17 Oct 19
 *  @brief This stores the location of the shp and allows the ship to move.
 */


#include "system.h"
#include "tinygl.h"
#include "ship.h"


#define MIN_Y 0
#define MAX_Y 6


/**
 * Sets the postion of loaded bullet.
 * @param ship - A pointer to the ship
 */
void set_loaded_pos (Ship* ship)
{
    tinygl_draw_point (ship->loaded_bullet_pos, 0);
    tinygl_point_t loaded_bullet_pos = {ship->ship_pos.x-1, ship->ship_pos.y};

    switch (ship->aim) {
        case LEFT:
            loaded_bullet_pos.y = loaded_bullet_pos.y - 1;
            ship->loaded_bullet_pos = loaded_bullet_pos;
            break;

        case RIGHT:
            loaded_bullet_pos.y = loaded_bullet_pos.y + 1;
            ship->loaded_bullet_pos = loaded_bullet_pos;
            break;

        case DIRECT:
            ship->loaded_bullet_pos = loaded_bullet_pos;
            break;
    }
}


/**
 * Draws ship on LED mat.
 * @param ship - A pointer to the ship
 * @param ready - Whether or not the ship can fire or not
 */
void show_ship (Ship* ship, uint8_t ready)
{
    tinygl_draw_point (ship->ship_pos, 1);
    tinygl_draw_point (ship->loaded_bullet_pos, ready);
}


/**
 * Hides the ship
 * @param ship - A pointer to the ship
 */
void hide_ship(Ship* ship)
{
    tinygl_draw_point (ship->ship_pos, 0);
    tinygl_draw_point (ship->loaded_bullet_pos, 0);
}


/**
 * Moves ship one position to the right.
 * @param ship - A pointer to the ship
 * @param ready - Whether or not the ship can fire or not
 */
void ship_move_right (Ship* ship, uint8_t ready)
{
    if (ship->ship_pos.y > MIN_Y) {

        tinygl_draw_point (ship->ship_pos, 0);
        tinygl_draw_point (ship->loaded_bullet_pos, 0);
        ship->ship_pos.y--;
        set_loaded_pos(ship);
    }

    show_ship (ship, ready);
}


/**
 * Moves ship one position to the left.
 * @param ship - A pointer to the ship
 * @param ready - Whether or not the ship can fire or not
 */
void ship_move_left (Ship* ship, uint8_t ready)
{
    if (ship->ship_pos.y < MAX_Y) {

        tinygl_draw_point (ship->ship_pos, 0);
        tinygl_draw_point (ship->loaded_bullet_pos, 0);

        ship->ship_pos.y++;
        set_loaded_pos(ship);
    }

    show_ship (ship, ready);
}


/**
 * Moves loaded bullet one position to the left.
 * @param ship - A pointer to the ship
 * @param ready - Whether or not the ship can fire or not
 */
void ship_aim_left(Ship* ship, uint8_t ready)
{
    if (ship->ship_pos.y < MAX_Y && ship->aim != LEFT) {
        ship->aim--;
    }

    set_loaded_pos(ship);
    show_ship (ship, ready);
}


/**
 * Moves loaded bullet one position to the right.
 * @param ship - A pointer to the ship
 * @param ready - Whether or not the ship can fire or not
 */
void ship_aim_right(Ship* ship, uint8_t ready)
{
    if (ship->ship_pos.y > MIN_Y && ship->aim != RIGHT) {
        ship->aim++;
    }

    set_loaded_pos(ship);
    show_ship (ship, ready);
}
