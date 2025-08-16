#include <iostream>
#include <vector>
#include <limits>
#include <memory>
#include <algorithm>
#include "persona.h"
#include "generador.h"
#include "monitor.h"

/**
 * Muestra el menú principal de la aplicación.
 * 
 * POR QUÉ: Guiar al usuario a través de las funcionalidades disponibles.
 * CÓMO: Imprimiendo las opciones en consola.
 * PARA QUÉ: Interacción amigable con el usuario.
 */
void mostrarMenu() {
    std::cout << "\n\n=== MENÚ PRINCIPAL ===";
    std::cout << "\n0. Crear nuevo conjunto de datos";
    std::cout << "\n1. Mostrar resumen de todas las personas";
    std::cout << "\n2. Mostrar detalle completo por índice";
    std::cout << "\n3. Buscar persona por ID";
    std::cout << "\n4. Busqueda Personalizada";
    std::cout << "\n5. Mostrar estadísticas de rendimiento";
    std::cout << "\n6. Exportar estadísticas a CSV";
    std::cout << "\n7. Salir";
    std::cout << "\nSeleccione una opción: ";
}

void mostrarMenuBusqueda() {
    std::cout << "\n\n=== BUSQUEDA PERSONALIZADA ===";
    std::cout << "\n0. Persona más longeva (En todo el país)";
    std::cout << "\n1. Persona más longeva (Por ciudad)";
    std::cout << "\n2. Persona con mayor patrimonio (En todo el país)";
    std::cout << "\n3. Persona con mayor patrimonio (Por ciudad)";
    std::cout << "\n4. Persona con mayor patrimonio (Por grupo A, B, C)";
    std::cout << "\n5. Declarantes de renta (Por grupo A, B, C)";
    std::cout << "\n6. Verificar grupo de declaración (Por ID)";
    std::cout << "\n7. ------";
    std::cout << "\n8. ------";
    std::cout << "\n9. ------";
    std::cout << "\n10. Volver";
    std::cout << "\nSeleccione una opción: ";

}

void mostrarCiudades() { 
    std::cout << "\n\n=== CIUDADES ===";
    std::cout << "\nBogotá, Medellín, Cali, Barranquilla, Cartagena, Bucaramanga, Pereira, Santa Marta, Cúcuta,\nIbagué, Manizales, Pasto, Neiva, Villavicencio, Armenia, Sincelejo, Valledupar, Montería, Popayán, Tunja";
    std::cout << "\nIngrese una opción: ";

}

void mostrarGrupos() { 
    std::cout << "\n\n=== Grupos ===";
    std::cout << "A, B, C";
    std::cout << "\nIngrese una opción: ";

}
/**
 * Punto de entrada principal del programa.
 * 
 * POR QUÉ: Iniciar la aplicación y manejar el flujo principal.
 * CÓMO: Mediante un bucle que muestra el menú y procesa la opción seleccionada.
 * PARA QUÉ: Ejecutar las funcionalidades del sistema.
 */
