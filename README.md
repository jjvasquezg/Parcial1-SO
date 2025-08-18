# README IMPORTANTE
Parcial 1 Sistemas Operativos Semestre 2025-2

## Integrantes

- Juan José Vasquez
- Santiago Alvarez
- Sebastian Giraldo

## Métricas y resultados:

### 1. `struct` vs `class`

Primero veamos una pequeña comparación de `struct` y `class`

| Aspecto                  | `struct`                                  | `class`                                   |
|--------------------------|--------------------------------------------|-------------------------------------------|
| Modificador por defecto  | `public`                                   | `private`                                  |
| Uso común                | Datos simples, estructuras POD             | Abstracción, encapsulamiento, lógica      |
| Herencia                 | Permitida, pero poco usada                 | Comúnmente usada                          |
| Encapsulamiento          | Manual (con `private:` explícito)         | Por defecto                               |
| Overhead                 | Mínimo                                     | Ligero (más con herencia/virtuales)      |
| Velocidad de acceso      | Ligeramente mejor (menos restricciones)    | Similar, depende de diseño y acceso       |

Para calcular nuestros resultados, cada integrante del grupo realizo una prueba con +1M de datos tanto para struct como para class y sacamos un promedio, estos fueron nuestros resultados:

**NOTA IMPORTANTE: En el caso de la pregunta en la cual se imprimen todas las personas declarantes que pertenencen a x grupo, en el video se muestra el tiempo de busqueda + el tiempo de impresión, por lo que para esta comparación solo se tuvo en cuenta el tiempo de busqueda.**

**NOTA IMPORTANTE 2: Para la memoria y el tiempo totales se restaron las respectivas metricas al generar los datos**

