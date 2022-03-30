#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FOOD_SCORE 10
#define MAX_GHOSTS 30
#define CAPSULE_DURATION 10
#define CAPSULE_SCORE 50
#define GHOST_SCORE 200

typedef enum gameState { losing, onGoing, winning } GameState;
typedef enum direction { up, down, left, right, idle } Direction;

typedef struct Gghost {
    int type;
    int ghostcount;
    int ghrow;
    int ghcolumn;

    Direction ddirection;
} Ghost;

typedef struct game {
    char **grid;   // a 2-dimensional array of characters to display the game;
    int rows;      // number of rows of the grid;
    int columns;   // number of columns of the grid;
    int foodCount; // number of remaining food in the game;
    int PacmanCount;
    int score; // current score;
    int superpower;
    int condition;
    // condition 1: 移动
    // condition 2: 静止
    Ghost *ghost; // ghost strust;
    GameState
        state; // the state of the game, one of losing, onGoing, or winning.
} Game;

/* Creates a new game, with given rows and columns.
 * The top-left corner is at row 0 and column 0.
 * Boundary is not included in either rows or columns.
 * You should dynamically allocate space for a Game pointer,
 * dynamically allocate a 2-dimensional char grid,
 * and initialize any other value in your Game structure.
 * Return that pointer to the game you created.
 */
Game *NewGame(int rows, int columns) {
    // dynamically allocate space for a Game pointer
    Game *pointer = (Game *)malloc(sizeof(Game));
    pointer->ghost = (Ghost *)malloc(sizeof(Ghost) * MAX_GHOSTS);
    pointer->ghost->ghostcount = 0;
    pointer->ghost->type = 1;
    pointer->superpower = 0;
    pointer->rows = rows;
    pointer->columns = columns;
    pointer->PacmanCount = 0;
    pointer->condition = 1;

    // dynamically allocate a 2-dimensional char grid
    pointer->grid = (char **)malloc(sizeof(char *) * (rows));
    for (int i = 0; i < rows; i++) {
        pointer->grid[i] = (char *)malloc(sizeof(char) * columns);
    }
    for (int i = 0; i < pointer->rows; i++) {
        for (int j = 0; j < pointer->columns; j++) {
            pointer->grid[i][j] = ' ';
        }
    }
    // initialize any other value in your Game structure
    pointer->foodCount = 0;
    pointer->score = 0;
    pointer->state = onGoing;
    // Return that pointer to the game you created.
    return pointer; // This is incorrect.
}

/* Ends your game by freeing any memory
 * you dynamically allocated.
 * After that, free the "game" pointer,
 * since it is also dynamically created.
 */
void EndGame(Game *game) {
    // free any merroy you dynamically allocated
    for (int i = 0; i < game->rows; i++) {
        free(game->grid[i]);
    }
    free(game->ghost);
    free(game->grid);
    // free the "game" pointer
    free(game);
    return; // This is incorrect.
}

/* Adds a wall to the game, at row = r, column = c.
 * Walls should be added only to an empty place.
 * After you successfully add a wall,
 * you should modify the "grid" to represent
 * the wall by '#'. Then, return true.
 * Return false on failures.
 */
bool AddWall(Game *game, int r, int c) {
    // Adds a wall to the game, at row = r, column = c.
    if (r >= 0 && r < game->rows) {
        if (c >= 0 && c < game->columns) {
            // Walls should be added only to an empty place.
            if (game->grid[r][c] == ' ') {
                // you should modify the "grid" to represent the wall by '#'
                game->grid[r][c] = '#';
                // Then, return true.
                return true;
            }
        }
    }
    return false; // This is not always correct.
}

/* Adds a food to the game, at row = r, column = c.
 * Foods should be added only to an empty place.
 * After you successfully add a food,
 * you should modify the "grid" to represent
 * the food by '.', also update the "foodCount"
 * correctly. Then, return true.
 * Return false on failures.
 */
bool AddFood(Game *game, int r, int c) {
    // Adds a food to the game, at row = r, column = c.
    if (r >= 0 && r < game->rows) {
        if (c >= 0 && c < game->columns) {
            // Foods should be added only to an empty place.
            if (game->grid[r][c] == ' ') {
                // you should modify the "grid" to represent the food by '.',
                // also update the "foodCount"
                game->grid[r][c] = '.';
                game->foodCount++;
                // Then, return true.
                return true;
            }
        }
    }
    return false; // This is not always correct.
}

/* Adds a Pacman to the game! At row = r, column = c.
 * If there is already a Pacman, return false.
 * Pacman should be added only to an empty place.
 * After you successfully add a Pacman,
 * you should modify the "grid" to represent
 * the Pacman by 'C'. Then, return true.
 * Return false on failures.
 */
bool AddPacman(Game *game, int r, int c) {
    // If there is already a Pacman, return false.
    if (game->PacmanCount == 0) {
        if (r >= 0 && r < game->rows) {
            if (c >= 0 && c < game->columns) {
                // Pacman should be added only to an empty place.
                if (game->grid[r][c] == ' ') {
                    // you should modify the "grid" to represent the Pacman by
                    // 'C', also update the "foodCount"
                    game->grid[r][c] = 'C';
                    game->PacmanCount++;
                    // Then, return true.
                    return true;
                }
            }
        }
    }
    // Return false on failures.
    return false; // This is not always correct.
}

/* Moves Pacman in the given direction.
 * The direction will always be valid.
 * Pacman should stay still if this move
 * will let it hit a wall or boundaries.
 * No matter what move is performed, the
 * game's score should decrease by 1.
 *
 * If Pacman eats a food, the game's score
 * should increase by FOOD_SCORE. If that
 * food is the last one in the game, you
 * should mark the game's state as "winning".
 *
 *
 * FOR PART B:
 *
 * Pacman is allowed to move onto a grid
 * with a ghost. In that case, Pacman loses,
 * and will not be able to eat the food or capsule
 * under that ghost, if any. Score should
 * still decrease by 1, and you should mark the
 * game's state as "losing".
 *
 * If Pacman eats a capsule, the game's score
 * should increase by CAPSULE_SCORE, and Pacman
 * will gain CAPSULE_DURATION = 10 turns of
 * superpower. If that capsule is the last food
 * in the game, mark the game's state as "winning".
 *
 */
