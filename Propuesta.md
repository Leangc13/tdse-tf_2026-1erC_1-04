
<img src="https://www.fi.uba.ar/images/logo-fiuba.png" alt="logo-fiuba" width="50%">

# **Sistema de Riego Automático con Gestión de Viento (SRAGV)**

**Autores:** Garcia Caneva Leandro, Vargas Joaquin, Molina Aban Florencia

**Padrones:** 1034756, 104323, 104153

**Fecha:** 1er cuatrimestre 2026

---

## **1. Selección del proyecto a implementar**

### **1.1 Objetivo del proyecto y resultados esperados**

El objetivo de este proyecto es diseñar e implementar un sistema embebido de riego automático que tome decisiones inteligentes en función del estado del viento. La idea central es que el sistema **modifique o bloquee el riego según la velocidad y dirección del viento**, dado que en condiciones de viento intenso el agua se dispersa fuera de la zona de cultivo, haciendo el riego ineficiente o directamente perjudicial.

Para evitar complejidades mecánicas en el prototipo físico, la velocidad y la dirección del viento se **emularán mediante dos potenciómetros analógicos o un joystick** conectados al ADC del microcontrolador. El riego se representará a través de **4 sectores independientes indicados por LEDs** (Norte, Sur, Este, Oeste). La condición de luz ambiente, leída mediante una **fotocélula (LDR)** conectada al ADC, determinará si el horario es apto para regar (por ejemplo, sin riego nocturno). En función del nivel de viento se definen tres comportamientos:

- **Viento bajo o nulo:** se activan en secuencia todos los sectores habilitados por el usuario.
- **Viento moderado:** se activan únicamente los sectores a favor del viento, evitando el desperdicio en las zonas que quedarían contra el viento.
- **Viento crítico → Modo FALLA:** el riego se suspende completamente y se activa una alarma sonora (buzzer) y visual (LED de falla).

El sistema contará con tres modos de operación:

- **Modo NORMAL:** monitorea continuamente los sensores analógicos (joystick/potenciómetros y LDR) y activa los LEDs de sector correspondientes según los umbrales configurados.
- **Modo SET_UP:** el usuario configura los umbrales de viento (moderado y crítico), los sectores habilitados y el horario de riego a través de un menú interactivo en pantalla LCD, o de forma remota mediante la aplicación Bluetooth.
- **Modo FALLA:** ante viento crítico o lectura inválida de algún sensor, el sistema inhibe todos los LEDs de sector, activa el LED de falla y el buzzer, y muestra en el LCD el motivo de la falla.

Como resultados esperados se pretende obtener un prototipo funcional montado sobre una **placa experimental con componentes soldados** (sin protoboard ni cables Dupont), que demuestre de forma clara los tres modos de operación e implemente:

- Lectura continua de los sensores analógicos (joystick/potenciómetros + LDR) por ADC.
- Control de los 4 LEDs de sector (Norte, Sur, Este, Oeste) y LEDs de estado mediante GPIO.
- Persistencia de la configuración (umbrales, sectores habilitados, horario de riego) en **EEPROM externa vía I²C**.
- Monitoreo y configuración remota mediante **Bluetooth (módulo HM-10)** desde una aplicación en el celular.
- Seguimiento del tiempo interno mediante un **soft RTC** basado en el contador de ticks de 1 ms del SysTick.
- Arquitectura de software **Bare Metal Event-Triggered**, con código estrictamente no bloqueante organizado en máquinas de estados.

---

### **1.2 Proyectos similares**

En base al objetivo se proponen posibles proyectos que requieren el uso de hardware específico y se ponderan los aspectos a tener en cuenta para seleccionar el más adecuado.

