#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int score;
    char name[20];

} Player;

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
        printf("CARACTER:%c!!!", str[0]);

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

        // printf("ID: %d, Name: %s, Score: %d\n", id, name, score);
    }

    return queue;
}

void sort_ranking(Queue* queue) {
    insertion_sort(queue->array, queue->size);
}

void register_ranking(FILE* file, Queue queue) {
    fprintf(file, "ID\tNOME\tPONTUAÇÃO\n\n");

    for (int i = queue.size - 1; i >= 0; i--) {
        fprintf(file, "%d\t%s\t%d\n", i + 1, queue.array[i].name, queue.array[i].score);
    }
}

int main() {
    FILE* ptr;
    char str[50];
    ptr = fopen("../teste.txt", "w");

    if (ptr == NULL) {
        printf("File cannot be opened.\n");
        return 1;
    }

    Queue queue = read_txt(ptr, str, 50);
    sort_ranking(&queue);

    fclose(ptr);

    FILE* output_ptr;
    output_ptr = fopen("../ranking.txt", "w");

    if (output_ptr == NULL) {
        printf("Output file cannot be opened.\n");
        return 1;
    }

    register_ranking(output_ptr, queue);

    fclose(output_ptr);

    return 0;
}