void MovePacman(Game *game, Direction direction) {
    int r;
    int c;
    game->score--;
    if (game->superpower > 0) {
        game->superpower--;
    }
    if (game->superpower > 0) {
        // game->superpower--;
        for (int i = 0; i < game->ghost->ghostcount; i++) {
            int rr = game->ghost[i].ghrow;
            int cc = game->ghost[i].ghcolumn;
            if (rr >= 0 && rr < game->rows && cc >= 0 && cc < game->columns) {
                game->grid[rr][cc] = 'X';
            }
        }
    }
    if (game->superpower == 0) {
        for (int i = 0; i < game->ghost->ghostcount; i++) {
            int rr = game->ghost[i].ghrow;
            int cc = game->ghost[i].ghcolumn;
            if (rr >= 0 && rr < game->rows && cc >= 0 && cc < game->columns) {
                game->grid[rr][cc] = '@';
            }
        }
    }
    // if (game->superpower > 0) {
    //     game->superpower--;
    // }
    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->columns; j++) {
            if (game->grid[i][j] == 'C') {
                r = i;
                c = j;
            }
        }
    }
    if (direction == up) {
        if ((r - 1) < 0) {
            game->grid[r][c] = 'C';
        } else if (game->grid[r - 1][c] == '#') {
            game->grid[r][c] = 'C';
        } else if (game->grid[r - 1][c] == '.') {
            game->grid[r - 1][c] = 'C';
            game->grid[r][c] = ' ';
            game->foodCount--;
            game->score = game->score + FOOD_SCORE;
        } else if (game->grid[r - 1][c] == ' ') {
            game->grid[r - 1][c] = 'C';
            game->grid[r][c] = ' ';
        } else if (game->grid[r - 1][c] == 'o') {
            game->grid[r - 1][c] = 'C';
            game->grid[r][c] = ' ';
            game->foodCount--;
            game->score = game->score + CAPSULE_SCORE;
            if (game->superpower == 0) {
                game->condition = 1;
            }
            game->superpower = CAPSULE_DURATION;
            int nn = game->ghost->ghostcount;
            for (int j = 0; j < nn; j++) {
                int rr = game->ghost[j].ghrow;
                int cc = game->ghost[j].ghcolumn;
                if (rr >= 0 && rr < game->rows && cc >= 0 &&
                    cc < game->columns) {
                    game->grid[rr][cc] = 'X';
                }
            }
        } else if (game->grid[r - 1][c] == '@') {
            // game->grid[r - 1][c] = 'C';
            game->grid[r][c] = ' ';
            game->state = losing;
        } else if (game->grid[r - 1][c] == 'X') {
            game->grid[r - 1][c] = 'C';
            game->grid[r][c] = ' ';
            game->score = game->score + GHOST_SCORE;
            for (int i = 0; i < game->ghost->ghostcount; i++) {
                int rr = game->ghost[i].ghrow;
                int cc = game->ghost[i].ghcolumn;
                if ((rr == r - 1) && (cc == c)) {
                    game->ghost[i].ghrow = -1;
                    game->ghost[i].ghcolumn = -1;
                    if (game->ghost[i].type == 2) {
                        game->score = game->score + FOOD_SCORE;
                        game->foodCount--;
                    } else if (game->ghost[i].type == 3) {
                        game->score = game->score + CAPSULE_SCORE;
                        game->superpower = CAPSULE_DURATION;
                        game->foodCount--;
                        int nn = game->ghost->ghostcount;
                        for (int j = 0; j < nn; j++) {
                            int rrr = game->ghost[j].ghrow;
                            int ccc = game->ghost[j].ghcolumn;
                            if (rrr >= 0 && rrr < game->rows && ccc >= 0 &&
                                ccc < game->columns) {
                                game->grid[rrr][ccc] = 'X';
                            }
                        }
                    }
                }
            }
        }
    }
    if (direction == down) {
        if ((r + 1) == game->rows) {
            game->grid[r][c] = 'C';
        } else if (game->grid[r + 1][c] == '#') {
            game->grid[r][c] = 'C';
        } else if (game->grid[r + 1][c] == '.') {
            game->grid[r + 1][c] = 'C';
            game->grid[r][c] = ' ';
            game->foodCount--;
            game->score = game->score + FOOD_SCORE;
        } else if (game->grid[r + 1][c] == ' ') {
            game->grid[r + 1][c] = 'C';
            game->grid[r][c] = ' ';
        } else if (game->grid[r + 1][c] == 'o') {
            game->grid[r + 1][c] = 'C';
            game->grid[r][c] = ' ';
            game->foodCount--;
            game->score = game->score + CAPSULE_SCORE;
            if (game->superpower == 0) {
                game->condition = 1;
            }
            game->superpower = 10;
            int nn = game->ghost->ghostcount;
            for (int j = 0; j < nn; j++) {
                int rr = game->ghost[j].ghrow;
                int cc = game->ghost[j].ghcolumn;
                if (rr >= 0 && rr < game->rows && cc >= 0 &&
                    cc < game->columns) {
                    game->grid[rr][cc] = 'X';
                }
            }
        } else if (game->grid[r + 1][c] == '@') {
            // game->grid[r + 1][c] = 'C';
            game->grid[r][c] = ' ';
            game->state = losing;
        } else if (game->grid[r + 1][c] == 'X') {
            game->grid[r + 1][c] = 'C';
            game->grid[r][c] = ' ';
            game->score = game->score + GHOST_SCORE;
            for (int i = 0; i < game->ghost->ghostcount; i++) {
                int rr = game->ghost[i].ghrow;
                int cc = game->ghost[i].ghcolumn;
                if ((rr == r + 1) && (cc == c)) {
                    game->ghost[i].ghrow = -1;
                    game->ghost[i].ghcolumn = -1;
                    if (game->ghost[i].type == 2) {
                        game->score = game->score + FOOD_SCORE;
                        game->foodCount--;
                    } else if (game->ghost[i].type == 3) {
                        game->score = game->score + CAPSULE_SCORE;
                        game->superpower = CAPSULE_DURATION;
                        game->foodCount--;
                        int nn = game->ghost->ghostcount;
                        for (int j = 0; j < nn; j++) {
                            int rrr = game->ghost[j].ghrow;
                            int ccc = game->ghost[j].ghcolumn;
                            if (rrr >= 0 && rrr < game->rows && ccc >= 0 &&
                                ccc < game->columns) {
                                game->grid[rrr][ccc] = 'X';
                            }
                        }
                    }
                }
            }
        }
    }
    if (direction == left) {
        if ((c - 1) < 0) {
            game->grid[r][c] = 'C';
        } else if (game->grid[r][c - 1] == '#') {
            game->grid[r][c] = 'C';
        } else if (game->grid[r][c - 1] == '.') {
            game->grid[r][c - 1] = 'C';
            game->grid[r][c] = ' ';
            game->foodCount--;
            game->score = game->score + FOOD_SCORE;
        } else if (game->grid[r][c - 1] == ' ') {
            game->grid[r][c - 1] = 'C';
            game->grid[r][c] = ' ';
        } else if (game->grid[r][c - 1] == 'o') {
            game->grid[r][c - 1] = 'C';
            game->grid[r][c] = ' ';
            game->foodCount--;
            game->score = game->score + CAPSULE_SCORE;
            if (game->superpower == 0) {
                game->condition = 1;
            }
            game->superpower = 10;
            int nn = game->ghost->ghostcount;
            for (int j = 0; j < nn; j++) {
                int rr = game->ghost[j].ghrow;
                int cc = game->ghost[j].ghcolumn;
                if (rr >= 0 && rr < game->rows && cc >= 0 &&
                    cc < game->columns) {
                    game->grid[rr][cc] = 'X';
                }
            }

        } else if (game->grid[r][c - 1] == '@') {
            // game->grid[r][c - 1] = 'C';
            game->grid[r][c] = ' ';
            game->state = losing;
        } else if (game->grid[r][c - 1] == 'X') {
            game->grid[r][c - 1] = 'C';
            game->grid[r][c] = ' ';
            game->score = game->score + GHOST_SCORE;
            for (int i = 0; i < game->ghost->ghostcount; i++) {
                int rr = game->ghost[i].ghrow;
                int cc = game->ghost[i].ghcolumn;
                if ((rr == r) && (cc == c - 1)) {
                    game->ghost[i].ghrow = -1;
                    game->ghost[i].ghcolumn = -1;
                    if (game->ghost[i].type == 2) {
                        game->score = game->score + FOOD_SCORE;
                        game->foodCount--;
                    } else if (game->ghost[i].type == 3) {
                        game->score = game->score + CAPSULE_SCORE;
                        game->superpower = CAPSULE_DURATION;
                        game->foodCount--;
                        int nn = game->ghost->ghostcount;
                        for (int j = 0; j < nn; j++) {
                            int rrr = game->ghost[j].ghrow;
                            int ccc = game->ghost[j].ghcolumn;
                            if (rrr >= 0 && rrr < game->rows && ccc >= 0 &&
                                ccc < game->columns) {
                                game->grid[rrr][ccc] = 'X';
                            }
                        }
                    }
                }
            }
        }
    }
    if (direction == right) {
        if ((c + 1) == game->columns) {
            game->grid[r][c] = 'C';
        } else if (game->grid[r][c + 1] == '#') {
            game->grid[r][c] = 'C';
        } else if (game->grid[r][c + 1] == '.') {
            game->grid[r][c + 1] = 'C';
            game->grid[r][c] = ' ';
            game->foodCount--;
            game->score = game->score + FOOD_SCORE;
        } else if (game->grid[r][c + 1] == ' ') {
            game->grid[r][c + 1] = 'C';
            game->grid[r][c] = ' ';
        } else if (game->grid[r][c + 1] == 'o') {
            game->grid[r][c + 1] = 'C';
            game->grid[r][c] = ' ';
            game->foodCount--;
            game->score = game->score + CAPSULE_SCORE;
            if (game->superpower == 0) {
                game->condition = 1;
            }
            game->superpower = CAPSULE_DURATION;
            int nn = game->ghost->ghostcount;
            for (int j = 0; j < nn; j++) {
                int rr = game->ghost[j].ghrow;
                int cc = game->ghost[j].ghcolumn;
                if (rr >= 0 && rr < game->rows && cc >= 0 &&
                    cc < game->columns) {
                    game->grid[rr][cc] = 'X';
                }
            }
        } else if (game->grid[r][c + 1] == '@') {
            // game->grid[r][c + 1] = 'C';
            game->grid[r][c] = ' ';
            game->state = losing;
        } else if (game->grid[r][c + 1] == 'X') {
            game->grid[r][c + 1] = 'C';
            game->grid[r][c] = ' ';
            game->score = game->score + GHOST_SCORE;
            for (int i = 0; i < game->ghost->ghostcount; i++) {
                int rr = game->ghost[i].ghrow;
                int cc = game->ghost[i].ghcolumn;
                if ((rr == r) && (cc == c + 1)) {
                    game->ghost[i].ghrow = -1;
                    game->ghost[i].ghcolumn = -1;
                    if (game->ghost[i].type == 2) {
                        game->score = game->score + FOOD_SCORE;
                        game->foodCount--;
                    } else if (game->ghost[i].type == 3) {
                        game->score = game->score + CAPSULE_SCORE;
                        game->superpower = CAPSULE_DURATION;
                        game->foodCount--;
                        int nn = game->ghost->ghostcount;
                        for (int j = 0; j < nn; j++) {
                            int rrr = game->ghost[j].ghrow;
                            int ccc = game->ghost[j].ghcolumn;
                            if (rrr >= 0 && rrr < game->rows && ccc >= 0 &&
                                ccc < game->columns) {
                                game->grid[rrr][ccc] = 'X';
                            }
                        }
                    }
                }
            }
        }
    }
    if (direction == idle) {
        game->grid[r][c] = 'C';
    }

    if (game->foodCount == 0) {
        game->state = winning;
    }
    return; // This is incorrect.
}

