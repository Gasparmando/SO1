

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

#include "manipulacion_archivos.c"
#include "path.c"
#include "builtin.c"

#define LIGHT_CYAN "\033[1;36m"
#define COLOR_RESET "\x1b[0m"


void hostname(void);
void directorioActual(void);
void imprimirTiempoActual(void);
int whoAmI(void);
void prompt(void);



int background(char* argv[]);


int main(int argc, char *argv[],char *env[])
{
	int pid, statusPID, bgProcess, pipeExecuted; //Banderas de control
	int argC;
	char* argV[20];
	char* argv1[20]; //Para el caso que se haga un pipeline.
	char* argv2[20];
	char executePath[256]; //Path del proceso que se va a ejecutar.
	char comando [256]; //Comando que ingresa el usuario.

	int pathCounter;
	char *paths[20];
	//Obtengo todos los paths que están en la variable de entorno PATH; y su cantidad.
	pathCounter = getDireccionesPath(paths);

	int status=1, buil;
	while(status){
		buil=0;

		statusPID = 0;	   //Limpio las banderas
		pipeExecuted = 0;

		prompt();
		fgets(comando,256,stdin);

		//Si comando tiene solo un salto de línea, no hago nada.
		if(!strcmp(comando, "\n")){
			//printf("\n");
			status=1;
		}

		//Esto es lo que se ejecuta si se ingresa algun comando
		else{
			argC = read_input(argV, comando);

			int i=0;
			//Comandos internos del bash.
			for(i=0;i < lsh_num_builtins();i++)
			  	  {
				      if (strcmp(argV[0], builtin_str[i]) == 0) {
				        buil = (*builtin_func[i])(argV);
				        if(buil==0)
				        	exit(0);
				      }
			  	  }

			int flagRedirect = 0;
			int doPipe = 0;
			char fileName[50];

			//Verifica si se debe ejecutar un pipe
			doPipe = checkPipe(argV, argv1, argv2);
			flagRedirect = checkRedirect(argV, fileName);

			//Verifico si el proceso se tiene que ejecutar en Background.
			bgProcess = background(argV);
			if(bgProcess){
				argV[argC-1] = NULL;
				argC--;
			}

			buscarDireccionDeArchivo(argV[0], paths, executePath);  //Arma el directorio del ejecutable
			if(executePath[0] == 'X' && status==0)
				printf("El archivo no fue encontrado\n");
			else{
				pid = fork();
				if (pid<0) {
					perror("Creando el hijo");
					exit(1);
				}
				else if (pid == 0) {	//Proceso hijo.
					if(flagRedirect == 2){
						status = out_Redirect(fileName);
					}
					else if(flagRedirect == 1){
						status = in_Redirect(fileName);
						if(status!=0){
							close(status);
							status=1;
						}
						//freopen(fileName,"r",stdin);
					}
					else if(doPipe == 1){
						ejecutarPipeline(argv1, argv2, paths);
						pipeExecuted = 1;
					}
					if(!pipeExecuted){
						if(!strcmp(argV[0],"man"))
						{
							env[15]=NULL;
							execve(executePath, argV, env);
						}
						else if(execv(executePath, argV)== -1 && buil==0)
						perror("Error");
						exit(1);
					}
				}
				else{	//Proces padre.
					statusPID = -1;
				}
				if(bgProcess)
					waitpid(pid,&statusPID,WNOHANG);
				else
					waitpid(pid,&statusPID,0);
			}
		}
	}

	return 0;
}

//----------------------------------------------------------------
/**
 * @brief Verifica si se quiere lanzar el proceso en background.
 * @param argv Vector de argumentos que pasa el usuario. Se necesita para verificar si el ultimo es un &.
 * @return Devuelve 0 si el proceso no debe ejecutarse en background, y 1 en caso contrario.
 */
int background(char* argv[]){
	int i;
	for(i = 0; i < 20; i++){
		if(argv[i] == NULL)
			break;
	}
	if(!strcmp(argv[i-1], "&"))
		return 1;
	return 0;
}


void prompt()
{
	imprimirTiempoActual();

	int valorId;
	valorId = whoAmI();

	hostname();

	directorioActual();

	if(valorId==0)
		printf("# "COLOR_RESET);
	else 	printf("$ "COLOR_RESET);
	return;
}

//----------------------------------------------------------------
// Obtiene el nombre del equipo

void hostname(){
	char nombre[25];
	FILE *fd;

	fd = fopen("/proc/sys/kernel/hostname","r");
	fscanf(fd, "%[^\n]s", nombre);
	printf("%s:", nombre);
	fclose(fd);

	return;
}

//----------------------------------------------------------------
// Obtiene tiempo actual
void imprimirTiempoActual(){
	char valor[256];
	char hora[13];

	getValor("/proc/driver/rtc", valor, "rtc_time");
	sscanf(valor, "rtc_time : %s", hora);
	printf(LIGHT_CYAN "[%s]", hora);

	return;
}

//----------------------------------------------------------------
/**
   @brief Obtiene el tipo de usuario
   @param no aplica
   @return devuelve 0 para usuario root. !=0 para otro tipo de usuario
 */
int whoAmI() {

          struct passwd *pw= NULL;
          char *user = NULL;

          pw = getpwuid(geteuid());
          if (pw)
               user = pw->pw_name;
          else if ((user = getenv("USER")) == NULL) {
               fprintf(stderr, "No tengo idea!\n");
               return 1;
          }
          printf("%s@", user);
          return geteuid();
}


//----------------------------------------------------------------
// Muestra el directorio actua

void directorioActual()
{
	char *current_dir;
	char cwd[1024];
	char *home_var = getenv("HOME");

	if (getcwd(cwd, sizeof(cwd)) != NULL){
		current_dir = cwd;

		if(strncmp(cwd, home_var, strlen(home_var)) == 0) {
					current_dir = current_dir + strlen(home_var);

					printf("~%s", current_dir);
				} else {

					printf("%s", current_dir);
				}
	}
	else
	    perror("getcwd() error: ");
	return;
}
