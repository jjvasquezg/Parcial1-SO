#include "generador.h"
#include <cstdlib>   // rand(), srand()
#include <ctime>     // time()
#include <random>    // std::mt19937, std::uniform_real_distribution
#include <vector>
#include <algorithm> // std::find_if
#include <tuple>
#include <sstream>
#include <string>
#include <map>

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
        grupo = ""[0];
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

std::tuple<int,int,int> parseFecha(std::string fecha) {
    int d, m, a;
    char sep;
    std::istringstream iss(fecha);
    iss >> d >> sep >> m >> sep >> a;
    return {a, m, d};
}

const Persona buscarLongevaV(std::vector<Persona> personas, std::string valor) {
    if (personas.empty()) {
        return personas[0];
    }

    if (valor == "pais") {
        Persona longeva = personas[0];
        for (auto p : personas) {
            if (parseFecha(p.getFechaNacimiento()) < parseFecha(longeva.getFechaNacimiento())) {
                longeva = p;
            }
        }
        return longeva;
    }

    std::vector<Persona> filtradas;
    for (auto p : personas) {
        if (p.getCiudadNacimiento() == valor) {
            filtradas.push_back(p);
        }
    }

    if (filtradas.empty()) {
        std::cout << "No hay personas registradas en " << valor << ".\n";
        return personas[0];
    }

    Persona longeva = filtradas[0];
    for (auto p : filtradas) {
        if (parseFecha(p.getFechaNacimiento()) < parseFecha(longeva.getFechaNacimiento())) {
            longeva = p;
        }
    }
    return longeva;
}

