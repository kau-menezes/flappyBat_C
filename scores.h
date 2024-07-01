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

Queue construct_fila() {
    Queue newqueue;
    newqueue.array = NULL;
    newqueue.capacity = 0;
    newqueue.size = 0;
    return newqueue;
}

void enqueue(Queue * queue, Player player) {
    if (queue->array == NULL) {
        queue->capacity = 1;
        queue->array = (Player*) malloc(queue->capacity * sizeof(Player));
        queue->array[queue->size++] = player;
        return;
    }

    if (queue->capacity == queue->size + 1) {
        queue->capacity *= 2;
        queue->array = (Player*) realloc(queue->array, queue->capacity * sizeof(Player));
    }

    queue->array[queue->size++] = player;
}

void insertion_sort(Player* array, int size)
{

    int smallest_index = NULL;
    Player aux;

    for (int i = 1; i < size; i ++)
    {
        smallest_index = i;

        for (int j = i; j > 0; j--)
        {
            if (array[j - 1].score > array[j].score)
            {
                aux = array[j - 1]; 
                array[j - 1] = array[j];
                array[j] = aux;
            }
        }
    }

}

Queue read_txt(FILE * ptr, char str[], int size)
{
    Queue queue = construct_fila();

    if (ptr == NULL) {
        printf("File cannot be opened.\n");

    }

    // Skip header line
    fgets(str, size, ptr); // Assuming header is not needed

    while (fgets(str, size, ptr) != NULL) {

        if (str[0] == 'I' || str[0] == '\n') {
            printf("Skipping line.\n");
            continue;
        }

        // Extracting ID, NAME, and PONTUAÇÃO
        int id, score;
        char name[15];

        sscanf(str, "%d\t%s\t%d", &id, name, &score);

        Player new_player;
        strcpy(new_player.name, name);
        new_player.score = score;

        enqueue(&queue, new_player);

        printf("ID: %d, Name: %s, Score: %d\n", id, name, score);
    }

    return queue;
}

void sort_ranking(Queue* queue) {
    insertion_sort(queue->array, queue->size);
}

void register_ranking(FILE* file, Queue queue) {
    fprintf(file, "ID\tNOME\tPONTUAÇÃO\n\n");
    printf("\nooi");
    for (int i = queue.size - 1; i >= 0; i--) {
        fprintf(file, "%d\t%s\t%d\n", queue.size - i, queue.array[i].name, queue.array[i].score);
        printf("\nooi");

    }
}

#endif