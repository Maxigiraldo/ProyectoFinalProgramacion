//Presentado por: Juan David García Arce y Maximiliano Giraldo Ocampo

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

//Se definen colores
#define RED     "\x1b[31m"
#define YELLOW  "\x1b[33m"
#define GREEN   "\033[32m"      /* Green */
#define RESET   "\x1b[0m"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>

char getch() {
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}
#endif

struct Usuario
{
    char nombre[50];
    char contrasena[50];
    char correo[50];
    char ocupacion[50];
    int voto;
};

struct Candidato
{
    int tarjeton;
    char nombre[50];
    int votos;
    int votos_estudiantes;
    int votos_docentes;
    int votos_egresados;
    int votos_administrativos;
    int votos_consejo;
    int estado;
};



typedef struct {
    int id;
    char* role;
} User;

bool validarCredenciales(char correo[], char contrasena[], char ocupacion[], char nombreusuario[]) {
    FILE *usuarios;
    usuarios = fopen("usuarios.txt", "r");
    
    if (usuarios == NULL) {
        printf(RED"Error al abrir el archivo de usuarios.\n"RESET);
        return false;
    }
    
    struct Usuario usuario;

    
    while (fscanf(usuarios, "%[^,],%[^,],%[^,],%[^,],%d.%*c\n",
                  usuario.nombre, usuario.contrasena, usuario.correo, usuario.ocupacion, &usuario.voto) == 5) {
        if (strcmp(usuario.correo, correo) == 0 && strcmp(usuario.contrasena, contrasena) == 0 && strcmp(usuario.ocupacion, ocupacion) == 0) {
            if (usuario.voto == 0) {
                printf("\n%s", usuario.nombre);
                strcpy(nombreusuario, usuario.nombre);
                fclose(usuarios);
                return true; 
            }
            else
            {
                printf("\nSolo puede votar una vez\n");
                fclose(usuarios);
                return false; 
            }
            
        }
    }
    
    fclose(usuarios);
    return false; 
}

void ingresodevoto(char nombreusuario[], char ocupacion[]){
    FILE *candidatos;
    candidatos = fopen("candidatos.txt", "r+");

    int voto;
    int tarjeton;

    struct Candidato candidato[6];

    //Se llena el arreglo de estructuras con los datos del archivo
    int i = 0;
    while (fscanf(candidatos, "%d,%[^,],%d,%d,%d,%d,%d,%d,%d.\n", &candidato[i].tarjeton, candidato[i].nombre, &candidato[i].votos, &candidato[i].votos_estudiantes, &candidato[i].votos_docentes, &candidato[i].votos_egresados, &candidato[i].votos_administrativos, &candidato[i].votos_consejo, &candidato[i].estado) == 9) {
        i++;
    }

    printf("\tCANDIDATOS\n");
    printf("\n1. Luis Fernando Gaviria Trujillo");
    printf("\n2. Carlos Alfonso Victoria Mena");
    printf("\n3. Giovanni Arias");
    printf("\n4. Xiomara Rocío González Ordoñez");
    printf("\n5. Alexander Molina Cabrera");
    printf("\n6. Juan Carlos Gutiérrez Arias");

    printf("\nIngrese el numero de su candidato:\t");
    scanf("%d", &voto);
    
    //Se busca el candidato y se le suma un voto a el y a su grupo
    for (int i = 0; i < 6; i++) {
        if (voto == candidato[i].tarjeton) {
            candidato[i].votos++;
            if (strcmp(ocupacion, "Estudiante") == 0) {
                candidato[i].votos_estudiantes++;
            } else if (strcmp(ocupacion, "Docente") == 0) {
                candidato[i].votos_docentes++;
            } else if (strcmp(ocupacion, "Egresado") == 0) {
                candidato[i].votos_egresados++;
            } else if (strcmp(ocupacion, "Administrativo") == 0) {
                candidato[i].votos_administrativos++;
            }
            //Se actualiza este archivo borrando todo y volviendo a escribirlo
            fseek(candidatos, 0, SEEK_SET);
            fprintf(candidatos, "1,Luis Fernando Gaviria Trujillo,%d,%d,%d,%d,%d,%d,%d.\n", candidato[0].votos, candidato[0].votos_estudiantes, candidato[0].votos_docentes, candidato[0].votos_egresados, candidato[0].votos_administrativos, candidato[0].votos_consejo, candidato[0].estado);
            fprintf(candidatos, "2,Carlos Alfonso Victoria Mena,%d,%d,%d,%d,%d,%d,%d.\n", candidato[1].votos, candidato[1].votos_estudiantes, candidato[1].votos_docentes, candidato[1].votos_egresados, candidato[1].votos_administrativos, candidato[1].votos_consejo, candidato[1].estado);
            fprintf(candidatos, "3,Giovanni Arias,%d,%d,%d,%d,%d,%d,%d.\n", candidato[2].votos, candidato[2].votos_estudiantes, candidato[2].votos_docentes, candidato[2].votos_egresados, candidato[2].votos_administrativos, candidato[2].votos_consejo, candidato[2].estado);
            fprintf(candidatos, "4,Xiomara Rocío González Ordoñez,%d,%d,%d,%d,%d,%d,%d.\n", candidato[3].votos, candidato[3].votos_estudiantes, candidato[3].votos_docentes, candidato[3].votos_egresados, candidato[3].votos_administrativos, candidato[3].votos_consejo, candidato[3].estado);
            fprintf(candidatos, "5,Alexander Molina Cabrera,%d,%d,%d,%d,%d,%d,%d.\n", candidato[4].votos, candidato[4].votos_estudiantes, candidato[4].votos_docentes, candidato[4].votos_egresados, candidato[4].votos_administrativos, candidato[4].votos_consejo, candidato[4].estado);
            fprintf(candidatos, "6,Juan Carlos Gutiérrez Arias,%d,%d,%d,%d,%d,%d,%d.\n", candidato[5].votos, candidato[5].votos_estudiantes, candidato[5].votos_docentes, candidato[5].votos_egresados, candidato[5].votos_administrativos, candidato[5].votos_consejo, candidato[5].estado);
            //Se borra el resto del archivo
            ftruncate(fileno(candidatos), ftell(candidatos));
            break;
        }
    }
    
    fclose(candidatos);

    FILE *usuarios;
    usuarios = fopen("usuarios.txt", "r+");

    printf("%s", nombreusuario);

    struct Usuario usuario;

    // Encuentra al usuario y marca que ya votó
    while (fscanf(usuarios, "%[^,],%*[^,],%[^,],%*[^,],%d.\n", usuario.nombre, usuario.correo, &usuario.voto) == 3) {
        if (strcmp(usuario.nombre, nombreusuario) == 0 && usuario.voto == 0) {
            fseek(usuarios, -4, SEEK_CUR); 
            fprintf(usuarios, "%d.", 1); 
            break;
        }
    }

    fclose(usuarios);

    printf("\nVoto realizado\n");
}



