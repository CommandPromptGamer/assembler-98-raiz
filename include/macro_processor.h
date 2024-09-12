#ifndef MACRO_PROCESSOR_H
#define MACRO_PROCESSOR_H

#include "montador_types.h"

void process_macros( program_t *program );
void add_macro( program_t *program, MACRO_T *m );
MACRO_T *find_macro( program_t *program, char *name );

#endif