//////////////////////// PART B ////////////////////////

/* Adds a ghost to the game,
 * at row = r, column = c, with given initial direction.
 * you should modify the "grid" to represent the ghost by '@'.
 * Ghosts can be added to an empty place or on a food.
 * For the latter case, the ghost will cover that food on display,
 * represented also by an '@'.
 * That food must still exist, and that grid should
 * display '.' when the ghost leaves.
 */
bool AddGhost(Game *game, int r, int c, Direction direction) {
    // at row = r, column = c, with given initial direction.
    int nn = game->ghost->ghostcount;
    if (nn >= MAX_GHOSTS) {
        return false;
    }
    if (r >= 0 && r < game->rows) {
        if (c >= 0 && c < game->columns) {
            if (game->grid[r][c] == '.') {
                game->ghost[nn].ddirection = direction;
                game->ghost[nn].type = 2;
                game->ghost[nn].ghrow = r;
                game->ghost[nn].ghcolumn = c;
                game->grid[r][c] = '@';
                game->ghost->ghostcount++;
                return true;
            } else if (game->grid[r][c] == ' ') {
                game->ghost[nn].ddirection = direction;
                game->ghost[nn].type = 1;
                game->ghost[nn].ghrow = r;
                game->ghost[nn].ghcolumn = c;
                game->grid[r][c] = '@';
                game->ghost->ghostcount++;
                return true;
            } else if (game->grid[r][c] == 'o') {
                game->ghost[nn].ddirection = direction;
                game->ghost[nn].type = 3;
                game->ghost[nn].ghrow = r;
                game->ghost[nn].ghcolumn = c;
                game->grid[r][c] = '@';
                game->ghost->ghostcount++;
                return true;
            }
        }
    }
    return false; // This is not always correct.
}

/* Moves all ghosts in their own directions.
 * Ghosts should be moved in the order they were added.
 * If a ghost will bump into a wall, another ghost, or a boundary,
 * Its direction will reverse, and it will try to move
 * in the new direction immediately this turn.
 * If it bumps into another wall/ghost/boundary,
 * it won't move for this turn.
 */
