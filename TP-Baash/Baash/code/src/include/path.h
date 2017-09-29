
#ifndef PATH_H_
#define PATH_H_

int getDireccionesPath(char* path[]);
void buscarDireccionDeArchivo(char* arch, char* paths[], char *execPath);
int read_input(char* argv[], char* cadena);
int checkRedirect(char* argv[], char fileName[]);
int checkPipe(char* argv[], char* argv1[], char* argv2[]);
void ejecutarPipeline(char* argv1[], char* argv2[], char* paths[]);

#endif /* PATH_H_ */
