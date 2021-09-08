#include"utils.h"

int iniciar_servidor(void)
{
	int socket_servidor = -1;
	int algo = -1;

	//basandome en la guia beej
/*
	struct sockaddr_in miDireccion;
	miDireccion.sin_family = AF_INET;
	miDireccion.sin_port = htons(4444);
	miDireccion.sin_addr.s_addr = INADDR_ANY;
	memset(&(miDireccion.sin_zero), '\0', 8);

	socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_servidor == -1){
		log_info(logger, "no se pudo crear el socket");
		exit(EXIT_FAILURE);
	}

	socket_servidor = bind(socket_servidor, (void*) &miDireccion, sizeof(miDireccion));
	if(socket_servidor != 0){
		log_info(logger, "no se pudo bindear el socket (asociar el socket a un puerto)");
		exit(EXIT_FAILURE);
	}

	listen(socket_servidor, SOMAXCONN);
	if(socket_servidor == -1){ //SOMAXCONN es el maximo de conexiones en backlog que puede haber
		log_info(logger, "El servidor no pudo escuchar");
		exit(EXIT_FAILURE);
	}

    log_trace(logger, "Listo para escuchar a mi cliente");

    return socket_servidor;

*/

    struct addrinfo hints;
    struct addrinfo *servinfo;
    struct addrinfo *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(IP, PUERTO, &hints, &servinfo);

    for (p=servinfo; p != NULL; p = p->ai_next) //por que usamos un for con un p no tengo la menor idea;
    {
        // Creamos el socket de escucha del servidor
     socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
     if(socket_servidor == -1){ log_info(logger, "No se pudo crear el socket"); exit(EXIT_FAILURE); }

    	// Asociamos el socket a un puerto
     algo = bind(socket_servidor, p->ai_addr, p->ai_addrlen);
     if(algo == -1){ log_info(logger, "No se pudo asociar el socket a un puerto"); exit(EXIT_FAILURE); }

     break;
    }


    // Escuchamos las conexiones entrantes
    algo = listen(socket_servidor, SOMAXCONN);
	if(algo == -1){ //SOMAXCONN es el maximo de conexiones en backlog que puede haber
		log_info(logger, "El servidor no pudo escuchar");
		exit(EXIT_FAILURE);
	}

    freeaddrinfo(servinfo);

    //log_trace(logger, "Listo para escuchar a mi cliente");

    return socket_servidor;

}

int esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;

	socklen_t tam_direccion = sizeof(struct sockaddr_in);
	int socket_cliente = 0;

	// Aceptamos un nuevo cliente
	while(1){
		socket_cliente = accept(socket_servidor, (void*)&dir_cliente, &tam_direccion);
		if(socket_cliente == -1) { log_info(logger, "Error al aceptar a un nuevo cliente"); continue;}

		log_info(logger, "Se conecto un cliente!");
		return socket_cliente;
	}
}

int recibir_operacion(int socket_cliente)
{
	int cod_op = -1;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) == -1)
		cod_op = -1;

	return cod_op;
}

char* copiar_palabra_de_buffer(void* buffer)
{
	int tamanio = 0;
	char* mensaje = NULL;
	int aux = 0;

	memcpy(&tamanio, buffer + aux, sizeof(int));
	aux = aux + sizeof(int);

	mensaje = malloc(tamanio);
	memcpy(mensaje, buffer + aux, tamanio);

	return mensaje;
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

//podemos usar la lista de valores para poder hablar del for y de como recorrer la lista
char* recibir_paquete(int socket_cliente)
{
	int size = 0;
	char* palabra = NULL;

	void* buffer = recibir_buffer(&size, socket_cliente);

	palabra = copiar_palabra_de_buffer(buffer);

	free(buffer);
	return palabra;

}