1. **Base:** sistema de riego por viento con emulación mediante joystick/potenciómetros, 4 LEDs de sector, LCD, Bluetooth HM-10 y EEPROM. El viento es el único sensor de entrada; la lógica decide qué sectores regar o si suspender el riego.
2. **Base + fotosensor:** igual que el proyecto Base, pero incorpora además una fotocélula (LDR) como segundo sensor analógico. La luz ambiente agrega la condición de horario diurno/nocturno al control del riego, enriqueciendo la lógica sin aumentar significativamente la complejidad.
3. **Base + fotosensor + control de humedad:** extiende el proyecto anterior con un sensor capacitivo de humedad de suelo (salida analógica). La humedad del suelo pasa a ser una condición adicional para activar el riego. Más completo, pero requiere más tiempo de integración y calibración del sensor de humedad.

Para comparar estas alternativas se tienen en cuenta seis aspectos:

1. **Disponibilidad del hardware (peso: 10):** si todos los módulos necesarios se consiguen en el mercado local sin importar.
2. **Viabilidad técnica para el equipo (peso: 10):** qué tan tratable es la integración considerando que el equipo es principiante en sistemas embebidos.
3. **Funcionalidad del sistema (peso: 8):** riqueza de las funciones que ofrece el prototipo al usuario final.
4. **Tiempo de implementación (peso: 9):** qué tan ajustado es el cronograma para lograr un prototipo funcional dentro del cuatrimestre.
5. **Costo estimado (peso: 6):** costo total aproximado de hardware incluyendo sensores, módulos y placa de desarrollo.
6. **Interés personal del equipo (peso: 8):** motivación para trabajar con el proyecto.

La disponibilidad del hardware es el criterio de mayor peso porque el equipo no puede permitirse demoras por componentes importados; todos los módulos deben conseguirse en el mercado local (MercadoLibre, Tienda Electrónica, etc.). La viabilidad técnica recibe el mismo peso máximo dado que es el primer proyecto de la materia y el tiempo disponible es menor a dos meses. El tiempo de implementación tiene alta ponderación por la misma razón: se prioriza un prototipo funcional sobre uno rico en funciones. La funcionalidad y el interés personal son criterios secundarios pero importantes para motivar al equipo. El costo recibe el menor peso porque los componentes involucrados son económicos en todos los casos.

| Criterio | Peso | Base | Base Pond. | Base + fotosensor | B+foto Pond. | B+foto+humedad | B+foto+hum Pond. |
| :---- | :--: | :--: | :--: | :--: | :--: | :--: | :--: |
| Disponibilidad del hardware | 10 | 10 | 100 | 10 | 100 | 9 | 90 |
| Viabilidad técnica para el equipo | 10 | 10 | 100 | 9 | 90 | 6 | 60 |
| Funcionalidad del sistema | 8 | 6 | 48 | 9 | 72 | 10 | 80 |
| Tiempo de implementación | 9 | 10 | 90 | 9 | 81 | 5 | 45 |
| Costo estimado | 6 | 9 | 54 | 9 | 54 | 7 | 42 |
| Interés personal del equipo | 8 | 7 | 56 | 9 | 72 | 8 | 64 |
| **TOTAL** | — | — | **448** | — | **469** | — | 381 |

<p align="center"><em>Tabla 1.2.1: Comparación de alternativas de proyecto</em></p>

---

### **1.3 Selección del proyecto**

Se seleccionó el proyecto **Base + fotosensor** por ser el que mejor equilibra funcionalidad, viabilidad técnica y tiempo de implementación.
Los desafíos principales identificados son la integración de múltiples canales ADC con lecturas no bloqueantes, el diseño de la máquina de estados principal con tres modos bien diferenciados, y la implementación del menú interactivo sobre LCD vía I²C.

#### **1.3.1 Diagrama en bloques**

<img width="1536" height="1024" alt="7a618d47-7b33-423d-8385-0af6063f6b5f" src="https://github.com/user-attachments/assets/3a9c7afa-f392-46e0-99d6-5bf6d9e0f1ff" />

---

## **2. Elicitación de requisitos y casos de uso**