void mostrarMenu(int opcion) {
    system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows
    printf("Bienvenido al sistema de votaciones:\n");
    if (opcion == 0) {
        printf(">> Ingresar\n");
        printf("   Consejo superior\n");
        printf("   Admin\n");
        printf("   Salir\n");
    } else if (opcion == 1) {
        printf("   Ingresar\n");
        printf(">> Consejo superior\n");
        printf("   Admin\n");
        printf("   Salir\n");
    } else if (opcion == 2) {
        printf("   Ingresar\n");
        printf("   Consejo superior\n");
        printf(">> Admin\n");
        printf("   Salir\n");
    } else {
        printf("   Ingresar\n");
        printf("   Consejo superior\n");
        printf("   Admin\n");
        printf(">> Salir\n");
    }
}

void menugrupo( int opcion){
    system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows
    printf("Ingrese el grupo al que pertenece: \n");
    if (opcion == 0) {
        printf(">> Administrativo\n");
        printf("   Docente\n");
        printf("   Egresado\n");
        printf("   Estudiante\n");
        printf("   Atras\n");
    } else if (opcion == 1) {
        printf("   Administrativo\n");
        printf(">> Docente\n");
        printf("   Egresado\n");
        printf("   Estudiante\n");
        printf("   Atras\n");
    } else if (opcion == 2) {
        printf("   Administrativo\n");
        printf("   Docente\n");
        printf(">> Egresado\n");
        printf("   Estudiante\n");
        printf("   Atras\n");
    } else if (opcion == 3) {
        printf("   Administrativo\n");
        printf("   Docente\n");
        printf("   Egresado\n");
        printf(">> Estudiante\n");
        printf("   Atras\n");
    } else {
        printf("   Administrativo\n");
        printf("   Docente\n");
        printf("   Egresado\n");
        printf("   Estudiante\n");
        printf(">> Atras\n");
    }
}

void MenuVotar (int opcion) {
    system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows
    printf("\nQue desea hacer?\n");
    if (opcion == 0) {
        printf(">> Votar\n");
        printf("   Salir\n");
    } else {
        printf("   Votar\n");
        printf(">> Salir\n");
    }
}

void MenuAdmin (int opcion) {
    system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows
    if (opcion == 0) {
        printf(">> Ver votos\n");
        printf("   Ver histograma\n");
        printf("   Mostrar terna\n");
        printf("   Ver ganador\n");
        printf("   Salir\n");
    } else if (opcion == 1) {
        printf("   Ver votos\n");
        printf(">> Ver histograma\n");
        printf("   Mostrar terna\n");
        printf("   Ver ganador\n");
        printf("   Salir\n");
    } else if (opcion == 2) {
        printf("   Ver votos\n");
        printf("   Ver histograma\n");
        printf(">> Mostrar terna\n");
        printf("   Ver ganador\n");
        printf("   Salir\n");
    } else if (opcion == 3) {
        printf("   Ver votos\n");
        printf("   Ver histograma\n");
        printf("   Mostrar terna\n");
        printf(">> Ver ganador\n");
        printf("   Salir\n");
    } else {
        printf("   Ver votos\n");
        printf("   Ver histograma\n");
        printf("   Mostrar terna\n");
        printf("   Ver ganador\n");
        printf(">> Salir\n");
    }
}

void MenuConsejo(int opcion, User users[]) {
    system("clear || cls");

    printf("Seleccione una opción:\n");

    for (int i = 0; i < 10; ++i) {
        if (opcion == i) {
            printf(">> %d. %s\n", users[i].id, users[i].role);
        } else {
            printf("   %d. %s\n", users[i].id, users[i].role);
        }
    }
}


void menuUsuario(char nombreusuario[], char ocupacion[]){
    int op = 0;
    char ch;
    do {
        MenuVotar(op);

        ch = getch();

        if (ch == 27) {  // Comprobamos si es una secuencia de escape (código para flechas en algunas terminales)
            getch(); // Descartamos el siguiente carácter
            switch (getch()) {
                case 65:  // Flecha arriba
                    op = (op - 1 + 2) % 2;
                    break;
                case 66:  // Flecha abajo
                    op = (op + 1) % 2;
                    break;
            }
        }

        system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows

    } while (ch != 10);  //  es el código ASCII para la tecla Enter
    switch (op){
        case 0:
            ingresodevoto(nombreusuario, ocupacion);
            break;
        default:
            printf("Cerrando...\n");
            break;
    }
}

void ingresar() {
    char contrasena[50];
    char correo[50];
    char ocupacion[50];
    int g = 0;
    char ch;
    do {
        menugrupo(g);

        ch = getch();

        if (ch == 27) {  // Comprobamos si es una secuencia de escape (código para flechas en algunas terminales)
            getch(); // Descartamos el siguiente carácter
            switch (getch()) {
                case 65:  // Flecha arriba
                    g = (g - 1 + 5) % 5;
                    break;
                case 66:  // Flecha abajo
                    g = (g + 1) % 5;
                    break;
            }
        }

        system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows

    } while (ch != 10);  //  es el código ASCII para la tecla Enter
    switch (g){
        case 0:
            strcpy(ocupacion, "Administrativo");
            break;
        case 1:
            strcpy(ocupacion, "Docente");
            break;  
        case 2:
            strcpy(ocupacion, "Egresado");
            break;
        case 3:
            strcpy(ocupacion, "Estudiante");
            break;
    }

    if (g == 4) {
        return;
    }

    //muestra la ocupacion
    // printf("%s", ocupacion);

    printf("Ingrese su correo: ");
    scanf("%s", correo);

    printf("Ingrese su contraseña: ");

    int i = 0;
    while (i < 49) {
        char ch = getch();

        if (ch == '\n' || ch == '\r') {
            contrasena[i] = '\0';
            break;
        } else if (ch == 8) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            contrasena[i] = ch;
            i++;
            printf("*");
        }
    }

    char nombreusuario[50];

    if (validarCredenciales(correo, contrasena, ocupacion, nombreusuario)) {
        printf("\nBienvenido.\n");
        menuUsuario(nombreusuario, ocupacion);

    } else {
        printf("\nHubo un error al verificar datos.\n");
        printf("\nVuelva a intentarlo\n");
        sleep(2);
        ingresar(); // Reintenta el ingreso
    }
}


