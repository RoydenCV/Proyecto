// Mariana Ramirez Quesada       C09190
// Royden Contreras Vallejos     C12260
// Proyecto de Programacion Bajo Plataformas Abiertas     IE-0117

// La finalidad de este proyecto es simular el juego llamado "Old maid" o tambien
// conocido como "tonto", el cual es un jugo de cartas el cual inicia al remober
// una de las cartas del maso, una vez hecho eso se procese a repartir las cartas
// entre los jugadores, los cuales se iran turnado para seleccionar una carta del
// otro jugador para ir formando parejas hasta que solo quede una carta, el jugador
// que haya quedado con la unica carta que no forma pareja se considera el perdedor,
// mientras que el otro sera el ganador.

// Dicho esto se procedera a ralizar un codigo el cual sea capaz de replicar el 
// juego que se les menciono anteriormente, esto con la implementacion de funciones
// y herramientas visuales como lo es glade para darle mayor dinamismo a nuestro
// programa.


// Implementacion de librerias. 
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> // rand para generar números
#include <stdbool.h> //  Simplifica la escritura de condiciones lógicas para verificar estados,
// como si un jugador tiene una pareja de cartas


// Estructura para una carta
typedef struct {
    char *palo;
    int valor;
} Carta;


// Constantes del juego
#define NUM_CARTAS 52
#define NUM_PALOS 4
char *palos[] = {"Corazones", "Diamantes", "Tréboles", "Espadas"};


// Variables globales
GtkWidget *window1, *window2, *texto3_label, *texto4_label,  *pareja_label, *pop_turno, *pop_salida, *Fin;
GtkListStore *list_store; 
Carta mazo[NUM_CARTAS];
Carta jugador1[NUM_CARTAS / 2];
Carta jugador2[NUM_CARTAS / 2];
int num_j1 = 0, num_j2 = 0;
int jugador_actual = 1; 


// Funciones auxiliares
void inicializarMazo(Carta mazo[]) {
    int i = 0;
    for (int p = 0; p < NUM_PALOS; p++) {
        for (int v = 1; v <= 13; v++) {
            mazo[i].palo = palos[p];
            mazo[i].valor = v;
            i++;
        }
    }
}


// Funcion encargada de revolver el maso.
void barajar(Carta mazo[], int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        int j = rand() % n;
        Carta temp = mazo[i];
        mazo[i] = mazo[j];
        mazo[j] = temp;
    }
}


// Funcion encargada de quitar una carta y repartirla entre los jugadores. 
void repartir(Carta mazo[], Carta jugador1[], Carta jugador2[], int *num_j1, int *num_j2) {
    for (int i = 0; i < NUM_CARTAS - 1; i++) { 
        if (i % 2 == 0) {
            jugador1[(*num_j1)++] = mazo[i];
        } else {
            jugador2[(*num_j2)++] = mazo[i];
        }
    }
}


// Funcion que elimina las cartas que forman pareja entre las cartas que se repartieron
void eliminarRepetidas(Carta jugador[], int *num_cartas) {
    for (int i = 0; i < *num_cartas; i++) {
        for (int j = i + 1; j < *num_cartas; j++) {
            if (jugador[i].valor == jugador[j].valor) {
                for (int k = j; k < *num_cartas - 1; k++) {
                    jugador[k] = jugador[k + 1];
                }
                (*num_cartas)--;
                j--;
            }
        }
    }
}


// Función para actualizar la ventana según el turno
void actualizarVentana() {

    char buffer[512];
    buffer[0] = '\0';

    if (jugador_actual == 1) {
        gtk_label_set_text(GTK_LABEL(texto3_label), "Jugador Uno");
        for (int i = 0; i < num_j1; i++) {
            char carta[32];
            snprintf(carta, sizeof(carta), "%d de %s\n", jugador1[i].valor, jugador1[i].palo);
            strcat(buffer, carta);
        }
    } else {
        gtk_label_set_text(GTK_LABEL(texto3_label), "Jugador Dos");
        for (int i = 0; i < num_j2; i++) {
            char carta[32];
            snprintf(carta, sizeof(carta), "%d de %s\n", jugador2[i].valor, jugador2[i].palo);
            strcat(buffer, carta);
        }
    }

    gtk_label_set_text(GTK_LABEL(texto4_label), buffer);
    
}


