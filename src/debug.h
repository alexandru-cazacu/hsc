#pragma once

#include "common.h"
#include "chunk.h"

// Prints the contents of a Chunk.
void disassembleChunk(Chunk* chunk, const char* name);
// Returns the offset of the next instruction (instructions can be made of
// multiple bytes).
// offset - Offset the instruction to disassemble in the given Chunk.
uint32_t disassembleInstruction(Chunk* chunk, uint32_t offset);