void VotarConsejoSuperior(char rol[]) {
    FILE *candidatos;
    candidatos = fopen("candidatos.txt", "r+");

    int voto;
    int tarjeton;

    struct Candidato candidato[6];

    //Se llena el arreglo de estructuras con los datos del archivo
    int i = 0;
    while (fscanf(candidatos, "%d,%[^,],%d,%d,%d,%d,%d,%d,%d.\n", &candidato[i].tarjeton, candidato[i].nombre, &candidato[i].votos, &candidato[i].votos_estudiantes, &candidato[i].votos_docentes, &candidato[i].votos_egresados, &candidato[i].votos_administrativos, &candidato[i].votos_consejo, &candidato[i].estado) == 9) {
        i++;
    }
    
    printf("\tCANDIDATOS\n");
    for (i = 0; i < 6; i++){
        if (i == 0 && candidato[i].estado == 1){
            printf("\n1. Luis Fernando Gaviria Trujillo");
        } else if (i == 1 && candidato[i].estado == 1){
            printf("\n2. Carlos Alfonso Victoria Mena");
        } else if (i == 2 && candidato[i].estado == 1){
            printf("\n3. Giovanni Arias");
        } else if (i == 3 && candidato[i].estado == 1){
            printf("\n4. Xiomara Rocío González Ordoñez");
        } else if (i == 4 && candidato[i].estado == 1){
            printf("\n5. Alexander Molina Cabrera");
        } else if (i == 5 && candidato[i].estado == 1){
            printf("\n6. Juan Carlos Gutiérrez Arias");
        }
    }
    
    printf("\nIngrese el numero de su candidato:\t");
    scanf("%d", &voto);
    
    bool Coincidencia = false;
    
    for (i = 0; i < 6; i++){
        if (voto == candidato[i].tarjeton){
            if (candidato[i].estado == 1){
                Coincidencia = true;
                break;
            }
        }
    }
    
    if (Coincidencia == true){
        //Se busca el candidato y se le suma un voto a el y a consejo
        for (int i = 0; i < 6; i++) {
            if (voto == candidato[i].tarjeton) {
                candidato[i].votos++;
                candidato[i].votos_consejo++;
                //Se actualiza este archivo borrando todo y volviendo a escribirlo
                fseek(candidatos, 0, SEEK_SET);
                fprintf(candidatos, "1,Luis Fernando Gaviria Trujillo,%d,%d,%d,%d,%d,%d,%d.\n", candidato[0].votos, candidato[0].votos_estudiantes, candidato[0].votos_docentes, candidato[0].votos_egresados, candidato[0].votos_administrativos, candidato[0].votos_consejo, candidato[0].estado);
                fprintf(candidatos, "2,Carlos Alfonso Victoria Mena,%d,%d,%d,%d,%d,%d,%d.\n", candidato[1].votos, candidato[1].votos_estudiantes, candidato[1].votos_docentes, candidato[1].votos_egresados, candidato[1].votos_administrativos, candidato[1].votos_consejo, candidato[1].estado);
                fprintf(candidatos, "3,Giovanni Arias,%d,%d,%d,%d,%d,%d,%d.\n", candidato[2].votos, candidato[2].votos_estudiantes, candidato[2].votos_docentes, candidato[2].votos_egresados, candidato[2].votos_administrativos, candidato[2].votos_consejo, candidato[2].estado);
                fprintf(candidatos, "4,Xiomara Rocío González Ordoñez,%d,%d,%d,%d,%d,%d,%d.\n", candidato[3].votos, candidato[3].votos_estudiantes, candidato[3].votos_docentes, candidato[3].votos_egresados, candidato[3].votos_administrativos, candidato[3].votos_consejo, candidato[3].estado);
                fprintf(candidatos, "5,Alexander Molina Cabrera,%d,%d,%d,%d,%d,%d,%d.\n", candidato[4].votos, candidato[4].votos_estudiantes, candidato[4].votos_docentes, candidato[4].votos_egresados, candidato[4].votos_administrativos, candidato[4].votos_consejo, candidato[4].estado);
                fprintf(candidatos, "6,Juan Carlos Gutiérrez Arias,%d,%d,%d,%d,%d,%d,%d.\n", candidato[5].votos, candidato[5].votos_estudiantes, candidato[5].votos_docentes, candidato[5].votos_egresados, candidato[5].votos_administrativos, candidato[5].votos_consejo, candidato[5].estado);
                //Se borra el resto del archivo
                ftruncate(fileno(candidatos), ftell(candidatos));
                break;
            }
        }
    }
    else {
        printf(RED"Ese candidato no se encuentra en la terna\n"RESET);
    }

    fclose(candidatos);

    FILE *usuarios;
    usuarios = fopen("consejo_superior.txt", "r+");

    printf("%s", rol);

    struct Usuario rolUsuario;
    
    if (Coincidencia == true){
        while (fscanf(usuarios, "%*d,%[^,],%*[^,],%d.\n", rolUsuario.ocupacion, &rolUsuario.voto) == 2) {
            if (strcmp(rolUsuario.ocupacion, rol) == 0 && rolUsuario.voto == 0) {
                fseek(usuarios, -3, SEEK_CUR); 
                fprintf(usuarios, "%d.\n", 1); 
                printf("\nVoto realizado\n");
                break;
            }
        }
    }
    fclose(usuarios);

    sleep(2);
}

void IngresoConsejoSuperior() {

    User users[10];

    // Inicializar los usuarios con los roles proporcionados
    users[0] = (User){1, "Ministro_Educacion"};
    users[1] = (User){2, "Gobernador_Risaralda"};
    users[2] = (User){3, "Miembro_Presidente"};
    users[3] = (User){4, "Representante_Directivas"};
    users[4] = (User){5, "Representante_Docentes"};
    users[5] = (User){6, "Representante_Estudiantes"};
    users[6] = (User){7, "Representante_Egresados"};
    users[7] = (User){8, "Representante_Sector_Productivo"};
    users[8] = (User){9, "Exrector_UTP"};
    //Para atras
    users[9] = (User){10, "Atras"};

    int opcion = 0;
    char ch;
    do {
        MenuConsejo(opcion, users);

        ch = getch();

        if (ch == 27) {  // Comprobamos si es una secuencia de escape (código para flechas en algunas terminales)
            getch(); // Descartamos el siguiente carácter
            switch (getch()) {
                case 65:  // Flecha arriba
                    opcion = (opcion - 1 + 10) % 10;
                    break;
                case 66:  // Flecha abajo
                    opcion = (opcion + 1) % 10;
                    break;
            }
        }

        system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows

    } while (ch != 10);  //  es el código ASCII para la tecla Enter

    //dependiendo de la opcion se le asigna el rol
    char rol[50];
    strcpy(rol, users[opcion].role);

    if (opcion == 9) {
        return;
    }

    system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows

    printf("Ingrese su contraseña: ");

    int i = 0;
    char contrasena[50];
    while (i < 49) {
        char ch = getch();

        if (ch == '\n' || ch == '\r') {
            contrasena[i] = '\0';
            break;
        } else if (ch == 8) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            contrasena[i] = ch;
            i++;
            printf("*");
        }
    }

    //Se busca en consejo_superior.txt el rol y la contraseña suponiendo que la estructura es id,Rol,contraseña,0.
    FILE *candidatos;
    
    candidatos = fopen("consejo_superior.txt", "r");

    // Se verifica que el rol y la contraseña sean correctos y que el ultimo numero sea 0
    //el id es la opcion + 1
    char rolconsejo[50];
    char contrasenaconsejo[50];
    // lo mas importante es que el ultimo numero sea 0
    int voto;
    while (fscanf(candidatos, "%*d,%[^,],%[^,],%d.\n", rolconsejo, contrasenaconsejo, &voto) == 3) {
        if (strcmp(rolconsejo, rol) == 0 && strcmp(contrasenaconsejo, contrasena) == 0 && voto == 0) {
            printf("\nBienvenido.\n");
            VotarConsejoSuperior(rol);
            break;
        } else if (strcmp(rolconsejo, rol) == 0 && strcmp(contrasenaconsejo, contrasena) == 0 && voto == 1) {
            printf("\nya ha votado.\n");
            break;
        }
    }
    fclose(candidatos);
}

