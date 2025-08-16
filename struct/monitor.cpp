#include "monitor.h"
#include <unistd.h>   // sysconf
#include <cstdio>     // FILE, fscanf

// Inicia la medición de tiempo
void Monitor::iniciar_tiempo() {
    inicio = std::chrono::high_resolution_clock::now();
}

// Detiene el cronómetro y retorna tiempo en milisegundos
double Monitor::detener_tiempo() {
    auto fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duracion = fin - inicio;
    return duracion.count();
}

// Obtiene la memoria residente actual en KB (Linux)
long Monitor::obtener_memoria() {
    FILE* file = fopen("/proc/self/statm", "r");
    if (!file) {
        perror("Error al abrir /proc/self/statm");
        return 0;
    }

    long size, resident, shared, text, lib, data, dt;
    if (fscanf(file, "%ld %ld %ld %ld %ld %ld %ld",
               &size, &resident, &shared, &text, &lib, &data, &dt) != 7) {
        fclose(file);
        return 0;
    }

    fclose(file);
    long page_size_kb = sysconf(_SC_PAGESIZE) / 1024;
    return resident * page_size_kb;
}

// Registra una operación en modo valor
void Monitor::registrarV(const std::string& operacion, double tiempo, long memoria) {
    registrosV.push_back({operacion, tiempo, memoria});
    total_tiempoV += tiempo;
    max_memoriaV += memoria;
}

// Registra una operación en modo referencia
void Monitor::registrarR(const std::string& operacion, double tiempo, long memoria) {
    registrosR.push_back({operacion, tiempo, memoria});
    total_tiempoR += tiempo;
    max_memoriaR += memoria;
}

// Muestra una estadística individual
void Monitor::mostrar_estadistica(const std::string& operacion, double tiempo, long memoria) {
    std::cout << "\n[ESTADÍSTICAS] " << operacion
              << " - Tiempo: " << tiempo << " ms, "
              << "Memoria: " << memoria << " KB\n";
}

// Muestra resumen del modo valor
void Monitor::mostrar_resumenV() {
    std::cout << "\n=== RESUMEN DE ESTADÍSTICAS VALOR ===";
    for (const auto& reg : registrosV) {
        std::cout << "\n" << reg.operacion << ": "
                  << reg.tiempo << " ms, " << reg.memoria << " KB";
    }
    std::cout << "\nTotal tiempo: " << total_tiempoV << " ms";
    std::cout << "\nMemoria máxima: " << max_memoriaV << " KB\n";
}

// Muestra resumen del modo referencia
void Monitor::mostrar_resumenR() {
    std::cout << "\n=== RESUMEN DE ESTADÍSTICAS REFERENCIA ===";
    for (const auto& reg : registrosR) {
        std::cout << "\n" << reg.operacion << ": "
                  << reg.tiempo << " ms, " << reg.memoria << " KB";
    }
    std::cout << "\nTotal tiempo: " << total_tiempoR << " ms";
    std::cout << "\nMemoria máxima: " << max_memoriaR << " KB\n";
}

// Exporta estadísticas valor a CSV
void Monitor::exportar_csvV(const std::string& nombre_archivo) {
    std::ofstream archivo(nombre_archivo);
    if (!archivo) {
        std::cerr << "Error al abrir archivo: " << nombre_archivo << std::endl;
        return;
    }

    archivo << "Operacion,Tiempo(ms),Memoria(KB)\n";
    for (const auto& reg : registrosV) {
        archivo << reg.operacion << "," << reg.tiempo << "," << reg.memoria << "\n";
    }

    archivo.close();
    std::cout << "Estadísticas exportadas a sus archivos respectivos\n";
}

// Exporta estadísticas referencia a CSV
void Monitor::exportar_csvR(const std::string& nombre_archivo) {
    std::ofstream archivo(nombre_archivo);
    if (!archivo) {
        std::cerr << "Error al abrir archivo: " << nombre_archivo << std::endl;
        return;
    }

    for (const auto& reg : registrosR) {
        archivo << reg.operacion << "," << reg.tiempo << "," << reg.memoria << "\n";
    }

    archivo.close();
    std::cout << "Estadísticas exportadas a sus archivos respectivos\n";
}
