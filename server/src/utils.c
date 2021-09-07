#include"utils.h"

int iniciar_servidor(void)
{
	int socket_servidor = 0;

	//basandome en la guia beej

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



	/*
	 * ESTO NO SE POR QUÉ NO FUNCIONA
	 *
    struct addrinfo hints, *servinfo;
    //struct addrinfo *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(IP, PUERTO, &hints, &servinfo);

    // Creamos el socket de escucha del servidor
    socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if(socket_servidor == -1){
    	log_info(logger, "No se pudo crear el socket");
    	exit(EXIT_FAILURE);
    }

	// Asociamos el socket a un puerto
    socket_servidor = bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);
    if(socket_servidor != 0){
    	log_info(logger, "No se pudo asociar el socket a un puerto");
    	exit(EXIT_FAILURE);
    }

    // Escuchamos las conexiones entrantes
    socket_servidor = listen(socket_servidor, SOMAXCONN);
	if(socket_servidor == -1){ //SOMAXCONN es el maximo de conexiones en backlog que puede haber
		log_info(logger, "El servidor no pudo escuchar");
		exit(EXIT_FAILURE);
	}

    freeaddrinfo(servinfo);

    log_trace(logger, "Listo para escuchar a mi cliente");

    return socket_servidor;


    */

}

int esperar_cliente(int socket_servidor)
{
	//struct sockaddr_in dir_cliente;
	struct sockaddr dir_cliente;
	//int tam_direccion = sizeof(struct sockaddr_in);
	socklen_t tam_direccion = sizeof(struct sockaddr_in);
	int socket_cliente = 0;

	// Aceptamos un nuevo cliente
	while(1){
		socket_cliente = accept(socket_servidor, &dir_cliente, &tam_direccion);
		if(socket_cliente == -1) {
			continue;
		}

		log_info(logger, "Se conecto un cliente!");
		return socket_cliente;
	}
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) != 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
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
t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
	return NULL;
}