void titulo () {
    printf(RED"  _____ _     _                             _       __      __   _             _                       \n");
    printf(" / ____(_)   | |                           | |      \\ \\    / /  | |           (_)                      \n");
    printf("| (___  _ ___| |_ ___ _ __ ___   __ _    __| | ___   \\ \\  / /__ | |_ __ _  ___ _  ___  _ __   ___  ___ \n");
    printf(" \\___ \\| / __| __/ _ \\ '_ ` _ \\ / _` |  / _` |/ _ \\   \\ \\/ / _ \\| __/ _` |/ __| |/ _ \\| '_ \\ / _ \\/ __|\n");
    printf(" ____) | \\__ \\ ||  __/ | | | | | (_| | | (_| |  __/    \\  / (_) | || (_| | (__| | (_) | | | |  __/\\__ \\\n");
    printf("|_____/|_|___/\\__\\___|_| |_| |_|\\__,_|  \\__,_|\\___|     \\/ \\___/ \\__\\__,_|\\___|_|\\___/|_| |_|\\___||___/\n");
    printf("\n"RESET);
    sleep(2);
}

void VerVotos () {
    FILE *candidatos;
    candidatos = fopen("candidatos.txt", "r");

    //Pero ahora se hace teniendo en cuenta que la estructura es tarjeton,Luis Fernando Gaviria Trujillo,11,votos-estudiantes,votos-docentes,votos-egresados,votos-administrativos,votos-consejo.
    struct Candidato candidato[6];

    //si la estructura es es tarjeton,candidato,votos,votos-estudiantes,votos-docentes,votos-egresados,votos-administrativos,votos-consejo.
    //Se guarda cada candidato en un arreglo de estructuras
    int i = 0;
    while (fscanf(candidatos, "%d,%[^,],%d,%d,%d,%d,%d,%d,%d.\n", &candidato[i].tarjeton, candidato[i].nombre, &candidato[i].votos, &candidato[i].votos_estudiantes, &candidato[i].votos_docentes, &candidato[i].votos_egresados, &candidato[i].votos_administrativos, &candidato[i].votos_consejo, &candidato[i].estado) == 9) {
        i++;
    }


    fclose(candidatos);

    //Se halla los votos totales
    float votostotales = 0;
    for (int i = 0; i < 6; i++) {
        votostotales += candidato[i].votos;
    }

    printf(YELLOW"\n");
    printf("Votos totales: %.0f\n", votostotales);
    printf("Votos por candidato:\n");
    printf("                                      ┌─────────────────────────────────────────────────────────────┐\n");
    printf("                                      │                              VOTOS                          │\n");
    printf("┌──────┬──────────────────────────────┼──────┬───────────┬────────┬─────────┬───────────────┬───────┤\n");
    printf("│numero│nombre:                       │total │estudiantes│docentes│egresados│administrativos│consejo│\n");
    printf("├──────┼──────────────────────────────┼──────┼───────────┼────────┼─────────┼───────────────┼───────┤\n");
    printf("│1     │Luis Fernando Gaviria Trujillo│%6d│%11d│%8d│%9d│%15d│%7d│\n", candidato[0].votos, candidato[0].votos_estudiantes, candidato[0].votos_docentes, candidato[0].votos_egresados, candidato[0].votos_administrativos, candidato[0].votos_consejo);
    printf("├──────┼──────────────────────────────┼──────┼───────────┼────────┼─────────┼───────────────┼───────┤\n");
    printf("│2     │Carlos Alfonso Victoria Mena  │%6d│%11d│%8d│%9d│%15d│%7d│\n", candidato[1].votos, candidato[1].votos_estudiantes, candidato[1].votos_docentes, candidato[1].votos_egresados, candidato[1].votos_administrativos, candidato[1].votos_consejo);
    printf("├──────┼──────────────────────────────┼──────┼───────────┼────────┼─────────┼───────────────┼───────┤\n");
    printf("│3     │Giovanni Arias                │%6d│%11d│%8d│%9d│%15d│%7d│\n", candidato[2].votos, candidato[2].votos_estudiantes, candidato[2].votos_docentes, candidato[2].votos_egresados, candidato[2].votos_administrativos, candidato[2].votos_consejo);
    printf("├──────┼──────────────────────────────┼──────┼───────────┼────────┼─────────┼───────────────┼───────┤\n");
    printf("│4     │Xiomara Rocío González Ordoñez│%6d│%11d│%8d│%9d│%15d│%7d│\n", candidato[3].votos, candidato[3].votos_estudiantes, candidato[3].votos_docentes, candidato[3].votos_egresados, candidato[3].votos_administrativos, candidato[3].votos_consejo);
    printf("├──────┼──────────────────────────────┼──────┼───────────┼────────┼─────────┼───────────────┼───────┤\n");
    printf("│5     │Alexander Molina Cabrera      │%6d│%11d│%8d│%9d│%15d│%7d│\n", candidato[4].votos, candidato[4].votos_estudiantes, candidato[4].votos_docentes, candidato[4].votos_egresados, candidato[4].votos_administrativos, candidato[4].votos_consejo);
    printf("├──────┼──────────────────────────────┼──────┼───────────┼────────┼─────────┼───────────────┼───────┤\n");
    printf("│6     │Juan Carlos Gutiérrez Arias   │%6d│%11d│%8d│%9d│%15d│%7d│\n", candidato[5].votos, candidato[5].votos_estudiantes, candidato[5].votos_docentes, candidato[5].votos_egresados, candidato[5].votos_administrativos, candidato[5].votos_consejo);
    printf("└──────┴──────────────────────────────┴──────┴───────────┴────────┴─────────┴───────────────┴───────┘\n");
    printf("\n\n"RESET);
    //Ahora en porcentajes
    printf("Votos por candidato en porcentaje:\n");
    printf(YELLOW"                                      ┌─────────────────────────────────────────────────────────────┐\n");
    printf("                                      │                              VOTOS                          │\n");
    printf("┌──────┬──────────────────────────────┼──────┬───────────┬────────┬─────────┬───────────────┬───────┤\n");
    printf("│numero│nombre:                       │total │estudiantes│docentes│egresados│administrativos│consejo│\n");
    printf("├──────┼──────────────────────────────┼──────┼───────────┼────────┼─────────┼───────────────┼───────┤\n");
    printf("│1     │Luis Fernando Gaviria Trujillo│%6.2f│%11.2f│%8.2f│%9.2f│%15.2f│%7.2f│\n", candidato[0].votos / votostotales * 100, candidato[0].votos_estudiantes / votostotales * 100, candidato[0].votos_docentes / votostotales * 100, candidato[0].votos_egresados / votostotales * 100, candidato[0].votos_administrativos / votostotales * 100, candidato[0].votos_consejo / votostotales * 100);
    printf("├──────┼──────────────────────────────┼──────┼───────────┼────────┼─────────┼───────────────┼───────┤\n");
    printf("│2     │Carlos Alfonso Victoria Mena  │%6.2f│%11.2f│%8.2f│%9.2f│%15.2f│%7.2f│\n", candidato[1].votos / votostotales * 100, candidato[1].votos_estudiantes / votostotales * 100, candidato[1].votos_docentes / votostotales * 100, candidato[1].votos_egresados / votostotales * 100, candidato[1].votos_administrativos / votostotales * 100, candidato[1].votos_consejo / votostotales * 100);
    printf("├──────┼──────────────────────────────┼──────┼───────────┼────────┼─────────┼───────────────┼───────┤\n");
    printf("│3     │Giovanni Arias                │%6.2f│%11.2f│%8.2f│%9.2f│%15.2f│%7.2f│\n", candidato[2].votos / votostotales * 100, candidato[2].votos_estudiantes / votostotales * 100, candidato[2].votos_docentes / votostotales * 100, candidato[2].votos_egresados / votostotales * 100, candidato[2].votos_administrativos / votostotales * 100, candidato[2].votos_consejo / votostotales * 100);
    printf("├──────┼──────────────────────────────┼──────┼───────────┼────────┼─────────┼───────────────┼───────┤\n");
    printf("│4     │Xiomara Rocío González Ordoñez│%6.2f│%11.2f│%8.2f│%9.2f│%15.2f│%7.2f│\n", candidato[3].votos / votostotales * 100, candidato[3].votos_estudiantes / votostotales * 100, candidato[3].votos_docentes / votostotales * 100, candidato[3].votos_egresados / votostotales * 100, candidato[3].votos_administrativos / votostotales * 100, candidato[3].votos_consejo / votostotales * 100);
    printf("├──────┼──────────────────────────────┼──────┼───────────┼────────┼─────────┼───────────────┼───────┤\n");
    printf("│5     │Alexander Molina Cabrera      │%6.2f│%11.2f│%8.2f│%9.2f│%15.2f│%7.2f│\n", candidato[4].votos / votostotales * 100, candidato[4].votos_estudiantes / votostotales * 100, candidato[4].votos_docentes / votostotales * 100, candidato[4].votos_egresados / votostotales * 100, candidato[4].votos_administrativos / votostotales * 100, candidato[4].votos_consejo / votostotales * 100);
    printf("├──────┼──────────────────────────────┼──────┼───────────┼────────┼─────────┼───────────────┼───────┤\n");
    printf("│6     │Juan Carlos Gutiérrez Arias   │%6.2f│%11.2f│%8.2f│%9.2f│%15.2f│%7.2f│\n", candidato[5].votos / votostotales * 100, candidato[5].votos_estudiantes / votostotales * 100, candidato[5].votos_docentes / votostotales * 100, candidato[5].votos_egresados / votostotales * 100, candidato[5].votos_administrativos / votostotales * 100, candidato[5].votos_consejo / votostotales * 100);
    printf("└──────┴──────────────────────────────┴──────┴───────────┴────────┴─────────┴───────────────┴───────┘\n");
    printf("\n"RESET);

}

