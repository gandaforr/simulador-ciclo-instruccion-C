## 💻 Simulador del Ciclo de Instrucciones de CPU (Arquitectura Von Neumann)

![C](https://img.shields.io/badge/Language-C-blue.svg)
![Architecture](https://img.shields.io/badge/Architecture-Von_Neumann_%2F_IAS-orange.svg)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen.svg)

El objetivo de este proyecto es modelar los componentes fundamentales de un computador a nivel de transferencia de registros (RTL) y demostrar cómo interactúan la **Unidad de Control**, la **Unidad Aritmético Lógica (ALU)** y la **Memoria RAM** durante la ejecución de un programa en lenguaje máquina.

El sistema implementa una arquitectura modular donde el módulo `lectura.h` se encarga del **procesamiento y parseo de un archivo de texto externo (`.txt`)** que contiene el programa en instrucciones hexadecimales. Estas instrucciones se cargan dinámicamente en la memoria RAM antes de iniciar el ciclo continuo de ejecución (`Fetch-Decode-Execute`) hasta encontrar la instrucción de paro (`HTL`).

---

## 📌 Descripción del Proyecto

El objetivo de este proyecto es modelar los componentes fundamentales de un computador a nivel de transferencia de registros (RTL) y demostrar cómo interactúan la **Unidad de Control**, la **Unidad Aritmético Lógica (ALU)** y la **Memoria RAM** durante la ejecución de un programa en lenguaje máquina.

El simulador lee las instrucciones iniciales de memoria desde un archivo externo (`lectura.h`) y ejecuta de forma continua el ciclo de instrucciones hasta encontrar una instrucción de paro (`HTL`).

---

## 🏗️ Componentes de Arquitectura Simulados

El sistema implementa las estructuras fundamentales de la arquitectura **IAS / Von Neumann**:

| Componente | Registro / Campo | Descripción |
| :--- | :--- | :--- |
| **Unidad de Control** | `contador` (PC) | **Program Counter**: Mantiene la dirección de la siguiente instrucción. |
| | `instruccion` (IR) | **Instruction Register**: Almacena la instrucción actual que se va a procesar. |
| | `intr` / `num` | Separación de la instrucción en **Opcode** (4 bits) y **Operando/Dirección** (4 bits). |
| | `decodifi` | Puntero a función dinámico que mapea la instrucción con su subrutina de ejecución. |
| **Memoria** | `tabla_memoria[16]` | Matriz de 16 posiciones que simula la memoria principal (RAM). |
| | `direccion` (MAR) | **Memory Address Register**: Dirección de memoria activa. |
| | `dato` (MBR) | **Memory Buffer Register**: Contenido leído o por escribir en memoria. |
| **ALU** | `acumulador` (AC) | **Accumulator**: Registro principal de trabajo aritmético y lógico. |
| | `MQ` | **Multiplier Quotient**: Registro auxiliar para operaciones de multiplicación. |

---

## 🔄 El Ciclo de Instrucciones (Fetch - Decode - Execute)

El bucle principal (`while(EnMarcha)`) emula rigurosamente las etapas del ciclo de máquina:
<img width="1518" height="385" alt="image" src="https://github.com/user-attachments/assets/435b4f64-012d-4d17-b352-9293ad655288" />


1. **Fetch (Búsqueda):** Se obtiene la instrucción de la memoria mediante el puntero `contador` (PC) y se incrementa el contador de programa.
2. **Decode (Decodificación):** Se extraen los 4 bits superiores (`Opcode`) y los 4 bits inferiores (`Operando`). La función `auxiliarDecodifi` vincula el puntero a función `decodifi` con la operación correspondiente.
3. **Execute (Ejecución):** Se busca el dato en la dirección especificada por el operando y se ejecuta la instrucción invocando `uni.decodifi(...)`.
4. **State Trace:** Se imprime el estado completo de los registros de la CPU y memoria tras cada paso.

---

## 🛠️ Conjunto de Instrucciones (ISA)

El simulador soporta las siguientes operaciones básicas:

* `LoadMX`: Carga el valor de una posición de memoria en el acumulador.
* `LoadMQ` / `Loadmq`: Carga de datos hacia/desde el registro auxiliar `MQ`.
* `storeMX`: Guarda el valor del acumulador en una posición de memoria.
* `add`: Suma el valor de la memoria especificada al acumulador.
* `mul`: Multiplica el acumulador utilizando el registro `MQ`.
* `HTL` (Halt): Detiene el bucle de ejecución de la CPU.

---

## Autor
Odilón Avila Castrejón — Estudiante de Ingeniería en Computación (UMSNH)
LinkedIn: https://www.linkedin.com/in/odil%C3%B3n-avila-castrejon-484757384/

