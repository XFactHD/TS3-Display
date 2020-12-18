#include <sort.hpp>

int compare(client_t* c1, client_t* c2) {
  int32_t cmp = c2->talkPower - c1->talkPower;
  if(cmp == 0) { cmp = strcmp(c1->name, c2->name); } //TODO: find proper sorting data
  return cmp;
}

void swap(client_t** c1, client_t** c2) {
  client_t* temp = *c1;
  *c1 = *c2;
  *c2 = temp;
}

int partition(client_t** arr, int lower, int upper) {
  int i = lower - 1;

  client_t* pivot = arr[upper];
  for(int j = lower; j < upper; j++) {
    if(compare(arr[j], pivot) < 0) {
      i++;
      swap(&arr[i], &arr[j]);
    }
  }

  swap(&arr[i + 1], &arr[upper]);
  return i + 1;
}

void quickSort(client_t** arr, int lower, int upper) {
  if(upper > lower) {
    int partIdx = partition(arr, lower, upper);

    quickSort(arr, lower, partIdx - 1);
    quickSort(arr, partIdx + 1, upper);
  }
}

int sortClientList(client_t** clients) {
  //TODO: find out how clients are sorted, expect them to be sorted by client id
  uint16_t ids[14];
  for(int i = 0; i < 14; i++) { ids[i] = clients[i]->id; }
  quickSort(clients, 0, 13);
  for(int i = 0; i < 14; i++) { if(ids[i] != clients[i]->id) { return i; } }
  return 0;
}