void MostrarTerna () {
    //hace lo mismo que VerVotos pero solo con los 3 primeros
    FILE *candidatos;
    candidatos = fopen("candidatos.txt", "r");

    struct Candidato candidato[6];

    //si la estructura es es tarjeton,candidato,votos,votos-estudiantes,votos-docentes,votos-egresados,votos-administrativos,votos-consejo.
    //Se guarda cada candidato en un arreglo de estructuras
    int i = 0;
    while (fscanf(candidatos, "%d,%[^,],%d,%d,%d,%d,%d,%d,%d.\n", &candidato[i].tarjeton, candidato[i].nombre, &candidato[i].votos, &candidato[i].votos_estudiantes, &candidato[i].votos_docentes, &candidato[i].votos_egresados, &candidato[i].votos_administrativos, &candidato[i].votos_consejo, &candidato[i].estado) == 9) {
        i++;
    }


    fclose(candidatos);

    //Se halla los votos totales
    float votostotales = 0;
    for (int i = 0; i < 6; i++) {
        votostotales += candidato[i].votos;
    }
    //se halla cuales fueron los 3 mas votados
    int mayor1 = 0;
    int mayor2 = 0;
    int mayor3 = 0;
    int pos1 = 0;
    int pos2 = 0;
    int pos3 = 0;
    for (int i = 0; i < 6; i++) {
        if (candidato[i].votos > mayor1) {
            mayor1 = candidato[i].votos;
            pos1 = i;
        }
    }
    for (int i = 0; i < 6; i++) {
        if (candidato[i].votos > mayor2 && candidato[i].votos < mayor1) {
            mayor2 = candidato[i].votos;
            pos2 = i;
        }
    }
    for (int i = 0; i < 6; i++) {
        if (candidato[i].votos > mayor3 && candidato[i].votos < mayor2) {
            mayor3 = candidato[i].votos;
            pos3 = i;
        }
    }

    printf("\n");
    printf("Votos totales: %.0f\n", votostotales);
    printf("Votos por candidato:\n");
    printf(YELLOW"                                      ┌─────────────────────────────────────────────────────────────┐\n");
    printf("                                      │                              VOTOS                          │\n");
    printf("┌──────┬──────────────────────────────┼──────┬───────────┬────────┬─────────┬───────────────┬───────┤\n");
    printf("│numero│nombre:                       │total │estudiantes│docentes│egresados│administrativos│consejo│\n");
    printf("├──────┼──────────────────────────────┼──────┼───────────┼────────┼─────────┼───────────────┼───────┤\n");
    //Se ingresa el primero con el mayor numero de votos
    printf("│%d     │%s      │%6d│%11d│%8d│%9d│%15d│%7d│\n", candidato[pos1].tarjeton, candidato[pos1].nombre, candidato[pos1].votos, candidato[pos1].votos_estudiantes, candidato[pos1].votos_docentes, candidato[pos1].votos_egresados, candidato[pos1].votos_administrativos, candidato[pos1].votos_consejo);
    printf("├──────┼──────────────────────────────┼──────┼───────────┼────────┼─────────┼───────────────┼───────┤\n");
    //Se ingresa el segundo con el segundo mayor numero de votos
    printf("│%d     │%s  │%6d│%11d│%8d│%9d│%15d│%7d│\n", candidato[pos2].tarjeton, candidato[pos2].nombre, candidato[pos2].votos, candidato[pos2].votos_estudiantes, candidato[pos2].votos_docentes, candidato[pos2].votos_egresados, candidato[pos2].votos_administrativos, candidato[pos2].votos_consejo);
    printf("├──────┼──────────────────────────────┼──────┼───────────┼────────┼─────────┼───────────────┼───────┤\n");
    //Se ingresa el tercero con el tercer mayor numero de votos
    printf("│%d     │%s   │%6d│%11d│%8d│%9d│%15d│%7d│\n", candidato[pos3].tarjeton, candidato[pos3].nombre, candidato[pos3].votos, candidato[pos3].votos_estudiantes, candidato[pos3].votos_docentes, candidato[pos3].votos_egresados, candidato[pos3].votos_administrativos, candidato[pos3].votos_consejo);
    printf("└──────┴──────────────────────────────┴──────┴───────────┴────────┴─────────┴───────────────┴───────┘\n");
    printf("\n\n");
    //Ahora en porcentajes
    printf("Votos por candidato en porcentaje:\n");
    printf("                                      ┌─────────────────────────────────────────────────────────────┐\n");
    printf("                                      │                              VOTOS                          │\n");
    printf("┌──────┬──────────────────────────────┼──────┬───────────┬────────┬─────────┬───────────────┬───────┤\n");
    printf("│numero│nombre:                       │total │estudiantes│docentes│egresados│administrativos│consejo│\n");
    printf("├──────┼──────────────────────────────┼──────┼───────────┼────────┼─────────┼───────────────┼───────┤\n");
    //Se ingresa el primero con el mayor numero de votos
    printf("│%d     │%s      │%6.2f│%11.2f│%8.2f│%9.2f│%15.2f│%7.2f│\n", candidato[pos1].tarjeton, candidato[pos1].nombre, candidato[pos1].votos / votostotales * 100, candidato[pos1].votos_estudiantes / votostotales * 100, candidato[pos1].votos_docentes / votostotales * 100, candidato[pos1].votos_egresados / votostotales * 100, candidato[pos1].votos_administrativos / votostotales * 100, candidato[pos1].votos_consejo / votostotales * 100);
    printf("├──────┼──────────────────────────────┼──────┼───────────┼────────┼─────────┼───────────────┼───────┤\n");
    //Se ingresa el segundo con el segundo mayor numero de votos
    printf("│%d     │%s  │%6.2f│%11.2f│%8.2f│%9.2f│%15.2f│%7.2f│\n", candidato[pos2].tarjeton, candidato[pos2].nombre, candidato[pos2].votos / votostotales * 100, candidato[pos2].votos_estudiantes / votostotales * 100, candidato[pos2].votos_docentes / votostotales * 100, candidato[pos2].votos_egresados / votostotales * 100, candidato[pos2].votos_administrativos / votostotales * 100, candidato[pos2].votos_consejo / votostotales * 100);
    printf("├──────┼──────────────────────────────┼──────┼───────────┼────────┼─────────┼───────────────┼───────┤\n");
    //Se ingresa el tercero con el tercer mayor numero de votos
    printf("│%d     │%s   │%6.2f│%11.2f│%8.2f│%9.2f│%15.2f│%7.2f│\n", candidato[pos3].tarjeton, candidato[pos3].nombre, candidato[pos3].votos / votostotales * 100, candidato[pos3].votos_estudiantes / votostotales * 100, candidato[pos3].votos_docentes / votostotales * 100, candidato[pos3].votos_egresados / votostotales * 100, candidato[pos3].votos_administrativos / votostotales * 100, candidato[pos3].votos_consejo / votostotales * 100);
    printf("└──────┴──────────────────────────────┴──────┴───────────┴────────┴─────────┴───────────────┴───────┘\n");
    printf("\n"RESET);
}

