# parcial1so2025-2
Parcial 1 Sistemas Operativos Semestre 2025-2






















































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
