#ifndef __MECHMISSION_GAME_ACTION_H_
#define __MECHMISSION_GAME_ACTION_H_

enum class GameAction {
    none,
    close_window,
    battlefield_ask_quit,
    battlefield_quit,
    battlefield_select,
    battlefield_ask_end_turn,
    battlefield_end_turn,
    battlefield_move_up,
    battlefield_move_down,
    battlefield_move_left,
    battlefield_move_right,
    battlefield_help,
};

#endif