//Ahora se genera un histograma con los votos
void histograma () {
    FILE *candidatos;
    candidatos = fopen("candidatos.txt", "r");

    struct Candidato candidato[6];

    //si la estructura es es tarjeton,candidato,votos,votos-estudiantes,votos-docentes,votos-egresados,votos-administrativos,votos-consejo.
    //Se guarda cada candidato en un arreglo de estructuras
    int i = 0;
    while (fscanf(candidatos, "%d,%[^,],%d,%d,%d,%d,%d,%d,%d.\n", &candidato[i].tarjeton, candidato[i].nombre, &candidato[i].votos, &candidato[i].votos_estudiantes, &candidato[i].votos_docentes, &candidato[i].votos_egresados, &candidato[i].votos_administrativos, &candidato[i].votos_consejo, &candidato[i].estado) == 9) {
        i++;
    }

    fclose(candidatos);

    //Se halla los votos totales
    float votostotales = 0;
    for (int i = 0; i < 6; i++) {
        votostotales += candidato[i].votos;
    }

    //Se realiza el histograma con * y se imprime tipo:
    printf("Histograma:\n");
    printf(GREEN"Luis Fernando Gaviria Trujillo(%.1f%%):", candidato[0].votos / votostotales * 100);
    for (int i = 0; i < candidato[0].votos / votostotales * 100; i++) {
        printf("═");
    }

    printf("\nCarlos Alfonso Victoria Mena(%.1f%%):  ", candidato[1].votos / votostotales * 100);
    for (int i = 0; i < candidato[1].votos / votostotales * 100; i++) {
        printf("═");
    }

    printf("\nGiovanni Arias(%.1f%%):                ", candidato[2].votos / votostotales * 100);
    for (int i = 0; i < candidato[2].votos / votostotales * 100; i++) {
        printf("═");
    }

    printf("\nXiomara Rocío González Ordoñez(%.1f%%): ", candidato[3].votos / votostotales * 100);
    for (int i = 0; i < candidato[3].votos / votostotales * 100; i++) {
        printf("═");
    }

    printf("\nAlexander Molina Cabrera(%.1f%%):      ", candidato[4].votos / votostotales * 100);
    for (int i = 0; i < candidato[4].votos / votostotales * 100; i++) {
        printf("═");
    }

    printf("\nJuan Carlos Gutiérrez Arias(%.1f%%):   ", candidato[5].votos / votostotales * 100);
    for (int i = 0; i < candidato[5].votos / votostotales * 100; i++) {
        printf("═");
    }
    printf("\n"RESET);
}

