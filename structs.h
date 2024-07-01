#ifndef STRUCTS_H
#define STRUCTS_H

#define BAT_X 9
#define BAT_Y 14
#define BAT_VALUE 9

#include <string.h>
#include <stdio.h>

typedef struct {
    int x;
    int y;

} Pipe;

typedef struct Player{
    char name[15];
    int score;

} Player;

typedef struct {
    int x, y;
    int pulando;
} boneco_t;

Pipe construct_pipe()
{
    Pipe newpipe;
    newpipe.x = 0;
    newpipe.y = 0;

    return newpipe;
}

Player construct_player(char name[6])
{
    Player newplayer;

    strcpy(newplayer.name, name);
    newplayer.score = 0;

    return newplayer;
}

#endif // !STRCTS