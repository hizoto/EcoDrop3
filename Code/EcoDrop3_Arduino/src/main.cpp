#include "communication.h"
#include "sensors.h"
#include "bewegung.h"

// Steps:
// 0 = Idle
// 1 = xxxx

// Konfigurationsvariablen
int wandabstand = 50;
int wandabstandLadezone = 60;
int abladeZoneWandabstand = 850;
int roboterbreite = 250; 
int sicherheitsmarge = 50;

unsigned long lastLogMessage = 0;

int currentStep = 0;

void setup() {
    startComm();
    //initSensors();
}

void loop() {
    if(Serial1.available()){
        getComm();
    }
    if(isRunning){
        switch(currentStep){
            // idle
            case 0:
                currentStep = 1;
                break;
            // kann noch genau definiert werden oder von Flowchart übernommen werden. Kommunikation ist notwendig.
            case 1:
                digitalWrite(12, HIGH);
                logMessage("EcoDrop on.");
                currentStep++;
                break;
            
            // Ladestation verlassen
            case 10:
                moveOutOfDock();
                currentStep = 20;
                break;

            // ersten Container finden
            case 20:
                goParallel();
                moveToRightWall(wandabstand);
                moveForwardParallelUntilContainer(wandabstand);
                currentStep = 30;
                break;

            // ersten Container aufnehmen
            case 30:
                turnRight(90);
                pickUpContainer();
                turnLeft(180);
                currentStep = 40;
                break;

            // zweiten Container finden
            case 40:
                goParallel();
                moveToRightWall(wandabstand);
                moveForwardParallelUntilContainer(wandabstand);
                currentStep = 50;
                break;

            // zweiten Container aufnehmen
            case 50:
                turnRight(90);
                pickUpContainer();
                turnLeft(180);
                currentStep = 60;
                break;

            // dritten Container finden
            case 60:
                goParallel();
                moveToRightWall(wandabstand);
                moveForwardParallelUntilContainer(wandabstand);
                currentStep = 70;
                break;

            // dritten Container aufnehmen
            case 70:
                turnRight(90);
                pickUpContainer();
                turnRight(90);
                currentStep = 80;
                break;

            // in Abladezone fahren
            case 80:
                moveToRightWall(abladeZoneWandabstand);
                currentStep = 90;
                break;

            // abladen
            case 90:
                abladen();
                currentStep = 100;
                break;

            case 100:
                moveToRightWall(abladeZoneWandabstand - roboterbreite + sicherheitsmarge);
                turnRight(90);
                moveToRightWall(wandabstandLadezone);
                parkieren();
                currentStep = 0;
                break;

            case 900:
                currentStep = 0;
                break;
        }
    }
    else {
        currentStep = 0;
        //logik für Kommunikationstests
        digitalWrite(12, LOW);
        if(millis() - lastLogMessage > 10000){
        logMessage("EcoDrop is idle.");
        lastLogMessage = millis();
        }
    }
    
}

