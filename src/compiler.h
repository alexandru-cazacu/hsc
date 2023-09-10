#pragma once

#include "chunk.h"
#include "object.h"

#define SCOPE_GLOBAL 0

// Returns a compiled ObjFunction, or NULL if an error occurred.
ObjFunction* compile(const char* source);
