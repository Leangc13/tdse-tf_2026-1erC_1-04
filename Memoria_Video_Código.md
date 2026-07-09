<img width="762" height="292" alt="image" src="https://github.com/user-attachments/assets/f4169b6a-832d-42a4-a46d-fa6cc8cc8f36" />

# **Sistema de Riego Automático con Gestión de Viento (SRAGV) - INFORME FINAL**

**Autores:** Garcia Caneva Leandro, Vargas Joaquin, Molina Aban Florencia  
**Padrones:** 103476, 104323, 104153  
**Fecha:** 1er cuatrimestre 2026

---

## 0.1 Objetivo del Trabajo

El objetivo de este trabajo es diseñar e implementar un sistema embebido de riego automático capaz de tomar decisiones inteligentes en función del estado del viento. La idea central consiste en que el sistema **modifique o bloquee el riego según la velocidad y dirección del viento**, dado que en condiciones de viento intenso el agua se dispersa fuera de la zona de interes, haciendo el proceso de riego ineficiente o directamente perjudicial para el cultivo.

El sistema gestiona cuatro sectores de riego independientes representados por LEDs (Norte, Sur, Este y Oeste) y tres modos de operación diferenciados:

*   **Viento bajo o nulo:** se activan en secuencia todos los sectores habilitados por el usuario.
*   **Viento moderado:** se activan únicamente los sectores a favor del viento para evitar el desperdicio en zonas expuestas.
*   **Viento crítico → Modo FALLA:** el riego se suspende completamente y se activa una alarma sonora (buzzer) y visual (LCD,APP Bluetooth).

Como resultado esperado se obtiene un prototipo funcional que demuestra de forma clara los tres modos de operación, implementando lectura de sensores analógicos por ADC con DMA, comunicación remota vía Bluetooth, persistencia de configuración en EEPROM externa y una arquitectura de software Bare Metal Event-Triggered con código estrictamente no bloqueante.

---

## 0.2 Motivaciones
En regiones de alta productividad agrícola y características geográficas particulares —como la **Patagonia argentina**— la presencia de vientos intensos y constantes se convierte en un factor adverso determinante para el riego por aspersión tradicional. Fenómenos como el **viento zonda** o las rachas típicas de las mesetas patagónicas pueden superar los 80 km/h con facilidad, provocando la **"deriva"** del agua: el riego es desviado de las zonas objetivo por la acción del viento, 

*   **Pérdidas económicas significativas** para productores que ya operan con márgenes ajustados.
*   **Desperdicio severo del recurso hídrico**, en zonas que padecen restricciones hídricas severas.
*   **En riegos urbanos**, mitiga el impacto del viento que suele desviar el agua hacia zonas imprevistas. Al mantener el riego bajo control, se evitan filtraciones en paredes y encharcamientos en la vía pública, los cuales—en zonas con temperaturas bajo cero—pueden derivar en congelamiento de calzadas y daños edilicios..

En este contexto, un sistema de riego inteligente que incorpore el viento como variable de decisión tiene aplicación directa y relevancia real en condiciones climaticas adversas.

---

## 1. Hardware Obligatorio & Adicional

El sistema se basa en la placa de desarrollo STM32 y diversos periféricos para censado y actuación:

**Hardware Obligatorio:**
*   **Placa de desarrollo:** NUCLEO-F103RB (ARM Cortex-M3).
*   **Interfaz de usuario visual:** Display LCD 16x2 alfanumérico. Se utiliza en modo de 4 bits, conectado a los pines PC4 a PC9.
*   **Interfaz de usuario física:** 2 botones pulsadores (para navegación e ingreso en el menú interactivo).

**Hardware Adicional:**
*   **Módulo de Comunicación Remota:** Módulo Bluetooth HM-10 (UART).
*   **Sensores Analógicos:** 
    *   Joystick analógico para emular la velocidad y la dirección del viento.
    *   Sensor LDR (fotocélula) para detectar el nivel de luminosidad ambiente.
*   **Actuadores Visuales y Sonoros:** 
    *   4 LEDs indicadores para los sectores de riego (Norte, Sur, Este, Oeste).
    *   2 LEDs de estado (verde para riego activo, rojo para falla).
    *   1 Buzzer activo para alertas sonoras.

---

## 2. Programación Obligatoria & Adicional

El firmware del sistema se desarrolló bajo el paradigma **Bare Metal Event-Triggered**, con código estrictamente no bloqueante estructurado modularmente:

**Programación Obligatoria:**
*   **Máquinas de Estados Finitos (FSM):** Arquitectura principal del programa dividida en estados bien definidos (`NORMAL`, `SET_UP`, `FALLA`) para coordinar las lecturas y el control de los actuadores sin bloqueos.
*   **GPIO:** Configuración y manejo de entradas (botones con antirrebote por software) y salidas (control de LEDs y buzzer).
*   **Soft RTC (SysTick):** Base de tiempos del sistema a partir de interrupciones del SysTick cada 1 ms, utilizado para temporización de tareas no bloqueantes y control de duración de riego.

**Programación Adicional:**
*   **ADC con DMA:** Lectura periódica de los tres canales analógicos (dos para el joystick y uno para la LDR). Se utiliza acceso directo a memoria (DMA) para realizar las conversiones de manera automática y no bloqueante.
*   **I2C (Inter-Integrated Circuit):** Comunicación bidireccional utilizada para interactuar con la EEPROM externa (AT24C02) para el almacenamiento no volátil de la configuración de usuario.
*   **USART2:** Protocolo serie utilizado para la comunicación asincrónica con el módulo Bluetooth HM-10, permitiendo la configuración remota y monitoreo desde una aplicación móvil.

---

## 3. Pruebas de Integración

En esta sección se detalla la prueba de integración final, validando el correcto funcionamiento en conjunto de todos los módulos del firmware y periféricos del hardware según los casos de uso definidos (Riego completo, Riego parcial, y Modo de configuración).

**Link al Video:**
[A COMPLETAR: enlace al video de demostración acá]

---

## 4. Esquema Eléctrico y Vistas de Cableado

A continuación se presenta el conexionado final de los componentes.

**Esquema Eléctrico:**
[ A COMPLETAR: Pegar captura del esquema eléctrico]

**Vista de Cableado (Prototipo Físico):**
[COMPLETAR: Pegar foto del conexionado final de la placa acá]

---

## 5. Descripción del Comportamiento

El comportamiento del sistema es gestionado a través de una Máquina de Estados Finitos (FSM) que centraliza la lógica de control en tres modos de operación principales:

*   **Modo NORMAL:** Es el estado por defecto. El sistema monitorea de manera continua los sensores a través del ADC. Basándose en la velocidad y dirección del viento leídas, el sistema decide si:
    *   Activar todos los sectores secuencialmente (viento bajo/nulo).
    *   Activar solo los sectores a favor del viento (viento moderado).
    
    Si la luminosidad indica horario nocturno y la configuración lo requiere, el riego automático se inhibe.
    
*   **Modo SET_UP:** El sistema ingresa a este modo mediante la pulsación prolongada del botón de menú. En este estado se suspende el riego automático y se presenta un menú interactivo en el display LCD. El usuario puede modificar los umbrales de viento (moderado y crítico), habilitar/deshabilitar sectores y establecer el tiempo de riego por sector. Al confirmar, los datos son persistidos en la memoria EEPROM vía I2C.

*   **Modo FALLA:** El sistema transita a este estado de emergencia cuando la lectura del viento supera el umbral crítico configurado o se detecta una lectura inválida (saturada o nula) en los sensores de viento. Se aborta inmediatamente cualquier ciclo de riego en curso, se inhiben todos los LEDs de sector, se enciende el LED rojo intermitente de forma no bloqueante y se activa la alarma mediante el buzzer, mostrando el motivo en el LCD.

### 5.1 Diagrama de Estados (Statechart)

El siguiente diagrama representa la Máquina de Estados Finitos (FSM) principal del SRAGV.

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

<p align="center"><em>Figura 5.1: Diagrama de estados (Statechart) del SRAGV — FSM principal</em></p>

---

## 6. Salida (Console & Build Analyzer)

Reporte del uso de memoria generado tras la compilación del firmware.

**Build Analyzer (Tamaños en bytes):**
*   `text` (Código y constantes en Flash): [A COMPLETAR: Pegar tamaño acá] bytes
*   `data` (Variables globales inicializadas): [A COMPLETAR: Pegar tamaño acá] bytes
*   `bss` (Variables globales sin inicializar): [A COMPLETAR: Pegar tamaño acá] bytes

**Regiones de Memoria:**
*   **RAM:** [COMPLETAR: Pegar uso en bytes] bytes ( [COMPLETAR: Pegar %] % )
*   **FLASH:** [COMPLETAR: Pegar uso en bytes] bytes ( [COMPLETAR: Pegar %] % )

[COMPLETAR: Pegar captura del Build Analyzer acá]

---

## 7. Medición de Tiempos (WCET)

Se han analizado las tareas y rutinas principales en el peor de los casos (Worst-Case Execution Time) para garantizar el cumplimiento de los requerimientos de tiempo real de la arquitectura (cada vuelta del super-loop debe demorar menos de 1 ms).

| Tarea / Función | Tiempo de ejecución (WCET) |
| :--- | :--- |
| `ADC_Read_DMA_Callback()` | [A COMPLETAR: Tiempo en µs] |
| `FSM_Update_State()` | [A COMPLETAR: Tiempo en µs] |
| `LCD_Update_Display()` | [A COMPLETAR: Tiempo en µs] |
| `EEPROM_Write_Config()` | [A COMPLETAR: Tiempo en µs] |
| `Bluetooth_Process_Rx()` | [A COMPLETAR: Tiempo en µs] |

---

## 8. Factor de Uso (U)

El factor de uso o utilización de la CPU se define a partir de la sumatoria de la relación entre el tiempo de ejecución en el peor de los casos ($C_i$) y el período de ejecución o de interrupción ($T_i$) de cada tarea/evento en el sistema:

$$U = \sum \frac{C_i}{T_i} = \frac{C_1}{T_1} + \frac{C_2}{T_2} + \dots + \frac{C_n}{T_n}$$

**Factor de Uso (U) calculado:** [A COMPLETAR: Pegar resultado numérico de U acá] %

---

## 9. Medición y Análisis de Consumo

El sistema puede ser alimentado con distintas tensiones y puede ingresar en modo de bajo consumo (Sleep Mode) ante un tiempo preestablecido de inactividad de riegos y comunicaciones.

| Condición de Alimentación | Modo de Trabajo | Consumo Medido (mA) | Observaciones |
| :--- | :--- | :--- | :--- |
| 3.3V | Normal (Sin bajo consumo) | [A COMPLETAR] | Sistema activo, todos los periféricos encendidos evaluando condiciones. |
| 3.3V | Sleep (Bajo consumo) | [A COMPLETAR] | CPU en Sleep, interrupciones habilitadas. Esperando Wake-Up. |
| 5V | Normal (Sin bajo consumo) | [A COMPLETAR] | Sistema activo, alimentado desde pin de 5V. |
| 5V | Sleep (Bajo consumo) | [A COMPLETAR] | CPU en Sleep, alimentado desde pin de 5V. |

[A COMPLETAR: Pegar capturas del osciloscopio o fotos del miliamperímetro acá]
