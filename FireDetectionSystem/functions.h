#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define SIZE 30

void *sensor_thread(void *arg);
void *fire_generator_thread(void *arg);
void *central_control_thread(void *arg);

#endif