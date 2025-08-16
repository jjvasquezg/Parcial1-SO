#include "generador.h"
#include <cstdlib>   // rand(), srand()
#include <ctime>     // time()
#include <random>    // std::mt19937, std::uniform_real_distribution
#include <vector>
#include <algorithm> // std::find_if
#include <tuple>
#include <sstream>
#include <string>

// Bases de datos para generación realista

// Nombres femeninos comunes en Colombia
const std::vector<std::string> nombresFemeninos = {
    "María", "Luisa", "Carmen", "Ana", "Sofía", "Isabel", "Laura", "Andrea", "Paula", "Valentina",
    "Camila", "Daniela", "Carolina", "Fernanda", "Gabriela", "Patricia", "Claudia", "Diana", "Lucía", "Ximena"
};

// Nombres masculinos comunes en Colombia
const std::vector<std::string> nombresMasculinos = {
    "Juan", "Carlos", "José", "James", "Andrés", "Miguel", "Luis", "Pedro", "Alejandro", "Ricardo",
    "Felipe", "David", "Jorge", "Santiago", "Daniel", "Fernando", "Diego", "Rafael", "Martín", "Óscar",
    "Edison", "Nestor", "Gertridis"
};

// Apellidos comunes en Colombia
const std::vector<std::string> apellidos = {
    "Gómez", "Rodríguez", "Martínez", "López", "García", "Pérez", "González", "Sánchez", "Ramírez", "Torres",
    "Díaz", "Vargas", "Castro", "Ruiz", "Álvarez", "Romero", "Suárez", "Rojas", "Moreno", "Muñoz", "Valencia",
};

// Principales ciudades colombianas
const std::vector<std::string> ciudadesColombia = {
    "Bogotá", "Medellín", "Cali", "Barranquilla", "Cartagena", "Bucaramanga", "Pereira", "Santa Marta", "Cúcuta", "Ibagué",
    "Manizales", "Pasto", "Neiva", "Villavicencio", "Armenia", "Sincelejo", "Valledupar", "Montería", "Popayán", "Tunja"
};

/**
 * Implementación de generarFechaNacimiento.
 * 
 * POR QUÉ: Simular fechas de nacimiento realistas.
 * CÓMO: Día (1-28), mes (1-12), año (1960-2009).
 * PARA QUÉ: Atributo fechaNacimiento de Persona.
 */
std::string generarFechaNacimiento() {
    int dia = 1 + rand() % 28;       // Día: 1 a 28 (evita problemas con meses)
    int mes = 1 + rand() % 12;        // Mes: 1 a 12
    int anio = 1960 + rand() % 50;    // Año: 1960 a 2009
    return std::to_string(dia) + "/" + std::to_string(mes) + "/" + std::to_string(anio);
}

/**
 * Implementación de generarID.
 * 
 * POR QUÉ: Generar identificadores únicos y secuenciales.
 * CÓMO: Contador estático que inicia en 1000000000 y se incrementa.
 * PARA QUÉ: Simular números de cédula.
 */
std::string generarID() {
    static long contador = 1000000000; // Inicia en 1,000,000,000
    return std::to_string(contador++); // Convierte a string e incrementa
}

/**
 * Implementación de randomDouble.
 * 
 * POR QUÉ: Generar números decimales aleatorios en un rango.
 * CÓMO: Mersenne Twister (mejor que rand()) y distribución uniforme.
 * PARA QUÉ: Valores de ingresos, patrimonio, etc.
 */
