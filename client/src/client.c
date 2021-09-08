#include "client.h"

int main(void){
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");

	config = iniciar_config();

	// Usando el config creado previamente
	// Lee las variables de IP, Puerto y Valor
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "VALOR");

	//Loggear valor de config
	log_info(logger, ip);
	log_info(logger, puerto);
	log_info(logger, valor);


	leer_consola(logger);
	log_info(logger, "\ntermine de leer consola\n");


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);
	log_info(logger, "socket creado");

	//enviar CLAVE al servidor
	enviar_mensaje(valor, conexion);
	log_info(logger, "envie la clave al servidor");

	//enviar PAQUETE al servidor
	paquete(conexion);
	log_info(logger, "envie el paquete de mensajes al servidor");

	log_info(logger, "termino el programa");
	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void){
	t_log* nuevo_logger = log_create("/home/utnso/workspace/tp0/client/tp0.log", "logTp0", 1, LOG_LEVEL_INFO);
	return nuevo_logger;
}

t_config* iniciar_config(void){
	t_config* nuevo_config = config_create("/home/utnso/workspace/tp0/client/tp0.config");
	return nuevo_config;
}

void leer_consola(t_log* logger){
	char* leido;

	//El primero te lo dejo de yapa
	// Acá la idea es que imprimas por el log lo que recibis de la consola.
	printf("\nPara salir de leer consola, apretar doble enter   \n");
	leido = readline(">");
	while(strcmp(leido, "")!=0) {
		log_info(logger, leido);
		free(leido);
		//printf("\nPara dejar de ingresar por consola, apretar enter   \n");
		leido = readline(">");
	}
	free(leido);
}
char* tomar_palabras_de_consola()
{
	char* leido = NULL;
	char* palabras = string_new();
	while(1)
	{
		leido = readline(">");

		if(strcmp(leido, "")==0) { break; }

		string_trim(&leido);// <-- esta no es necesario
		string_append_with_format(&palabras, "%s ", leido);

		free(leido);
	}

	string_trim(&palabras);// <-- esta no es necesario
	return palabras;
}
void paquete(int conexion){
	//Ahora toca lo divertido!

	char* leido;
	t_paquete* paquete;

	paquete = crear_paquete();

	printf("\nPara dejar de ingresar por consola, aprete doble enter   ");

	leido = tomar_palabras_de_consola();

	agregar_a_paquete(paquete, (void*)leido, strlen(leido)+1);

	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config){
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}


