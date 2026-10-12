#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include "lectura.h"

struct lectura {
    int num[7];
    int instrucciones[9];
};

extern struct lectura l1;



int buscarSubcadena(char *linea, const char *cadena) {
    char *subcadena = strstr(linea, cadena);
    
    if (subcadena != NULL) {
        return 1; // Se encontró subcadena
    } else {
        return 0; // no se encontro subcadena
    }
}

int buscarParentesis(char *linea) {
    char *parentesis = strchr(linea, '(');
    
    if (parentesis != NULL) {
        return (int)(parentesis - linea);  // Devuelve la posición del paréntesis
    } else {
        return -1;  // No se encontró el paréntesis
    }
}

int es_numero_entero(const char *cadena) {
    char *ptr;
    long numero = strtol(cadena, &ptr, 10);

    
    if (*ptr != '\0') {
        return 0;  // No es un número válido o está fuera del rango
    }

    return 1;
}

// Función para eliminar el salto de línea
void eliminarSaltoDeLinea(char *cadena) {
    // Buscamos si hay un salto de línea en la cadena
    char *salto = strchr(cadena, '\n');
    
    // Si encontramos el salto de línea, lo reemplazamos por '\0'
    if (salto != NULL) {
        *salto = '\0';
    }
}

// Función que divide una línea usando un delimitador y procesa los tokens
int dividirLinea(char *linea, const char *delimitador) {
	// eliminamo el salto de linea
	eliminarSaltoDeLinea(linea);
	
    // Usamos strtok para dividir la línea por el delimitador especificado
    char *token = strtok(linea, delimitador);
    
    // Mientras haya más tokens
    while (token != NULL) {
        // Verificamos si el token es un número entero
        if (es_numero_entero(token)) {
            int numero = atoi(token);  // Convertimos el token a entero
            return numero;
            //printf("El token '%s' es un numero entero: %d\n", token, numero);
        } 
        
        // Obtenemos el siguiente token
        token = strtok(NULL, delimitador);
    }
    return 0;
}

int hexCharToInt(char s) {
    // Convertimos la letra a minúscula para manejar 'A'-'F' y 'a'-'f' de la misma forma
    s = tolower(s);

    if (s >= '0' && s <= '9') {
        // Si el carácter está entre '0' y '9', restamos '0' para obtener el valor numérico
        return s - '0';
    } else if (s >= 'a' && s <= 'f') {
        // Si el carácter está entre 'a' y 'f', restamos 'a' y le sumamos 10
        return s - 'a' + 10;
    } else {
        // Si no es un valor hexadecimal válido, regresamos -1 para indicar error
        return -1;
    }
}


void mostrar() {
    for (int i = 0; i < 9; i++) {
       // printf("%d Numero reg: %d\n", i, l1.num[i]);
        printf("%d instruccion %X\n", i, l1.instrucciones[i]);  
    }
    for(int i = 0; i < 7; i++){
    	printf("%d numero %X\n",i,l1.num[i]);
	}
}

void procesar_archivo(const char *nombre_archivo) {
   int bu;
   int nume = 0;
   int aux;
   int instr = 0;

    FILE *archivo;
    char linea[100];  // Buffer para almacenar una palabra leída

    archivo = fopen(nombre_archivo, "r");

    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

       // Leer el archivo línea por línea
    while (fgets(linea, sizeof(linea), archivo)) {
  
     	if(buscarSubcadena(linea,"var") || buscarSubcadena(linea,"result")){
     		aux = dividirLinea(linea," ");
     		l1.num[nume] = aux;
     		nume++;
     		
		 }
		 
		 else if(buscarSubcadena(linea,"LOAD MQ")){

			bu = buscarParentesis(linea);
			if(bu != -1){
				int n1 = 0x6;
				int n2 = hexCharToInt(linea[bu+1]);
				l1.instrucciones[instr] = (n1 << 4) | n2;
				instr++;
			}
			else{
				l1.instrucciones[instr] = 0x50;
				instr++;
			}
		}
		
		else if(buscarSubcadena(linea,"LOAD")){

			bu = buscarParentesis(linea);
			if(bu != -1){
		
				int n1 = 0x7;
				int n2 = hexCharToInt(linea[bu+1]);
				l1.instrucciones[instr] = (n1 << 4) | n2;
		
				instr++;
			}
		}
		else if(buscarSubcadena(linea,"ADD")){
		
			bu = buscarParentesis(linea);
			if(bu != -1){
			
				int n1 = 0x0;
				int n2 = hexCharToInt(linea[bu+1]);
				l1.instrucciones[instr] = (n1 << 4) | n2;
			
				instr++;
			}
			
		}
		else if(buscarSubcadena(linea,"STORE")){
		
			bu = buscarParentesis(linea);
			if(bu != -1){
			
				int n1 = 0x4;
				int n2 = hexCharToInt(linea[bu+1]);
				l1.instrucciones[instr] = (n1 << 4) | n2;
			
				instr++;
			}
			
		}
		
		else if(buscarSubcadena(linea,"MUL")){
	
			bu = buscarParentesis(linea);
			if(bu != -1){
			
				int n1 = 0x1;
				int n2 = hexCharToInt(linea[bu+1]);
				l1.instrucciones[instr] = (n1 << 4) | n2;
			
				instr++;
			}
		}
		else if(buscarSubcadena(linea,"HTL")){
		
			l1.instrucciones[instr] = 0xF0;
		}
		
        
    }

    // Cerrar el archivo
    fclose(archivo);
    
}