double randomDouble(double min, double max) {
    static std::mt19937 generator(time(nullptr)); // Semilla basada en tiempo
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

/**
 * Implementación de generarPersona.
 * 
 * POR QUÉ: Crear una persona con datos aleatorios.
 * CÓMO: Seleccionando aleatoriamente de las bases de datos y generando números.
 * PARA QUÉ: Generar datos de prueba.
 */
Persona generarPersona() {
    // Decide si es hombre o mujer
    bool esHombre = rand() % 2;
    
    // Selecciona nombre según género
    std::string nombre = esHombre ? 
        nombresMasculinos[rand() % nombresMasculinos.size()] :
        nombresFemeninos[rand() % nombresFemeninos.size()];
    
    // Construye apellido compuesto (dos apellidos aleatorios)
    std::string apellido = apellidos[rand() % apellidos.size()];
    apellido += " ";
    apellido += apellidos[rand() % apellidos.size()];
    
    // Genera los demás atributos
    char grupo;
    std::string id = generarID();
    std::string ciudad = ciudadesColombia[rand() % ciudadesColombia.size()];
    std::string fecha = generarFechaNacimiento();
    
    // Genera datos financieros realistas
    double ingresos = randomDouble(10000000, 500000000);   // 10M a 500M COP
    double patrimonio = randomDouble(0, 2000000000);       // 0 a 2,000M COP
    double deudas = randomDouble(0, patrimonio * 0.7);     // Deudas hasta el 70% del patrimonio
    bool declarante = (ingresos > 50000000) && (rand() % 100 > 30); // Probabilidad 70% si ingresos > 50M
    if (declarante == true){
        grupo = asignarGrupo(id);
    }else {
        grupo = NULL;
    }
    
    return Persona(nombre, apellido, id, ciudad, fecha, ingresos, patrimonio, deudas, declarante, grupo);
}

/**
 * Implementación de generarColeccion.
 * 
 * POR QUÉ: Generar un conjunto de n personas.
 * CÓMO: Reservando espacio y agregando n personas generadas.
 * PARA QUÉ: Crear datasets para pruebas.
 */
std::vector<Persona> generarColeccion(int n) {
    std::vector<Persona> personas;
    personas.reserve(n); // Reserva espacio para n personas (eficiencia)
    
    for (int i = 0; i < n; ++i) {
        personas.push_back(generarPersona());
    }
    
    return personas;
}

/**
 * Implementación de asignarGrupo.
 * 
 * POR QUÉ: Asignar un grupo a un declarante dependiendo de los ultimos digistos de su id.
 * CÓMO: Usando comparaciones.
 * PARA QUÉ: Para operaciones de búsqueda en la aplicación.
 */
char asignarGrupo(std::string id){

    // Tomar los últimos 2 caracteres del string
    std::string ultimosDosStr = id.substr(id.size() - 2);

    // Convertirlos a entero
    int ultimosDos = stoi(ultimosDosStr);

    if (ultimosDos >= 0 && ultimosDos <= 39) {
        return 'A';
    } 
    else if (ultimosDos >= 40 && ultimosDos <= 79) {
        return 'B';
    } 
    else if (ultimosDos >= 80 && ultimosDos <= 99) {
        return 'C';
    } 
    else {
        return '?'; // Caso de error
    }

}

/**
 * Implementación de buscarPorID.
 * 
 * POR QUÉ: Encontrar una persona por su ID en una colección.
 * CÓMO: Usando un algoritmo de búsqueda secuencial (lineal).
 * PARA QUÉ: Para operaciones de búsqueda en la aplicación.
 */
const Persona* buscarPorID(const std::vector<Persona>& personas, const std::string& id) {
    // Usa find_if con una lambda para buscar por ID
    auto it = std::find_if(personas.begin(), personas.end(),
        [&id](const Persona& p) { return p.getId() == id; });
    
    if (it != personas.end()) {
        return &(*it); // Devuelve puntero a la persona encontrada
    } else {
        return nullptr; // No encontrado
    }
}

const Persona buscarLongevaV(const std::vector<Persona> personas, const std::string valor){



}



















































































































const Persona buscarMayorPatrimonioV(const std::vector<Persona> personas, std::string valor, int opcion) {

    auto coincide = [opcion, valor](Persona p) -> bool {
        if (opcion == 0) {
            return true; // sin filtro
        } else if (opcion == 1) {
            return p.getCiudadNacimiento() == valor;
        } else if (opcion == 2) {
            if (valor.empty()) return false;
            return p.getGrupo() == valor[0];
        }
        return false; // opción inválida
    };

    Persona mejor = personas[0]; // primer elemento como base

    for (Persona p : personas) { // recorrido por valor O(n)
        if (coincide(p) && p.getPatrimonio() > mejor.getPatrimonio()) {
            mejor = p; // nueva copia del mejor
        }
    }

    return mejor; // retorno por valor

}


const Persona* buscarMayorPatrimonioV(const std::vector<Persona>& personas, const std::string& valor, int opcion) {
    auto coincide = [&](const Persona& p) -> bool {
        if (opcion == 0) {
            return true;
        } else if (opcion == 1) {
            return p.getCiudadNacimiento() == valor;
        } else if (opcion == 2) {
            if (valor.empty()) return false;
            return p.getGrupo() == valor[0];
        }
        return false;
    };

    const Persona* mejor = nullptr;

    for (const Persona& p : personas) {
        if (!coincide(p)) continue;
        if (mejor == nullptr || p.getPatrimonio() > mejor->getPatrimonio()) {
            mejor = &p;
        }
    }

    return mejor;
}

const Persona buscarDeclarantesV(const std::vector<Persona> personas, const std::string id){



}