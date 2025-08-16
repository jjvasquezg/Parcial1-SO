#ifndef PERSONA_H
#define PERSONA_H

#include <string>
#include <iostream>
#include <iomanip>

// Representa una persona con datos personales y financieros
struct Persona {
    std::string nombre;
    std::string apellido;
    std::string id;
    std::string ciudadNacimiento;
    std::string fechaNacimiento;
    double ingresosAnuales;
    double patrimonio;
    double deudas;
    bool declaranteRenta;
    char grupo;

    // Constructor completo
    Persona(std::string nom, std::string ape, std::string id,
            std::string ciudad, std::string fecha, double ingresos,
            double patri, double deud, bool declara, char grupo)
        : nombre(std::move(nom)), apellido(std::move(ape)), id(std::move(id)),
          ciudadNacimiento(std::move(ciudad)), fechaNacimiento(std::move(fecha)),
          ingresosAnuales(ingresos), patrimonio(patri), deudas(deud),
          declaranteRenta(declara), grupo(grupo) {}

    // Métodos de visualización
    void mostrar() const;         // Detalles completos
    void mostrarResumen() const;  // Vista resumida

    // Accesores
    std::string getNombre() const { return nombre; }
    std::string getApellido() const { return apellido; }
    std::string getId() const { return id; }
    std::string getCiudadNacimiento() const { return ciudadNacimiento; }
    std::string getFechaNacimiento() const { return fechaNacimiento; }
    double getIngresosAnuales() const { return ingresosAnuales; }
    double getPatrimonio() const { return patrimonio; }
    double getDeudas() const { return deudas; }
    bool getDeclaranteRenta() const { return declaranteRenta; }
    char getGrupo() const { return grupo; }
};

// Imprime todos los atributos de la persona
inline void Persona::mostrar() const {
    std::cout << "-------------------------------------\n";
    std::cout << "[" << id << "] Nombre: " << nombre << " " << apellido << "\n";
    std::cout << "   - Ciudad de nacimiento: " << ciudadNacimiento << "\n";
    std::cout << "   - Fecha de nacimiento: " << fechaNacimiento << "\n\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "   - Ingresos anuales: $" << ingresosAnuales << "\n";
    std::cout << "   - Patrimonio: $" << patrimonio << "\n";
    std::cout << "   - Deudas: $" << deudas << "\n";
    std::cout << "   - Declarante de renta: " << (declaranteRenta ? "Sí" : "No") << "\n";
    std::cout << "   - Grupo:                " << grupo << "\n";
}

// Imprime un resumen compacto
inline void Persona::mostrarResumen() const {
    std::cout << "[" << id << "] " << nombre << " " << apellido
              << " | " << fechaNacimiento
              << " | " << ciudadNacimiento
              << " | $" << std::fixed << std::setprecision(2) << ingresosAnuales
              << " | " << patrimonio
              << " | Declara: " << (declaranteRenta ? "Sí" : "No")
              << " Grupo: " << grupo;
}

#endif // PERSONA_H