![comparativa](https://github.com/user-attachments/assets/bbe0e424-921a-42ea-958b-b808b8f35a4e)

## Video

A continuación anexamos link del video explicativo y demostrativo del código en funcionamiento:  
[Link al video](https://drive.google.com/file/d/10dmXq6jP1kz55r8UxE3UqnKzHeSYbKy6/view?usp=sharing)


## Preguntas Obligatorias

### 1. Persona más longeva:

La función buscarLongevaV se encarga de encontrar a la persona más longeva dentro de un conjunto de personas. Si se recibe el valor "pais", la función recorre todo el vector comparando las fechas de nacimiento para quedarse con la persona de mayor edad. En caso de que se pase una ciudad específica, primero filtra las personas que hayan nacido en esa ciudad y, entre ellas, selecciona a la más longeva. El resultado lo retorna por valor, es decir, como una copia del objeto.

La función buscarLongevaR cumple la misma tarea, pero trabaja por referencia. Si el valor es "pais", recorre todas las personas del vector y guarda un puntero a la persona más longeva. Si se trata de una ciudad en particular, solo compara las personas nacidas en esa ciudad y retorna un puntero a la más longeva. De esta forma evita copias innecesarias y retorna un const Persona*.

### 2. Persona con mayor patrimonio:

En el caso de la función buscarMayorPatrimonioV, su propósito es encontrar la persona con el patrimonio más alto. Para lograrlo define un lambda que determina si una persona debe entrar en la búsqueda dependiendo de la opción: puede ser sin filtro, filtrando por ciudad o filtrando por grupo. Una vez definido el criterio, la función recorre el vector y compara patrimonios, quedándose con el mayor. El retorno es un objeto Persona por valor.

La función buscarMayorPatrimonioR es la versión por referencia. Utiliza el mismo filtro por medio del lambda, pero en lugar de hacer copias de objetos guarda un puntero a la persona que tiene el mayor patrimonio en cada iteración, devolviendo finalmente un const Persona*.

### 3. Declarantes de renta:

Por otro lado, la función buscarDeclarantesV recorre la lista de personas y muestra aquellas que pertenecen a un grupo específico. Cada vez que encuentra una coincidencia imprime el resumen de la persona y al final muestra el número total de declarantes en ese grupo. La versión equivalente por referencia, buscarDeclarantesR, repite la lógica, pero recibe el vector como referencia constante y recorre con referencias para mejorar la eficiencia, evitando copias.

Las funciones validarDeclarantesV y validarDeclarantesR permiten verificar si una persona existe dentro del vector a partir de su ID. En caso de encontrarla, se valida si declara renta y, de ser así, se determina a qué grupo pertenece mediante la función asignarGrupo, mostrando luego un resumen de la persona. Si existe pero no declara, se indica el caso; si no se encuentra, se informa a qué grupo pertenecería el ID ingresado. La diferencia entre ambas versiones radica en el manejo por valor o por referencia.

## Preguntas Adicionales:

### 1. Porcentaje de Personas con patrimonio mayor a $ 650M:

La función porcentajePatrimonioMayor650MV calcula el porcentaje de personas que tienen un patrimonio superior a 650 millones. Para lograrlo recorre la lista, cuenta cuántos cumplen la condición, divide entre el total de personas y multiplica por cien, retornando un número de tipo double. 

Su contraparte, porcentajePatrimonioMayor650MR, realiza exactamente el mismo cálculo, pero en vez de retornarlo lo guarda en una variable double que se pasa como argumento por referencia.

### 2. Ciudad con menor Ingreso Anual promedio:

En cuanto a los ingresos, la función ciudadMenorIngresoPromedioV busca la ciudad con menor ingreso promedio anual. Utiliza un mapa que acumula los ingresos y la cantidad de personas por cada ciudad, calcula el promedio correspondiente y selecciona la ciudad con el valor más bajo. Retorna el nombre de la ciudad como un string. 

La versión ciudadMenorIngresoPromedioR sigue el mismo procedimiento, pero en lugar de retornar el resultado lo asigna a un string& recibido como parámetro.

### 3. Declarante de renta más joven:

las funciones buscarMasJovenDeclaranteV y buscarMasJovenDeclaranteR identifican a la persona más joven que declara renta. En el caso de la primera, se filtran primero las personas declarantes en un nuevo vector y luego se compara entre ellas para quedarse con la fecha de nacimiento más reciente, retornando el objeto por valor. 

En la versión por referencia, en lugar de generar un nuevo vector, se recorre directamente la lista original y se guarda un puntero a la persona más joven que declara renta, retornando un const Persona*.

## Preguntas de Pensamiento Crítico

### Memoria: ¿Por qué usar apuntadores reduce 75% de memoria con 10M registros?

Porque si usamos paso por valor estariamos duplicando datos que pueden ser pesados por registro (strings, blobs, structs grandes) y al cambiar a apuntadores o referencias: Se evita la copia, puesto que pasar un std::string/struct grande por valor duplica memoria temporalmente (stack/temporales). Pasarlo por referencia (const T&) o guardar T* evita copias.
Ademas, comparten valores repetidos: si muchos registros comparten campos (p. ej., “Bogotá”, “Cali”, “Medellin”), guardar un solo objeto en un pool y referenciarlo con un T*/std::shared_ptr<T> ahorra muchísimo.

***Ejemplo:***
Supongamos que tenemos 10M de registros, con 3 campos grandes como (País, Ciudad, Grupo) de ~32B de header cada uno (ignoro el buffer para simplificar lo relativo):
Por Valor: ≈ 3 × 32B × 10M = 960 MB (solo headers) + buffers repetidos.
Por Referencia: 3 × 8B × 10M = 240 MB.
Ahorro ≈ 75% solo en headers; si además deduplicamos el buffers de texto, el ahorro real suele ser aún mayor.

### Datos: Si el calendario depende de dígitos del documento, ¿cómo optimizar búsquedas por grupo?

Como los grupos dependen solo de los dos últimos dígitos del ID, podemos aprovechar eso para organizar la información y buscar mucho más rápido:

* Idea de buckets: en lugar de recorrer a lo loco, podemos crear tres “cajones” (A, B y C), y cada persona la metemos directamente en el cajón que le corresponde según sus dos últimos dígitos. Así, cuando quieras buscar alguien del grupo B, ya no recorres todos los registros, sino solo los que están en el cajón B. Esto reduce bastante el tiempo de búsqueda, porque filtras desde el inicio.

* Otra forma: si en lugar de solo 3 grupos quisieramos algo más fino (por ejemplo, dividir por cada par de dígitos 00–99), puedes crear un arreglo con 100 posiciones. Cada posición corresponde a un número del 00 al 99, y ahí guardas las personas que caen en ese final de ID. Buscar se vuelve casi inmediato, porque solo vas al bucket que corresponde.

### Localidad: ¿Cómo afecta el acceso a memoria al usar array de structs vs. vector de clases?

Aquí la idea es cómo la computadora accede a la memoria y qué tan ordenados están los datos.

* Array de Structs (AoS): aquí tenemos que cada fila es un registro completo (Ciudad, País, Grupo, etc.), todo seguido uno detrás del otro.
Ventaja: si siempre recorres todos los registros y usas varios campos, es rápido porque la computadora los tiene todos juntos en la memoria.
Desventaja: si solo necesitas un campo (por ejemplo, “Grupo”), igual te toca cargar todo el registro, aunque no lo uses.

* Vector de clases: parece parecido, pero como las clases a veces guardan cosas en otras partes de la memoria (por ejemplo un string que apunta a otro lado), al recorrer puede volverse más lento porque la computadora tiene que estar “saltando” en diferentes lugares de la memoria.

* Struct of Arrays (SoA): aquí en lugar de guardar todo junto, se separa cada campo en un arreglo propio (uno con todos los paises, otro con todos los grupos, etc.).
Ventaja: si vas a trabajar solo con un campo, es rapidísimo porque están todos seguidos y caben muchos en la caché.
Desventaja: es más incómodo de programar porque tienes que manejar varios arreglos en lugar de una sola estructura.

De manera resumida:
* Si trabajas con registros completos → usa array de structs.
* Si analizas o filtras por un solo campo → mejor separar (SoA).
* Y ojo con las clases, que a veces guardan cosas fuera y eso rompe la “continuidad” en memoria.

### Escalabilidad (Consulta): Si los datos exceden la RAM, ¿cómo usar mmap o memoria virtual?

Cuando tenemos más datos de los que caben en la memoria, el sistema operativo empieza a hacer algunos trucos por decirlo asi: trae de disco solo lo que vamos usando. Una técnica común es mmap, que básicamente te permite tratar un archivo en disco como si fuera un arreglo gigante en memoria.

* Acceso secuencial: si lees los datos en orden, el sistema lo hace muy bien porque va trayendo bloques grandes de una vez (como si fuera un streaming).
* Acceso aleatorio: si saltas por todos lados, se vuelve más lento porque tiene que estar cargando y descargando bloques a cada rato. En ese caso, conviene tener algún índice para ubicar más rápido lo que buscas.

Lo importante es diseñar los datos de manera que sean fáciles de leer sin estar saltando tanto y para esto se usan algunas técnicas como:

* Usar estructuras simples y alineadas.
* Si buscas siempre por una clave (ejemplo: País), tener un índice que te diga en qué parte del archivo está.
* Y si solo necesitas una columna de información, guardar los datos por columnas puede ser más eficiente que por filas.
