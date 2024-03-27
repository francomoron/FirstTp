#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	/* https://github.com/sisoputnfrba/so-commons-library/blob/72470a6732312c085185ae08d911e9f0d7c7fc1a/src/commons/log.h#L40-L57
	  
	  	ESTRUCTURA DEL t_log:
		******************************
		typedef struct {
		FILE* file;
		bool is_active_console;
		t_log_level detail;
		char *program_name;
		pid_t pid;
		}t_log;
		******************************
	*/

	/* ---------------- LOGGING ---------------- */
	
	printf("******************************\n");
	printf("EMPIEZA LOGGING:");
	printf("\n******************************\n");

	t_log* logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	logger = log_create("tp0.log", "tp0", true, LOG_LEVEL_INFO);

	if(logger == NULL){
		printf("Fallo el logger.\n");
		log_destroy(logger);
		return 1;
	}

	printf("Logger creado.\n\n");
	char* message = "Hola! Soy un Log";

	log_info(logger, message);

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	/* https://github.com/sisoputnfrba/so-commons-library/blob/master/src/commons/config.h

		ESTRUCTURA DEL t_config:
		******************************
		typedef struct {
			char *path;
			t_dictionary *properties;
		} t_config;
		******************************

		ESTRUCTURA DEL t_dictionary:
		******************************
		typedef struct {
		t_hash_element **elements;
		int table_max_size;
		int table_current_size;
		int elements_amount;
		} t_dictionary;
		******************************
	*/

	printf("\n******************************\n");
	printf("EMPIEZA CONFIGURACION:");
	printf("\n******************************\n");

	t_config* config = iniciar_config();

	config = config_create("/home/fran/Desktop/SO/tp0/client/cliente.config");
	if(config == NULL){
		printf("\nConfiguracion no encontrada.\n");
		return 1;
	}

	printf("\nConfiguracion creada.\n\n");

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	
	char* value_atribute = "CLAVE";
	char* ip_atribute = "IP";
	char* port_atribute = "PUERTO";

	if(!config_has_property(config, value_atribute) || !config_has_property(config, ip_atribute) || !config_has_property(config, port_atribute) ){
		printf("\nFalta algun atributo (CLAVE, IP, PUERTO) en el archivo de configuracion.\n");
		return 1;
	}

	char* valor = config_get_string_value(config, value_atribute);
	char* ip = config_get_string_value(config, ip_atribute);
	char* puerto = config_get_string_value(config, port_atribute);

	// Loggeamos el valor de config

	log_info(logger, valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	printf("\n******************************\n");
	printf("EMPIEZA LEER CONSOLA:");
	printf("\n******************************\n");

	leer_consola(logger);


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor

	printf("\n******************************\n");
	printf("EMPIEZA CONEXION:");
	printf("\n******************************\n");

	int conexion = crear_conexion(ip, puerto);
	if(conexion == -1){
		printf("\nError al crear el socket.\n\n");
		return 1;
	}

	printf("\nSocket creado correctamente.\n\n");

	// Enviamos al servidor el valor de CLAVE como mensaje

	enviar_mensaje(valor, conexion);
	
	printf("\nMensaje enviado correctamente.\n\n");

	// Armamos y enviamos el paquete
	printf("\n******************************\n");
	printf("\nEscriba los mensajes que quiere enviar al server:\n\n");
	printf("\n******************************\n");

	paquete(conexion);

	printf("\nPaquete enviado correctamente.\n\n");

	printf("\n******************************\n");
	printf("LIBERANDO RECURSOS DEL PROGRAMA...");
	printf("\n******************************\n");

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente

	return 0;
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = NULL;

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = NULL;

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido = NULL;
	while(1){
		leido = readline(">");
		if(strcmp( leido, "") == 0){
			free(leido);
			printf("\nCaracteres liberados.\n");
			printf("\nSaliendo del readline...\n\n");
            break;
		}
		log_info(logger, leido);
		printf("\nEscrito en el logger correctamente.\n");
		free(leido);
		printf("\nCaracteres liberados.\n\n");
	}
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!

	/*	ESTRUCTURA DEL t_paquete:
		******************************
		typedef struct
			{
				op_code codigo_operacion;
				t_buffer* buffer;
			} t_paquete;
		******************************

		ESTRUCTURA DEL t_buffer:
		******************************
		typedef struct
			{
				int size;
				void* stream;
			} t_buffer;
		******************************
	*/

	char* leido = NULL;
	t_paquete* paquete = crear_paquete();
	
	// Leemos y esta vez agregamos las lineas al paquete

	while(1){
		leido = readline(">");
		if(strcmp( leido, "") == 0){
			free(leido);
			printf("\nCaracteres liberados.\n");
			printf("\nSaliendo del readline...\n\n");
            break;
		}
		agregar_a_paquete(paquete, leido, strlen(leido) + 1 );
		printf("\nAgregado al packete correctamente.\n");
		free(leido);
		printf("\nCaracteres liberados.\n\n");
	}

	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);

}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	
	liberar_conexion(conexion);
	printf("\n\nConexion cerrada.\n");
	
	config_destroy(config);
	printf("\nConfiguracion cerrada.\n");

	log_destroy(logger);
	printf("\nLogger destruido.\n");
	  
}
