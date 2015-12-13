#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

void menuPrincipal();	
int numeroAleatorio(int min, int max);
void llenarArregloAlien(int* alien, int maximo, int minimo, int tamano);
void llenarArregloPlaneta(int* planeta, int maximo, int minimo, int tamano);
void juego(int opcion);
void imprimirArreglo(int arr[], int n);

//Cambio: Comprobar derrota ahora recibe la nueva nave como parametro
int comprobarDerrota(int aliens[], int aliens2[], int planeta[], int tamanoAlien, int tamanoPlaneta);

//Cambio: Planeta ahora dispara a alien 2 tambien
void disparar_Planeta(int* aliens, int* aliens2, int tamanoAlien, int maximo, int minimo);
void disparar_Aliens(int* planeta, int tamanoPlaneta, int maximo, int minimo);

//Cambio: Imprimir resultados ahora recibe nueva nave como parametro
void imprimirResultados(int disparos, int* aliens, int* aliens2, int* planeta, int tamAliens, int tamPlaneta);

//Cambio: Disparo de aliens 2
void disparar_Aliens2(int* planeta, int tamanoPlaneta, int maximoPlaneta, int minimoPlaneta);

static int rangoMin_Planeta = 2;	// Minimo numero aleatorio para asignar a las celdas de los planetas
static int rangoMax_Planeta = 18;	// Maximo numero aleatorio para asignar a las celdas de los planetas
static int rangoMin_Alien = 3;		// Minimo numero aleatorio para asignar a las celdas de los aliens
static int rangoMax_Alien = 12;		// Maximo numero aleatorio para asignar a las celdas de los aliens
static int maxDisparos = 20;		// Maximo numero de disparos antes de validar la tregua
static int tamanoAlien = 10;		// Tamano del arreglo de aliens
static int tamanoPlaneta = 12;		// Tamano del arreglo del planeta
static int arregloAlien[10];		// Arreglo de los aliens (debe coincidir con tamanoAlien)

//Cambio: Nueva nave
static int arregloAlien2[10];		// Arreglo de segunda nave de los aliens

static int arregloPlaneta[12];		// Arreglo del planeta (debe coincidir con tamanoPlaneta)
static float defensa = 0.75;		// Condicion para perder el juego, porcentaje de defensa restante
static char disparo;						// Tecla para disparar
static int disparosTotal;			// Numero total de disparos realizados en una ronda

void main()
{
	int opcion = 0;
	while (opcion != 4){
		menuPrincipal();	// Muestra el menu principal del juego y permite al usuario ingresar opcion
		scanf_s("%d", &opcion);	// Recibe la opcion ingresada por teclado
		juego(opcion);			// Envia la opcion como parametro y efectua la accion correspondiente
	}
}

// Funcion que presenta el menu principal del juego
void menuPrincipal(){
	printf("\n============= D D C A ==============\n");
	printf("========= Invasion alienigena! =========\n");
	printf("\n1. Activar defensas.\n");

	printf("2. Empezar combate.\n");

	printf("3. Mostrar Resumen Final.\n");

	printf("4. Salir.\n");
	printf("\n========================================\n");
}

