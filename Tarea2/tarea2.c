#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>

void printHostname(void);
void printDatosCPU(void);
void printKernerVersion(void);
void printDate(void);

const struct option long_options[] = {
	{"cpuinfo",	0,	NULL,	'c'},
	{"kernel",	0,	NULL,	'k'},
	{NULL, 		0,	NULL,	 0}
};

const char * short_options = "ck";



int main(int argc, char **argv){
	
	printHostname();	//IMPRIME NOMBRE DE PC
	printDate();		//IMPRIME FECHA Y HORA

	char next_option;
	do{
		next_option =getopt_long(argc, argv, short_options, long_options, NULL);
		
		switch(next_option){
			case 'c':		// -c o --cpuinfo
				printDatosCPU();
				break;
			case 'k':		// -k o --kernel
				printKernerVersion();
				break;
			case '?':		//Opcion invalida
				printf("Opcion Invalida\n");
				break;
			case -1:		//No hay mas opciones
				break;
			default: 
				abort();
		}
	}while(next_option !=-1);

}

void printHostname(void){
	FILE * ptr = fopen("/proc/sys/kernel/hostname", "r");
	char info[30];
	fscanf(ptr, "%s", info);
	printf("NOMBRE DE PC: \t%s\n",info);
}

void printDatosCPU(void){

	system("../Tarea1/datoscpu");
}

void printKernerVersion(void){
	FILE * ptr = fopen("/proc/sys/kernel/version", "r");
	char info[500];
	//fscanf(ptr, "%[^n]", info);
	fgets(info, 500, ptr);
	printf("Version de Kernel: %s\n",info);

}

void printDate(void){
	time_t t;
	struct tm *tm;
	char fechahora[100];
	t=time(NULL);
	tm=localtime(&t);
	tm->tm_mon+=1;
	tm->tm_year+=1900;
	printf("Fecha: \t\t%d/%d/%d\n", tm->tm_mday, tm->tm_mon, tm->tm_year);	
	printf("Hora: \t\t%d:%d:%d\n",tm->tm_hour, tm->tm_min, tm->tm_sec);
}