// Funcion que indica que hacer en caso de ser presionado el boton "salida"
void on_salida_clicked(GtkButton *button, gpointer user_data) {
    // Ocultar todas las ventanas y mostrar solo window1
    gtk_widget_hide(window2);
    gtk_widget_hide(pop_turno);
    gtk_widget_hide(pop_salida);
    gtk_widget_show_all(window1); 

    // Restablecer las cartas y el turno
    num_j1 = 0;
    num_j2 = 0;
    jugador_actual = 1;  
}


// Función que indica que hacer en caso de ser presionado el boton "inicio"
void on_inicio_clicked(GtkButton *button, gpointer user_data) {
    // Oculta window1 y muestra window2
    gtk_widget_hide(window1);
    gtk_widget_show_all(window2);

    // Inicialización del juego
    inicializarMazo(mazo);
    barajar(mazo, NUM_CARTAS);

    // Retirar una carta al azar (Old Maid)
    int carta_retirada = rand() % NUM_CARTAS;
    Carta old_maid = mazo[carta_retirada];
    printf("La carta retirada (Old Maid) es: %d de %s\n", old_maid.valor, old_maid.palo);

    // Ajustar el mazo eliminando la carta retirada
    for (int i = carta_retirada; i < NUM_CARTAS - 1; i++) {
        mazo[i] = mazo[i + 1];
    }

    // Redefinir el número total de cartas en el mazo
    int cartas_restantes = NUM_CARTAS - 1; // Una carta ha sido retirada

    // Repartir las cartas entre los jugadores
    repartir(mazo, jugador1, jugador2, &num_j1, &num_j2);

    // Eliminar cartas repetidas de ambos jugadores
    eliminarRepetidas(jugador1, &num_j1);
    eliminarRepetidas(jugador2, &num_j2);

    // Mostrar las cartas después de eliminar las repetidas
    for (int i = 0; i < num_j1; i++) {
    }

    for (int i = 0; i < num_j2; i++) {
    }

    // Actualizar la ventana para mostrar las cartas
    actualizarVentana();
}


// Función para verificar si un jugador ha ganado (si se quedó sin cartas)
void verificarFinJuego() {
    if (num_j1 == 0) {
        // El jugador 1 ha ganado
        gtk_label_set_text(GTK_LABEL(Fin), "¡Ha ganado el Jugador 1!");
        gtk_widget_hide(window2);
        gtk_widget_hide(pop_turno);
        gtk_widget_show_all(pop_salida);  
    } else if (num_j2 == 0) {
        // El jugador 2 ha ganado
        gtk_label_set_text(GTK_LABEL(Fin), "¡Ha ganado el Jugador 2!");
        gtk_widget_hide(window2);
        gtk_widget_hide(pop_turno);
        gtk_widget_show_all(pop_salida); 
    }
}


// Funcion encargada de tomar una carta al azar del otro jugador.
void tomarCartaAlAzar(Carta jugador_actual[], int *num_jugador_actual, Carta jugador_otro[], int *num_jugador_otro) {
    if (*num_jugador_otro > 0) {
        // Seleccionar una carta aleatoria del otro jugador
        int carta_aleatoria = rand() % (*num_jugador_otro);
        Carta carta_tomada = jugador_otro[carta_aleatoria];

        // Eliminar la carta tomada del otro jugador
        for (int i = carta_aleatoria; i < *num_jugador_otro - 1; i++) {
            jugador_otro[i] = jugador_otro[i + 1];
        }
        (*num_jugador_otro)--;

        // Agregar la carta tomada al jugador actual
        jugador_actual[(*num_jugador_actual)++] = carta_tomada;
        
    }
}


// Funcion encargada de cuando se toma la carta al azar verificar que no hayan 
// parejas que puedan ser eliminadas 
// Esta funcion tambien se encarga de imprimir un mensaje en caso que haya
// encontrado pareja.
bool eliminarParejas(Carta jugador[], int *num_jugador) {
    bool pareja_formada = false; 

    for (int i = 0; i < *num_jugador; i++) {
        for (int j = i + 1; j < *num_jugador; j++) {
            if (jugador[i].valor == jugador[j].valor) {
                // Eliminar pareja
                // Primero eliminamos la carta con el índice mayor (j)
                for (int k = j; k < *num_jugador - 1; k++) {
                    jugador[k] = jugador[k + 1];
                }
                (*num_jugador)--;

                // Luego eliminamos la carta con el índice menor (i)
                for (int k = i; k < *num_jugador - 1; k++) {
                    jugador[k] = jugador[k + 1];
                }
                (*num_jugador)--;

                pareja_formada = true; 
                i = -1; 
                break;
            }
        }
        if (pareja_formada) break; 
    }
    return pareja_formada; 
}