// Funcion que maneja todos las funcionalidades del juego, recibe como parametro la opcion del menu
void juego(int opcion){
	int disparos = 0;

	switch (opcion){			// Validacion de la opcion del juego

	case 1:	// Opcion 1: Se llenan los arreglos del planeta y de los aliens con numeros aleatorios no repetidos
		printf("\nActivando defensas...\n");
		Sleep(1000);
		printf("\n===== ALIENS 1 =====\n");
		llenarArregloAlien(&arregloAlien[0], rangoMax_Alien, rangoMin_Alien, tamanoAlien);
		printf("\n");
		Sleep(1000);

		// Cambio: Inicializa nueva nave
		printf("\n===== ALIENS 2 =====\n");
		llenarArregloAlien(&arregloAlien2[0], rangoMax_Alien, rangoMin_Alien, tamanoAlien);
		printf("\n");

		Sleep(1000);
		printf("\n======= TIERRA =======\n");
		llenarArregloPlaneta(&arregloPlaneta[0], rangoMax_Planeta, rangoMin_Planeta, tamanoPlaneta);
		printf("\nLas defensas han sido activadas!\n");
		disparosTotal = 0;
		break;
	
	case 2:	// Si la opcion es 2, empieza el juego (una vez que se inicializan los arreglos)
		if (comprobarDerrota(arregloAlien, arregloAlien2, arregloPlaneta, tamanoAlien, tamanoPlaneta) == 0){
			printf("Empezando el combate!!!\n");
			Sleep(1000);
			imprimirArreglo(arregloAlien, tamanoAlien);
			printf("\n");

			// Cambio: Imprime nueva nave
			imprimirArreglo(arregloAlien2, tamanoAlien);
			printf("\n");

			imprimirArreglo(arregloPlaneta, tamanoPlaneta);
			printf("\nDispare con la tecla 'd' seguido de la tecla ENTER!\n");
			// Valida si hay tregua o derrota para poder continuar con la batalla
			while ((disparos < maxDisparos) && (comprobarDerrota(arregloAlien, arregloAlien2, arregloPlaneta, tamanoAlien, tamanoPlaneta) == 0)){
				scanf_s("%c", &disparo);
				switch (disparo){
				case 'd':
					disparos++;
					int multiplo5 = disparos % 5;
					if (comprobarDerrota(arregloAlien, arregloAlien2, arregloPlaneta, tamanoAlien, tamanoPlaneta) != 1){
						disparar_Planeta(arregloAlien, arregloAlien2, tamanoAlien, rangoMax_Alien, rangoMin_Alien);
						if (comprobarDerrota(arregloAlien, arregloAlien2, arregloPlaneta, tamanoAlien, tamanoPlaneta) != 2)
							disparar_Aliens(arregloPlaneta, tamanoPlaneta, rangoMax_Planeta, rangoMin_Planeta);
						if (multiplo5 == 0 && comprobarDerrota(arregloAlien, arregloAlien2, arregloPlaneta, tamanoAlien, tamanoPlaneta) != 2){
							printf("Se activo el escudo! Se protegen los numeros 4, 5, 6 y 7!\n");
							disparar_Aliens2(arregloPlaneta, tamanoPlaneta, rangoMax_Planeta, rangoMin_Planeta);
						}
							
					}
					printf("Numero de disparos: %d\n", disparos);
					printf("\n======  ALIENS ======\n");
					imprimirArreglo(arregloAlien, tamanoAlien);
					printf("\n");
					printf("\n======  ALIENS 2 ======\n");
					imprimirArreglo(arregloAlien2, tamanoAlien);
					printf("\n");
					printf("\n=========  TIERRA =========\n");
					imprimirArreglo(arregloPlaneta, tamanoPlaneta);
					printf("\n");
					break;
				default:
					break;
				}
				// Comprueba si hay tregua
				if (disparos == maxDisparos && comprobarDerrota(arregloAlien, arregloAlien2, arregloPlaneta, tamanoAlien, tamanoPlaneta) == 0)	// Condicion de tregua alcanzada
					printf("Tregua!\n");
				// Comprueba si gana la tierra, pierden los aliens
				if (comprobarDerrota(arregloAlien, arregloAlien2, arregloPlaneta, tamanoAlien, tamanoPlaneta) == 1)
					printf("Derrotados los aliens! El planeta defendio la invasion extraterrestre!\n");
				// Comprueba si ganan los aliens, pierde la tierra
				if (comprobarDerrota(arregloAlien, arregloAlien2, arregloPlaneta, tamanoAlien, tamanoPlaneta) == 2)
					printf("Los aliens han conquistado el planeta...\n");
				disparosTotal = disparos; // Asigna el total de disparos realizados en esta ronda
			}
		}
		// Si existe una tregua o una derrota, el juego termina
		else
			printf("\nEl juego ha terminado, volver a activar las defensas!\n");
		break;
	
	case 3:	// Si la opcion es 3, muestra el resultado final de la ultima batalla
		printf("\nMostrando resultado final...\n");
		Sleep(1000);
		imprimirResultados(disparosTotal, arregloAlien, arregloAlien2, arregloPlaneta, tamanoAlien, tamanoPlaneta);
		break;
	
	case 4:	// Si la opcion es 4, sale del juego
		printf("Gracias por jugar! \nSaliendo del juego...\n");
		Sleep(2000);			// El programa espera 2 segundos antes de salir
		exit(EXIT_SUCCESS);		// Salir del programa exitosamente
		break;
	
	default:
		break;

	}
}

// Funcion que retorna un numero aleatorio entre un rango de valores
int numeroAleatorio(int minimo, int maximo){
	return rand() % (maximo - minimo + 1) + minimo;
}

