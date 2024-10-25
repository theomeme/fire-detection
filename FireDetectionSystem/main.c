#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "functions.h"

#define SIZE 30

char forest[SIZE][SIZE];
pthread_mutex_t forest_lock;
pthread_cond_t cond;

void print_forest()
{
    pthread_mutex_lock(&forest_lock);
    system("clear");
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            printf("%c ", forest[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    pthread_mutex_unlock(&forest_lock);
}

void *periodic_print(void *arg)
{
    while (1)
    {
        sleep(1);
        print_forest();
    }
    return NULL;
}

int main()
{
    pthread_t sensor_threads[SIZE][SIZE];
    pthread_t fire_thread, central_thread, print_thread;

    pthread_mutex_init(&forest_lock, NULL);
    pthread_cond_init(&cond, NULL);

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            forest[i][j] = 'T';
        }
    }

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (rand() % 2 == 0)
            {
                forest[i][j] = '-';
            }
        }
    }

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            pthread_create(&sensor_threads[i][j], NULL, sensor_thread, (void *)(long)(i * SIZE + j));
        }
    }

    pthread_create(&fire_thread, NULL, fire_generator_thread, NULL);

    pthread_create(&central_thread, NULL, central_control_thread, NULL);

    pthread_create(&print_thread, NULL, periodic_print, NULL);

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            pthread_join(sensor_threads[i][j], NULL);
        }
    }

    pthread_join(fire_thread, NULL);
    pthread_join(central_thread, NULL);
    pthread_join(print_thread, NULL);

    pthread_mutex_destroy(&forest_lock);
    pthread_cond_destroy(&cond);

    return 0;
}