void HistogramaTerna () {
    //hace lo mismo que histograma pero solo con los 3 primeros
    FILE *candidatos;
    candidatos = fopen("candidatos.txt", "r");

    struct Candidato candidato[6];

    //si la estructura es es tarjeton,candidato,votos,votos-estudiantes,votos-docentes,votos-egresados,votos-administrativos,votos-consejo.
    //Se guarda cada candidato en un arreglo de estructuras
    int i = 0;
    while (fscanf(candidatos, "%d,%[^,],%d,%d,%d,%d,%d,%d,%d.\n", &candidato[i].tarjeton, candidato[i].nombre, &candidato[i].votos, &candidato[i].votos_estudiantes, &candidato[i].votos_docentes, &candidato[i].votos_egresados, &candidato[i].votos_administrativos, &candidato[i].votos_consejo, &candidato[i].estado) == 9) {
        i++;
    }


    fclose(candidatos);

    //Se halla los votos totales
    float votostotales = 0;
    for (int i = 0; i < 6; i++) {
        votostotales += candidato[i].votos;
    }

    //se halla cuales fueron los 3 mas votados
    int mayor1 = 0;
    int mayor2 = 0;
    int mayor3 = 0;
    int pos1 = 0;
    int pos2 = 0;
    int pos3 = 0;
    for (int i = 0; i < 6; i++) {
        if (candidato[i].votos > mayor1) {
            mayor1 = candidato[i].votos;
            pos1 = i;
        }
    }
    for (int i = 0; i < 6; i++) {
        if (candidato[i].votos > mayor2 && candidato[i].votos < mayor1) {
            mayor2 = candidato[i].votos;
            pos2 = i;
        }
    }
    for (int i = 0; i < 6; i++) {
        if (candidato[i].votos > mayor3 && candidato[i].votos < mayor2) {
            mayor3 = candidato[i].votos;
            pos3 = i;
        }
    }
    //Se muestra el histograma igual que en histograma pero solo con los 3 primeros
    printf("Histograma:\n");
    //el nombre tambien sale de la posicion del arreglo
    printf("%s(%.1f%%):      ", candidato[pos1].nombre, candidato[pos1].votos / votostotales * 100);
    for (int i = 0; i < candidato[pos1].votos / votostotales * 100; i++) {
        printf("═");
    }
    
    printf("\n%s(%.1f%%):  ", candidato[pos2].nombre, candidato[pos2].votos / votostotales * 100);
    for (int i = 0; i < candidato[pos2].votos / votostotales * 100; i++) {
        printf("═");
    }

    printf("\n%s(%.1f%%):   ", candidato[pos3].nombre, candidato[pos3].votos / votostotales * 100);
    for (int i = 0; i < candidato[pos3].votos / votostotales * 100; i++) {
        printf("═");
    }

    printf("\n");
}



void reinicioDatos() {
    FILE *candidatos = fopen("candidatos.txt", "r+");

    if (candidatos == NULL) {
        printf("Error opening candidatos.txt\n");
        return;
    }

    struct Candidato candidato[6];
    int i = 0;

    // Read candidate data
    while (fscanf(candidatos, "%d,%[^,],%d,%d,%d,%d,%d,%d,%d.\n", &candidato[i].tarjeton, candidato[i].nombre, &candidato[i].votos, &candidato[i].votos_estudiantes, &candidato[i].votos_docentes, &candidato[i].votos_egresados, &candidato[i].votos_administrativos, &candidato[i].votos_consejo, &candidato[i].estado) == 9) {
        i++;
    }

    int votoTotalConsejo = 0;

    // Reset consejo votes
    for (i = 0; i < 6; i++) {
        votoTotalConsejo = candidato[i].votos_consejo;
        candidato[i].votos -= votoTotalConsejo;
        candidato[i].votos_consejo = 0;
    }

    // Clear file content
    fclose(candidatos);
    candidatos = fopen("candidatos.txt", "w");
    fclose(candidatos);

    // Rewrite candidate data
    candidatos = fopen("candidatos.txt", "a");
    for (i = 0; i < 6; i++) {
        fprintf(candidatos, "%d,%s,%d,%d,%d,%d,%d,%d,%d.\n", candidato[i].tarjeton, candidato[i].nombre, candidato[i].votos, candidato[i].votos_estudiantes, candidato[i].votos_docentes, candidato[i].votos_egresados, candidato[i].votos_administrativos, candidato[i].votos_consejo, candidato[i].estado);
    }
    fclose(candidatos);

    FILE *usuarios = fopen("consejo_superior.txt", "r+");

    if (usuarios == NULL) {
        printf("Error opening consejo_superior.txt\n");
        return;
    }

    struct Usuario rolUsuario[9];
    int j = 0;

    // Read user data
    while (fscanf(usuarios, "%*d,%[^,],%[^,],%d.\n", rolUsuario[j].ocupacion, rolUsuario[j].contrasena, &rolUsuario[j].voto) == 3) {
        j++;
    }

    // Reset consejo votes
    for (j = 0; j < 9; j++) {
        rolUsuario[j].voto = 0;
    }

    // Clear file content
    fclose(usuarios);
    usuarios = fopen("consejo_superior.txt", "w");
    fclose(usuarios);

    // Rewrite user data
    usuarios = fopen("consejo_superior.txt", "a");
    for (j = 0; j < 9; j++) {
        fprintf(usuarios, "%d,%s,%s,%d.\n", j + 1, rolUsuario[j].ocupacion, rolUsuario[j].contrasena, rolUsuario[j].voto);
    }
    fclose(usuarios);

    printf("\nSe ha reiniciado exitosamente\n");
    sleep(2);
}


