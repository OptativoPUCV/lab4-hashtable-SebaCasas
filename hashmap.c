#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

#include <stdbool.h>
void enlarge(HashMap * );
void insertMap(HashMap * , char * , void * );
Pair * firstMap(HashMap *);

typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  if (map == NULL)
  {
    return;
  }
  if (map->capacity == map->size)
  {
    enlarge(map);
  }

  long pos = hash(key,map->capacity);
  long original_pos = pos;
  
  while(map->buckets[pos] != NULL && map->buckets[pos]->key != NULL){
    if(strcmp(key,map->buckets[pos]->key) == 0)
    {
      return;
    }
    pos = (pos+1) % map->capacity;
    if (pos == original_pos) {
      return;
    }
  }
  
  map->size += 1;
  map->buckets[pos] = createPair(key,value);
  map->current = pos;
}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  Pair** old_array = map->buckets;
  int old_capacity = map->capacity;
  map->capacity *= 2;
  Pair** nuevo_hashArray = (Pair**)malloc(map->capacity * sizeof(Pair*));
  map->buckets = nuevo_hashArray;
  for(int i = 0; i < old_capacity; i++){
    if(old_array[i] != NULL){
      insertMap(map, old_array[i]->key, old_array[i]->value);
    }
  }

}


HashMap * createMap(long capacity) {
  HashMap *mapa = (HashMap*) malloc(capacity * sizeof(HashMap));
  mapa->buckets = (Pair**) calloc(capacity, sizeof(Pair*));
  mapa->size = 0;
  mapa->capacity = capacity;
  mapa->current = -1;
  return mapa;
}

void eraseMap(HashMap * map,  char * key) {    
  long pos = hash(key , map->capacity);
  long original_pos = pos;

  while (map->buckets[pos] != NULL) {
    if (strcmp(map->buckets[pos]->key, key) == 0) {
      map->size -= 1;
      map->buckets[pos]->key = NULL;
      map->buckets[pos]->value = NULL;
      break;
    }
    pos = (pos + 1) % map->capacity;
    if (pos == original_pos) {
        break;
    }
  }
}

Pair * searchMap(HashMap * map, char * key) {
  long pos = hash(key, map->capacity);
  long original_pos = pos;
  
  while (map->buckets[pos] != NULL) {
    if (strcmp(map->buckets[pos]->key, key) == 0) {
        map->current = pos;
        return map->buckets[pos];
    }
    pos = (pos + 1) % map->capacity;
    if (pos == original_pos) {
        break;
    }
  }
  map->current = pos;
  return NULL;
}

Pair * firstMap(HashMap * map) {
  if (map == NULL || map->buckets == NULL) {
      return NULL;
    }


    for (int i = 0; i < map->capacity; i++) {
      if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {

          map->current = i;
          return map->buckets[i];
      }
    }


    return NULL;
  }

Pair * nextMap(HashMap * map) {
  if (map == NULL || map->buckets == NULL) {
    return NULL;
  }


  for (int i = map->current + 1; i < map->capacity; i++) {
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {

        map->current = i;
        return map->buckets[i];
    }
  }

  return NULL;
}
