#pragma once

#include "chunk.h"
#include "object.h"

// Returns a compiled ObjFunction, or NULL if an error occurred.
ObjFunction* compile(const char* source);