const Persona* buscarLongevaR(const std::vector<Persona>& personas, const std::string& valor) {
    if (personas.empty()) {
        throw std::runtime_error("No hay personas registradas."); // evitar UB
    }

    if (valor == "pais") {
        const Persona* longeva = &personas[0];
        for (const auto& p : personas) {
            if (parseFecha(p.getFechaNacimiento()) < parseFecha(longeva->getFechaNacimiento())) {
                longeva = &p;
            }
        }
        return longeva;
    }

    const Persona* longeva = nullptr;
    for (const auto& p : personas) {
        if (p.getCiudadNacimiento() == valor) {
            if (!longeva || parseFecha(p.getFechaNacimiento()) < parseFecha(longeva->getFechaNacimiento())) {
                longeva = &p;
            }
        }
    }

    if (!longeva) {
        throw std::runtime_error("No hay personas registradas en " + valor);
    }

    return longeva;
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


const Persona* buscarMayorPatrimonioR(const std::vector<Persona>& personas, const std::string& valor, int opcion) {
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

// Buscar declarantes por Valor
void buscarDeclarantesV(const std::vector<Persona> personas, char grupo){

    int contador = 1;
    std::cout << "\n=== PERSONAS EN EL GRUPO (" << grupo << ") ===\n";
    for (Persona p : personas) {
        if (p.getGrupo() == grupo) {
            std::cout << contador << ". ";
            p.mostrarResumen();
            contador++;
            std::cout << "\n";
        }
    }

    std::cout << "\n";
    std::cout << "Número de personas declarantes en el grupo " 
    << grupo << ": " << contador-1 << std::endl;

}


// Buscar declarantes por referencia
void buscarDeclarantesR(const std::vector<Persona>& personas, char grupo) {
    
    int contador = 1;
    std::cout << "\n=== PERSONAS EN EL GRUPO (" << grupo << ") ===\n";
    for (const Persona& p : personas) {
        if (p.getGrupo() == grupo) {
            std::cout << contador << ". ";
            p.mostrarResumen();
            contador++;
            std::cout << "\n";
        }
    }

    std::cout << "\n";
    std::cout << "Número de personas declarantes en el grupo " 
    << grupo << ": " << contador-1 << std::endl;
}

void validarDeclarantesV(const std::vector<Persona> personas, std::string id){

    bool band = false;

    for(Persona p : personas){
        if (p.getId() == id) {
            if (p.getDeclaranteRenta()){
                char grupo = asignarGrupo(id);
                if (grupo == 'A' || grupo == 'B' || grupo == 'C') {
                    std::cout << "\nLa persona existe y su tipo de grupo es: " 
                                << grupo << std::endl;
                    p.mostrarResumen();
                    std::cout << "\n";
                    band = true;
                }
            } else {
                std::cout << "\nLa persona existe pero no es declarante";
                band = true;
            }
        }
    }

    if (!band) {
        char grupo = asignarGrupo(id);
        if (grupo != '?') {
            std::cout << "\nEl usuario no se encontró." << std::endl;
            std::cout << "El Id correspondería al grupo " << grupo << "\n" << std::endl;
        }
    }
}

// Validar declarantes por referencia
void validarDeclarantesR(const std::vector<Persona>& personas, const std::string& id) {

    bool band = false;

    for (const Persona& p : personas) {
        if (p.getId() == id) {
            if (p.getDeclaranteRenta()){
                char grupo = asignarGrupo(id);
                if (grupo == 'A' || grupo == 'B' || grupo == 'C') {
                    std::cout << "\nLa persona existe y su tipo de grupo es: " 
                                << grupo << std::endl;
                    p.mostrarResumen();
                    std::cout << "\n";
                    band = true;
                }
            } else {
                std::cout << "\nLa persona existe pero no es declarante";
                band = true;
            }
        }
    }

    if (!band) {
        char grupo = asignarGrupo(id);
        if (grupo != '?') {
            std::cout << "\nEl usuario no se encontró." << std::endl;
            std::cout << "El Id correspondería al grupo " << grupo << std::endl;
        }
    }
}

double porcentajePatrimonioMayor650MV(std::vector<Persona> personas) {

    int contador = 0;
    for (auto p : personas) {
        if (p.getPatrimonio() > 650000000) {
            contador++;
        }
    }

    double porcentaje = (static_cast<double>(contador) / personas.size()) * 100.0;
    return porcentaje;
}

void porcentajePatrimonioMayor650MR(const std::vector<Persona>& personas, double& porcentaje) {

    int contador = 0;
    for (const auto& p : personas) {
        if (p.getPatrimonio() > 650000000) {
            contador++;
        }
    }

    porcentaje = (static_cast<double>(contador) / personas.size()) * 100.0;
}


std::string ciudadMenorIngresoPromedioV(std::vector<Persona> personas) {

    std::map<std::string, std::pair<double, int>> ingresosPorCiudad;

    for (auto p : personas) {
        ingresosPorCiudad[p.getCiudadNacimiento()].first += p.getIngresosAnuales();
        ingresosPorCiudad[p.getCiudadNacimiento()].second += 1;
    }

    std::string ciudadMin;
    double menorPromedio = std::numeric_limits<double>::max();

    for (auto& par : ingresosPorCiudad) {
        double promedio = par.second.first / par.second.second;
        if (promedio < menorPromedio) {
            menorPromedio = promedio;
            ciudadMin = par.first;
        }
    }

    return ciudadMin;
}

void ciudadMenorIngresoPromedioR(const std::vector<Persona>& personas, std::string& ciudad) {

    std::map<std::string, std::pair<double, int>> ingresosPorCiudad;

    for (const auto& p : personas) {
        ingresosPorCiudad[p.getCiudadNacimiento()].first += p.getIngresosAnuales();
        ingresosPorCiudad[p.getCiudadNacimiento()].second += 1;
    }

    double menorPromedio = std::numeric_limits<double>::max();

    for (const auto& par : ingresosPorCiudad) {
        double promedio = par.second.first / par.second.second;
        if (promedio < menorPromedio) {
            menorPromedio = promedio;
            ciudad = par.first;
        }
    }
}

const Persona buscarMasJovenDeclaranteV(std::vector<Persona> personas) {

    std::vector<Persona> declarantes;
    for (auto p : personas) {  
        if (p.getDeclaranteRenta()) {
            declarantes.push_back(p);
        }
    }

    if (declarantes.empty()) {
        std::cout << "No hay personas que declaren renta.\n";
        return personas[0];
    }

    Persona masJoven = declarantes[0];
    for (auto p : declarantes) {  
        if (parseFecha(p.getFechaNacimiento()) > parseFecha(masJoven.getFechaNacimiento())) {
            masJoven = p;
        }
    }

    return masJoven;
}

const Persona* buscarMasJovenDeclaranteR(const std::vector<Persona>& personas) {

    const Persona* masJoven = nullptr;

    for (const auto& p : personas) { 
        if (p.getDeclaranteRenta()) {
            if (masJoven == nullptr || 
                parseFecha(p.getFechaNacimiento()) > parseFecha(masJoven->getFechaNacimiento())) {
                masJoven = &p;
            }
        }
    }

    if (masJoven == nullptr) {
        std::cout << "No hay personas que declaren renta.\n";
        return masJoven;
    }

    return masJoven;
}