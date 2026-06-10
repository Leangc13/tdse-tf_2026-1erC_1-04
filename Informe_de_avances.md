# Sistema de Riego Automático con Gestión de Viento (SRAGV)
## Informe de Avances

**Autores:** Garcia Caneva Leandro, Vargas Joaquin, Molina Aban Florencia

**Padrones:** 1034756, 104323, 104153

**Fecha:** Semana 14 — 1er cuatrimestre 2026

---

## Estado de avance

| Estado | Descripción |
|--------|-------------|
| 🟢 | Ya implementado |
| 🟡 | En proceso de implementarse |
| 🔴 | No se implementará |

> **Nota:** Al momento de esta entrega el equipo se encuentra con retraso, aún terminando los TPs de la cursada. Todos los materiales de hardware ya fueron adquiridos. Ningún módulo de software ha sido codificado todavía. Todos los requisitos están planificados para implementarse. 
---

### 1. Sensores analógicos

| Req ID | Descripción | Estado |
|--------|-------------|--------|
| 1.1 | El sistema contará con un joystick analógico (o dos potenciómetros) para emular la velocidad y dirección del viento, conectado a dos canales ADC del STM32. | 🟡 |
| 1.2 | El eje X del joystick representará la velocidad del viento; el eje Y representará la dirección (Norte, Sur, Este, Oeste). | 🟡 |
| 1.3 | El sistema contará con una fotocélula (LDR) conectada a un tercer canal ADC para medir la luminosidad ambiente. | 🟡 |
| 1.4 | Las lecturas de los tres canales ADC se realizarán mediante DMA con callback, sin polling bloqueante. | 🟡 |

---

### 2. Actuadores — LEDs de sector

| Req ID | Descripción | Estado |
|--------|-------------|--------|
| 2.1 | El sistema contará con 4 LEDs que representan los sectores de riego: Norte, Sur, Este y Oeste. | 🟡 |
| 2.2 | Con viento bajo o nulo, se activarán en secuencia todos los sectores habilitados por el usuario. | 🟡 |
| 2.3 | Con viento moderado, se activarán únicamente los sectores a favor del viento. | 🟡 |
| 2.4 | Con viento crítico (Modo FALLA), todos los LEDs de sector se apagarán. | 🟡 |
| 2.5 | Cada LED de sector permanecerá encendido durante el tiempo de riego configurado, apagándose de forma no bloqueante. | 🟡 |

---

### 3. Indicadores de estado

| Req ID | Descripción | Estado |
|--------|-------------|--------|
| 3.1 | El sistema contará con al menos 2 LEDs de estado adicionales: uno verde (sistema activo / riego en curso) y uno rojo (falla). | 🟡 |
| 3.2 | El sistema contará con un buzzer activo para señales sonoras: beep largo en falla. | 🟡 |
| 3.3 | El sistema contará con un display LCD 16×2 con interfaz I²C para mostrar estado actual, lecturas de sensores y menús. | 🟡 |
---

### 4. Modos de operación

| Req ID | Descripción | Estado |
|--------|-------------|--------|
| 4.1 | El sistema operará en tres modos: NORMAL, SET_UP y FALLA, implementados mediante una máquina de estados. | 🟡 |
| 4.2 | En Modo NORMAL, el sistema monitorea continuamente los sensores y activa los LEDs de sector según umbrales y condición de luz. | 🟡 |
| 4.3 | En Modo NORMAL, si la LDR indica noche y el usuario configuró "solo riego diurno", el sistema inhibe el riego. | 🟡 |
| 4.4 | En Modo SET_UP, el sistema suspende el riego automático y presenta el menú de configuración en el LCD. | 🟡 |
| 4.5 | En Modo FALLA, el sistema inhibe todos los LEDs de sector, activa el LED rojo y el buzzer, y muestra el motivo en el LCD. | 🟡 |
| 4.6 | El sistema iniciará siempre en Modo NORMAL con todos los LEDs de sector apagados. | 🟡 |

---

### 5. Menú interactivo

| Req ID | Descripción | Estado |
|--------|-------------|--------|
| 5.1 | El sistema contará con 3 botones para navegar el menú (avanzar opción, confirmar/seleccionar y retroceder), con antirrebote no bloqueante. | 🟡 |
| 5.2 | El sistema contará con DIP switches para opciones de configuración fija (habilitar/deshabilitar sectores). | 🟡 |
| 5.3 | El menú permitirá configurar: umbral de viento moderado, umbral crítico, duración del ciclo de riego y habilitación de sectores. | 🟡 |
| 5.4 | El menú mostrará en el LCD las lecturas actuales de velocidad de viento (% de rango ADC) y nivel de luz ambiente. | 🟡 |

---

### 6. Comunicación Bluetooth

| Req ID | Descripción | Estado |
|--------|-------------|--------|
| 6.1 | El sistema se comunicará con el módulo HM-10 vía UART con protocolo de comandos ASCII simple. | 🟡 |
| 6.2 | La app Bluetooth permitirá visualizar en tiempo real el estado de cada sector, nivel de viento y nivel de luz. | 🟡 |
| 6.3 | La app Bluetooth permitirá configurar umbrales de viento y duración del riego de forma remota. | 🔴 |
| 6.4 | La app Bluetooth permitirá activar o desactivar manualmente un sector de riego. | 🔴 |

---

### 7. Almacenamiento EEPROM

| Req ID | Descripción | Estado |
|--------|-------------|--------|
| 7.1 | El sistema almacenará la configuración del usuario en la EEPROM AT24C02 vía I²C. | 🟡 |

### 8. Soft RTC

| Req ID | Descripción | Estado |
|--------|-------------|--------|
| 8.1 | El sistema llevará un contador de ticks de 1 ms basado en SysTick, usado como referencia de tiempo para duración del riego y temporizadores. | 🟡 |

