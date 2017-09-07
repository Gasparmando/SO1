#include <stdio.h>
#include <stdlib.h>

char *paths;		//String donde se almacena el valor de la variable de entorno PATH
char *rutas[];
int n;

int get_path(){
	
//	printf("PATH: %s\n", getenv("PATH"));
	paths = getenv("PATH");

	char *token;
	token = strtok (paths, ":");

	int pos=0;
	while (token != NULL) {
		rutas[pos] = token;
		pos++;
		
		token = strtok(NULL, ":");
	}
	rutas[pos] = '\0';
	return pos;

}

int main(){
	n=get_path();
	printf("%s\n", paths);

	for (int i =0; i<n;i++)
		printf("%s\n",rutas[i]);
	

}