Argentina se destaca a nivel mundial como un actor clave en la producción de materias primas y bienes agrícolas. Sin embargo, optimizar el uso de los recursos hídricos en los sistemas de producción locales representa un desafío crítico. En regiones de alta productividad o con características geográficas particulares como la Patagonia argentina, la presencia de vientos intensos y constantes se convierte en un factor adverso para el riego por aspersión tradicional. Las ráfagas de viento provocan la "deriva" del agua (desviándola de las zonas objetivo), aumentan la tasa de evaporación antes de que el recurso llegue al suelo y generan pérdidas económicas significativas y un desperdicio severo del recurso hídrico.
En este contexto, cualquier sistema de riego inteligente que incorpore el viento como variable de decisión tiene aplicación directa y relevancia real. Esto posiciona al SRAGV como una solución viable, de bajo costo y con componentes accesibles en el mercado local.

---

### **2.1 Requisitos del proyecto**

| Grupo | ID | Descripción |
| :---- | :---- | :---- |
| Sensores analógicos | 1.1 | El sistema contará con un joystick analógico (o dos potenciómetros) para emular la velocidad y dirección del viento, conectado a dos canales ADC del STM32. |
| | 1.2 | El eje X del joystick (o potenciómetro 1) representará la velocidad del viento; el eje Y (o potenciómetro 2) representará la dirección del viento (Norte, Sur, Este, Oeste). |
| | 1.3 | El sistema contará con una fotocélula (LDR) conectada a un tercer canal ADC del STM32 para medir la luminosidad ambiente. |
| | 1.4 | El sistema realizará lecturas periódicas de los tres canales ADC (velocidad de viento, dirección de viento, luz) mediante DMA, con callback de conversión completa, sin polling bloqueante. |
| | 1.5 | El sistema aplicará un filtro de promediado simple (media de N muestras) sobre las lecturas de velocidad de viento para reducir el efecto de ruido eléctrico. |
| Actuadores — LEDs de sector | 2.1 | El sistema contará con 4 LEDs que representan los sectores de riego: Norte (N), Sur (S), Este (E) y Oeste (O). |
| | 2.2 | Con viento bajo o nulo, se activarán en secuencia todos los sectores que el usuario haya habilitado en el SET_UP. |
| | 2.3 | Con viento moderado, se activarán únicamente los sectores a favor del viento (determinados por la lectura de dirección del joystick), apagando los sectores contrarios. |
| | 2.4 | Con viento crítico (Modo FALLA), todos los LEDs de sector se apagarán. |
| | 2.5 | Cada LED de sector permanecerá encendido durante el tiempo de riego configurado por el usuario, apagándose de forma no bloqueante al cumplirse el tiempo. |
| Indicadores de estado | 3.1 | El sistema contará con al menos 2 LEDs de estado adicionales: uno verde (sistema activo / riego en curso) y uno rojo (falla). |
| | 3.2 | Durante el Modo FALLA, el LED rojo parpadeará a 1 Hz de forma no bloqueante. |
| | 3.3 | El sistema contará con un buzzer activo para señales sonoras: beep corto al confirmar una acción, beep largo en falla, melodía breve al completar un ciclo de riego. |
| | 3.4 | El sistema contará con un display LCD 16×2 con interfaz I²C para mostrar el estado actual, lecturas de sensores y menús de configuración. |
| Modos de operación | 4.1 | El sistema operará en tres modos bien diferenciados: NORMAL, SET_UP y FALLA. Las transiciones entre modos se implementarán mediante una máquina de estados. |
| | 4.2 | En Modo NORMAL, el sistema monitorea continuamente los sensores analógicos y activa los LEDs de sector según los umbrales configurados y la condición de luz. |
| | 4.3 | En Modo NORMAL, si la LDR indica ausencia de luz (noche), el sistema puede inhibir el riego si el usuario configuró la opción "solo riego diurno". |
| | 4.4 | En Modo SET_UP, el sistema suspende el riego automático y presenta al usuario el menú de configuración en el LCD. |
| | 4.5 | En Modo FALLA, el sistema inhibe todos los LEDs de sector, activa el LED rojo y el buzzer, y muestra en el LCD el motivo de la falla. |
| | 4.6 | El sistema iniciará siempre en Modo NORMAL con todos los LEDs de sector apagados. |
| Menú interactivo | 5.1 | El sistema contará con 2 botones para navegar el menú (avanzar opción, confirmar/seleccionar), con antirrebote implementado por software de forma no bloqueante. |
| | 5.2 | El sistema contará con un conjunto de DIP switches para opciones de configuración fija (por ejemplo, habilitar/deshabilitar sectores o el modo solo riego diurno). |
| | 5.3 | El menú permitirá configurar: umbral de viento moderado, umbral de viento crítico, duración del ciclo de riego por sector, y habilitación/deshabilitación de sectores individuales. |
| | 5.4 | El menú mostrará en el LCD las lecturas actuales de velocidad de viento (en porcentaje de rango ADC) y el nivel de luz ambiente. |
| Comunicación Bluetooth | 6.1 | El sistema se comunicará con el módulo HM-10 a través de UART, con un protocolo de comandos ASCII simple (por ejemplo: `GET:STATE`, `SET:WIND_THR:60`). |
| | 6.2 | La aplicación Bluetooth permitirá visualizar en tiempo real el estado de cada sector, el nivel de viento y el nivel de luz. |
| | 6.3 | La aplicación Bluetooth permitirá configurar los umbrales de viento y la duración del riego de forma remota. |
| | 6.4 | La aplicación Bluetooth permitirá activar o desactivar manualmente un sector de riego. |
| Almacenamiento EEPROM | 7.1 | El sistema almacenará la configuración del usuario (umbrales de viento, sectores habilitados, duración de riego, modo solo diurno) en una EEPROM externa AT24C02 vía I²C. |
| | 7.2 | Al iniciar, el sistema leerá la configuración almacenada en EEPROM y validará su integridad mediante un byte de verificación (checksum simple). |
| | 7.3 | Si la EEPROM no tiene configuración válida (primer uso o datos corruptos), el sistema cargará valores por defecto y los escribirá en EEPROM. |
| Soft RTC | 8.1 | El sistema llevará un contador de ticks de 1 ms basado en el SysTick del STM32, utilizado como referencia de tiempo para la duración del riego y los temporizadores de las tareas no bloqueantes. |
| Bajo consumo | 9.1 | En ausencia de actividad (sin pulsaciones, sin riego activo, sin comunicación BT) por un tiempo configurable, el sistema entrará en modo de bajo consumo (Sleep) del STM32. |
| | 9.2 | El sistema saldrá del modo Sleep ante interrupción de botón, tick de SysTick o recepción de dato por UART. |
| Robustez y seguridad | 10.1 | El sistema detectará lecturas de ADC fuera de rango (saturadas o nulas) como condición de falla y transitará al Modo FALLA. |
| | 10.2 | La arquitectura de software seguirá el patrón Escrutar / Procesar / Actuar, organizada de forma modular (un módulo por periférico). |
| | 10.3 | El super-loop completará cada vuelta en menos de 1 ms. |

