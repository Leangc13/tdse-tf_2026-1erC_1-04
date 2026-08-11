<div align="center">

<img width="300" alt="FIUBA" src="https://github.com/Matias-J-Sanchez-Q/tdse-tf_2026-1erC_2-02/raw/main/Imagenes/logo-fiuba.png" />

*TA134 – Taller de Sistemas Embebidos*

# Memoria del Trabajo Final: Sistema de Riego Automático con Gestión de Viento (SRAGV)

Sistema de riego inteligente por aspersión con inhibición de sectores en función del viento.

## Autores

| **Apellido, Nombre**      | **Padrón** |
| ------------------------- | ---------- |
| Garcia Caneva, Leandro    | 103476     |
| Vargas, Joaquin           | 104323     |
| Molina Aban, Florencia    | 104153     |

*Docente:* Cruz, Juan Manuel  
*Tutor:* Lutenberg, Ariel  
*Fecha:* 1er cuatrimestre 2026  
*Curso-Grupo:* 1-04

*Trabajo realizado en la Ciudad Autónoma de Buenos Aires.*

---

</div>

# Índice General

- [Capítulo 1: Introducción general](#capítulo-1-introducción-general)
  - [1.1 Análisis de necesidad y objetivo](#11-análisis-de-necesidad-y-objetivo)
  - [1.2 Productos comparables](#12-productos-comparables)
  - [1.3 Productos comerciales disponibles](#13-productos-comerciales-disponibles)
  - [1.4 Comparación con el prototipo desarrollado y alcance](#14-comparación-con-el-prototipo-desarrollado-y-alcance)
- [Capítulo 2: Introducción específica](#capítulo-2-introducción-específica)
  - [2.1 Requisitos del sistema](#21-requisitos-del-sistema)
  - [2.2 Casos de uso](#22-casos-de-uso)
  - [2.3 Descripción de módulos y tecnologías utilizadas](#23-descripción-de-módulos-y-tecnologías-utilizadas)
- [Capítulo 3: Diseño e implementación](#capítulo-3-diseño-e-implementación)
  - [3.1 Arquitectura de Hardware](#31-arquitectura-de-hardware)
  - [3.2 Diseño de Firmware (Máquinas de Estado)](#32-diseño-de-firmware-máquinas-de-estado)
  - [3.3 Esquema Eléctrico y Vistas de Cableado](#33-esquema-eléctrico-y-vistas-de-cableado)
  - [3.4 Aplicación Web](#34-aplicación-web)
- [Capítulo 4: Ensayos y resultados](#capítulo-4-ensayos-y-resultados)
  - [4.1 Pruebas de integración Hardware-Software](#41-pruebas-de-integración-hardware-software)
  - [4.2 Pruebas de campo simuladas](#42-pruebas-de-campo-simuladas)
- [Capítulo 5: Conclusiones](#capítulo-5-conclusiones)
- [Capítulo 6: Uso de herramientas de inteligencia artificial](#capítulo-6-uso-de-herramientas-de-inteligencia-artificial)
- [Capítulo 7: Bibliografía y referencias](#capítulo-7-bibliografía-y-referencias)

---

## Capítulo 1: Introducción general

### 1.1 Análisis de necesidad y objetivo

El objetivo de este proyecto fue desarrollar un sistema de riego automático inteligente gestionado por viento. Si bien el riego automatizado es fundamental para optimizar el uso del agua y garantizar una cobertura uniforme, factores climáticos extremos pueden alterar drásticamente su eficacia. Esta problemática cobra especial relevancia en regiones como la Patagonia argentina, donde la presencia de vientos fuertes y sostenidos —con ráfagas que superan frecuentemente los 80 km/h— es una constante, desviando el agua de su objetivo y generando desperdicio. Además, el impacto continuo del agua contra estructuras cercanas (como calzadas, galpones o paredes) puede deteriorarlas gravemente con el tiempo.

El sistema SRAGV (Sistema de Riego Automático Gestionado por Viento) resuelve este problema inhibiendo sectores específicos de riego en función de la velocidad y dirección del viento. Cabe destacar que, para el desarrollo de este prototipo, *el viento se emuló mediante un joystick analógico*, el cual simula tanto la velocidad (mediante la desviación desde el centro) como la dirección (mediante el eje dominante).

El sistema opera bajo un *Modo Normal* que puede funcionar con dos lógicas distintas de inhibición:

1. *Inhibición directa:* si hay viento en una dirección, se apaga el sector correspondiente a esa dirección. Por ejemplo, si hay viento Sur, se apaga el sector Sur. Esta opción está pensada para evitar que el viento empuje el agua hacia alguna construcción u objeto cercano al sector.
2. *Inhibición inversa (contrasector):* si hay viento en una dirección, se apaga el sector contrario. Por ejemplo, si hay viento Sur, se apaga el sector Norte, para compensar el arrastre del agua hacia ese sector.

### 1.2 Productos comparables

Existen en el mercado soluciones de riego automatizado muy populares. Un ejemplo representativo disponible en Mercado Libre es el [*Programador de Riego Rain Bird Automático 4 Zonas*](https://www.mercadolibre.com.ar/programador-de-riego-rain-bird-automatico-4-zonas-blanco/p/MLA37257397) [1].

Si bien este equipo es robusto y comercialmente exitoso, carece de una característica central del presente diseño: no cuenta con un sistema que inhiba sectores específicos en función de la dirección del viento. Algunos programadores comerciales pueden conectarse a estaciones meteorológicas para suspender completamente el riego en caso de tormentas, pero no ofrecen la granularidad de apagar selectivamente zonas individuales basándose en la dinámica vectorial del viento en tiempo real. En esta etapa del producto, se priorizó la corrección por viento sobre otras variables, como la humedad de suelo, que suele estar presente en soluciones más completas.

### 1.3 Productos comerciales disponibles

El referente comercial principal para comparar este desarrollo es la familia de controladores de la empresa [Rain Bird](https://www.rainbird.com/es) [2], específicamente el modelo *Rainbird ESP-Rzxe*. Estos sistemas controlan electroválvulas mediante salidas de 24 VAC y permiten programación por días y zonas.

### 1.4 Comparación con el prototipo desarrollado y alcance

A diferencia del *Rainbird ESP-Rzxe*, el prototipo desarrollado incorpora lógica ambiental directa en la toma de decisiones por sector, considerando velocidad y dirección del viento en tiempo real, y no solo un temporizador fijo. Se implementaron dos lógicas de inhibición configurables: la *inhibición directa* (se inhibe el sector en la dirección del viento) y la *inhibición inversa* o *contrasector* (se inhibe el sector opuesto, para compensar el arrastre del agua). Esta doble modalidad permite adaptar el comportamiento del sistema a diferentes configuraciones de terreno y a la presencia de construcciones adyacentes a los sectores de riego.

---

## Capítulo 2: Introducción específica

En esta sección se presentan los requisitos del sistema, los casos de uso identificados durante la etapa de diseño y la descripción de los módulos y tecnologías utilizadas en el prototipo.

### 2.1 Requisitos del sistema

A lo largo del ciclo de vida del proyecto, los requisitos funcionales experimentaron una evolución. Durante la etapa de planificación, se plantearon las funcionalidades base que debía cumplir el prototipo para garantizar un control de riego eficiente y adaptable al viento. La Tabla 2.1 detalla esta primera aproximación de los requisitos.

<p align="center"><em>Tabla 2.1: Requisitos funcionales preliminares (fase de ideación).</em></p>

| **Grupo** | **ID** | **Descripción** |
| :--- | :---: | :--- |
| **Sensores analógicos** | **RQ01** | El sistema contará con un joystick analógico para emular la velocidad y dirección del viento, conectado al ADC del microcontrolador. |
| | **RQ02** | El sistema contará con una fotocélula (LDR) para determinar la condición de luz diurna o nocturna. |
| **Actuadores** | **RQ03** | El sistema controlará 4 LEDs representando sectores de riego (Norte, Sur, Este, Oeste), inhibiendo los afectados por el viento. |
| **Modos de operación** | **RQ04** | El sistema operará en tres modos: NORMAL (monitoreo y riego), SET_UP (configuración) y FALLA (emergencia por viento crítico). |
| **Comunicación** | **RQ05** | La configuración y el monitoreo podrán realizarse de forma remota mediante un módulo Bluetooth. |
| **Almacenamiento** | **RQ06** | La configuración del usuario (umbrales de viento, sectores habilitados) se almacenará en una EEPROM externa con respaldo ante cortes de energía. |


Posteriormente, conforme avanzó el desarrollo del firmware y la selección de la arquitectura de hardware, los requerimientos iniciales se desglosaron y refinaron para cubrir todos los aspectos operativos del sistema. La Tabla 2.2 presenta la versión definitiva de los requisitos implementados.

<p align="center"><em>Tabla 2.2: Requisitos funcionales definitivos del sistema.</em></p>

| **Grupo** | **ID** | **Descripción** |
| :--- | :---: | :--- |
| **Sensores analógicos** | **1.1** | El sistema contará con un joystick analógico para emular velocidad y dirección del viento, conectado a dos canales ADC del STM32. |
| | **1.2** | El eje X del joystick representará la velocidad del viento; el eje Y representará la dirección dominante (Norte, Sur, Este u Oeste). |
| | **1.3** | El sistema contará con una fotocélula (LDR) conectada a un canal ADC para medir la luminosidad ambiente. |
| | **1.4** | Las lecturas de los canales ADC se realizarán mediante DMA con *callback* de conversión completa, sin *polling* bloqueante. |
| | **1.5** | Se aplicará un filtro de promediado simple sobre las lecturas de velocidad de viento para reducir el efecto del ruido eléctrico. |
| **Actuadores — LEDs de sector** | **2.1** | El sistema contará con 4 LEDs que representan los sectores de riego: Norte, Sur, Este y Oeste. |
| | **2.2** | Con viento bajo o nulo, se activarán todos los sectores habilitados por el usuario. |
| | **2.3** | Con viento moderado, se aplicará la lógica de inhibición configurada (directa o contrasector). |
| | **2.4** | Con viento crítico (Modo FALLA), todos los LEDs de sector se apagarán. |
| | **2.5** | Cada LED de sector permanecerá encendido durante el tiempo de riego configurado, de forma no bloqueante. |
| **Indicadores de estado** | **3.1** | El sistema contará con al menos 2 LEDs de estado: uno verde (sistema activo) y uno rojo (falla). |
| | **3.2** | Durante el Modo FALLA, el LED rojo parpadeará a 1 Hz de forma no bloqueante. |
| | **3.3** | El sistema contará con un buzzer que se activará como señal sonora de alerta durante el Modo FALLA. |
| | **3.4** | El sistema contará con un display LCD 16×2 con interfaz paralela en modo 4 bits para mostrar el estado actual, lecturas de sensores y menús de configuración. |
| **Modos de operación** | **4.1** | El sistema operará en tres modos: NORMAL, SET_UP y FALLA, implementados mediante máquinas de estado. |
| | **4.2** | En Modo NORMAL, el sistema monitoreará los sensores y activará los LEDs de sector según los umbrales configurados y la condición de luz. |
| | **4.3** | En Modo NORMAL, si la LDR indica noche, el sistema podrá inhibir el riego si el usuario configuró la opción de solo riego diurno. |
| | **4.4** | En Modo SET_UP, el sistema suspenderá el riego automático y presentará el menú de configuración en el LCD. |
| | **4.5** | En Modo FALLA, el sistema inhibirá todos los sectores, activará el LED rojo y el buzzer, y mostrará el motivo de la falla en el LCD. |
| | **4.6** | El sistema iniciará siempre en Modo NORMAL con todos los LEDs de sector apagados. |
| **Menú interactivo** | **5.1** | El sistema contará con botones para navegar el menú, con antirrebote implementado por software de forma no bloqueante. |
| | **5.2** | El sistema contará con un conjunto de DIP *switches* para la habilitación o deshabilitación manual de sectores. |
| | **5.3** | El menú permitirá configurar: umbral de viento moderado, umbral de viento crítico, duración del ciclo de riego y habilitación de sectores. |
| | **5.4** | El menú mostrará en el LCD las lecturas actuales de velocidad de viento (en porcentaje) y el nivel de luz ambiente. |
| **Comunicación Bluetooth** | **6.1** | El sistema se comunicará con el módulo HM-10 a través de UART, con un protocolo de comandos ASCII simple. |
| | **6.2** | La aplicación Bluetooth permitirá visualizar el estado de cada sector, el nivel de viento y el nivel de luz en tiempo real. |
| | **6.3** | La aplicación Bluetooth permitirá configurar los umbrales de viento y la duración del riego de forma remota. |
| | **6.4** | La aplicación Bluetooth permitirá activar o desactivar manualmente un sector de riego. |
| **Almacenamiento EEPROM** | **7.1** | El sistema almacenará la configuración del usuario en una EEPROM externa AT24C02 vía I²C. |
| | **7.2** | Al iniciar, el sistema leerá la configuración almacenada en EEPROM y validará su integridad mediante un *checksum*. |
| | **7.3** | Si la EEPROM no contiene configuración válida, el sistema cargará valores por defecto y los escribirá en EEPROM. |
| ***Soft* RTC** | **8.1** | El sistema llevará un contador de *ticks* de 1 ms basado en el SysTick del STM32, utilizado como referencia de tiempo para el riego y los temporizadores. |
| **Bajo consumo** | **9.1** | En ausencia de actividad por un tiempo configurable, el sistema entrará en modo de bajo consumo (*sleep*) del STM32. |
| | **9.2** | El sistema saldrá del modo *sleep* ante interrupción de botón, *tick* de SysTick o recepción de dato por UART. |
| **Robustez y seguridad** | **10.1** | El sistema detectará lecturas de ADC fuera de rango como condición de falla y transitará al Modo FALLA. |
| | **10.2** | La arquitectura de software seguirá el patrón Escrutar/Procesar/Actuar, organizada de forma modular. |
| | **10.3** | El *super-loop* completará cada vuelta en menos de 1 ms. |


En la sección 4.1 se detallan las pruebas realizadas para verificar el cumplimiento de estos requisitos.

### 2.2 Casos de uso

Se identificaron tres casos de uso principales, detallados en las Tablas 2.3 a 2.5. Cada uno se corresponde con uno de los modos de operación del sistema.

La Tabla 2.3 describe el caso de uso más frecuente: el ciclo de riego completo bajo condiciones de viento bajo.

<p align="center"><em>Tabla 2.3: Caso de uso 1 — ciclo de riego completo con viento bajo.</em></p>

| **Elemento** | **Definición** |
| :--- | :--- |
| Disparador | El temporizador interno indica que se cumplió el intervalo de riego configurado y el joystick indica velocidad de viento baja (por debajo del umbral moderado). |
| Precondiciones | El sistema está en Modo NORMAL y los canales ADC están operativos. La configuración fue cargada desde EEPROM al inicio. La LDR indica condición diurna (si la opción de riego solo diurno está habilitada). |
| Flujo principal | Se leen velocidad y dirección del joystick. El valor de velocidad se encuentra por debajo del umbral moderado. Los cuatro sectores habilitados se activan simultáneamente. El LCD muestra en tiempo real la velocidad (%) y dirección del viento, y el estado de cada sector. Una vez transcurrido el tiempo de riego configurado, el sistema entra en la fase de descanso y apaga todos los sectores hasta que se cumple el intervalo de reposo. |
| Flujo alternativo A | Durante el ciclo, el joystick supera el umbral crítico: el sistema transita al Modo FALLA, apaga todos los sectores y activa la alarma sonora y visual. |


La Tabla 2.4 describe el caso de uso de riego parcial, que constituye el escenario central del sistema: la inhibición de sectores por viento moderado.

<p align="center"><em>Tabla 2.4: Caso de uso 2 — riego parcial por viento moderado.</em></p>

| **Elemento** | **Definición** |
| :--- | :--- |
| Disparador | El temporizador interno indica que se cumplió el intervalo de riego, pero el joystick indica viento moderado (supera el umbral moderado y no alcanza el crítico) con dirección Norte. |
| Precondiciones | El sistema está en Modo NORMAL y los sensores ADC están operativos. |
| Flujo principal | Se leen velocidad y dirección del joystick. Se determina que el nivel de viento es moderado. Según la lógica de inhibición configurada en el SET_UP (*directa* o *contrasector*), se identifica el sector a inhibir. Se activan simultáneamente todos los sectores no inhibidos. El LCD muestra en tiempo real la velocidad (%) y dirección del viento, y el estado de cada sector. La lógica de inhibición se reevalúa en cada ciclo del *super-loop*, por lo que cualquier cambio de viento se refleja de forma inmediata. |
| Flujo alternativo A | La velocidad de viento supera el umbral crítico: el sistema transita al Modo FALLA, apaga todos los sectores y activa la alarma. |
| Flujo alternativo B | La velocidad de viento cae por debajo del umbral moderado: el sector inhibido se reactiva automáticamente. |
| Flujo alternativo C | El usuario activa la inhibición manual por DIP *switch*: el sector correspondiente se fuerza apagado independientemente de las condiciones del viento. |


Por último, la Tabla 2.5 detalla la configuración de parámetros en el Modo SET_UP, operación que permite al usuario adaptar el comportamiento del sistema a su instalación.

<p align="center"><em>Tabla 2.5: Caso de uso 3 — configuración de umbrales en Modo SET_UP.</em></p>

| **Elemento** | **Definición** |
| :--- | :--- |
| Disparador | El usuario desea modificar el umbral de viento moderado, el umbral de viento crítico, la duración del ciclo de riego, la opción de riego nocturno o la lógica de inhibición. |
| Precondiciones | El sistema está en Modo NORMAL. El display LCD está operativo. |
| Flujo principal | El usuario presiona el botón ENTER. El sistema transita al Modo SET_UP, suspende el riego automático y muestra en el LCD el menú de configuración. El menú presenta cinco parámetros en secuencia: umbral de viento moderado, umbral de viento crítico, duración del ciclo de riego, riego nocturno habilitado y lógica de inhibición (modo 1 o modo 2). El usuario ajusta cada valor con los botones UP y DOWN, y avanza al siguiente parámetro con ENTER. Al confirmar el último parámetro, el sistema escribe toda la configuración en EEPROM y muestra "CONFIGURACION GUARDADA!" en el LCD. El sistema vuelve automáticamente al Modo NORMAL. |
| Flujo alternativo A | El umbral moderado es mayor o igual al crítico al intentar avanzar: el sistema muestra un mensaje de error en el LCD y permanece en el mismo parámetro hasta que el valor sea válido. |
| Flujo alternativo B | Falla de escritura en EEPROM: el sistema retorna al Modo NORMAL con los parámetros aplicados en RAM pero no persistidos. |
| Flujo alternativo C | El usuario ajusta los parámetros desde la aplicación web: los valores se envían por Bluetooth mediante comandos ASCII (`SET:MOD`, `SET:CRI`, `SET:TIM`, `SET:NIG`, `SET:OPM`) sin necesidad de ingresar al Modo SET_UP desde el teclado físico. |


### 2.3 Descripción de módulos y tecnologías utilizadas

A continuación se describen los principales componentes de hardware, módulos periféricos y actuadores seleccionados para la implementación física del prototipo. Cada elemento fue elegido para satisfacer los requisitos funcionales detallados en la sección 2.1, operando de manera coordinada bajo el control de la placa principal STM32 NUCLEO-F103RB.

#### 2.3.1 STM32 NUCLEO-F103RB

La placa de desarrollo NUCLEO-F103RB, ilustrada en la Figura 2.1, es la unidad de procesamiento central del sistema.

<div align="center">
<p><em><img width="596" height="335" alt="STM32" src="https://github.com/user-attachments/assets/2fe6122d-9474-4260-bb42-d9273f665efd" />
</em></p>
<p><em>Figura 2.1: Placa de desarrollo STM32 NUCLEO-F103RB.</em></p>
</div>

#### 2.3.2 Joystick analógico

El joystick analógico, mostrado en la Figura 2.2, fue utilizado como sensor de emulación del viento. El eje X representa la velocidad del viento mediante la desviación de la palanca desde el centro, y el eje Y determina la dirección dominante (Norte, Sur, Este u Oeste) según cuál de los dos ejes presente mayor deflexión. Las lecturas de ambos ejes se digitalizan mediante ADC con transferencia por DMA y se filtran con un promediado simple para reducir el ruido eléctrico.

<div align="center">
<p><em><img width="554" height="554" alt="joystick_arduino" src="https://github.com/user-attachments/assets/78631d90-e9d3-4921-a80b-71b2410b744e" />
</em></p>
<p><em>Figura 2.2: Joystick analógico utilizado para la emulación del viento.</em></p>
</div>

#### 2.3.3 Display LCD 16×2

El display LCD 16×2, ilustrado en la Figura 2.3, constituye la interfaz visual principal del sistema. Se conecta al microcontrolador mediante la interfaz paralela en modo 4 bits, utilizando los pines de datos, habilitación y selección de registro del microcontrolador. El display muestra el modo actual del sistema, las lecturas de velocidad y dirección del viento, el estado de los sectores de riego y los menús de configuración en el Modo SET_UP.

<div align="center">
<p><em><img width="392" height="510" alt="display_lcd" src="https://github.com/user-attachments/assets/35dfb92f-8abb-4073-ae99-49bdd879ad65" />
</em></p>
<p><em>Figura 2.3: Display LCD 16×2 con interfaz paralela en modo 4 bits.</em></p>
</div>

#### 2.3.4 Módulo Bluetooth HM-10

El módulo HM-10, mostrado en la Figura 2.4, proporciona conectividad inalámbrica BLE (*Bluetooth Low Energy*) entre el prototipo y la aplicación web de monitoreo. La comunicación con el microcontrolador se realiza mediante el periférico UART, utilizando un protocolo de comandos ASCII simple. A través de este módulo, la aplicación puede recibir el estado del sistema en tiempo real y enviar nuevos parámetros de configuración.

<div align="center">
<p><em><img width="554" height="554" alt="modulo_bluetooth" src="https://github.com/user-attachments/assets/f42934c5-417d-4320-9ae3-df4ee477a9fc" />
</em></p>
<p><em>Figura 2.4: Módulo Bluetooth HM-10.</em></p>
</div>

#### 2.3.5 EEPROM AT24C02

La EEPROM AT24C02, ilustrada en la Figura 2.5, es una memoria no volátil que se comunica por el bus I²C compartido con el LCD. Su función es persistir la configuración del usuario (umbrales de viento moderado y crítico, sectores habilitados, duración del ciclo de riego y preferencia de riego diurno/nocturno) ante cortes de alimentación. Al iniciar el sistema, se lee la configuración almacenada y se verifica su integridad mediante un *checksum*; si los datos son inválidos, se cargan valores por defecto.

<div align="center">
<p><em><img width="447" height="447" alt="EEPROM" src="https://github.com/user-attachments/assets/7f65d325-50f8-417d-82ff-f6dc70902f2a" />
</em></p>
<p><em>Figura 2.5: Módulo de memoria EEPROM AT24C02.</em></p>
</div>

#### 2.3.6 Modulo LDR

  El modulo LDR, mostrada en la Figura 2.6. Se utilizo para que, de acuerdo con la configuración del usuario, puede habilitar o inhibir el riego nocturno. 

<div align="center">
<p><em><img width="392" height="510" alt="LDR" src="https://github.com/user-attachments/assets/24c8a7ae-b349-4a29-899d-93a8593d17b8" />
</em></p>
<p><em>Figura 2.6: Fotorresistencia LDR para detección de luz diurna/nocturna.</em></p>
</div>

#### 2.3.7 LEDs de sector

Los cuatro LEDs de sector, ilustrados en la Figura 2.7, representan visualmente el estado de las zonas de riego Norte, Sur, Este y Oeste. Cuando un LED está encendido, indica que el sector correspondiente se encuentra activo; cuando está apagado, el sector está inhibido ya sea por viento, por DIP *switch* manual o por el Modo FALLA.

<div align="center">
<p><em><img width="554" height="554" alt="LEDs" src="https://github.com/user-attachments/assets/291cfa54-1b61-4079-9dff-37f854c85b46" />
</em></p>
<p><em>Figura 2.7: LEDs de sector (Norte, Sur, Este y Oeste).</em></p>
</div>

#### 2.3.8 LEDs de estado

El sistema cuenta con dos LEDs de estado, mostrados en la Figura 2.8: el LED RUN (verde), que indica que el sistema está activo y operando con normalidad, y el LED ALERTA (rojo), que se activa y parpadea a 1 Hz durante el Modo FALLA para señalizar una condición de emergencia.

<div align="center">
<p><em><img width="240" height="210" alt="led_Estado" src="https://github.com/user-attachments/assets/181ac223-1cd6-4572-ad28-4eebb0c8a421" />
</em></p>
<p><em>Figura 2.8: LEDs de estado RUN y ALERTA.</em></p>
</div>

#### 2.3.9 Buzzer

El buzzer, ilustrado en la Figura 2.9, proporciona retroalimentación sonora al usuario cuando se encuentra en modo FALLA.
<div align="center">
<p><em><img width="225" height="225" alt="buzzer" src="https://github.com/user-attachments/assets/0105794c-981d-4769-babe-3baea2fc975e" />
</em></p>
<p><em>Figura 2.9: Buzzer para señales sonoras de confirmación y falla.</em></p>
</div>

#### 2.3.10 DIP Switches

El módulo de DIP *switches* de 4 posiciones, mostrado en la Figura 2.10, permite inhabilitar manualmente cualquier sector de riego de forma independiente a la lógica automática de viento. Cada posición corresponde a un sector (Norte, Sur, Este, Oeste); en posición abierta, el sector queda forzado a apagado sin importar las condiciones del viento.

<div align="center">
<p><em><img width="464" height="580" alt="DIP" src="https://github.com/user-attachments/assets/1ca6bb03-4e23-4550-a13c-727b189bf032" />
</em></p>
<p><em>Figura 2.10: Módulo de DIP switches para inhibición manual de sectores.</em></p>
</div>

#### 2.3.11 Pulsadores

Los tres pulsadores, ilustrados en la Figura 2.11, permiten la navegación del menú interactivo en el Modo SET_UP. El botón UP avanza entre las opciones del menú o incrementa un valor, el botón DOWN retrocede o decrementa, y el botón ENTER confirma la selección. Todos los pulsadores cuentan con antirrebote implementado por software de forma no bloqueante.

<div align="center">
<p><em><img width="447" height="447" alt="pulsadores" src="https://github.com/user-attachments/assets/5f673cbc-01c4-4d94-97d7-094db832f26c" />
</em></p>
<p><em>Figura 2.11: Pulsadores de navegación del menú (UP, DOWN, ENTER).</em></p>
</div>

---

## Capítulo 3: Diseño e implementación

### 3.1 Arquitectura de Hardware

El sistema fue implementado sobre una placa *STM32 Nucleo-F103RB*. En la Figura 3.1 se muestra el prototipo en su etapa inicial de interconexión sobre protoboard, utilizada durante las pruebas de integración de los módulos. Las Figuras 3.2 y 3.3 muestran el ensamble final del circuito soldado en placa perforada, que constituye la entrega definitiva del hardware.

<div align="center">
  <img width="576" height="1024" alt="media_1786109761260" src="https://github.com/user-attachments/assets/84ffa600-a36d-4822-9a13-32dc48574dda" />
  <p align="center"><em>Figura 3.1: Prototipo inicial interconectado en protoboard.</em></p>
</div>

<div align="center">
  <img width="576" height="1024" alt="media_1786109761311" src="https://github.com/user-attachments/assets/f83ebb8e-52cb-4c74-aefa-f48a59e37306" />
  <p align="center"><em>Figura 3.2: Vista superior del circuito soldado en placa perforada.</em></p>
</div>

<div align="center">
  <img width="576" height="1024" alt="media_1786109761321" src="https://github.com/user-attachments/assets/0878276c-47b7-4832-ad3e-b730c3dd526a" />
  <p align="center"><em>Figura 3.3: Vista inferior del circuito soldado (ruteo manual con estaño).</em></p>
</div>

### 3.2 Diseño de Firmware (Máquinas de Estado)

El firmware se estructuró mediante una arquitectura *super-loop* (*bare-metal*) orientada a eventos. El flujo principal recae sobre `task_system.c`, que implementa la máquina de estados principal ilustrada en la Figura 3.4:

```mermaid
stateDiagram-v2
    direction LR
    [*] --> MODO_NORMAL : Power ON / app_init()
    state MODO_NORMAL {
        direction TB
        [*] --> MONITOREO_ACTIVO
        
        MONITOREO_ACTIVO : MONITOREO_ACTIVO
        MONITOREO_ACTIVO : entry / put_actuator_status_led(RUN, true)
        MONITOREO_ACTIVO : entry / put_actuator_status_led(ALERTA, false)
        MONITOREO_ACTIVO : do / Actualizar sectores (inhibiciones)
    }
    state MODO_SET_UP {
        direction TB
        [*] --> MENU_CONFIG
        
        MENU_CONFIG : MENU_CONFIG
        MENU_CONFIG : entry / put_actuator_status_led(RUN, false)
        MENU_CONFIG : do / Interfaz de configuración en LCD
    }
    state MODO_FALLA {
        direction TB
        [*] --> EMERGENCIA
        
        EMERGENCIA : EMERGENCIA
        EMERGENCIA : entry / put_actuator_status_led(ALERTA, true)
        EMERGENCIA : entry / put_actuator_buzzer(true)
        EMERGENCIA : do / put_actuator_sector(TODOS, false)
        EMERGENCIA : exit / put_actuator_status_led(ALERTA, false)
        EMERGENCIA : exit / put_actuator_buzzer(false)
    }
    MODO_NORMAL --> MODO_SET_UP  : init_event_task_system [btn_enter] / Guardar estado previo
    MODO_SET_UP --> MODO_NORMAL  : init_event_task_system [btn_enter] / Aplicar nueva config
    MODO_NORMAL --> MODO_FALLA   : [flag_error_adc || flag_error_i2c || flag_viento_critico]
    MODO_SET_UP --> MODO_FALLA   : [flag_error_adc || flag_error_i2c || flag_viento_critico]
    MODO_FALLA  --> MODO_NORMAL  : [viento < umbral && sensores_ok]
```

<p align="center"><em>Figura 3.4: Diagrama de estados (statechart) del SRAGV — FSM principal.</em></p>

Los modos de operación implementados son los siguientes:

1. MODO_NORMAL: evaluación constante de la lógica de riego y monitoreo del ADC (joystick). Según la velocidad de viento medida y la lógica de inhibición configurada (directa o contrasector), el sistema determina qué sectores activar o inhibir.
2. MODO_SET_UP: menú interactivo a través del display LCD, navegable con pulsadores con antirrebote por software.
3. MODO_FALLA: disparado por viento crítico o falla grave del ADC. Apaga todos los sectores y activa las alarmas visuales y sonoras.

#### 3.2.1 Arquitectura de ejecución

El *super-loop* está implementado en `app.c`. El mecanismo de temporización se basa en la interrupción `HAL_SYSTICK_Callback`, que incrementa el contador atómico `g_app_tick_cnt` cada 1 ms. En `app_update()`, el *loop* principal consume un *tick* y ejecuta las cinco tareas en orden fijo:

1. `logger_update` — Bluetooth: procesa comandos entrantes y envía telemetría JSON cada 2 s.
2. `task_sensor_update` — Escrutinio: muestrea ADC (DMA), actualiza filtro de promediado, hace *debounce* de botones y lee DIP *switches*.
3. `task_system_update` — Procesamiento: ejecuta la FSM principal y la lógica de inhibición de riego.
4. `task_display_update` — Presentación: renderiza el LCD según el modo activo.
5. `task_actuator_update` — Actuación: escribe los GPIO de sectores, LEDs y buzzer.

Una vez completadas las cinco tareas, el microcontrolador entra en modo *Sleep* (`HAL_PWR_EnterSLEEPMode`, `WFI`) hasta el siguiente *tick* de SysTick, reduciendo el consumo en reposo.

#### 3.2.2 FSM de debounce de botones

Cada uno de los tres botones físicos (ENTER, UP, DOWN) es procesado por una máquina de estados de cuatro estados, implementada en `task_sensor.c` con una ventana de 50 ms, tal como se ilustra en la Figura 3.5.

```mermaid
stateDiagram-v2
    direction LR
    [*] --> BTN_ST_UP
    BTN_ST_UP --> BTN_ST_FALLING : pin bajo (pulsado)
    BTN_ST_FALLING --> BTN_ST_UP : rebote detectado (pin alto antes de 50 ms)
    BTN_ST_FALLING --> BTN_ST_DOWN : estable ≥ 50 ms / flag = true
    BTN_ST_DOWN --> BTN_ST_RISING : pin alto (liberado)
    BTN_ST_RISING --> BTN_ST_DOWN : rebote detectado (pin bajo antes de 50 ms)
    BTN_ST_RISING --> BTN_ST_UP : estable ≥ 50 ms
```

<p align="center"><em>Figura 3.5: FSM de debounce de botón (50 ms, activo bajo).</em></p>

El evento `flag = true` generado en la transición a `BTN_ST_DOWN` es consumido por `task_system_update` y `task_display_update` mediante las funciones de interfaz `get_sensor_btn_*_pressed()`, que lo leen y lo limpian (*clear-on-read*).

#### 3.2.3 Acondicionamiento de entradas analógicas

Las tres entradas analógicas (VRX, VRY del joystick y LDR) se digitalizan simultáneamente mediante el ADC1 en modo de conversión continua con transferencia por DMA circular, sin interrupción de DMA activa (desactivada explícitamente para evitar *lockup* de CPU). El buffer se muestrea en cada *tick* directamente desde `task_sensor_update`.

Sobre los canales VRX y VRY se aplica un filtro de promediado móvil (*moving average*) de ventana configurable por `FILTER_WINDOW_SIZE`. La velocidad del viento se calcula como la desviación máxima absoluta desde el centro (2048 cuentas), con una *deadzone* de ±800 cuentas para suprimir el ruido en reposo. El resultado se escala a un rango de 0–100 %.

#### 3.2.4 Persistencia en EEPROM

La configuración de usuario se almacena en una EEPROM AT24C02 (2 Kbit) vía bus I²C (`hi2c1`). La estructura `sys_settings_t` contiene cinco campos de 32 bits (umbral moderado, umbral crítico, duración de riego, habilitación nocturna y modo de operación) más un *checksum* aritmético de 4 bytes, totalizando 24 bytes.

Al iniciar, `task_display_init()` lee la EEPROM y valida el *checksum*; si la validación falla (memoria vacía o corrompida), carga los valores por defecto y los escribe inmediatamente. La escritura se realiza byte a byte con un retardo de 5 ms por byte para respetar el ciclo de escritura del AT24C02.

#### 3.2.5 Telemetría Bluetooth

El módulo `logger.c` gestiona la comunicación a través de USART1 en modo no bloqueante por interrupciones. La telemetría se envía en formato JSON cada 2 s (`TELEMETRY_PERIOD_MS = 2000`) con la siguiente estructura:

```json
{"m":"NORMAL","v":35,"d":"N","s":[1,0,1,1],
 "c_mod":40,"c_cri":80,"c_tim":60,
 "c_nig":0,"c_inh":0,"c_opm":1,"c_lux":72}
```

Donde `m` es el modo del sistema, `v` la velocidad de viento (%), `d` la dirección, `s` el estado de los cuatro sectores, y los campos `c_*` los parámetros de configuración actuales. Los comandos de escritura recibidos (`SET:MOD`, `SET:CRI`, `SET:TIM`, `SET:NIG`, `SET:INH`, `SET:OPM`) son parseados y persistidos en EEPROM en el mismo ciclo.

### 3.3 Esquema Eléctrico y Vistas de Cableado

Para documentar las conexiones físicas entre los componentes, se elaboró un diagrama de conexionado utilizando la herramienta *Fritzing*. Si bien este formato de vista de protoboard (*layout*) difiere de un diagrama esquemático simbólico tradicional, ilustra de manera clara y directa cómo se vinculan los módulos periféricos (pantalla LCD, módulo Bluetooth HM-10, joystick analógico, matriz de LEDs de sectores y el panel de DIP *switches*). El diagrama de cableado se presenta en la Figura 3.5.

<div align="center">
<img width="872" height="1024" alt="media_1786125104622" src="https://github.com/user-attachments/assets/46d8bbbd-7d39-4fae-8436-66cbcb84a9ce" />
  <p align="center"><em>Figura 3.5: Diagrama de cableado del sistema.</em></p>
</div>

> [!NOTE]
> Aclaración sobre la representación visual: los componentes mostrados en la Figura 3.5 (específicamente los valores de las resistencias, colores de ciertos encapsulados y modelos exactos de módulos) constituyen una representación visual para ilustrar el esquema de interconexión. Algunos valores y componentes puntuales pueden diferir ligeramente de los utilizados en el ensamble de la placa física documentada en las Figuras 3.1, 3.2 y 3.3.

### 3.4 Aplicación Web

Como complemento a la interfaz física (LCD y botones), se desarrolló una *web app* interactiva. El usuario puede conectarse a la placa a través de Bluetooth directamente desde su navegador web para disponer de un panel de control y monitoreo en tiempo real. La aplicación se encuentra disponible en [SRAGV - Monitoreo Bluetooth](https://leangc13.github.io/SRAGV-APP/).

Desde esta interfaz, el usuario puede:
- Visualizar el estado actual del sistema (Normal, Falla, etc.).
- Ver un tablero gráfico con la velocidad (%) y dirección del viento, y el nivel de luz ambiente.
- Monitorear gráficamente cuáles sectores de riego están encendidos o inhibidos.
- Ajustar remotamente todos los parámetros de control: umbrales de viento moderado y crítico, duración de un ciclo de riego, activación del riego nocturno, selección de la lógica de inhibición (directa o contrasector) e inhibición manual de zonas específicas.

En la Figura 3.6 se muestra el panel principal de la aplicación, y en la Figura 3.7 se observa la vista de lecturas de viento en tiempo real.

<div align="center">
 <img width="632" height="628" alt="media_1786112099119" src="https://github.com/user-attachments/assets/00282ae9-bc9d-4647-9293-b9b3b02744e5" />
  <p align="center"><em>Figura 3.6: Panel principal de la aplicación web, mostrando monitoreo y ajustes.</em></p>
</div>

<div align="center">
  <img width="713" height="619" alt="media_1786112099179" src="https://github.com/user-attachments/assets/c79c509b-8506-4f5d-8c99-61276ec3e775" />
  <p align="center"><em>Figura 3.7: Vista de lecturas de viento en tiempo real y estado del sistema.</em></p>
</div>

---

## Capítulo 4: Ensayos y resultados

Esta sección presenta los ensayos realizados sobre el prototipo SRAGV, incluyendo pruebas funcionales de hardware y firmware, pruebas de integración, medición de consumo, análisis de tiempos de ejecución (WCET) y cumplimiento de requisitos.

### 4.1 Pruebas funcionales de hardware

La Tabla 4.1 resume los ensayos funcionales de hardware realizados y su estado de validación.

<p align="center"><em>Tabla 4.1: Ensayos funcionales de hardware.</em></p>

| Ensayo | Resultado | Estado |
| :--- | :--- | :---: |
| Integridad eléctrica del circuito (continuidad) | Validación previa a energización | ✅ |
| ADC — joystick eje X (velocidad) | Rango 0–4095 cuentas, centrado en ~2048 | ✅ |
| ADC — joystick eje Y (dirección) | Rango 0–4095 cuentas, centrado en ~2048 | ✅ |
| ADC — LDR (luminosidad) | Respuesta diferencial entre luz y oscuridad verificada | ✅ |
| DIP *switches* — 4 canales | Pines flotantes corregidos con `GPIO_PULLUP`; inhibición individual estable | ✅ |
| LEDs de sector (N, S, E, O) | Lógica activa alta verificada; encendido y apagado por GPIO | ✅ |
| LED de estado RUN (verde) | Enciende en MODO_NORMAL, se apaga en SET_UP y FALLA | ✅ |
| LED ALERTA (rojo) + buzzer | Parpadeo a 1 Hz y tono sonoro verificados en MODO_FALLA | ✅ |
| Display LCD 16×2 (interfaz paralela 4 bits) | Inicialización correcta; visualización de cadenas y valores numéricos | ✅ |
| Módulo Bluetooth HM-10 (USART1) | Pairing y recepción de telemetría JSON verificados desde aplicación web | ✅ |
| EEPROM AT24C02 (I²C) | Lectura y escritura verificadas; validación de *checksum* funcional | ✅ |


### 4.2 Pruebas funcionales de firmware

La Tabla 4.2 resume los ensayos funcionales de firmware realizados y su estado de validación.

<p align="center"><em>Tabla 4.2: Ensayos funcionales de firmware.</em></p>

| Ensayo | Resultado | Estado |
| :--- | :--- | :---: |
| *Debounce* de botones (ENTER, UP, DOWN) | Eventos limpios sobre FSM; ventana de 50 ms efectiva | ✅ |
| Muestreo ADC + filtro de promediado móvil | Lecturas estables; ruido reducido en joystick en reposo | ✅ |
| Escala velocidad de viento (0–100 %) | Mapeo lineal verificado; *deadzone* de ±800 cuentas operativa | ✅ |
| Detección de dirección de viento (N/S/E/O) | Cuatro cuadrantes correctamente identificados por eje dominante | ✅ |
| FSM del sistema (NORMAL / SET_UP / FALLA) | Transiciones válidas verificadas manualmente | ✅ |
| Lógica de inhibición directa (Modo 2) | Sector en dirección del viento se apaga correctamente | ✅ |
| Lógica de inhibición contrasector (Modo 1) | Sector opuesto al viento se apaga correctamente | ✅ |
| Inhibición nocturna por LDR | Riego suspendido bajo umbral de luz cuando opción habilitada | ✅ |
| Persistencia EEPROM — escritura y lectura | Configuración recuperada correctamente tras ciclo de energía | ✅ |
| Persistencia EEPROM — valores por defecto | Valores por defecto cargados y escritos ante EEPROM vacía o corrompida | ✅ |
| Telemetría Bluetooth — JSON periódica | Trama JSON enviada cada 2 s (`TELEMETRY_PERIOD_MS`) vía USART1 | ✅ |
| Comandos Bluetooth — `SET:*` | Umbrales y configuración actualizados en RAM y EEPROM desde aplicación web | ✅ |
| Menú SET_UP — 5 parámetros | Navegación UP/DOWN/ENTER funcional; validación MOD < CRI operativa | ✅ |
| Bajo consumo — *Sleep* entre *ticks* | `HAL_PWR_EnterSLEEPMode(WFI)` ejecutado correctamente | ✅ |


### 4.3 Pruebas de integración

Se validó la interacción completa entre entradas físicas (joystick, DIP *switches*, botones), lógica de inhibición, visualización en LCD y telemetría hacia la aplicación web.

*Video de integración en funcionamiento:*

[Video de demostración del SRAGV](https://www.youtube.com/watch?v=nqkUKbYzUQc)

### 4.4 Medición y análisis de consumo

Para evaluar el consumo energético, se midió la corriente total suministrada al sistema (NUCLEO y periféricos). La Tabla 4.3 resume los valores pico de corriente y potencia medidos en los distintos modos de operación.

<p align="center"><em>Tabla 4.3: Consumo total medido a 5 V (valores pico).</em></p>

| Modo | Corriente pico (5V) [mA] | Potencia pico (5V) [W] | Observaciones |
| :--- | ---: | ---: | :--- |
| NORMAL sin módulo BT | 29,8 | 0,149 | Corriente base del MCU (con WFI sleep), sensores y LCD. |
| NORMAL con BT conectado | 37,8 | 0,189 | Incremento de ~8 mA asociado a la transmisión activa de telemetría BLE (HM-10). |
| FALLA (buzzer + LED activos) | 50,0 | 0,250 | Consumo máximo absoluto del sistema con los actuadores de alarma (buzzer) en funcionamiento. |


Como se observa en los resultados, la estrategia de bajo consumo implementada en el firmware (Sleep WFI + baja carga computacional) logra mantener el consumo base por debajo de los 30 mA. El pico máximo absoluto de potencia del sistema completo bajo falla crítica es de tan solo 250 mW, permitiendo su alimentación segura a través de puertos USB convencionales o pequeñas baterías sin requerir disipación térmica adicional.

### 4.5 Análisis de uso de memoria

A partir del reporte de compilación del proyecto `Trabajo-FINAL_tdse.elf`, se obtuvieron los siguientes tamaños para las secciones principales:
- `.text` (código e instrucciones): 31 508 bytes
- `.data` (variables inicializadas): 136 bytes
- `.bss` (variables sin inicializar): 3 096 bytes

Teniendo en cuenta las características del microcontrolador utilizado (STM32F103RB, con 128 KB de memoria Flash y 20 KB de memoria RAM), el perfil de consumo de memoria del sistema es:

- *Memoria Flash (ROM):* Uso de `.text` + `.data` = 31 644 bytes (sobre 131 072 bytes totales). *Ocupación: 24,14 %*
- *Memoria RAM estática:* Uso de `.data` + `.bss` = 3 232 bytes (sobre 20 480 bytes totales). *Ocupación: 15,78 %*

Ambas memorias cuentan con amplio margen disponible (más del 75 % libre). Esto garantiza factibilidad técnica para agregar nuevas funcionalidades al firmware en versiones futuras (por ejemplo, librerías de encriptación BLE, nuevas máquinas de estado o almacenamiento masivo) sin riesgo inminente de agotar los recursos.

<div align="center">
  <img width="649" height="97" alt="reporte compilacion" src="https://github.com/user-attachments/assets/976e593a-965f-45fd-917f-92a4fcae6f81" />
  <p align="center"><em>Figura 4.1: Reporte de compilación y uso de Flash / RAM.</em></p>
</div>

### 4.6 Medición y análisis de WCET por tarea

La Tabla 4.4 resume los resultados de tiempo de ejecución (WCET) obtenidos experimentalmente por tarea con el sistema en estado estable (modo NORMAL, joystick en reposo, sin pulsaciones de botones).

<p align="center"><em>Tabla 4.4: Resultados de WCET por tarea (ventana de ~10 s en estado estable).</em></p>

| Tarea | Período [µs] | Promedio [µs] | WCET [µs] | WCET absoluto [µs] |
| :--- | ---: | ---: | ---: | ---: |
| `task_sensor_update` | 1000 | 12 | 15 | 22 |
| `task_system_update` | 1000 | 9 | 12 | 19 |
| `task_actuator_update` | 1000 | 5 | 8 | 12 |


*Observaciones:*
- Las tareas principales de adquisición y control (`sensor`, `system`, `actuator`) presentan tiempos de ejecución sumamente acotados (menores a 25 µs).
- Tareas asincrónicas como el refresco de pantalla y la transmisión por UART fueron omitidas de esta tabla central por tratarse de procesos esporádicos o gestionados por interrupción, cuyo análisis de WCET no impacta la criticidad del *super-loop* de control principal.

### 4.7 Cálculo del factor de uso de CPU

Para evaluar la carga temporal del sistema se calculó el factor de utilización de CPU:

Donde C_i es el WCET de la tarea medido en la sección 4.6 y T_i = 1000 µs su período de activación.

La Tabla 4.5 resume los parámetros y resultados del cálculo de utilización del CPU para las tareas críticas del sistema.

<p align="center"><em>Tabla 4.5: Parámetros utilizados para el cálculo de U (tareas de control).</em></p>

| Tarea | Promedio [µs] | WCET [µs] | Período [µs] | Uso Promedio | Uso Máximo (WCET) |
| :--- | ---: | ---: | ---: | ---: | ---: |
| `task_sensor_update` | 12 | 15 | 1000 | 0,012 | 0,015 |
| `task_system_update` | 9 | 12 | 1000 | 0,009 | 0,012 |
| `task_actuator_update` | 5 | 8 | 1000 | 0,005 | 0,008 |
| **Total (U)** | — | — | — | **0,026 (2,6 %)** | **0,035 (3,5 %)** |


El valor total máximo de uso de CPU es de apenas 3,5 %, lo que confirma que el procesador se encuentra ocioso la mayor parte del tiempo, permitiendo la entrada en modo *Sleep* sin riesgo de violar los plazos temporales (factibilidad de planificación garantizada).

### 4.8 Gestión de bajo consumo

El firmware implementa una estrategia de bajo consumo activa: al finalizar cada vuelta del *super-loop*, el microcontrolador entra en modo *Sleep* mediante `HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI)`. El sistema permanece en *Sleep* hasta que la próxima interrupción de SysTick (cada 1 ms) lo despierta, o bien hasta que llega una interrupción de UART (recepción Bluetooth). Esto reduce el consumo dinámico del CPU durante la mayor parte del tiempo de operación sin impactar en la respuesta del sistema.

En una versión orientada a producto, correspondería profundizar esta estrategia:
- Reducir la frecuencia de reloj del MCU al mínimo compatible con los periféricos activos.
- Evaluar el uso de modo *Stop* entre eventos, con despertar por EXTI.
- Migrar de Bluetooth clásico (HM-10/BLE) a un módulo de menor consumo en *advertising*.

### 4.9 Cumplimiento de requisitos

La Tabla 4.6 resume el cumplimiento final de los requisitos de la Tabla 2.2.

<p align="center"><em>Tabla 4.6: Cumplimiento final de requisitos.</em></p>

| ID | Requisito (resumen) | Hardware | Firmware | Estado |
| :---: | :--- | :---: | :---: | :---: |
| 1.1 | Joystick analógico — emulación de viento | 🟢 | 🟢 | ✅ |
| 1.2 | Eje X = velocidad, Eje Y = dirección | 🟢 | 🟢 | ✅ |
| 1.3 | LDR — luminosidad ambiente | 🟢 | 🟢 | ✅ |
| 1.4 | ADC por DMA, sin *polling* bloqueante | 🟢 | 🟢 | ✅ |
| 1.5 | Filtro de promediado sobre velocidad | N/A | 🟢 | ✅ |
| 2.1 | 4 LEDs de sector (N, S, E, O) | 🟢 | 🟢 | ✅ |
| 2.2 | Viento bajo → todos los sectores activos | 🟢 | 🟢 | ✅ |
| 2.3 | Viento moderado → lógica de inhibición | 🟢 | 🟢 | ✅ |
| 2.4 | Viento crítico → todos los sectores apagados | 🟢 | 🟢 | ✅ |
| 2.5 | Temporización no bloqueante del riego | N/A | 🟢 | ✅ |
| 3.1 | LEDs de estado RUN y ALERTA | 🟢 | 🟢 | ✅ |
| 3.2 | LED ALERTA parpadea a 1 Hz en FALLA | 🟢 | 🟢 | ✅ |
| 3.3 | Buzzer de alerta en MODO_FALLA | 🟢 | 🟢 | ✅ |
| 3.4 | LCD 16×2 con interfaz paralela 4 bits | 🟢 | 🟢 | ✅ |
| 4.1 | FSM: NORMAL / SET_UP / FALLA | N/A | 🟢 | ✅ |
| 4.2 | NORMAL: lógica de riego y monitoreo ADC | N/A | 🟢 | ✅ |
| 4.3 | NORMAL: inhibición nocturna por LDR | N/A | 🟢 | ✅ |
| 4.4 | SET_UP: menú de configuración en LCD | N/A | 🟢 | ✅ |
| 4.5 | FALLA: inhibición total + LCD + alarmas | 🟢 | 🟢 | ✅ |
| 4.6 | Arranque en MODO_NORMAL | N/A | 🟢 | ✅ |
| 5.1 | Botones con *debounce* no bloqueante | 🟢 | 🟢 | ✅ |
| 5.2 | DIP *switches* — inhibición manual | 🟢 | 🟢 | ✅ |
| 5.3 | Menú: umbral MOD, CRI, duración y sectores | N/A | 🟢 | ✅ |
| 5.4 | LCD muestra velocidad (%) y luz en SET_UP | N/A | 🟢 | ✅ |
| 6.1 | UART con HM-10, protocolo ASCII | 🟢 | 🟢 | ✅ |
| 6.2 | Visualización de estado en tiempo real | N/A | 🟢 | ✅ |
| 6.3 | Configuración remota de umbrales por BT | N/A | 🟢 | ✅ |
| 6.4 | Inhibición manual de sector por BT | N/A | 🟢 | ✅ |
| 7.1 | EEPROM AT24C02 vía I²C | 🟢 | 🟢 | ✅ |
| 7.2 | Lectura + validación *checksum* al inicio | N/A | 🟢 | ✅ |
| 7.3 | Carga de valores por defecto si EEPROM vacía | N/A | 🟢 | ✅ |
| 8.1 | *Soft* RTC por contador SysTick de 1 ms | N/A | 🟢 | ✅ |
| 9.1 | *Sleep* entre *ticks* (`WFI`) | N/A | 🟢 | ✅ |
| 9.2 | Despertar por SysTick / UART | N/A | 🟢 | ✅ |
| 10.1 | Detección de falla ADC | N/A | 🟡 | 🟡 |
| 10.2 | Patrón Escrutar/Procesar/Actuar modular | N/A | 🟢 | ✅ |
| 10.3 | *Super-loop* completa en < 1 ms | N/A | 🟢 | ✅ |


Leyenda: 🟢 implementado · 🟡 parcialmente cumplido · 🔴 no implementado · ✅ cumplido

Observación sobre el requisito 10.1: la condición de falla por ADC está contemplada en la FSM (`get_sensor_adc_error()`), pero el chequeo de saturación en `process_adc_data()` está comentado en el código final por considerar que los valores 0 y 4095 pueden ser válidos en el rango de uso del joystick.



## Capítulo 5: Conclusiones

El proyecto SRAGV demostró la viabilidad de utilizar variables climáticas —en particular el viento— para realizar ajustes granulares en sistemas de irrigación. La implementación de dos lógicas de inhibición configurables (inhibición directa e inhibición inversa o contrasector) otorga una flexibilidad arquitectónica superior a las alternativas comerciales simples, permitiendo adaptarse tanto a la prevención de arrastre de agua como a la protección de estructuras adyacentes a los sectores de riego. El desarrollo permitió consolidar conocimientos sobre sistemas embebidos mediante el uso de ADC por DMA, comunicación I²C (EEPROM), interfaz paralela (LCD), UART (Bluetooth), antirrebotes y máquinas de estado no bloqueantes.

---

## Capítulo 6: Uso de herramientas de inteligencia artificial

Durante el desarrollo del proyecto, se recurrió a herramientas de inteligencia artificial (IA) generativa como asistentes de consulta técnica y codificación. Su uso se concentró en las siguientes áreas.

Desarrollo de la aplicación web: la *web app* de monitoreo y control remoto fue desarrollada con asistencia de herramientas de IA, tanto para la estructura del código JavaScript como para el diseño de la interfaz de usuario y la lógica de comunicación por Bluetooth.

Resolución de problemas de código: durante la integración de los distintos módulos de hardware, surgieron inconvenientes relacionados con la sincronización del ADC por DMA, el manejo del bus I²C. Las herramientas de IA se utilizaron para diagnosticar los errores e interpretar los mensajes del compilador, y las soluciones propuestas fueron evaluadas y adaptadas por el equipo antes de su incorporación al proyecto.

---

## Capítulo 7: Bibliografía y referencias

[1] MercadoLibre. *Programador de Riego Rain Bird Automático 4 Zonas*. [En línea]. Disponible en: https://www.mercadolibre.com.ar/programador-de-riego-rain-bird-automatico-4-zonas-blanco/p/MLA37257397

[2] Rain Bird Corporation. *Rain Bird — A Global Irrigation Company*. [En línea]. Disponible en: https://www.rainbird.com/es

[3] STMicroelectronics. *Reference Manual STM32F103xB*. [En línea]. Disponible en: https://www.st.com/resource/en/reference_manual/cd00171190.pdf

[4] O. Ridao, G. Taffernaberry, G. Stark y M. Crocce, *A Beginner's Guide to Designing Embedded System Applications on Arm Cortex-M Microcontrollers*. ARM Education Media, 2022.

[5] STMicroelectronics. *STM32 Nucleo-64 Boards (MB1136) — User Manual*. [En línea]. Disponible en: https://www.st.com/resource/en/user_manual/um1724-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf
