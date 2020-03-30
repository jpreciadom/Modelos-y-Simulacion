#include <stdio.h>
#include <stdlib.h>

//Entidades del sistema
struct Servidor {
    short Id_Servidor;
    short Estado_Ocupacion;
    int Total_Personas_Antendidas;
    long Tiempo_Uso;
    double Media_Atender;
};

struct Persona {
    long Tiempo_Llegada_Fila1;
    long Tiempo_Llegada_Servidor1;
    long Tiempo_Llegada_Fila2;
    long Tiempo_Llegada_Servidor2;
};

//Estado del sistema
long Reloj;
//Cola servidor 1
//Cola prioritaria servidor 1
//Cola servidor 2
//Cola prioritaria servidor 2

//Contadores y acumuladore
int Total_Clientes_Atendidos;
int Total_Personas_Cola1;
int Total_Personas_Cola2;
long Tiempo_Total_Esperado;

int main()
{
    printf("Hello world!\n");
    return 0;
}