void MoveGhosts(Game *game) {
    int con = game->condition;
    if (game->superpower > 0) {
        // game->superpower--;
        for (int i = 0; i < game->ghost->ghostcount; i++) {
            int rr = game->ghost[i].ghrow;
            int cc = game->ghost[i].ghcolumn;
            if (rr >= 0 && rr < game->rows && cc >= 0 && cc < game->columns) {
                game->grid[rr][cc] = 'X';
            }
        }
        if (game->condition == 1) {
            game->condition = 2;
        } else {
            game->condition = 1;
        }
    }
    if (game->superpower == 0) {
        for (int i = 0; i < game->ghost->ghostcount; i++) {
            int rr = game->ghost[i].ghrow;
            int cc = game->ghost[i].ghcolumn;
            if (rr >= 0 && rr < game->rows && cc >= 0 && cc < game->columns) {
                game->grid[rr][cc] = '@';
            }
        }
        if (game->condition == 1 || game->condition == 2) {
            game->condition = 1;
        }
    }
    int num = game->ghost->ghostcount;
    for (int i = 0; i < num; i++) {
        int r = game->ghost[i].ghrow;
        int c = game->ghost[i].ghcolumn;
        //如果ghost在游戏中的话
        if (r >= 0 && r < game->rows) {
            if (c >= 0 && c < game->columns) {
                // up
                if (game->ghost[i].ddirection == up) {
                    //如果没有超能力
                    if (game->superpower == 0) {
                        //如果前方有阻碍 转向
                        if ((r - 1) < 0 || game->grid[r - 1][c] == '#' ||
                            game->grid[r - 1][c] == '@' ||
                            game->grid[r - 1][c] == 'X') {
                            game->ghost[i].ddirection = down;
                            //如果前方仍有阻碍 继续转向
                            if (r + 1 == game->rows ||
                                game->grid[r + 1][c] == '#' ||
                                game->grid[r + 1][c] == '@' ||
                                game->grid[r + 1][c] == 'X') {
                                // game->ghost[i].ddirection = up;
                                game->grid[r][c] = '@';
                            } else {
                                //如果转向后前方没有阻碍
                                //输出前一个位置的值
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                }
                                if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                }
                                if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                //输出后一个位置的值
                                if (game->grid[r + 1][c] == 'C') {
                                    game->state = losing;
                                    game->grid[r + 1][c] = '@';
                                    game->ghost[i].ghrow++;

                                } else if (game->grid[r + 1][c] == '.') {
                                    game->ghost[i].type = 2;
                                    game->grid[r + 1][c] = '@';
                                    game->ghost[i].ghrow++;
                                } else if (game->grid[r + 1][c] == 'o') {
                                    game->ghost[i].type = 3;
                                    game->grid[r + 1][c] = '@';
                                    game->ghost[i].ghrow++;
                                } else if (game->grid[r + 1][c] == ' ') {
                                    game->ghost[i].type = 1;
                                    game->grid[r + 1][c] = '@';
                                    game->ghost[i].ghrow++;
                                }
                            }
                        }
                        //如果前方没有阻碍并且出现food
                        else if (game->grid[r - 1][c] == '.') {
                            if (game->ghost[i].type == 1) {
                                game->grid[r][c] = ' ';
                            }
                            if (game->ghost[i].type == 2) {
                                game->grid[r][c] = '.';
                            }
                            if (game->ghost[i].type == 3) {
                                game->grid[r][c] = 'o';
                            }
                            game->ghost[i].type = 2;
                            game->grid[r - 1][c] = '@';
                            game->ghost[i].ghrow--;
                        }
                        //如果前方没有阻碍并且出现capsule
                        else if (game->grid[r - 1][c] == 'o') {
                            if (game->ghost[i].type == 1) {
                                game->grid[r][c] = ' ';
                            }
                            if (game->ghost[i].type == 2) {
                                game->grid[r][c] = '.';
                            }
                            if (game->ghost[i].type == 3) {
                                game->grid[r][c] = 'o';
                            }
                            game->ghost[i].type = 3;
                            game->grid[r - 1][c] = '@';
                            game->ghost[i].ghrow--;
                        }
                        //如果前方没有阻碍 且没有food或者capsule
                        else if (game->grid[r - 1][c] == ' ') {
                            if (game->ghost[i].type == 1) {
                                game->grid[r][c] = ' ';
                            }
                            if (game->ghost[i].type == 2) {
                                game->grid[r][c] = '.';
                            }
                            if (game->ghost[i].type == 3) {
                                game->grid[r][c] = 'o';
                            }
                            game->ghost[i].type = 1;
                            game->grid[r - 1][c] = '@';
                            game->ghost[i].ghrow--;
                        }
                        //如果前方没有阻碍 但是遇见了C
                        else if (game->grid[r - 1][c] == 'C') {
                            game->ghost[i].ghrow--;
                            if (game->ghost[i].type == 1) {
                                game->grid[r][c] = ' ';
                            }
                            if (game->ghost[i].type == 2) {
                                game->grid[r][c] = '.';
                            }
                            if (game->ghost[i].type == 3) {
                                game->grid[r][c] = 'o';
                            }
                            game->grid[r - 1][c] = '@';
                            game->state = losing;
                        }
                    }
                    //如果有超能力的话
                    else {
                        if (con == 2) {
                            game->grid[r][c] = 'X';

                        } else {
                            if ((r - 1) < 0 || game->grid[r - 1][c] == '#' ||
                                game->grid[r - 1][c] == '@' ||
                                game->grid[r - 1][c] == 'X') {
                                game->ghost[i].ddirection = down;
                                //如果前方仍有阻碍 继续转向
                                if (r + 1 == game->rows ||
                                    game->grid[r + 1][c] == '#' ||
                                    game->grid[r + 1][c] == '@' ||
                                    game->grid[r + 1][c] == 'X') {
                                    // game->ghost[i].ddirection = up;
                                    game->grid[r][c] = 'X';
                                } else {
                                    //如果转向后前方没有阻碍
                                    //输出前一个位置的值
                                    if (game->ghost[i].type == 1) {
                                        game->grid[r][c] = ' ';
                                    }
                                    if (game->ghost[i].type == 2) {
                                        game->grid[r][c] = '.';
                                    }
                                    if (game->ghost[i].type == 3) {
                                        game->grid[r][c] = 'o';
                                    }
                                    //输出后一个位置的值
                                    if (game->grid[r + 1][c] == 'C') {
                                        game->score = game->score + GHOST_SCORE;
                                        if (game->ghost[i].type == 1) {
                                            game->grid[r][c] = ' ';
                                        } else if (game->ghost[i].type == 2) {
                                            game->grid[r][c] = '.';
                                        } else if (game->ghost[i].type == 3) {
                                            game->grid[r][c] = 'o';
                                        }
                                        game->ghost[i].ghrow = -1;
                                        game->ghost[i].ghcolumn = -1;
                                    } else if (game->grid[r + 1][c] == '.') {
                                        game->ghost[i].type = 2;
                                        game->grid[r + 1][c] = 'X';
                                        game->ghost[i].ghrow++;
                                    } else if (game->grid[r + 1][c] == 'o') {
                                        game->ghost[i].type = 3;
                                        game->grid[r + 1][c] = 'X';
                                        game->ghost[i].ghrow++;
                                    } else if (game->grid[r + 1][c] == ' ') {
                                        game->ghost[i].type = 1;
                                        game->grid[r + 1][c] = 'X';
                                        game->ghost[i].ghrow++;
                                    }
                                }
                            }

                            else if (game->grid[r - 1][c] == '.') {
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                }
                                if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                }
                                if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                game->ghost[i].type = 2;
                                game->grid[r - 1][c] = 'X';
                                game->ghost[i].ghrow--;
                            } else if (game->grid[r - 1][c] == 'o') {
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                }
                                if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                }
                                if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                game->ghost[i].type = 3;
                                game->grid[r - 1][c] = 'X';
                                game->ghost[i].ghrow--;
                            } else if (game->grid[r - 1][c] == ' ') {
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                }
                                if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                }
                                if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                game->ghost[i].type = 1;
                                game->grid[r - 1][c] = 'X';
                                game->ghost[i].ghrow--;
                            } else if (game->grid[r - 1][c] == 'C') {
                                game->score = game->score + GHOST_SCORE;
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                } else if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                } else if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                game->ghost[i].ghrow = -1;
                                game->ghost[i].ghcolumn = -1;
                            }
                        }
                    }
                }
                // down
                else if (game->ghost[i].ddirection == down) {
                    //如果没有超能力
                    if (game->superpower == 0) {
                        //如果前方有阻碍 转向
                        if ((r + 1) == game->rows ||
                            game->grid[r + 1][c] == '#' ||
                            game->grid[r + 1][c] == '@' ||
                            game->grid[r + 1][c] == 'X') {
                            game->ghost[i].ddirection = up;
                            //如果前方仍有阻碍 继续转向
                            if ((r - 1) < 0 || game->grid[r - 1][c] == '#' ||
                                game->grid[r - 1][c] == '@' ||
                                game->grid[r - 1][c] == 'X') {
                                // game->ghost[i].ddirection = down;
                                game->grid[r][c] = '@';
                            } else {
                                //如果前方没有阻碍
                                //输出前一个位置的值
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                }
                                if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                }
                                if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                //输出后一个位置的值
                                if (game->grid[r - 1][c] == 'C') {
                                    game->ghost[i].ghrow--;

                                    game->grid[r - 1][c] = '@';
                                    game->state = losing;
                                } else if (game->grid[r - 1][c] == '.') {
                                    game->ghost[i].type = 2;
                                    game->grid[r - 1][c] = '@';
                                    game->ghost[i].ghrow--;
                                } else if (game->grid[r - 1][c] == 'o') {
                                    game->ghost[i].type = 3;
                                    game->grid[r - 1][c] = '@';
                                    game->ghost[i].ghrow--;
                                } else if (game->grid[r - 1][c] == ' ') {
                                    game->ghost[i].type = 1;
                                    game->grid[r - 1][c] = '@';
                                    game->ghost[i].ghrow--;
                                }
                            }

                        }
                        //如果前方没有阻碍并且出现food
                        else if (game->grid[r + 1][c] == '.') {
                            if (game->ghost[i].type == 1) {
                                game->grid[r][c] = ' ';
                            }
                            if (game->ghost[i].type == 2) {
                                game->grid[r][c] = '.';
                            }
                            if (game->ghost[i].type == 3) {
                                game->grid[r][c] = 'o';
                            }
                            game->ghost[i].type = 2;
                            game->grid[r + 1][c] = '@';
                            game->ghost[i].ghrow++;
                        }
                        //如果前方没有阻碍并且出现capsule
                        else if (game->grid[r + 1][c] == 'o') {
                            if (game->ghost[i].type == 1) {
                                game->grid[r][c] = ' ';
                            }
                            if (game->ghost[i].type == 2) {
                                game->grid[r][c] = '.';
                            }
                            if (game->ghost[i].type == 3) {
                                game->grid[r][c] = 'o';
                            }
                            game->ghost[i].type = 3;
                            game->grid[r + 1][c] = '@';
                            game->ghost[i].ghrow++;
                        }
                        //如果前方没有阻碍 且没有food或者capsule
                        else if (game->grid[r + 1][c] == ' ') {
                            if (game->ghost[i].type == 1) {
                                game->grid[r][c] = ' ';
                            }
                            if (game->ghost[i].type == 2) {
                                game->grid[r][c] = '.';
                            }
                            if (game->ghost[i].type == 3) {
                                game->grid[r][c] = 'o';
                            }
                            game->ghost[i].type = 1;
                            game->grid[r + 1][c] = '@';
                            game->ghost[i].ghrow++;
                        }
                        //如果前方没有阻碍 但是遇见了C
                        else if (game->grid[r + 1][c] == 'C') {
                            game->ghost[i].ghrow++;

                            if (game->ghost[i].type == 1) {
                                game->grid[r][c] = ' ';
                            }
                            if (game->ghost[i].type == 2) {
                                game->grid[r][c] = '.';
                            }
                            if (game->ghost[i].type == 3) {
                                game->grid[r][c] = 'o';
                            }
                            game->grid[r + 1][c] = '@';
                            game->state = losing;
                        }
                    }
                    //如果有超能力的话
                    else {
                        if (con == 2) {
                            game->grid[r][c] = 'X';
                        } else {
                            if ((r + 1) == game->rows ||
                                game->grid[r + 1][c] == '#' ||
                                game->grid[r + 1][c] == '@' ||
                                game->grid[r + 1][c] == 'X') {
                                game->ghost[i].ddirection = up;
                                //如果前方仍有阻碍 继续转向
                                if (r - 1 < 0 || game->grid[r - 1][c] == '#' ||
                                    game->grid[r - 1][c] == '@' ||
                                    game->grid[r - 1][c] == 'X') {
                                    // game->ghost[i].ddirection = up;
                                    game->grid[r][c] = 'X';
                                } else {
                                    //如果转向后前方没有阻碍
                                    //输出前一个位置的值
                                    if (game->ghost[i].type == 1) {
                                        game->grid[r][c] = ' ';
                                    }
                                    if (game->ghost[i].type == 2) {
                                        game->grid[r][c] = '.';
                                    }
                                    if (game->ghost[i].type == 3) {
                                        game->grid[r][c] = 'o';
                                    }
                                    //输出后一个位置的值
                                    if (game->grid[r - 1][c] == 'C') {
                                        game->score = game->score + GHOST_SCORE;
                                        if (game->ghost[i].type == 1) {
                                            game->grid[r][c] = ' ';
                                        } else if (game->ghost[i].type == 2) {
                                            game->grid[r][c] = '.';
                                        } else if (game->ghost[i].type == 3) {
                                            game->grid[r][c] = 'o';
                                        }
                                        game->ghost[i].ghrow = -1;
                                        game->ghost[i].ghcolumn = -1;
                                    } else if (game->grid[r - 1][c] == '.') {
                                        game->ghost[i].type = 2;
                                        game->grid[r - 1][c] = 'X';
                                        game->ghost[i].ghrow--;
                                    } else if (game->grid[r - 1][c] == 'o') {
                                        game->ghost[i].type = 3;
                                        game->grid[r - 1][c] = 'X';
                                        game->ghost[i].ghrow--;
                                    } else if (game->grid[r - 1][c] == ' ') {
                                        game->ghost[i].type = 1;
                                        game->grid[r - 1][c] = 'X';
                                        game->ghost[i].ghrow--;
                                    }
                                }
                            }

                            else if (game->grid[r + 1][c] == '.') {
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                }
                                if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                }
                                if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                game->ghost[i].type = 2;
                                game->grid[r + 1][c] = 'X';
                                game->ghost[i].ghrow++;
                            } else if (game->grid[r + 1][c] == 'o') {
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                }
                                if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                }
                                if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                game->ghost[i].type = 3;
                                game->grid[r + 1][c] = 'X';
                                game->ghost[i].ghrow++;
                            } else if (game->grid[r + 1][c] == ' ') {
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                }
                                if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                }
                                if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                game->ghost[i].type = 1;
                                game->grid[r + 1][c] = 'X';
                                game->ghost[i].ghrow++;
                            } else if (game->grid[r + 1][c] == 'C') {
                                game->score = game->score + GHOST_SCORE;
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                } else if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                } else if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                game->ghost[i].ghrow = -1;
                                game->ghost[i].ghcolumn = -1;
                            }
                        }
                    }
                }
                // left
                else if (game->ghost[i].ddirection == left) {
                    //如果没有超能力
                    if (game->superpower == 0) {
                        //如果前方有阻碍 转向
                        if ((c - 1) < 0 || game->grid[r][c - 1] == '#' ||
                            game->grid[r][c - 1] == '@' ||
                            game->grid[r][c - 1] == 'X') {
                            game->ghost[i].ddirection = right;
                            //如果前方仍有阻碍 继续转向
                            if (c + 1 == game->columns ||
                                game->grid[r][c + 1] == '#' ||
                                game->grid[r][c + 1] == '@' ||
                                game->grid[r][c + 1] == 'X') {
                                // game->ghost[i].ddirection = left;
                                game->grid[r][c] = '@';

                            } else {
                                //如果前方没有阻碍
                                //输出前一个位置的值
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                }
                                if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                }
                                if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                //输出后一个位置的值
                                if (game->grid[r][c + 1] == 'C') {
                                    game->ghost[i].ghcolumn++;
                                    game->grid[r][c + 1] = '@';
                                    game->state = losing;
                                } else if (game->grid[r][c + 1] == '.') {
                                    game->ghost[i].type = 2;
                                    game->grid[r][c + 1] = '@';
                                    game->ghost[i].ghcolumn++;
                                } else if (game->grid[r][c + 1] == 'o') {
                                    game->ghost[i].type = 3;
                                    game->grid[r][c + 1] = '@';
                                    game->ghost[i].ghcolumn++;
                                } else if (game->grid[r][c + 1] == ' ') {
                                    game->ghost[i].type = 1;
                                    game->grid[r][c + 1] = '@';
                                    game->ghost[i].ghcolumn++;
                                }
                            }

                        }
                        //如果前方没有阻碍并且出现food
                        else if (game->grid[r][c - 1] == '.') {
                            if (game->ghost[i].type == 1) {
                                game->grid[r][c] = ' ';
                            }
                            if (game->ghost[i].type == 2) {
                                game->grid[r][c] = '.';
                            }
                            if (game->ghost[i].type == 3) {
                                game->grid[r][c] = 'o';
                            }
                            game->ghost[i].type = 2;
                            game->grid[r][c - 1] = '@';
                            game->ghost[i].ghcolumn--;
                        }
                        //如果前方没有阻碍并且出现capsule
                        else if (game->grid[r][c - 1] == 'o') {
                            if (game->ghost[i].type == 1) {
                                game->grid[r][c] = ' ';
                            }
                            if (game->ghost[i].type == 2) {
                                game->grid[r][c] = '.';
                            }
                            if (game->ghost[i].type == 3) {
                                game->grid[r][c] = 'o';
                            }
                            game->ghost[i].type = 3;
                            game->grid[r][c - 1] = '@';
                            game->ghost[i].ghcolumn--;
                        }
                        //如果前方没有阻碍 且没有food或者capsule
                        else if (game->grid[r][c - 1] == ' ') {
                            if (game->ghost[i].type == 1) {
                                game->grid[r][c] = ' ';
                            }
                            if (game->ghost[i].type == 2) {
                                game->grid[r][c] = '.';
                            }
                            if (game->ghost[i].type == 3) {
                                game->grid[r][c] = 'o';
                            }
                            game->ghost[i].type = 1;
                            game->grid[r][c - 1] = '@';
                            game->ghost[i].ghcolumn--;
                        }
                        //如果前方没有阻碍 但是遇见了C
                        else if (game->grid[r][c - 1] == 'C') {
                            game->ghost[i].ghcolumn--;
                            if (game->ghost[i].type == 1) {
                                game->grid[r][c] = ' ';
                            }
                            if (game->ghost[i].type == 2) {
                                game->grid[r][c] = '.';
                            }
                            if (game->ghost[i].type == 3) {
                                game->grid[r][c] = 'o';
                            }
                            game->grid[r][c - 1] = '@';
                            game->state = losing;
                        }
                    }
                    //如果有超能力的话
                    else {
                        if (con == 2) {
                            game->grid[r][c] = 'X';
                        } else {
                            if ((c - 1) < 0 || game->grid[r][c - 1] == '#' ||
                                game->grid[r][c - 1] == '@' ||
                                game->grid[r][c - 1] == 'X') {
                                game->ghost[i].ddirection = right;
                                //如果前方仍有阻碍 继续转向
                                if (c + 1 == game->columns ||
                                    game->grid[r][c + 1] == '#' ||
                                    game->grid[r][c + 1] == '@' ||
                                    game->grid[r][c + 1] == 'X') {
                                    // game->ghost[i].ddirection = up;
                                    game->grid[r][c] = 'X';
                                } else {
                                    //如果转向后前方没有阻碍
                                    //输出前一个位置的值
                                    if (game->ghost[i].type == 1) {
                                        game->grid[r][c] = ' ';
                                    }
                                    if (game->ghost[i].type == 2) {
                                        game->grid[r][c] = '.';
                                    }
                                    if (game->ghost[i].type == 3) {
                                        game->grid[r][c] = 'o';
                                    }
                                    //输出后一个位置的值
                                    if (game->grid[r][c + 1] == 'C') {
                                        game->score = game->score + GHOST_SCORE;
                                        if (game->ghost[i].type == 1) {
                                            game->grid[r][c] = ' ';
                                        } else if (game->ghost[i].type == 2) {
                                            game->grid[r][c] = '.';
                                        } else if (game->ghost[i].type == 3) {
                                            game->grid[r][c] = 'o';
                                        }
                                        game->ghost[i].ghrow = -1;
                                        game->ghost[i].ghcolumn = -1;
                                    } else if (game->grid[r][c + 1] == '.') {
                                        game->ghost[i].type = 2;
                                        game->grid[r][c + 1] = 'X';
                                        game->ghost[i].ghcolumn++;
                                    } else if (game->grid[r][c + 1] == 'o') {
                                        game->ghost[i].type = 3;
                                        game->grid[r][c + 1] = 'X';
                                        game->ghost[i].ghcolumn++;
                                    } else if (game->grid[r][c + 1] == ' ') {
                                        game->ghost[i].type = 1;
                                        game->grid[r][c + 1] = 'X';
                                        game->ghost[i].ghcolumn++;
                                    }
                                }
                            }

                            else if (game->grid[r][c - 1] == '.') {
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                }
                                if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                }
                                if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                game->ghost[i].type = 2;
                                game->grid[r][c - 1] = 'X';
                                game->ghost[i].ghcolumn--;
                            } else if (game->grid[r][c - 1] == 'o') {
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                }
                                if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                }
                                if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                game->ghost[i].type = 3;
                                game->grid[r][c - 1] = 'X';
                                game->ghost[i].ghcolumn--;
                            } else if (game->grid[r][c - 1] == ' ') {
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                }
                                if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                }
                                if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                game->ghost[i].type = 1;
                                game->grid[r][c - 1] = 'X';
                                game->ghost[i].ghcolumn--;
                            } else if (game->grid[r][c - 1] == 'C') {
                                game->score = game->score + GHOST_SCORE;
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                } else if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                } else if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                game->ghost[i].ghrow = -1;
                                game->ghost[i].ghcolumn = -1;
                            }
                        }
                    }

                }
                // right
                else if (game->ghost[i].ddirection == right) {
                    //如果没有超能力
                    if (game->superpower == 0) {
                        //如果前方有阻碍 转向
                        if ((c + 1) == game->columns ||
                            game->grid[r][c + 1] == '#' ||
                            game->grid[r][c + 1] == '@' ||
                            game->grid[r][c + 1] == 'X') {
                            game->ghost[i].ddirection = left;
                            //如果前方仍有阻碍 继续转向
                            if (c - 1 < 0 || game->grid[r][c - 1] == '#' ||
                                game->grid[r][c - 1] == '@' ||
                                game->grid[r][c - 1] == 'X') {
                                // game->ghost[i].ddirection = right;
                                game->grid[r][c] = '@';
                            }

                            else {
                                //如果前方没有阻碍
                                //输出前一个位置的值
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                }
                                if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                }
                                if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                //输出后一个位置的值
                                if (game->grid[r][c - 1] == 'C') {
                                    game->ghost[i].ghcolumn--;
                                    game->grid[r][c - 1] = '@';
                                    game->state = losing;
                                } else if (game->grid[r][c - 1] == '.') {
                                    game->ghost[i].type = 2;
                                    game->grid[r][c - 1] = '@';
                                    game->ghost[i].ghcolumn--;
                                } else if (game->grid[r][c - 1] == 'o') {
                                    game->ghost[i].type = 3;
                                    game->grid[r][c - 1] = '@';
                                    game->ghost[i].ghcolumn--;
                                } else if (game->grid[r][c - 1] == ' ') {
                                    game->ghost[i].type = 1;
                                    game->grid[r][c - 1] = '@';
                                    game->ghost[i].ghcolumn--;
                                }
                            }

                        }
                        //如果前方没有阻碍并且出现food
                        else if (game->grid[r][c + 1] == '.') {
                            if (game->ghost[i].type == 1) {
                                game->grid[r][c] = ' ';
                            }
                            if (game->ghost[i].type == 2) {
                                game->grid[r][c] = '.';
                            }
                            if (game->ghost[i].type == 3) {
                                game->grid[r][c] = 'o';
                            }
                            game->ghost[i].type = 2;
                            game->grid[r][c + 1] = '@';
                            game->ghost[i].ghcolumn++;
                        }
                        //如果前方没有阻碍并且出现capsule
                        else if (game->grid[r][c + 1] == 'o') {
                            if (game->ghost[i].type == 1) {
                                game->grid[r][c] = ' ';
                            }
                            if (game->ghost[i].type == 2) {
                                game->grid[r][c] = '.';
                            }
                            if (game->ghost[i].type == 3) {
                                game->grid[r][c] = 'o';
                            }
                            game->ghost[i].type = 3;
                            game->grid[r][c + 1] = '@';
                            game->ghost[i].ghcolumn++;
                        }
                        //如果前方没有阻碍 且没有food或者capsule
                        else if (game->grid[r][c + 1] == ' ') {
                            if (game->ghost[i].type == 1) {
                                game->grid[r][c] = ' ';
                            }
                            if (game->ghost[i].type == 2) {
                                game->grid[r][c] = '.';
                            }
                            if (game->ghost[i].type == 3) {
                                game->grid[r][c] = 'o';
                            }
                            game->ghost[i].type = 1;
                            game->grid[r][c + 1] = '@';
                            game->ghost[i].ghcolumn++;
                        }
                        //如果前方没有阻碍 但是遇见了C
                        else if (game->grid[r][c + 1] == 'C') {
                            game->ghost[i].ghcolumn++;
                            if (game->ghost[i].type == 1) {
                                game->grid[r][c] = ' ';
                            }
                            if (game->ghost[i].type == 2) {
                                game->grid[r][c] = '.';
                            }
                            if (game->ghost[i].type == 3) {
                                game->grid[r][c] = 'o';
                            }
                            game->grid[r][c + 1] = '@';
                            game->state = losing;
                        }
                    }
                    //如果有超能力的话
                    else {
                        if (con == 2) {
                            game->grid[r][c] = 'X';
                        } else {
                            if ((c + 1) == game->columns ||
                                game->grid[r][c + 1] == '#' ||
                                game->grid[r][c + 1] == '@' ||
                                game->grid[r][c + 1] == 'X') {
                                game->ghost[i].ddirection = left;
                                //如果前方仍有阻碍 继续转向
                                if (c - 1 < 0 || game->grid[r][c - 1] == '#' ||
                                    game->grid[r][c - 1] == '@' ||
                                    game->grid[r][c - 1] == 'X') {
                                    // game->ghost[i].ddirection = up;
                                    game->grid[r][c] = 'X';
                                } else {
                                    //如果转向后前方没有阻碍
                                    //输出前一个位置的值
                                    if (game->ghost[i].type == 1) {
                                        game->grid[r][c] = ' ';
                                    }
                                    if (game->ghost[i].type == 2) {
                                        game->grid[r][c] = '.';
                                    }
                                    if (game->ghost[i].type == 3) {
                                        game->grid[r][c] = 'o';
                                    }
                                    //输出后一个位置的值
                                    if (game->grid[r][c - 1] == 'C') {
                                        game->score = game->score + GHOST_SCORE;
                                        if (game->ghost[i].type == 1) {
                                            game->grid[r][c] = ' ';
                                        } else if (game->ghost[i].type == 2) {
                                            game->grid[r][c] = '.';
                                        } else if (game->ghost[i].type == 3) {
                                            game->grid[r][c] = 'o';
                                        }
                                        game->ghost[i].ghrow = -1;
                                        game->ghost[i].ghcolumn = -1;
                                    } else if (game->grid[r][c - 1] == '.') {
                                        game->ghost[i].type = 2;
                                        game->grid[r][c - 1] = 'X';
                                        game->ghost[i].ghcolumn--;
                                    } else if (game->grid[r][c - 1] == 'o') {
                                        game->ghost[i].type = 3;
                                        game->grid[r][c - 1] = '@';
                                        game->ghost[i].ghcolumn--;
                                    } else if (game->grid[r][c - 1] == ' ') {
                                        game->ghost[i].type = 1;
                                        game->grid[r][c - 1] = 'X';
                                        game->ghost[i].ghcolumn--;
                                    }
                                }
                            }

                            else if (game->grid[r][c + 1] == '.') {
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                }
                                if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                }
                                if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                game->ghost[i].type = 2;
                                game->grid[r][c + 1] = 'X';
                                game->ghost[i].ghcolumn++;
                            } else if (game->grid[r][c + 1] == 'o') {
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                }
                                if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                }
                                if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                game->ghost[i].type = 3;
                                game->grid[r][c + 1] = 'X';
                                game->ghost[i].ghcolumn++;
                            } else if (game->grid[r][c + 1] == ' ') {
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                }
                                if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                }
                                if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                game->ghost[i].type = 1;
                                game->grid[r][c + 1] = 'X';
                                game->ghost[i].ghcolumn++;
                            } else if (game->grid[r][c + 1] == 'C') {
                                game->score = game->score + GHOST_SCORE;
                                if (game->ghost[i].type == 1) {
                                    game->grid[r][c] = ' ';
                                } else if (game->ghost[i].type == 2) {
                                    game->grid[r][c] = '.';
                                } else if (game->ghost[i].type == 3) {
                                    game->grid[r][c] = 'o';
                                }
                                game->ghost[i].ghrow = -1;
                                game->ghost[i].ghcolumn = -1;
                            }
                        }
                    }
                }
            }
        }
    }
    return; // This is incorrect.
}

