// gcc  -I. -o joguinho main.c -lpthread
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>

#include "graphics.h"
#include "scores.h"
#include "scenario.h"
#include "structs.h"
#include "terminal.c"

// ========================================== TRANSFERIR BONECO_t STRUCT PARA STRUCTS
// #define ALTURA_TERMINAL 20
// #define LARGURA_TERMINAL 40
#define BONECO "^~^"
int map[HEIGHT][WIDTH];

pthread_mutex_t mutex;
pthread_cond_t cond_var;
int should_scenario = 0;
int pontos = 0;

// ========================================== TRANSFERIR BONECO_t STRUCT PARA STRUCTS

bool jogo_ativo = true;
bool colisao_detectada = false;

void* morceguinho(void *arg) {
    boneco->x = 5;
    boneco->y = HEIGHT / HEIGHT;
    boneco->pulando = 0;

    int pulo_atual = 0;

    while (jogo_ativo) {
        pthread_mutex_lock(&mutex);
        while (should_cenario == 1) {
            pthread_cond_wait(&cond_var, &mutex);
        }

        if (_kbhit()) {
            char tecla = _getch();
            
            if (tecla == ' ') {
                if (!boneco->pulando) {
                    boneco->pulando = 1;
                    pulo_atual = 0;
                }
            }
        }

        if (boneco->pulando) {
            if (pulo_atual < 4) {
                boneco->y--;
                pulo_atual++;
            } else {
                boneco->pulando = 0;
            }
            // Sleep(50);
        } else {
            boneco->y++;
        }

        if (boneco->y >= HEIGHT - 1) {
            boneco->y = HEIGHT - 1;
        }

        // Verifica colisão com algum obstáculo
        if (map[boneco->y][boneco->x] != 0 && map[boneco->y][boneco->x] != 2 && map[boneco->y][boneco->x] != 9) {
            colisao_detectada = true;  // Marca a colisão detectada
            jogo_ativo = false;  // Marca o jogo como não ativo
            // Sem break aqui para permitir o término do loop principal no main
        }

        Sleep(25);
        should_cenario = 1;
        pthread_cond_signal(&cond_var);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *scenario_movement(void *arg) 
{
    printf("Thread scenario_movement started.\n");
    pthread_mutex_lock(&mutex);
    while (!should_cenario) {
        pthread_cond_wait(&cond_var, &mutex);
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            map[i][j] = 0;
        }
    }

    // inicializa três obstáclos
    Pipe first_pipe = construct_pipe();
    Pipe second_pipe = construct_pipe();
    Pipe third_pipe = construct_pipe();

    // define a altura inicial dos obstáculos
    int y1  = 5;
    int y2  = 13;
    int y3  = 9;

    first_pipe.x = WIDTH - 4;
    first_pipe.y = y1;
    second_pipe.x = -1;
    third_pipe.x = -1;

    for (int i = (HEIGHT - y1); i < (HEIGHT); i++) {
        map[i][first_pipe.x] = 1;
        map[i][first_pipe.x + 1] = 1;
        map[i][first_pipe.x + 2] = 1;
    }

    for (int i = (HEIGHT - y1) - SPACE_BTW; i >= 0; i--) {
        map[i][first_pipe.x] = 1;
        map[i][first_pipe.x + 1] = 1;
        map[i][first_pipe.x + 2] = 1;
    }

    while (1) {
        Sleep(150);
        ERASE_ALL();
        MOVE_HOME();

        if (first_pipe.x == 0) {
            restart_pipe(map, &first_pipe);
        }

        if (second_pipe.x == 0) {
            restart_pipe(map, &second_pipe);
        }

        if (third_pipe.x == 0) {
            restart_pipe(map, &third_pipe);
        }

        if (first_pipe.x == WIDTH - 16) {
            second_pipe.x = WIDTH - 4;
            second_pipe.y = y2;
        }

        if (second_pipe.x == WIDTH - 16) {
            third_pipe.x = WIDTH - 4;
            third_pipe.y = y3;
        }

        if (first_pipe.x > 0) {
            pipe_movement(map, &first_pipe);
            first_pipe.x--; 
        }

        if (second_pipe.x > 0) {
            pipe_movement(map, &second_pipe);
            second_pipe.x--;
        }

        if (third_pipe.x > 0) {
            pipe_movement(map, &third_pipe);
            third_pipe.x--;
        }

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (map[i][j] == 9) map[i][j] = 0;
            }
        }

        // Colisão
        if (map[boneco->y][boneco->x] != 0 && map[boneco->y][boneco->x] != 2 && map[boneco->y][boneco->x] != 9) {
            jogo_ativo = false;  // Marca o jogo como não ativo
            ERASE_ALL();
            printf("Voce perdeu!");
            break;  // Sai do loop principal
        }

        // Coloca as coordenadas do boneco na matriz
        map[boneco->y][boneco->x] = 9;

        // Contagem de pontos caso o boneco passe do cano
        if (boneco->x == first_pipe.x || boneco->x == second_pipe.x || boneco->x == third_pipe.x) {
            pontos++;
        }

        // printf("%d - %d - %d | Pontos: %d\n", first_pipe.x, second_pipe.x, third_pipe.x, pontos);

        rendering(map);
        fflush(stdout);

        should_cenario = 0;
        pthread_cond_signal(&cond_var);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main() 
{
    Player player;
    int escolha;

    FILE *ranking, *history;
    ranking = fopen("ranking.txt", "w");
    history = fopen("history.txt", "a");

    char str[50];

    print_flappy_bat();
    printf("\n\nDigite seu nome para iniciar: ");
    scanf("%s", player.name);
    system("cls");

    print_menu();
    scanf(" %d", &escolha);
    system("cls");

    switch(escolha) 
    
    {
    case 1:
        // printf("================================Tutorial===============================\n- Aperte SPACE para pular\n- Cuidado com EASTER EGGS (nao sao bugs) no meio do jogo\n- As vezes o jogo trava, mas e so fechar e abrir que ele funciona :)");
        // Sleep(10000);
        configureTerminal();
        Queue queue = construct_fila();
        pthread_t threadA, threadB;

        MOVE_HOME();

        boneco = (boneco_t*)malloc(sizeof(boneco_t));

        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&cond_var, NULL);

        pthread_create(&threadA, NULL, scenario_movement, NULL);
        pthread_create(&threadB, NULL, morceguinho, NULL);

        // Espera pela finalização das threads
        pthread_join(threadB, NULL);
        pthread_join(threadA, NULL);

        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond_var);
        break;
    
    case 2:
        if (history == NULL) {
        printf("File cannot be opened.\n");
        return 1;
        }

        Queue queue = read_txt(history, str, 50);
        sort_ranking(&queue);

        fclose(history);

        if (ranking == NULL) {
            printf("Output file cannot be opened.\n");
            return 1;
        }

        register_ranking(ranking, queue);
        break;
    
    case 3:
        printf("======================================================CREDITOS=========================================================\n\nQueridos amigos e apoiadores,\nQueremos agradecer imensamente por todo o apoio e encorajamento ao longo da criacao do nosso jogo 'Flappy Bat'.\nSabrina, Karlla e Kauane enfrentamos uma jornada repleta de desafios e aprendizados.\nDesde o inicio, sabiamos que queriamos criar algo especial inspirado no classico Flappy Bird.\n\nDurante o processo, enfrentamos muitos obstaculos, desde bugs complexos ate momentos de duvida e medo de nao\nconseguirmos concluir o projeto.\nMas, com trabalho arduo, colaboracao e o apoio mutuo, conseguimos superar cada dificuldade.\n\nO 'Flappy Bat' nao seria possivel sem a nossa determinacao conjunta e sem o suporte de todos voces.\nE com grande alegria que compartilhamos o resultado final deste projeto, que representa nao apenas\num jogo, mas tambem nosso comprometimento e paixao por criar algo unico e divertido.\n\nAgradecemos do fundo do coracao por fazerem parte dessa jornada conosco.\nEsperamos que voces se divirtam tanto jogando quanto nos tivemos criando.\n\nCom carinho,\nSabrina, Karlla e Kauane\n\nPs: Tambem gostariamos de agradecer muito ao nosso querido professor Nicolas que esteve ao nosso lado em todos os\nmomentos, sem seus ensinamentos nos nao teriamos chegado ate onde estamos!");
        Sleep(1000000);
        break;

    case 0:
        printf("Tchau, %s!", player.name);
        Sleep(3000);
        break;

    default:
        printf("Insira um número valido!");
        break;
    }

    player.score = pontos;

    // Se uma colisão foi detectada, ainda assim, permite salvar no arquivo de ranking
    if (colisao_detectada) 
    {
        printf("Você perdeu!");
        // Exibe a pontuação final
        fprintf(history, "%s\t%d", player.name, player.score);
        

    }

    fclose(ranking);
    fclose(history);
    free(boneco);

    return 0;
}
















































































































