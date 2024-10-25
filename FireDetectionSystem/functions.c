#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "functions.h"

extern char forest[SIZE][SIZE];
extern pthread_mutex_t forest_lock;
extern pthread_cond_t cond;

void *sensor_thread(void *arg)
{
    int idx = (int)(long)arg;
    int row = idx / SIZE;
    int col = idx % SIZE;

    while (1)
    {
        pthread_mutex_lock(&forest_lock);
        if (forest[row][col] == '@')
        {
            forest[row][col] = '/';

            if (row > 0 && forest[row - 1][col] == 'T')
            {
                forest[row - 1][col] = forest[row - 1][col];
            }
            if (row < SIZE - 1 && forest[row + 1][col] == 'T')
            {
                forest[row + 1][col] = forest[row + 1][col];
            }
            if (col > 0 && forest[row][col - 1] == 'T')
            {
                forest[row][col - 1] = forest[row][col - 1];
            }
            if (col < SIZE - 1 && forest[row][col + 1] == 'T')
            {
                forest[row][col + 1] = forest[row][col + 1];
            }

            if (row == 0 || row == SIZE - 1 || col == 0 || col == SIZE - 1)
            {
                pthread_cond_signal(&cond);
            }
        }
        pthread_mutex_unlock(&forest_lock);
        usleep(100000);
    }
    return NULL;
}

void *fire_generator_thread(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&forest_lock);
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        if (forest[row][col] == '-')
        {
            forest[row][col] = '@';
        }
        pthread_mutex_unlock(&forest_lock);
        sleep(3);
    }
    return NULL;
}

void *central_control_thread(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&forest_lock);
        pthread_cond_wait(&cond, &forest_lock);
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                if (forest[i][j] == '@')
                {
                    forest[i][j] = '/';
                }
            }
        }
        pthread_mutex_unlock(&forest_lock);
        sleep(1);
    }
    return NULL;
}