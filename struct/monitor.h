#ifndef MONITOR_H
#define MONITOR_H

#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

// Struct que monitorea tiempo y memoria de ejecución
struct Monitor {
    // Registra el tiempo de inicio
    void iniciar_tiempo();

    // Devuelve el tiempo transcurrido en ms
    double detener_tiempo();

    // Retorna la memoria residente actual en KB
    long obtener_memoria();

    // Registra operación para pruebas por valor
    void registrarV(const std::string& operacion, double tiempo, long memoria);

    // Registra operación para pruebas por referencia
    void registrarR(const std::string& operacion, double tiempo, long memoria);

    // Muestra estadística de una operación
    void mostrar_estadistica(const std::string& operacion, double tiempo, long memoria);

    // Muestra resumen de estadísticas por valor
    void mostrar_resumenV();

    // Muestra resumen de estadísticas por referencia
    void mostrar_resumenR();

    // Exporta registros por valor a CSV
    void exportar_csvV(const std::string& nombre_archivo = "estadisticasValor.csv");

    // Exporta registros por referencia a CSV
    void exportar_csvR(const std::string& nombre_archivo = "estadisticasReferencia.csv");

    // Registro individual de operación
    struct Registro {
        std::string operacion;
        double tiempo;
        long memoria;
    };

    std::chrono::high_resolution_clock::time_point inicio;
    std::vector<Registro> registrosV;
    std::vector<Registro> registrosR;
    double total_tiempoV = 0;
    long max_memoriaV = 0;
    double total_tiempoR = 0;
    long max_memoriaR = 0;
};

#endif // MONITOR_H