//        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//       < :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: >
//       < ::::::::::::            ::::::::::::            :::::::::::: >
//       < :::::::::                   ::::                   ::::::::: >
//       < :::::::         ********     ::     ********         ::::::: >
//       < :::::      ****          ****  ****         ****       ::::: >
//       < ::::     ****               ****               ****     :::: >
//       < :::     ****                 **                 ****     ::: >
//       < :::     ****                                    ****     ::: >
//       < ::::     ****                EU                *****    :::: >
//       < :::::     ****               TE               ****     ::::: >
//       < :::::::     ****             AMO            ****     ::::::: >
//       < :::::::::     ****                        ****     ::::::::: >
//       < :::::::::::     ****                    ****     ::::::::::: >
//       < ::::::::::::::      ****            ****      :::::::::::::: >
//       < :::::::::::::::::       ****    ****       ::::::::::::::::: >
//       < ::::::::::::::::::::        ****        :::::::::::::::::::: >
//       < :::::::::::::::::::::::      **      ::::::::::::::::::::::: >
//       < :::::::::::::::::::::::::          ::::::::::::::::::::::::: >
//       < :::::::::::::::::::::::::::      ::::::::::::::::::::::::::: >
//       < :::::::::::::::::::::::::::::  ::::::::::::::::::::::::::::: >
//       < :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: >
//         ()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()