<p align="center"><em>Tabla 2.1: Requisitos del proyecto</em></p>

---

### **2.2 Caso de uso 1: Ciclo de riego completo con viento bajo**

| Elemento | Definición |
| :---- | :---- |
| **Disparador** | El soft RTC indica que se cumplió el intervalo de riego programado y el joystick indica velocidad de viento baja. |
| **Precondiciones** | El sistema está encendido y en Modo NORMAL. Los dos canales ADC del joystick y el canal de la LDR están operativos y entregan lecturas válidas. La configuración fue cargada correctamente desde EEPROM. La LDR indica condición de luz diurna (si la opción "solo riego diurno" está habilitada). |
| **Flujo principal** | El sistema lee el canal ADC de velocidad de viento: el valor cae por debajo del umbral de viento moderado. El sistema determina que corresponde activar todos los sectores habilitados. Activa los LEDs de sector en secuencia (N → S → E → O) de forma no bloqueante, manteniendo cada uno encendido durante el tiempo de riego configurado. El LED verde de estado permanece encendido durante todo el ciclo. Al finalizar la secuencia, el LED verde se apaga, el buzzer emite un beep corto y el LCD muestra "RIEGO COMPLETADO". Si hay conexión Bluetooth activa, el sistema notifica a la app el resultado del ciclo. |
| **Flujos alternativos** | a. Durante el ciclo, el joystick sube por encima del umbral de viento moderado: el sistema cancela los sectores restantes, muestra "VIENTO – RIEGO PAUSADO" en el LCD y aguarda a que la velocidad baje para retomar. b. Durante el ciclo, el joystick supera el umbral crítico: el sistema transita a Modo FALLA, apaga todos los LEDs de sector y activa la alarma. c. El usuario presiona el botón de cancelación: el riego se detiene, todos los LEDs de sector se apagan y el sistema vuelve al estado de monitoreo normal. |

