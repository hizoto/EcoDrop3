# 📐 Design Guidelines – EcoDrop3 Projekt

Diese Guidelines sollen sicherstellen, dass unser Code **lesbar, wartbar und erweiterbar** bleibt.  
Bitte haltet euch an diese Regeln, wenn ihr Code in dieses Repository pusht oder Pull Requests erstellt.  

---

## 🔹 Allgemeine Prinzipien
- **Klarheit vor Cleverness**: Code soll **leicht lesbar** sein, auch für jemanden, der neu ins Projekt kommt.  
- **Eine Funktion, eine Aufgabe**: Jede Funktion soll genau **eine klar definierte Aufgabe** erfüllen.  
- **Keine unnötige Duplikation**: Nutzt Funktionen, Klassen oder Konstanten, statt Code mehrfach zu kopieren.  
- **So wenig wie möglich hardcoden**: Werte (z. B. Pins, Limits, Strings) als **Konstanten** oder in **Konfigurationsdateien** definieren.  
- **Kommentare sparsam, aber sinnvoll**: Kommentare dort, wo Logik nicht sofort selbsterklärend ist.  

---

## 🔹 Namenskonventionen
- **Variablen:** `camelCase`  
  ```cpp
  int motorSpeed;
  String sensorName;
```

Funktionen: camelCase, Verb am Anfang, beschreibt klar die Aufgabe

```cpp
void readTemperature();
int calculateChecksum();
```
Konstanten & Defines:
```cpp
ALL_CAPS_WITH_UNDERSCORES

const int MAX_SPEED = 100;
#define DEBUG_MODE 1
```

Klassen & Structs: PascalCase

```cpp
class MotorController { ... };
struct SensorData { ... };
```

Dateien: beschreibend, klein geschrieben, Unterstrich statt CamelCase

```cpp
webinterface.cpp
motor_control.h
```


---

🔹 Struktur & Organisation

Header (.h) enthalten Deklarationen (Funktionen, Klassen, Konstanten).

Source (.cpp) enthalten die Implementierungen.

Keine „God Files“: lieber in thematisch passende Module aufteilen.

Globale Variablen so weit wie möglich vermeiden. Falls nötig, klar dokumentieren.



---

🔹 Funktionen & Klassen

Jede Funktion hat maximal eine Kernaufgabe.

Funktionen möglichst kurz und modular halten (< 50 Zeilen, wenn möglich).

Wiederverwendbare Logik in Hilfsfunktionen oder Klassen auslagern.

Input-Parameter prüfen und sinnvolle Default-Werte setzen.



---

🔹 Flexibilität & Wartbarkeit

Nutzt Enums oder Konstanten statt „magischer Zahlen“:

```cpp
// schlecht
if(mode == 3) { ... }

// besser
enum RobotMode { IDLE = 0, ACTIVE = 1, ERROR = 2, TEST = 3 };
if(mode == TEST) { ... }

Nutzt Config-Files oder zentrale Header für Pinbelegungen und Settings.

Logging über eine zentrale Methode (logToWebinterface()), nicht verstreut per Serial.println.
```


---

🔹 Git & Zusammenarbeit

Branches pro Feature oder Bugfix (z. B. feature/motor-control).

Commits klein und beschreibend.

✅ „Add motor control class“

❌ „fix stuff“


Pull Requests nur, wenn der Code kompiliert und getestet wurde.

Reviews ernst nehmen: Feedback einbauen oder begründet ablehnen.



---

🔹 Beispiel guter Praxis

```cpp
// motor_control.h
#pragma once

class MotorControl {
public:
    MotorControl(int pinPWM, int pinDir);
    void setSpeed(int speedPercent);
    int getSpeed() const;

private:
    int pwmPin;
    int dirPin;
    int currentSpeed;
};

// motor_control.cpp
#include "motor_control.h"
#include <Arduino.h>

MotorControl::MotorControl(int pinPWM, int pinDir)
    : pwmPin(pinPWM), dirPin(pinDir), currentSpeed(0) {
    pinMode(pwmPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
}

void MotorControl::setSpeed(int speedPercent) {
    speedPercent = constrain(speedPercent, -100, 100);
    currentSpeed = speedPercent;

    digitalWrite(dirPin, speedPercent >= 0 ? HIGH : LOW);
    analogWrite(pwmPin, abs(speedPercent) * 2.55); // 0-255 PWM
}

int MotorControl::getSpeed() const {
    return currentSpeed;
}
```