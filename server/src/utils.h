#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include <pthread.h>

#define IP "127.0.0.1"
#define PUERTO "4444"
#define SI 1
#define CHAU -1

typedef enum
{
	MENSAJE,
	PAQUETE
}op_code;

t_log* logger;

void* recibir_buffer(int*, int);
void iniciar_servidor(void);
void trabajemos_con_el_cliente(int socket_cliente);
char* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);
char* copiar_palabra_de_buffer(void* buffer);

#endif /* UTILS_H_ */
