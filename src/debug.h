#pragma once

#include "common.h"
#include "chunk.h"

// Prints the contents of a Chunk.
void disassembleChunk(Chunk* chunk, const char* name);
uint32_t disassembleInstruction(Chunk* chunk, uint32_t offset);