/* Adds a capsule to the game, at row = r, column = c.
 * Capsules should be added only to an empty place,
 * or added to a cell with food to upgrade that food.
 * After you successfully add a capsule, return true.
 * Return false on failures.
 *
 * Capsules will grant Pacman CAPSULE_SCORE = 50 and
 * CAPSULE_DURATION = 10 turns of superpower when eaten.
 *
 * When with superpower, scared ghosts will be displayed in 'X',
 * and will be unable to move every other turn.
 * (they can move on the turn when Pacman eats a capsule.)
 * When Pacman eats a scared ghost, it earns GHOST_SCORE = 200.
 *
 * Pacman's superpower activates immediately when it eats a
 * capsule, and counts down immediately after Pacman's turn,
 * starting from its next turn. In other words, Pacman's superpower
 * ends after 10 turns at the same moment of eating a capsule.
 */
bool AddCapsule(Game *game, int r, int c) {
    // Adds a food to the game, at row = r, column = c.
    if (r >= 0 && r < game->rows) {
        if (c >= 0 && c < game->columns) {
            // Foods should be added only to an empty place.
            if (game->grid[r][c] == ' ') {
                // you should modify the "grid" to represent the food by '.',
                // also update the "foodCount"
                game->grid[r][c] = 'o';
                game->foodCount++;
                // Then, return true.
                return true;
            } else if (game->grid[r][c] == '.') {
                game->grid[r][c] = 'o';
                return true;
            }
        }
    }
    return false; // This is not always correct.
}

