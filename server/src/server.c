#include "server.h"

int main(void) {
	logger = log_create("/home/utnso/workspace/tp0/server/tp0.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");
	int cliente_fd = esperar_cliente(server_fd);

	while (SI) {
		sleep(3);
		char* palabra = NULL;
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
		case MENSAJE:
			log_info(logger, "recibi codigo de operacion mensaje");
			recibir_mensaje(cliente_fd);
			break;
		case PAQUETE:
			palabra = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			mostrar_palabras(logger, palabra);
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,
					"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
	return EXIT_SUCCESS;
}

int cantidad_de_palabras(char** doble_char) {
	int i = 0;
	while(*doble_char!=NULL) //mientras no encuentre null sigue
	{
		i++;
		doble_char++;
	}
	return i;
}

void mostrar_palabras(t_log* logger, char* palabra) {
	log_info(logger,"%s", palabra);
	/*
	 * no es necesario
	 *
	char** palabras;
	int cantidad;

	palabras = string_split(palabra, " ");
	cantidad = cantidad_de_palabras(palabras);

	for(int i=0; i<cantidad; i++)
	{
		log_info(logger,"%s\n", palabras[i]);
	}*/

}