// Funcion que llena el arreglo de los aliens con numeros aleatorios no repetidos
void llenarArregloAlien(int *alien, int maximo, int minimo, int tamano){
	int x, p;
	int count;
	int i = 0;
	srand(time(NULL));
	for (count = 0; count<tamano; count++){
		alien[count] = numeroAleatorio(minimo, maximo);
	}

	while (i<tamano){
		int r = numeroAleatorio(minimo,maximo);

		for (x = 0; x < i; x++)
		{
			if (alien[x] == r){
				break;
			}
		}
		if (x == i){ // Si el numero YA EXISTE, utilizar otro numero
			alien[i++] = r;
		}
	}
	for (p = 0; p<tamano; p++){
		printf("[%d] ", alien[p]);
	}
}

//Funcion que llena el arreglo del planeta con numeros aleatorios no repetidos
void llenarArregloPlaneta(int *planeta, int maximo, int minimo, int tamano){
	int x, p;
	int count;
	int i = 0;
	srand(time(NULL));
	for (count = 0; count<tamano; count++){
		planeta[count] = numeroAleatorio(minimo, maximo);
	}

	while (i<tamano){
		int r = numeroAleatorio(minimo, maximo);

		for (x = 0; x < i; x++)
		{
			if (planeta[x] == r){
				break;
			}
		}
		if (x == i){
			planeta[i++] = r;
		}
	}
	for (p = 0; p<tamano; p++){
		printf("[%d] ", planeta[p]);
	}
}

//Funcion que se encarga de disparar (planeta hacia aliens) un numero aleatorio entre un rango de valores
void disparar_Planeta(int* aliens, int* aliens2, int tamanoAlien, int maximoAlien, int minimoAlien){
	int valorDisparo = numeroAleatorio(minimoAlien, maximoAlien);
	int acierto = 0;
	printf("La tierra ha disparado el numero %d!!\n", valorDisparo);
	/*
	if (aliens[valorDisparo - 1] == 0 || (valorDisparo-1)>tamanoAlien)
		printf("La tierra ha fallado el disparo...\n");
	else{
		aliens[valorDisparo-1] = 0;
		printf("La tierra ha debilitado la defensa de los aliens!\n");
	}
	*/
	//Recorre el arreglo de los aliens, busca el numero del disparo y elimina la celda correspondiente
	for (int i = 0; i < tamanoAlien; i++){
		if (aliens[i] == valorDisparo){
			aliens[i] = 0;
			acierto = 1;
		}
	}

	for (int i = 0; i < tamanoAlien; i++){
		if (aliens2[i] == valorDisparo){
			aliens2[i] = 0;
			acierto = 1;
		}
	}

	if (acierto == 1){
		printf("La tierra ha debilitado la defensa de los aliens!\n");
	}
	else
		printf("La tierra ha fallado el disparo...\n"); 
}

//Funcion que se encarga de disparar (aliens hacia planeta) un numero aleatorio entre un rango de valores
void disparar_Aliens(int* planeta, int tamanoPlaneta, int maximoPlaneta, int minimoPlaneta){
	int valorDisparo = numeroAleatorio(minimoPlaneta, maximoPlaneta);
	int acierto = 0;
	printf("Los aliens han disparado el numero %d!!\n", valorDisparo);
	/*
	if (planeta[valorDisparo-1] == 0 || valorDisparo-1>tamanoPlaneta)
		printf("Los aliens han fallado el disparo!\n");
	else{
		planeta[valorDisparo-1] = 0;
		printf("Los aliens han debilitado la defensa del planeta...\n");
	}
	*/
	
	for (int i = 0; i < tamanoPlaneta; i++){
		if (planeta[i] == valorDisparo){
			planeta[i] = 0;
			acierto = 1;
		}
	}	
	if (acierto == 1){
		printf("Los aliens han debilitado la defensa del planeta...\n");
	}
	else
		printf("Los aliens han fallado el disparo!\n");
}

void disparar_Aliens2(int* planeta, int tamanoPlaneta, int maximoPlaneta, int minimoPlaneta){
	int valorDisparo = numeroAleatorio(minimoPlaneta, maximoPlaneta);
	int acierto = 0;
	printf("La NUEVA NAVE ha disparado el numero %d!!\n", valorDisparo);
	/*
	if (planeta[valorDisparo-1] == 0 || valorDisparo-1>tamanoPlaneta)
	printf("Los aliens han fallado el disparo!\n");
	else{
	planeta[valorDisparo-1] = 0;
	printf("Los aliens han debilitado la defensa del planeta...\n");
	}
	*/
	if (valorDisparo < 4 || valorDisparo > 7){
		for (int i = 0; i < tamanoPlaneta; i++){
			if (planeta[i] == valorDisparo){
				planeta[i] = 0;
				acierto = 1;
			}
		}
	}
	
	if (acierto == 1){
		printf("Los aliens han debilitado la defensa del planeta...\n");
	}
	else
		printf("Los aliens han fallado el disparo!\n");
}

