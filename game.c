/** @file game.c
 *  @authors Lewis Thorp, Lydia Looi
 *  @date 17 Oct 19
 *  @brief A simple shooter game where bullets can bounce of the side and total score is shown between rounds
 */


#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "navswitch.h"
#include "task.h"
#include "ir_uart.h"
#include "game_data.h"
#include "../fonts/font5x5_1.h"


#define DISPLAY_RATE 500
#define INPUT_RATE 100
#define BULLET_MOVE_RATE 5
#define CHECK_RATE 100
#define COOLDOWN_RATE 3
#define MESSAGE_RATE 20


static Game_Data game_data;


/**
 * Initializes the move bullet task.
 */
static void bullet_move_task_init (void)
{
    game_data.bullet_index = 0;
}


/**
 * Calls a task responsible for checking if there are any incoming signals.
 */
static void check_incoming_signal_task (__unused__ void *data)
{
    char received_sig = check_incoming_signal();
    process_signal(&game_data, received_sig);

}


/**
 * Calls a task responsible for moving all bullets that have been shot.
 * It also checks whether or not the bullet has collided into the ship
 */
static void bullet_move_task (__unused__ void *data)
{
    update_bullets(game_data.bullets);

    uint8_t collided = collision_check(game_data.ship.ship_pos, game_data.bullets);

    if (collided == COLLISION) {
        own_ship_hit(&game_data);
    }
}


/**
 * Initializes the navswitch.
 */
static void navswitch_task_init (void)
{
    navswitch_init ();
}


/**
 * Calls a task that checks for navswitch input. If input detected an action is preformed.
 * If the current state is playing, then the navswitch controls the ship
 * If the current state is score screen, then the navswitch controls the
 * starting of the next round.
 */
static void navswitch_task (__unused__ void *data)
{
    navswitch_update ();

    if (game_data.state == STATE_PLAYING) {
        if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
            ship_move_right (&game_data.ship, game_data.ready);
        }

        if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
            ship_move_left (&game_data.ship, game_data.ready);
        }

        if (navswitch_push_event_p (NAVSWITCH_EAST)) {
            ship_aim_right (&game_data.ship, game_data.ready);
        }

        if (navswitch_push_event_p (NAVSWITCH_WEST)) {
            ship_aim_left (&game_data.ship, game_data.ready);
        }

        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            if (game_data.ready == READY) {
                shoot_bullet(&game_data);
            }
        }

    } else if (game_data.state == STATE_SCORE) {
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            game_data.state = STATE_PLAYING;
            tinygl_clear();
            show_ship(&game_data.ship, game_data.ready);
            pio_output_low(LED1_PIO);
            ir_uart_putc(ENEMY_HIT);
        }
    }
}


/**
 * Initializes the display task.
 */
static void display_task_init (void)
{
    tinygl_init (DISPLAY_RATE);
    tinygl_font_set (&font5x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
}


/**
 * A task that displays everything on the LED mat.
 */
static void display_task (__unused__ void *data)
{
    tinygl_update();
}


/**
 * Initializes the gun cooldown task.
 */
static void gun_cooldown_task_init (void)
{
    game_data.cooldown_count = 0;
}


/**
 * A task that keeps track of the ship's gun cooldown counter
 * and whether or not it is ready to fire again
 */
static void gun_cooldown_task (__unused__ void *data)
{
    update_ready(&game_data);
}


/**
 * Initialises system and tasks needed for the game.
 */
void setup_environment(void)
{
    system_init ();
    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);
    navswitch_task_init ();
    display_task_init ();
    bullet_move_task_init ();
    ir_uart_init ();
    gun_cooldown_task_init ();
}


/**
 * Sets up and runs game.
 */
int main (void)
{
    setup_environment();
    setup_game(&game_data);
    game_data.state = STATE_PLAYING;

    task_t tasks[] = {
        {.func = display_task, .period = TASK_RATE / DISPLAY_RATE},
        {.func = check_incoming_signal_task, .period = TASK_RATE / CHECK_RATE},
        {.func = navswitch_task, .period = TASK_RATE / INPUT_RATE},
        {.func = bullet_move_task, .period = TASK_RATE / BULLET_MOVE_RATE},
        {.func = gun_cooldown_task, .period = TASK_RATE / COOLDOWN_RATE},
    };

    task_schedule (tasks, ARRAY_SIZE (tasks));
}

