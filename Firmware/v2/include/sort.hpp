#ifndef SORT_H
#define SORT_H

#include <Arduino.h>

#include <client.hpp>

class client_t;

void quickSort(client_t** arr, int lower, int upper);
int sortClientList(client_t** clients);

#endif