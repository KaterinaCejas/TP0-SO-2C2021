#include"utils.h"

void iniciar_servidor(void)
{
	int socket_servidor = 0;
	int valor = 0;

    struct addrinfo hints;
    struct addrinfo *servinfo;
    struct addrinfo *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(IP, PUERTO, &hints, &servinfo);

    for (p=servinfo; p != NULL; p = p->ai_next)
    {
        // Creamos el socket de escucha del servidor
     socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
     if(socket_servidor == -1){ log_info(logger, "No se pudo crear el socket"); exit(EXIT_FAILURE); }

    	// Asociamos el socket a un puerto
     valor = bind(socket_servidor, p->ai_addr, p->ai_addrlen);
     if(valor == -1){ log_info(logger, "No se pudo asociar el socket a un puerto"); exit(EXIT_FAILURE); }

     break;
    }

    // Escuchamos las conexiones entrantes
    valor = listen(socket_servidor, SOMAXCONN);
	if(valor == -1){ //SOMAXCONN es el maximo de conexiones en backlog que puede haber
		log_info(logger, "El servidor no pudo escuchar");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(servinfo);
	log_info(logger, "Servidor listo pa recibir al cliente");

    // Esperamos al cliente
	while(1)
	{
		struct sockaddr_in dir_cliente;
		socklen_t tam_direccion = sizeof(struct sockaddr_in);
		int socket_cliente = 0;

		// Aceptamos un nuevo cliente

		socket_cliente = accept(socket_servidor, (void*)&dir_cliente, &tam_direccion);

		if(socket_cliente == -1) { log_info(logger, "Error al aceptar a un nuevo cliente"); continue;}

		log_info(logger, "Se conecto un cliente!");

		trabajemos_con_el_cliente(socket_cliente);

	}
}

void trabajemos_con_el_cliente(int socket_cliente)
{
//	while (SI)
//	{
		char* palabra = NULL;
		int cod_op = recibir_operacion(socket_cliente);

		switch (cod_op)
		{
			case MENSAJE:
				log_info(logger, "recibi codigo de operacion mensaje");
				recibir_mensaje(socket_cliente);
				break;
			case PAQUETE:
				palabra = recibir_paquete(socket_cliente);
				log_info(logger, "Me llegaron los siguientes valores:\n");
				log_info(logger,"%s", palabra);
				break;
			case -1:
				log_error(logger, "cliente no pude leer lo que me enviaste");
				//exit (EXIT_FAILURE);
				break;
			default:
				log_warning(logger,	"Operacion desconocida. No quieras meter la pata");
				break;
		}

//	}
}


int recibir_operacion(int socket_cliente){
	int cod_op = -1;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) == -1)
	{
		cod_op = -1;
		log_warning(logger,	"cod_op = -1. No quieras meter la pata");
	}

	return cod_op;
}

char* copiar_palabra_de_buffer(void* buffer){
	int tamanio = 0;
	char* mensaje = NULL;
	int aux = 0;

	memcpy(&tamanio, buffer + aux, sizeof(int));
	aux = aux + sizeof(int);

	mensaje = malloc(tamanio);
	memcpy(mensaje, buffer + aux, tamanio);

	return mensaje;
}

void* recibir_buffer(int* size, int socket_cliente){
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente){
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

//podemos usar la lista de valores para poder hablar del for y de como recorrer la lista
char* recibir_paquete(int socket_cliente){
	int size = 0;
	char* palabra = NULL;

	void* buffer = recibir_buffer(&size, socket_cliente);

	palabra = copiar_palabra_de_buffer(buffer);

	free(buffer);
	return palabra;

}