void verGanador() {
    FILE *usuario;
    FILE *candidatos;
    candidatos = fopen("candidatos.txt", "r+");

    struct Candidato candidato[6];

    // Si la estructura es es tarjeton, candidato, votos, votos-estudiantes, votos-docentes, votos-egresados, votos-administrativos, votos-consejo.
    // Se guarda cada candidato en un arreglo de estructuras
    int i = 0;
    while (fscanf(candidatos, "%d,%[^,],%d,%d,%d,%d,%d,%d,%d.\n", &candidato[i].tarjeton, candidato[i].nombre, &candidato[i].votos, &candidato[i].votos_estudiantes, &candidato[i].votos_docentes, &candidato[i].votos_egresados, &candidato[i].votos_administrativos, &candidato[i].votos_consejo, &candidato[i].estado) == 9) {
        i++;
    }

    int posicion = 0;
    int mayor = 0;
    int repeticion = 0;
    bool ganador = false;

    mayor = candidato[0].votos_consejo;

    fclose(candidatos);

    // Se comparan los datos del voto del consejo para verificar el de mayor votación
    for (i = 0; i < 6; i++) {
        if (candidato[i].votos_consejo > mayor) {
            mayor = candidato[i].votos_consejo;
            posicion = i;
        }
    }

    // Se revisa que el voto mayor sea por diferencia y no haya otro votante con la misma cantidad de votos
    for (i = 0; i < 6; i++) {
        if (candidato[i].votos_consejo == mayor) {
            repeticion++;
        } else if (candidato[i].votos_consejo < mayor) {
            candidato[i].estado = 0;
        }
    }

    // Abrir el archivo en modo de escritura para borrar su contenido
    candidatos = fopen("candidatos.txt", "w");

    // Reescribir los datos actualizados en el archivo
    for (i = 0; i < 6; i++) {
        fprintf(candidatos, "%d,%s,%d,%d,%d,%d,%d,%d,%d.\n", candidato[i].tarjeton, candidato[i].nombre, candidato[i].votos, candidato[i].votos_estudiantes, candidato[i].votos_docentes, candidato[i].votos_egresados, candidato[i].votos_administrativos, candidato[i].votos_consejo, candidato[i].estado);
    }

    fclose(candidatos);

    if (repeticion == 1) {
        ganador = true;
    } else {
        ganador = false;
    }

    if (ganador == true) {
        printf("El ganador es:\n");
        printf("\n%s", candidato[posicion].nombre);
        printf("\nCon un total de %d votos totales y %d del consejo superior\n", candidato[posicion].votos, candidato[posicion].votos_consejo);
        printf("\n");
    } else {
        printf("Todavía no hay ganador\n");
        printf("Se reiniciarán los datos\n");
        reinicioDatos();
    }
}


void ComprobarConsejo() {
    FILE *usuarios;
    usuarios = fopen("consejo_superior.txt", "r");

    bool votoRealizado = true;
    
    struct Usuario rolUsuario;

    while (fscanf(usuarios, "%*d,%[^,],%*[^,],%d.\n", rolUsuario.ocupacion, &rolUsuario.voto) == 2) {
        if (rolUsuario.voto != 1){
            votoRealizado = false;
            break;
        }
    }
    fclose(usuarios);
    
    if (votoRealizado == false){
        printf(RED"Hacen falta votos del consejo superior\n");
        printf("No se puede definir un ganador\n"RESET);
    }
    else {
        verGanador();
    }
}


void Admin() {

    int op = 0;
    char ch;
    do {
        MenuAdmin(op);

        ch = getch();

        if (ch == 27) {  // Comprobamos si es una secuencia de escape (código para flechas en algunas terminales)
            getch(); // Descartamos el siguiente carácter
            switch (getch()) {
                case 65:  // Flecha arriba
                    op = (op - 1 + 5) % 5;
                    break;
                case 66:  // Flecha abajo
                    op = (op + 1) % 5;
                    break;
            }
        }

        system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows

    } while (ch != 10);  //  es el código ASCII para la tecla Enter
    switch (op){
        case 0:
            VerVotos();
            printf("Presione cualquier tecla para continuar...\n");
            getch();
            Admin();
            break;
        case 1:
            histograma();
            printf("Presione cualquier tecla para continuar...\n");
            getch();
            Admin();
            break;
        case 2:
            MostrarTerna();
            HistogramaTerna();
            printf("Presione cualquier tecla para continuar...\n");
            getch();
            Admin();
            break;
        case 3:
            ComprobarConsejo();
            printf("Presione cualquier tecla para continuar...\n");
            getch();
            Admin();
            break;
        default:
            printf("Cerrando...\n");
            break;
    }
}

void ValidarAdmin() {
    //Valida el usuario y la contraseña del admin
    char contrasena[50];
    char usuario[50];
    printf("Ingrese su usuario: ");
    scanf("%s", usuario);

    printf("Ingrese su contraseña: ");
    int i = 0;
    while (i < 49) {
        char ch = getch();

        if (ch == '\n' || ch == '\r') {
            contrasena[i] = '\0';
            break;
        } else if (ch == 8) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            contrasena[i] = ch;
            i++;
            printf("*");
        }
    }

    //busca en admin.txt el usuario y la contraseña suponiendo que la estructura es administrador,1234
    FILE *admin;
    admin = fopen("admin.txt", "r");

    char usuarioadmin[50];
    char contrasenaadmin[50];

    fscanf(admin, "%[^,],%[^,].\n", usuarioadmin, contrasenaadmin);

    fclose(admin);

    if (strcmp(usuario, usuarioadmin) == 0 && strcmp(contrasena, contrasenaadmin) == 0) {
        printf("\nBienvenido.\n");
        Admin();
    } else {
        printf(RED"\nHubo un error al verificar datos.\n"RESET);
        sleep(2);
    }
}


int main() {
    titulo();
    int opcion = 0;
    printf("Ingrese una opción: ");

    char ch;

    //para mac
    do {
        mostrarMenu(opcion);

        ch = getch();

        if (ch == 27) {  // Comprobamos si es una secuencia de escape (código para flechas en algunas terminales)
            getch(); // Descartamos el siguiente carácter
            switch (getch()) {
                case 65:  // Flecha arriba
                    opcion = (opcion - 1 + 4) % 4;
                    break;
                case 66:  // Flecha abajo
                    opcion = (opcion + 1) % 4;
                    break;
            }
        }

    } while (ch != 10);  //  es el código ASCII para la tecla Enter

    system("clear || cls");  // Limpiar pantalla en sistemas macOS o Windows

    switch (opcion) {
        case 0:
            ingresar();
            main();
            break;
        case 1:
            IngresoConsejoSuperior();
            main();
            break;
        case 2:
            ValidarAdmin();
            main();
            break;
        case 3:
            printf(RED"Cerrando...\n"RESET);
            return 0;
            break;
    }
    return 0;
}