//Funcion que comprueba si hay una derrota entre los dos equipos. 
//Perdio Aliens = 1, Perdio Tierra = 2, Empate = 0
int comprobarDerrota(int aliens[], int aliens2[], int planeta[], int tamanoAlien, int tamanoPlaneta){
	int derrotado = 0;
	float contadorAlien = 0; // Contador de ceros de los aliens
	float contadorPlaneta = 0; // Contador de ceros del planeta

	//Cambio: Contador de ceros para nueva nave
	float contadorAlien2 = 0;

	for (int i = 0; i < tamanoAlien; i++){
		if (aliens[i] == 0)
			contadorAlien++;
	}

	//Cambio: Contando ceros de la nueva nave
	for (int i = 0; i < tamanoAlien; i++){
		if (aliens2[i] == 0)
			contadorAlien2++;
	}

	for (int i = 0; i < tamanoPlaneta; i++){
		if (planeta[i] == 0)
			contadorPlaneta++;
	}

	float tasaAlien = (contadorAlien / tamanoAlien);

	//Cambio: Calculo de defensa destruida de nueva nave
	float tasaAlien2 = (contadorAlien2 / tamanoAlien);

	float tasaPlaneta = (contadorPlaneta / tamanoPlaneta);
	
	// Verificamos si el porcentaje de defensa destruida es mayor o igual al permitido
	//Cambio: Ahora se comprueba si AMBAS naves tienen defensa destruida mayor o igual al permitido
	if (tasaAlien >= defensa && tasaAlien2 >= defensa)
		derrotado = 1;

	if (tasaPlaneta >= defensa)
		derrotado = 2;

	return derrotado;
}

//Funcion que imprime los resultados del juego
void imprimirResultados(int disparos, int* aliens, int* aliens2, int* planeta, int tamAliens, int tamPlaneta){
	printf("\nNumero de disparos realizados: %d\n", disparos);
	
	//Cambio: LOS MISMOS CAMBIOS REALIZADOS EN COMPROBAR DERROTA

	float contadorAlien = 0;
	float contadorAlien2 = 0;
	float contadorPlaneta = 0;

	for (int i = 0; i < tamanoAlien; i++){
		if (aliens[i] == 0)
			contadorAlien++; // Cuenta el numero de CEROS en el arreglo de aliens
	}

	for (int i = 0; i < tamanoAlien; i++){
		if (aliens2[i] == 0)
			contadorAlien2++; // Cuenta el numero de CEROS en el arreglo de aliens
	}

	for (int i = 0; i < tamanoPlaneta; i++){
		if (planeta[i] == 0) // Cuenta el numero de CEROS en el arreglo del planeta
			contadorPlaneta++;
	}

	float tasaAlien = (contadorAlien / tamanoAlien);  // Calcula el % de defensa restante de los aliens
	float tasaAlien2 = (contadorAlien2 / tamanoAlien);  // Calcula el % de defensa restante de los aliens nave 2
	float tasaPlaneta = (contadorPlaneta / tamanoPlaneta);	// Calcula el % de defensa restante del planeta

	printf("Porcentaje de defensa derrotada (Planeta): %1.0f%%\n", (tasaPlaneta)*100);
	printf("Porcentaje de defensa derrotada (Aliens): %1.0f%%\n", (tasaAlien)*100);
	
	if (disparos >= 20 && tasaPlaneta < defensa && tasaAlien < defensa) // Valida si hubo tregua
		printf("La ultima batalla resulto en una tregua.\n");

	if (disparos <= 20 && tasaPlaneta >= defensa) // Valida si ganaron los aliens
		printf("Ultima batalla: Los aliens conquistaron el planeta...\n");

	if (disparos <= 20 && tasaAlien >= defensa) // Valida si gano la tierra
		printf("Ultima batalla: Los aliens fueron derrotados!\n");
}

// Funcion que imprime un arreglo
void imprimirArreglo(int arreglo[], int tamano){
	printf("\n");
	for (int i = 0; i < tamano; i++)
		printf("[%d] ", arreglo[i]);
	printf("\n");
}