int main() {
    srand(time(nullptr)); // Semilla para generación aleatoria
    
    // Puntero inteligente para gestionar la colección de personas
    // POR QUÉ: Evitar fugas de memoria y garantizar liberación automática.
    std::unique_ptr<std::vector<Persona>> personas = nullptr;
    std::vector<Persona> personasValor;
    
    Monitor monitor; // Monitor para medir rendimiento
    
    int opcion, opcionB;
    do {
        mostrarMenu();
        std::cin >> opcion;
        
        // Variables locales para uso en los casos
        size_t tam = 0;
        int indice;
        std::string idBusqueda;
        
        // Iniciar medición de tiempo y memoria para la operación actual
        monitor.iniciar_tiempo();
        long memoria_inicio = monitor.obtener_memoria();
        
        switch(opcion) {
            case 0: { // Crear nuevo conjunto de datos
                int n;
                std::cout << "\nIngrese el número de personas a generar: ";
                std::cin >> n;
                
                if (n <= 0) {
                    std::cout << "Error: Debe generar al menos 1 persona\n";
                    break;
                }
                
                // Generar el nuevo conjunto de personas
                auto nuevasPersonas = generarColeccion(n);
                tam = nuevasPersonas.size();
                
                personasValor = nuevasPersonas;
                // Mover el conjunto al puntero inteligente (propiedad única)
                personas = std::make_unique<std::vector<Persona>>(std::move(nuevasPersonas));
                
                // Medir tiempo y memoria usada
                double tiempo_gen = monitor.detener_tiempo();
                long memoria_gen = monitor.obtener_memoria() - memoria_inicio;
                
                std::cout << "Generadas " << tam << " personas en " 
                          << tiempo_gen << " ms, Memoria: " << memoria_gen << " KB\n";
                
                // Registrar la operación
                monitor.registrar("Crear datos", tiempo_gen, memoria_gen);
                break;
            }
                
            case 1: { // Mostrar resumen de todas las personas
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                
                tam = personas->size();
                std::cout << "\n=== RESUMEN DE PERSONAS (" << tam << ") ===\n";
                for(size_t i = 0; i < tam; ++i) {
                    std::cout << i << ". ";
                    (*personas)[i].mostrarResumen();
                    std::cout << "\n";
                }
                
                double tiempo_mostrar = monitor.detener_tiempo();
                long memoria_mostrar = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Mostrar resumen", tiempo_mostrar, memoria_mostrar);
                break;
            }
                
            case 2: { // Mostrar detalle por índice
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                
                tam = personas->size();
                std::cout << "\nIngrese el índice (0-" << tam-1 << "): ";
                if(std::cin >> indice) {
                    if(indice >= 0 && static_cast<size_t>(indice) < tam) {
                        (*personas)[indice].mostrar();
                    } else {
                        std::cout << "Índice fuera de rango!\n";
                    }
                } else {
                    std::cout << "Entrada inválida!\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                
                double tiempo_detalle = monitor.detener_tiempo();
                long memoria_detalle = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Mostrar detalle", tiempo_detalle, memoria_detalle);
                break;
            }
                
            case 3: { // Buscar por ID
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                
                std::cout << "\nIngrese el ID a buscar: ";
                std::cin >> idBusqueda;
                
                if(const Persona* encontrada = buscarPorID(*personas, idBusqueda)) {
                    encontrada->mostrar();
                } else {
                    std::cout << "No se encontró persona con ID " << idBusqueda << "\n";
                }
                
                double tiempo_busqueda = monitor.detener_tiempo();
                long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Buscar por ID", tiempo_busqueda, memoria_busqueda);
                break;
            }
            case 4:
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                do {
                    mostrarMenuBusqueda();
                    std::cin >> opcionB;
                    std::string valor;

                    switch (opcionB) {
                        case 0: { // Persona más longeva (En todo el país) Tanto valor como referencia
                            valor = "pais";
                            monitor.iniciar_tiempo();
                            long memoria_inicio = monitor.obtener_memoria();

                            auto encontrada = buscarLongevaV(personasValor, valor);
                            // std::cout<<encontrada.getFechaNacimiento()<<std::endl;
                            encontrada.mostrar();

                            double tiempo_busqueda = monitor.detener_tiempo();
                            long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                            monitor.mostrar_estadistica("Opción 4." + std::to_string(opcionB), tiempo_busqueda, memoria_busqueda);
                            monitor.registrar("Persona más longeva (En todo el país) VALOR", tiempo_busqueda, memoria_busqueda);
                            break;
                        }

                        case 1: { // Persona más longeva (Por ciudad) Tanto valor como referencia
                            mostrarCiudades();
                            std::cin >> valor;

                            auto it = std::find(ciudadesColombia.begin(), ciudadesColombia.end(), valor);

                            while (it == ciudadesColombia.end()) {
                                std::cout << "La ciudad ingresada (" << valor << ") no existe en la lista.\n";
                                std::cout << "Ingrese una opción: ";
                                std::cin >> valor;
                                it = std::find(ciudadesColombia.begin(), ciudadesColombia.end(), valor); 
                            }
                            monitor.iniciar_tiempo();
                            long memoria_inicio = monitor.obtener_memoria();
                            
                            auto encontrada = buscarLongevaV(personasValor, valor);
                            encontrada.mostrar();

                            double tiempo_busqueda = monitor.detener_tiempo();
                            long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                            monitor.mostrar_estadistica("Opción 4." + std::to_string(opcionB), tiempo_busqueda, memoria_busqueda);
                            monitor.registrar("Persona más longeva (Por ciudad) VALOR", tiempo_busqueda, memoria_busqueda);
                            break;

                        }

                        case 2: { // Persona con mayor patrimonio (En todo el país) Tanto valor como referencia
                            valor = "pais";
                            monitor.iniciar_tiempo();
                            long memoria_inicio = monitor.obtener_memoria();

                            auto encontrada = buscarMayorPatrimonioV(personasValor, valor, 0);
                            encontrada.mostrar();
                            
                            double tiempo_busqueda = monitor.detener_tiempo();
                            long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                            monitor.mostrar_estadistica("Opción 4." + std::to_string(opcionB), tiempo_busqueda, memoria_busqueda);
                            monitor.registrar("Persona con mayor patrimonio (En todo el país) VALOR", tiempo_busqueda, memoria_busqueda);
                            break;

                        }
                        case 3: { // Persona con mayor patrimonio (Por ciudad) Tanto valor como referencia
                            mostrarCiudades();
                            std::cin >> valor; 
                            monitor.iniciar_tiempo();
                            long memoria_inicio = monitor.obtener_memoria();
                            
                            auto encontrada = buscarMayorPatrimonioV(personasValor, valor, 1);
                            encontrada.mostrar();

                            double tiempo_busqueda = monitor.detener_tiempo();
                            long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                            monitor.mostrar_estadistica("Opción 4." + std::to_string(opcionB), tiempo_busqueda, memoria_busqueda);
                            monitor.registrar("Persona con mayor patrimonio (Por ciudad) VALOR", tiempo_busqueda, memoria_busqueda);
                            break;

                        }

                        case 4: { // Persona con mayor patrimonio (Por grupo) Tanto valor como referencia
                            mostrarGrupos();
                            std::cin >> valor; 
                            monitor.iniciar_tiempo();
                            long memoria_inicio = monitor.obtener_memoria();
                            
                            auto encontrada = buscarMayorPatrimonioV(personasValor, valor, 2);
                            encontrada.mostrar();

                            double tiempo_busqueda = monitor.detener_tiempo();
                            long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                            monitor.mostrar_estadistica("Opción 4." + std::to_string(opcionB), tiempo_busqueda, memoria_busqueda);
                            monitor.registrar("Persona con mayor patrimonio (Por grupo) VALOR", tiempo_busqueda, memoria_busqueda);
                            break;

                        }

                        case 5:

                        case 6:

                        case 7:

                        case 8:

                        case 9:

                        case 10:
                            std::cout << "Volviendo...\n";
                            break;
                        
                        default:
                            std::cout << "Opción inválida!\n";
                    }

                } while (opcionB != 10);

            break;

            case 5: // Mostrar estadísticas de rendimiento
                monitor.mostrar_resumen();
                break;
                
            case 6: // Exportar estadísticas a CSV
                monitor.exportar_csv();
                break;
                
            case 7: // Salir
                std::cout << "Saliendo...\n";
                break;
                
            default:
                std::cout << "Opción inválida!\n";
        }
        
        // Mostrar estadísticas de la operación (excepto para opciones 4,5,6)
        if (opcion >= 0 && opcion <= 3) {
            double tiempo = monitor.detener_tiempo();
            long memoria = monitor.obtener_memoria() - memoria_inicio;
            monitor.mostrar_estadistica("Opción " + std::to_string(opcion), tiempo, memoria);
        }
        
    } while(opcion != 7);
    
    return 0;
}