///////////////////////////////////////////////////////////////////////////////
/////////////////// Please submit all code above to OJ ////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Declarations of provided functions
void PlayGame(Game *game);

int main() {
    Game *game = NewGame(20, 20);
    // test pacman
    for (int i = 0; i < 3; i++) {
        AddFood(game, i, 0);
        AddFood(game, i, 2);
    }
    AddPacman(game, 3, 0);
    AddFood(game, 3, 2);
    AddFood(game, 0, 1);
    AddFood(game, 3, 1);
    AddWall(game, 1, 1);
    AddWall(game, 2, 1);
    AddGhost(game, 0, 0, up);
    AddCapsule(game, 3, 2);
    AddCapsule(game, 2, 2);
    // test ghost
    for (int i = 0; i < 3; i++) {
        AddFood(game, i, 0);
        AddFood(game, i, 2);
    }
    AddPacman(game, 3, 0);
    AddFood(game, 3, 2);
    AddFood(game, 0, 1);
    AddFood(game, 3, 1);
    AddWall(game, 1, 1);
    AddWall(game, 2, 1);
    AddGhost(game, 0, 2, up);
    AddGhost(game, 0, 2, down);
    AddCapsule(game, 1, 0);
    AddCapsule(game, 2, 0);
    // large test
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 19; j++) {
            if (i == 7 && j == 7)
                AddPacman(game, i, j);
            else if (i & 1 || j == 3 || j == 5) {
                AddFood(game, i, j);
                // if (i==j&&i!=1) AddGhost(game,i,j,down);
            } else
                AddWall(game, i, j);
        }
    }
    AddGhost(game, 2, 3, up);
    AddGhost(game, 3, 2, left);
    AddGhost(game, 3, 1, left);
    AddGhost(game, 3, 0, left);
    AddGhost(game, 5, 0, right);
    AddGhost(game, 19, 19, down);
    AddGhost(game, 11, 0, right);
    AddCapsule(game, 5, 5);
    AddCapsule(game, 6, 5);
    AddCapsule(game, 7, 5);

    PlayGame(game);
}

