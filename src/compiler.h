#pragma once

#include "chunk.h"
#include "scanner.h"

// Returns true if the compilation was successful, false otherwise.
bool compile(const char* source, Chunk* chunk);
