#include "communication.h"
#include "sensors.h"
#include "bewegung.h"
#include "Pixy2Cam.h"

// Steps:
// 0 = Idle
// 1 = xxxx

// Konfigurationsvariablen
uint16_t wandabstand = 50;
uint16_t wandabstandLadezone = 60;
uint16_t abladeZoneWandabstand = 850;
int roboterbreite = 250; 
int sicherheitsmarge = 50;

unsigned long lastLogMessage = 0;

int currentStep = 0;
bool firstTry = true;

void setup() {
    startComm();
    //initSensors();
    pixySetup();
}

void loop() {
    /*if(Serial1.available()){
        getComm();
    }
        */
    if(isRunning){
        switch(currentStep){
            // idle
            case 0:
                currentStep = 1;        //Auskommentieren zum testen
                break;
            // Testcase
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
                moveForward(sicherheitsmarge);
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
        if(millis() - lastLogMessage > 10000){
        logMessage("EcoDrop is idle.");
        lastLogMessage = millis();
        }
    }
    pixyTestfunktion();
    //goParallel();
    //moveToRightWall(50);
    /*moveForward(50);
    delay(1000);
    moveBackward(50);
    delay(1000);*/
    //testVorwaerts();
    
}

