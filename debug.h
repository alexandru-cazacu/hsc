#pragma once

void disassembleChunk(Chunk* chunk, const char* name);
uint32_t disassembleIstruction(Chunk* chunk, uint32_t offset);