<p align="center"><em>Tabla 2.2: Caso de uso 1 – Ciclo de riego completo con viento bajo</em></p>

---

### **2.3 Caso de uso 2: Riego parcial por viento moderado**

| Elemento | Definición |
| :---- | :---- |
| **Disparador** | El soft RTC indica que se cumplió el intervalo de riego pero el joystick indica viento moderado con dirección Norte. |
| **Precondiciones** | El sistema está encendido y en Modo NORMAL. Los sensores ADC están operativos. La velocidad de viento leída supera el umbral moderado pero no el umbral crítico. La dirección de viento apunta hacia el Norte (eje Y del joystick). |
| **Flujo principal** | El sistema lee velocidad y dirección del joystick. Determina que el nivel de viento es moderado: no se pueden regar todos los sectores. Calcula los sectores a favor del viento según la dirección leída: con viento del Norte, los sectores Sur y Este/Oeste quedan a favor; el sector Norte queda contra el viento y se inhibe. Activa únicamente los LEDs de sector habilitados y a favor del viento. El LED amarillo del viento (si se dispone) o el LED verde parpadea para indicar riego parcial. El LCD muestra "VIENTO MODERADO – RIEGO PARCIAL" y los sectores activos. Al finalizar el tiempo de riego, el buzzer emite un beep corto. |
| **Flujos alternativos** | a. La velocidad de viento sube al nivel crítico durante el ciclo parcial: el sistema cancela todos los sectores restantes y transita al Modo FALLA. b. La velocidad de viento baja al nivel bajo durante el ciclo: el sistema reactiva los sectores inhibidos y continúa el riego completo. c. El usuario activa el override por DIP switch: el sistema fuerza el riego en todos los sectores habilitados ignorando la condición de viento, mostrando "MODO MANUAL" en el LCD. |

<p align="center"><em>Tabla 2.3: Caso de uso 2 – Riego parcial por viento moderado</em></p>

---

### **2.4 Caso de uso 3: Configuración de umbrales en Modo SET_UP**

| Elemento | Definición |
| :---- | :---- |
| **Disparador** | El usuario quiere modificar el umbral de viento moderado, el umbral de viento crítico o la duración del ciclo de riego por sector. |
| **Precondiciones** | El sistema está encendido. No hay un ciclo de riego activo. El display LCD está operativo. |
| **Flujo principal** | El usuario mantiene presionado el botón de menú durante 2 segundos. El sistema transita al Modo SET_UP, suspende el riego automático y muestra en el LCD el menú de configuración. El usuario navega con el botón de avance entre las opciones: umbral viento moderado, umbral viento crítico, duración de riego, habilitación de sectores, modo solo diurno. Al seleccionar una opción, el LCD muestra el valor actual y el usuario lo ajusta incrementando/decrementando con los botones. Al confirmar, el sistema escribe el nuevo valor en EEPROM y muestra brevemente "GUARDADO" en el LCD. El usuario sale del SET_UP presionando el botón de menú: el sistema vuelve al Modo NORMAL con los nuevos parámetros activos. Alternativamente, la app Bluetooth puede enviar los nuevos parámetros vía UART; el sistema los aplica y los guarda en EEPROM del mismo modo. |
| **Flujos alternativos** | a. El usuario sale del SET_UP sin confirmar cambios: el sistema mantiene los parámetros anteriores sin modificar la EEPROM. b. Falla de escritura en EEPROM (dispositivo no responde en I²C): el sistema muestra "ERROR EEPROM" en el LCD, emite un sonido de falla y vuelve al Modo NORMAL con los parámetros previos en RAM (no persistidos). c. La app envía un valor fuera del rango válido (por ejemplo, umbral de viento negativo): el sistema descarta el comando, notifica el error a la app y mantiene el parámetro sin cambios. |

