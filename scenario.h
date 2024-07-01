#ifndef SCENARIO_H
#define SCENARIO_H

#define WIDTH 40
#define HEIGHT 20
#define SPACE_BTW 6

#include <stdio.h>

#include "structs.h"

void pipe_movement(int matrix[][WIDTH], Pipe *pipe)
{
    // check if the pipe has reached the end of the map
    if (pipe->x == 0)
    {
        for (int i = (HEIGHT - pipe->y); i < (HEIGHT); i++)
        {
            matrix[i][pipe->x] = 0;
            matrix[i][pipe->x + 1] = 0;
            matrix[i][pipe->x + 2] = 0;
        }

        for (int i = (HEIGHT - pipe->y) - SPACE_BTW; i > 0; i--)
        {
            matrix[i][pipe->x] = 0;
            matrix[i][pipe->x + 1] = 0;
            matrix[i][pipe->x + 2] = 0;
        }
        return;
    }
    for (int i = (HEIGHT - pipe->y); i < (HEIGHT); i++)
    {
        matrix[i][pipe->x - 1] = 1;
        matrix[i][pipe->x + 2] = 0;
    }

    for (int i = (HEIGHT - pipe->y) - SPACE_BTW; i >= 0; i--)
    {
        matrix[i][pipe->x - 1] = 1;
        matrix[i][pipe->x + 2] = 0;
    }
    
}

void rendering(int matrix[][WIDTH])
{

   
    for (int i = 0; i < WIDTH; i++)
    {
        printf(" []");
    }
    printf("\n");

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (matrix[i][j] == 0)
            {
                printf("   ");
            } else if (matrix[i][j] == 9){
                printf("\x1b[35m^~^");

            } else if (matrix[i][j] == 2)
            {
                printf(" 2");
            
            } else {
                printf(" \x1b[32m[]");

            }
            // printf("%d ", matrix[i][j]);
        }
        
    printf("\n");     
    }
}

void restart_pipe(int matrix[][WIDTH], Pipe *pipe)
{
    for (int i = (HEIGHT - pipe->y); i < (HEIGHT); i++)
            {
                matrix[i][pipe->x] = 0;
                matrix[i][pipe->x + 1] = 0;
                matrix[i][pipe->x + 2] = 0;
            }

            for (int i = (HEIGHT - pipe->y) - SPACE_BTW; i >= 0; i--)
            {
                matrix[i][pipe->x] = 0;
                matrix[i][pipe->x + 1] = 0;
                matrix[i][pipe->x + 2] = 0;
            }

            pipe->x = WIDTH - 4;
}

#endif // !SCENARIO