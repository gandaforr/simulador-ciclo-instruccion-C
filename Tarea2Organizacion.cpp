#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include "lectura.h" //lee el archivo

struct lectura l1; //la informacion se guarda aqui

//ahora todo en hexadecimal
// profe descrubi que todo los enteros son tambien considerados hexadecimales en c
// haci que si en el codigo parece que es un entero no es un hexadecimal profe
// estoy tratando todos los enteros como hexadecimales 0xFF
// en el codigo solo voy a poner hexadecimal tambien profe [con excepciones



// Componentes del simulador
struct Unidad_de_control{
	int contador;
	void (*decodifi)(int *);
	int instruccion;
	
	// para facilitarme la logica
	int intr;
	int num;
	int EnMarcha;
}uni;

struct Memoria{
	int direccion;
	int tabla_memoria[16];
	int dato;
}mem;

struct ALU{
	int acumulador;
	int Entrada;
	int MQ;
}alu;


//set
// Función para establecer el valor
void setHexValue(int *num, unsigned int newValue);
void imprimirTabla();
//void getHexValue()

// inicializar los componentes de la simulacion
void cargar();

//funciones parea el decodifi
void add(int *);
void LoadMX(int *);
void LoadMQ(int *); 
void Loadmq(int *);
void storeMX(int *);
void mul(int *); 
void HTL(int *);
void auxiliarDecodifi(int);


int Primero4(int);
int Ultimo4(int);



// liberar la memoria de los apuntadores de los componentes de la simulacion
void finalizar();


// funciones para mostrar los componentes de la simulacion
void imprimir_unidad_de_control(struct Unidad_de_control uni);
void imprimir_memoria(struct Memoria mem);
void imprimir_alu(struct ALU alu);
void imprimir_componentes(struct Unidad_de_control uni, struct Memoria mem, struct ALU alu);

int main(){
	
	//cargar memmoria
	cargar();
	
	// la simulacion()
	
	//inicilizar los demas componentes
	uni.EnMarcha = 0x1;
	uni.contador = 0;  
	
	imprimir_componentes(uni,mem,alu);
	
	printf("\n-----inicializa el bucle-----\n\n");
	while(uni.EnMarcha == 0x1){
	mem.direccion = uni.contador;
	uni.contador = uni.contador + 0x1;
	mem.dato = mem.tabla_memoria[mem.direccion];
	uni.instruccion = mem.dato; 
		//cosa extra para ayudarme en la logica
		uni.intr = Primero4(uni.instruccion);
		uni.num= Ultimo4(uni.instruccion);
	auxiliarDecodifi(uni.intr); 
	mem.direccion = uni.num;
	mem.dato = mem.tabla_memoria[mem.direccion];
	uni.decodifi(&mem.tabla_memoria[mem.direccion]);
	imprimir_componentes(uni,mem,alu);
	}
	
	
	
	return 0;
}



// Función para establecer el valor
void setHexValue(int *num, int newValue) {
    // Verificar que el nuevo valor esté dentro del rango de números hexadecimales permitidos
    if (newValue <= 0xFF) {  // Por ejemplo, permitir solo valores de 0x00 a 0xFF
        *num = newValue;  // Asignar el nuevo valor usando desreferencia
    } else {
        printf("Error: Valor fuera del rango permitido (0x00 a 0xFF).\n");
        exit(EXIT_FAILURE);  // Terminar el programa
    }
}

void imprimirTabla(){
	for(int i = 0;i<16;i++){
		printf("%X: %X\n",i,mem.tabla_memoria[i]);
	}
}

void cargar(){
	procesar_archivo("instr.txt");
	
	for(int i = 0; i < 9;i++){
		mem.tabla_memoria[i] = l1.instrucciones[i];
	}
	
	for(int i = 0;i<7;i++){
		mem.tabla_memoria[i+9] = l1.num[i];
	}
		
}


