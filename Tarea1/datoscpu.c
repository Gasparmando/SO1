#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 255

/*Datos del Cpu que se requieren*/
struct cpu{
	char tipocpu[SIZE];
	char model[SIZE];
	char cache[SIZE];
	char cores[SIZE];
};



void getLinea( FILE * ff, char * valor, char* llave);

/*Imprime los datos del CPU*/
void printcpu(struct cpu cp){
	printf("Tipo de CPU: \t%s \nModelo: \t%s \nCaché Size: \t%s \nCPU Cores: \t%s\n",cp.tipocpu,cp.model,cp.cache,cp.cores);
}


/*-----------------------------------------------------------------------------------------------*/

int main(int argc, char* argv[]){

	struct cpu mycpu;
	FILE * cpuinfo;
	cpuinfo=fopen("/proc/cpuinfo","r") ;
	char * buffer[SIZE];

	/*Procesador*/
	getLinea(cpuinfo, buffer, "vendor_id");
	sscanf(buffer, "vendor_id : %[^\n]",mycpu.tipocpu );

	/*Modelo*/
	getLinea(cpuinfo, buffer, "model name");
	sscanf(buffer, "model name : %[^\n]",mycpu.model );
	//mycpu->model = strchr(buffer, ":");

	/*Memoria Chache*/
	getLinea(cpuinfo, buffer, "cache size");
	//mycpu.cache = buffer;
	sscanf(buffer, "cache size : %[^\n]",mycpu.cache );

	/*Numero de Cores*/
	getLinea(cpuinfo, buffer, "cpu cores");
	sscanf(buffer, "cpu cores : %[^\n]",mycpu.cores );

	printcpu(mycpu);
	fclose(cpuinfo);


	return EXIT_SUCCESS;
}


void getLinea(FILE * fd, char* valor, char* llave){
	char buffer[500];
	char* match = NULL;


	while(!feof(fd)){
		fgets(buffer, 500, fd);
		match = strstr(buffer, llave);
		if(match!=NULL)
			break;
	}

	strcpy(valor,match);

	return;
}
