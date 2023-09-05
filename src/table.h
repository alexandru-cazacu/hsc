#pragma once

#include "value.h"

//
// Open addressing closed hashing table
//

typedef struct {
    ObjString* key;
    Value value;
} Entry;

typedef struct {
    int count;
    int capacity;
    Entry* entries;
} Table;

void initTable(Table* table);
void freeTable(Table* table);
bool tableGet(Table* table, ObjString* key, Value* oValue);
bool tableSet(Table* table, ObjString* key, Value value);
bool tableDelete(Table* table, ObjString* key);
// Copies all of the entries of one table into another.
void tableAddAll(Table* from, Table* to);
// Used instead of findEntry because findEntry uses == for equality
// check but we need to do interning first to make it work.
ObjString* tableFindString(Table* table, const char* chars, int length,
                           uint32_t hash);