<p align="center"><em>Tabla 2.4: Caso de uso 3 – Configuración de umbrales en Modo SET_UP</em></p>

---

## **3. Detalle de hardware y programación**

### **3.1 Hardware obligatorio**

| Componente | Uso en el proyecto |
| :---- | :---- |
| STM32 Nucleo-F103RB | Microcontrolador principal |
| DIP Switches (4 posiciones) | Habilitar/deshabilitar cada zona de riego de forma manual |
| Botones (2) | Navegación del menú interactivo |
| LEDs (4 zonas + 1 alarma) | Indicación visual del estado de cada zona de riego y alarma |
| Buzzer | Alarma en modo FALLA y confirmaciones sonoras de cambio de modo |
| Módulo HM-10 (BLE) | Comunicación con app móvil para monitoreo y configuración remota |
| EEPROM 24LC02 (I²C) | Almacenamiento persistente de franjas horarias y umbrales de viento |
| Potenciómetro 1 (ADC) | Emulación analógica de velocidad de viento |
| Potenciómetro 2 (ADC) | Emulación analógica de dirección de viento |
| Fotocélula LDR (ADC) | Medición de luminosidad ambiente (condición diurna/nocturna) |
| Joystick analógico (ADC) *(alternativa a los dos potenciómetros)* | Emulación de velocidad (eje X) y dirección (eje Y) del viento |
| LCD 16×2 con módulo I²C | Visualización del modo, tiempo interno, valores de viento y estado de zonas |
| Placa experimental soldada | Interconexión de componentes sin protoboard ni cables Dupont |

### **3.2 Programación obligatoria**

| Requisito | Implementación en el proyecto |
| :---- | :---- |
| Bare Metal, Event-Triggered System | Sin RTOS; toda la lógica gestionada por eventos y el Super-Loop |
| Estructurada/Modular (Escrutar/Procesar/Actuar) | Módulos: `sensor.c`, `controller.c`, `actuator.c`, `menu.c`, `eeprom.c`, `ble.c`, `rtc_soft.c` |
| Ejecutor cíclico Super-Loop (< 1 ms/ciclo) | Loop principal con polling de colas de eventos; ninguna tarea bloquea el ciclo |
| Tick = 1 ms (SysTick → Callback) | Base de tiempo para el soft RTC, parpadeo de LED, muestreo ADC y secuenciador de zonas |
| Tareas no bloqueantes temporizadas y por evento | Secuenciador de zonas, parpadeo de alarma, escritura EEPROM diferida, soft RTC |
| Máquinas de estado con interfaces (colas, estructuras, arrays) | FSM de modos (NORMAL / SET_UP / FALLA) + FSM de cada zona; comunicación por colas de eventos |
| Menú interactivo | Navegable por botones físicos y por comandos BLE desde la app |
| Bajo consumo | Sleep mode del STM32 entre ciclos inactivos; wakeup por SysTick o interrupciones externas |
| Al menos dos modos de operación | NORMAL, SET_UP y FALLA (tres modos) |
| Periféricos I²C | EEPROM 24LC02 y LCD 16×2 por I²C |
| ADC con DMA + Callback | Lectura continua de los dos potenciómetros (o joystick) y la LDR sin bloquear el Super-Loop |
| UART con interrupciones | Comunicación con HM-10 (recepción en ISR, procesamiento en loop) |
