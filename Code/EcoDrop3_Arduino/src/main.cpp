#include "communication.h"
#include "sensors.h"
#include "bewegung.h"
#include "Pixy2Cam.h"

// Steps:
// 0 = Idle
// 1 = xxxx

// Konfigurationsvariablen
uint16_t wandabstand = 150;
uint16_t wandabstandLadezone = 60;
uint16_t abladeZoneWandabstand = 850;
int roboterbreite = 250; 
int sicherheitsmarge = 50;

unsigned long lastLogMessage = 0;
unsigned long lastTofLog = 0;

int currentStep = 0;
bool firstTry = true;

void setup() {
    startComm();
    initSensors();
    pixySetup();
    startMotors();
    stopMotors();
}

void loop() {
    if(Serial1.available()){
        getComm();
    }
    if(isRunning){
        switch(currentStep){
            // idle
            case 0:
                currentStep = 10;  // 1 -> Testcase  // 10 -> Schrittkette
                break;
            // Testcase
            case 1:
                if (millis() - lastLogMessage > 10000){
                    logMessage("EcoDrop on.");
                    lastLogMessage = millis();
                }
                goParallelRight();
                //containerAufladen();
                isRunning = false;
                break;
            
            // Ladestation verlassen
            case 10:
                moveOutOfDock();
                currentStep = 20;
                break;

            // ersten Container finden
            case 20:
                moveForwardParallelUntilContainer(wandabstand);
                currentStep = 30;
                break;

            // ersten Container aufnehmen
            case 30:
                goParallelRight();
                moveLeft(50);
                turnRight(90);
                // goParallelLeft();
                pickUpContainer();
                turnLeft(180);
                currentStep = 40;
                break;

            // zweiten Container finden
            case 40:
                moveForwardParallelUntilContainer(wandabstand);
                currentStep = 50;
                break;

            // zweiten Container aufnehmen
            case 50:
                goParallelRight();
                moveLeft(50);
                turnRight(90);
                goParallelRight();
                pickUpContainer();
                turnLeft(90);
                goParallelRight();
                moveForward(300);
                currentStep = 60;
                break;

            // dritten Container finden
            case 60:
                turnLeft(90);
                //goParallel();
                //moveToRightWall(wandabstand);
                moveForwardParallelUntilContainer(wandabstand);
                currentStep = 70;
                break;

            // dritten Container aufnehmen
            case 70:
                goParallelRight();
                moveLeft(50);
                turnRight(90);
                goParallelRight();
                pickUpContainer();
                currentStep = 80;
                break;

            // in Abladezone fahren
            case 80:
                moveToRightWall(abladeZoneWandabstand); // TODO moveToLeftWall
                currentStep = 90;
                break;

            // abladen
            case 90:
                rueckwaertsBisAnschlag();
                abladen();
                moveForward(sicherheitsmarge);
                currentStep = 100;
                break;

            // zurück zur Ladestation
            case 100:
                moveRight(600);
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
        if(millis() - lastLogMessage > 2000){
        logMessage("EcoDrop is idle.");
        lastLogMessage = millis();
        stopMotors();
        }
    }    
}

