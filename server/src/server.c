#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include "utils.h"

int main(void)
{

	logger = log_create("/home/utnso/workspace/tp0/server/tp0.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	iniciar_servidor();

	return EXIT_SUCCESS;
}