// Funcion que indica que hacer en caso de ser presionado el boton "tomar carta al azar"
void on_button3_clicked(GtkButton *button, gpointer user_data) {
    // Oculta window2 y muestra cambio de turno
    gtk_widget_hide(window2);
    gtk_widget_show_all(pop_turno);
    
    bool pareja_formada = false;
    char mensaje_pareja[128];

    // Determinar qué jugador está en turno
    if (jugador_actual == 1) {
        tomarCartaAlAzar(jugador1, &num_j1, jugador2, &num_j2);
        pareja_formada = eliminarParejas(jugador1, &num_j1);
    } else {
        tomarCartaAlAzar(jugador2, &num_j2, jugador1, &num_j1);
        pareja_formada = eliminarParejas(jugador2, &num_j2);
    }

    // Mensaje de pareja formada
    snprintf(mensaje_pareja, sizeof(mensaje_pareja), 
             pareja_formada ? "Se encontró una pareja." : "No se encontró ninguna pareja.");
    gtk_label_set_text(GTK_LABEL(pareja_label), mensaje_pareja);

    // Actualizar la ventana y verificar el estado del juego
    actualizarVentana();
    verificarFinJuego();

    // Cambiar el turno al siguiente jugador al final
    jugador_actual = (jugador_actual == 1) ? 2 : 1;
}


// Funcion encargada de indicar que realizar en caso de ser presionado el boton
// "cambio de turno"
void on_cambio_turno_clicked(GtkButton *button, gpointer user_data) {
    gtk_widget_hide(pop_turno);
    gtk_widget_show_all(window2);

    // Actualizar la ventana con el texto y las cartas del jugador actual
    actualizarVentana();
}


// Este es el main encargado de iniciar el programa
int main(int argc, char *argv[]) {
    // Definicion de variables 
    GtkBuilder *builder;
    GtkWidget *inicio_button, *cambio_turno_button, *salida_button, *button3;

    gtk_init(&argc, &argv);

    // Cargar archivo Glade
    builder = gtk_builder_new_from_file("oldmaid.glade");

    // Inicialización de ventanas
    window1 = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    window2 = GTK_WIDGET(gtk_builder_get_object(builder, "window2"));


    // Centrar las ventanas
    gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
    gtk_window_set_position(GTK_WINDOW(window2), GTK_WIN_POS_CENTER);


    // Inicialización de otros widgets
    texto3_label = GTK_WIDGET(gtk_builder_get_object(builder, "Texto3"));
    texto4_label = GTK_WIDGET(gtk_builder_get_object(builder, "Texto4"));
    Fin = GTK_WIDGET(gtk_builder_get_object(builder, "Fin"));
    
    pareja_label = GTK_WIDGET(gtk_builder_get_object(builder, "pareja"));
    pop_turno = GTK_WIDGET(gtk_builder_get_object(builder, "pop_turno"));
    pop_salida = GTK_WIDGET(gtk_builder_get_object(builder, "pop_salida"));


    // Conexión de botones
    inicio_button = GTK_WIDGET(gtk_builder_get_object(builder, "inicio"));
    cambio_turno_button = GTK_WIDGET(gtk_builder_get_object(builder, "cambio_turno"));
    salida_button = GTK_WIDGET(gtk_builder_get_object(builder, "salida"));
    button3 = GTK_WIDGET(gtk_builder_get_object(builder, "button3"));


    // Conexion del boton a la funcion 
    g_signal_connect(inicio_button, "clicked", G_CALLBACK(on_inicio_clicked), NULL);
    g_signal_connect(cambio_turno_button, "clicked", G_CALLBACK(on_cambio_turno_clicked), NULL);
    g_signal_connect(salida_button, "clicked", G_CALLBACK(on_salida_clicked), NULL);
    g_signal_connect(button3, "clicked", G_CALLBACK(on_button3_clicked), NULL);
    g_signal_connect(salida_button, "clicked", G_CALLBACK(on_salida_clicked), NULL);


    // Conexión para cerrar las ventanas
    g_signal_connect(window1, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window2, "destroy", G_CALLBACK(gtk_main_quit), NULL);


    // Mostrar la ventana inicial
    gtk_widget_show_all(window1);


    // Iniciar el bucle principal
    gtk_main();


    return 0;
}