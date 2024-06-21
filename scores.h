#ifndef SCORES
#define SCORES

#include <stdio.h>
#include <stdlib.h>

#include "structs.h"

typedef struct {
    int capacity;
    int size;
    Player * array;


} Queue;

Queue  construct_fila(){
    Queue newqueue;
    newqueue.array = NULL;
    newqueue.capacity = 0;
    newqueue.size = 0;

    return newqueue;
}

void enqueue(Queue * queue,Player player){

    if(queue->array == NULL){

        queue->capacity = 1;

        queue->array = (Player*) malloc(queue->capacity * sizeof(Player));

        queue->array[queue->size++] = player;

        return;

    }

    if(queue->capacity == queue->size + 1){
        
        queue->capacity *= 2;

        realloc ( queue->array, queue->capacity * sizeof(Queue));

    }
    
    queue->array[queue->size++] = player;
}

// ordena a lista
void sort_highscore(Queue *queue)
{
    // MERGE SORT
}

// printa no .txt
void display_scores(FILE * file, Queue *queue)
{
    for (int i = 0; i < queue->size; i++)
    {
        fprintf(file, "=== %d - %s ===\n\n   SCORE: %d\n\n", i, queue->array[i].name, queue->array[i].score);
    }
}

#endif // !SCORES