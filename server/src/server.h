#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include<commons/string.h>
#include "utils.h"

#define SI 1

int cantidad_de_palabras(char** doble_char);
void mostrar_palabras(t_log* logger, char* value);

#endif