///////////////////////////////////////////////////////////////////////////////
//////////////////////// DO NOT MODIFY ANY CODE BELOW! ////////////////////////
///////////////////////////////////////////////////////////////////////////////

/* This function prints the game grid with boundary,
 * and any message depending on game state.
 * Please make sure that, after any action is done,
 * game->state, game->score, and game->foodCount
 * are correctly updated.
 */
#ifdef _WIN32

#include <windows.h>

#endif // _WIN32

void PrintGame(Game *game) {
    if (!game || !game->grid)
        return;

// If Windows, clear console.
#ifdef _WIN32
    system("cls");
#else
    // Clears terminal screen
    fflush(stdout);
    system("clear");
#endif

    // The game grid
    printf("the superpower duration is %d\n", game->superpower);

    printf("the condition is %d\n", game->condition);

    printf(" ");
    for (int i = 0; i < game->columns; i++)
        printf("_");
    printf(" \n");

    for (int i = 0; i < game->rows; i++) {
        printf("|");
        for (int j = 0; j < game->columns; j++)
            printf("%c", game->grid[i][j]);
        printf("|\n");
    }
    printf("\\");
    for (int i = 0; i < game->columns; i++)
        printf("-");
    printf("/\n\n");

    // Messages on different states
    switch (game->state) {
    case losing:
        printf("Pacman died! Your score: %d\n", game->score);
        break;
    case onGoing:
        printf("Score: %d\n", game->score);
        printf("There are %d foods remaining!\n", game->foodCount);
        printf(
            "Pacman wants food! (control by w/a/s/d/i, confirm by Enter)\n\n");
        break;
    case winning:
        printf("Victory! Your score: %d\n", game->score);
        break;
    default:
        printf("ERROR: invalid game state!\n");
    }
}