void add(int *n){
	int s = *n;
	setHexValue(&alu.Entrada,s);
	int aux = alu.Entrada + alu.acumulador;
	setHexValue(&alu.acumulador,aux);
}
void LoadMX(int *n){
	int s = *n;
	setHexValue(&alu.acumulador,s);
}
void LoadMQ(int *n){
	int s = *n;
	setHexValue(&alu.MQ,s);
}
void Loadmq(int *n){
	setHexValue(&alu.acumulador,alu.MQ); 
}
void storeMX(int *n){
	int s = *n;
	setHexValue(&s,alu.acumulador);
	*n = s;
}
void mul(int *n){
	unsigned char a = (unsigned char)alu.MQ;
    unsigned char b = (unsigned char)*n;  	
	
	 // Multiplicación completa (el resultado puede ser de hasta 16 bits)
    unsigned int resultado = a * b;

    // Los 8 bits menos significativos (parte baja)
    unsigned char menos_significativos = resultado & 0xFF;

    // Los 8 bits más significativos (parte alta)
    unsigned char mas_significativos = (resultado >> 8) & 0xFF;
    
    //printf("Resultado completo: %X\n", resultado);
    //printf("Menos significativos: %X\n", menos_significativos);
    //printf("Más significativos: %X\n", mas_significativos);
    
    int me = (int)menos_significativos;
    int ma = (int)mas_significativos;
    
    setHexValue(&alu.acumulador,ma);
    setHexValue(&alu.MQ,me);
	
}

void HTL(int *n){
	uni.EnMarcha = 0x0;
}
void auxiliarDecodifi(int n){
	if(n == 0x0){
		uni.decodifi = &add;
		
	}else if(n == 0x1){
		uni.decodifi = &mul;
	}else if(n == 0x7){
		uni.decodifi = &LoadMX;
	}
	else if(n == 0x4){
		uni.decodifi = &storeMX;
	}
	else if(n == 0x6){
		uni.decodifi = &LoadMQ;
	}
	else if(n == 0x5){
		uni.decodifi = &Loadmq;
	}
	else if(n == 0xF){
		uni.decodifi = &HTL;
	}
	else{
		uni.decodifi = &HTL;
	}
}


int Primero4(int n){
	int highNibble = 0x0;
	if(n > 0xF){
		highNibble = (n & 0xF0) >> 4;
	}

	return highNibble;
}
int Ultimo4(int n){
	int lowNibble = 0x0;
	if(n > 0xF){
		lowNibble = (n & 0x0F);
	}
	else{
		lowNibble = n;
	}
	return lowNibble;
}

void imprimirUnidadDeControl(struct Unidad_de_control uni) {
    printf("Unidad de control:\n");
    printf("Contador: %X\n", uni.contador);
    printf("Instruccion: %X\n", uni.instruccion);


    //printf("Decodificador: %p\n", (void*)uni.decodifi);
    if(uni.decodifi == LoadMQ){
    	printf("Decodificador: LoadMQ \n");
	}
	else if(uni.decodifi == add){
		printf("Decodificador: ADD \n");
	}else if(uni.decodifi == mul){
		printf("Decodificador: MUL \n");
	}else if(uni.decodifi == LoadMX){
		printf("Decodificador: LOADMX \n");
	}else if(uni.decodifi == storeMX){
		printf("Decodificador: STOREMX \n");
	}else if(uni.decodifi == LoadMQ){
		printf("Decodificador: LOADMQ \n");
	}else if(uni.decodifi == Loadmq){
		printf("Decodificador: LOADmq \n");
	}else if(uni.decodifi == HTL){
		printf("Decodificador: HTL \n");
	}
	else {
		printf("¡¡¡¡sin funcion!!!!!\n");
	}
	
}

void imprimirMemoria(struct Memoria mem) {
    printf("Memoria:\n");
    printf("Direccion: %X\n", mem.direccion);
    printf("Dato: %X\n", mem.dato);
    printf("Tabla de memoria: ");
    for (int i = 0; i < 16; i++) {
        printf("%X ", mem.tabla_memoria[i]);
    }
    printf("\n");
}

void imprimirALU(struct ALU alu) {
    printf("ALU:\n");
    printf("Acumulador: %X\n", alu.acumulador);
    printf("Entrada: %X\n", alu.Entrada);
    printf("MQ: %X\n", alu.MQ);
}

void imprimir_componentes(struct Unidad_de_control uni, struct Memoria mem, struct ALU alu){
	printf("\n---------------------\n");
	imprimirUnidadDeControl(uni);
	printf("\n");
	imprimirMemoria(mem);
	printf("\n");
	imprimirALU(alu);
	printf("\n--------------------\n");
}