/* Plays the game, and calls EndGame on winning or losing.
 */
void PlayGame(Game *game) {

    // Prints the initial grid
    PrintGame(game);

    // Main loop of game. Terminates when the game ends.
    while (true) {
        // We only care about one charater, but should eat the whole line of
        // input.
        char input[127];
        input[0] = '\0';
        scanf("%[^\n]", input);
        // Handles the '\n'.
        getchar();
        Direction direction;
        switch (input[0]) {
        case 'w':
        case 'W':
            direction = up;
            break;
        case 'a':
        case 'A':
            direction = left;
            break;
        case 's':
        case 'S':
            direction = down;
            break;
        case 'd':
        case 'D':
            direction = right;
            break;
        case 'i':
        case 'I':
            direction = idle;
            break;
        default:
            PrintGame(game);
            continue;
        }

        // Pacman moves first.
        MovePacman(game, direction);

        // Wins if Pacman eats all the food, loses if Pacman bumps into a ghost.
        if (game->state != onGoing) {
            break;
        }

        // If you haven't implemented ghosts, this does nothing.
        MoveGhosts(game);

        // Loses if a ghost kills Pacman.
        if (game->state != onGoing) {
            break;
        }
        PrintGame(game);
    }

    // End game message
    PrintGame(game);
    printf("Press Enter to exit.\n");
    EndGame(game);
    getchar();
}
