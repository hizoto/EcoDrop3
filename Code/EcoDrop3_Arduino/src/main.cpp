#include "communication.h"
#include "sensors.h"
#include "bewegung.h"
#include "Pixy2Cam.h"

// Steps:
// 0 = Idle
// 1 = xxxx

// Konfigurationsvariablen
uint16_t wandabstand = 150;
uint16_t wandabstandLadezone = 70;
uint16_t abladeZoneWandabstand = 50;
int roboterbreite = 250; 
int sicherheitsmarge = 50;

unsigned long lastLogMessage = 0;
unsigned long lastTofLog = 0;

int currentStep = 0;
bool firstTry = true;
const bool isTestCase = false;

void setup() {
    startComm();
    initSensors();
    pixySetup();
    startMotors();
    idleMotors();
}

void loop() {
    if(Serial1.available()){
        getComm();
    }
    if(isRunning){
        switch(currentStep){
            // idle
            case 0:
                if (isTestCase){
                    currentStep = 1;
                }
                else {
                    currentStep = 10;  // 1 -> Testcase  // 10 -> Schrittkette
                }
                break;
            // Testcase
            case 1:
                if (millis() - lastLogMessage > 1000){
                    // logMessage(endschalterStatusHinten().c_string);
                    lastLogMessage = millis();
                }
                //oeffnen();
                staplerOben();
                //goParallelRight();
                //containerAufladen();

                break;
            
            // Ladestation verlassen
            case 10:
                if(setOffsetsLeft()){
                    logMessage("Offsets für linke TOF gesetzt. Aktuelle Messwerte: ");
                    logTofs(true,false,true,false);
                }
                else {
                    logMessage("Fehler beim nullen von TOFs links");
                }
                moveOutOfDock();
                if(setOffsetsRight()){
                    logMessage("Offsets für rechte TOF gesetzt. Aktuelle Messwerte: ");
                    logTofs(false, true, false, true);
                }
                else {
                    logMessage("Fehler beim nullen von TOFs rechts");
                }
                wandabstandLadezone = tofFR().readRaw();
                currentStep = 20;
                break;

            // ersten Container finden
            case 20:
                moveForwardParallelUntilContainer(wandabstand);
                currentStep = 30;
                break;

            // ersten Container aufnehmen
            case 30:
                //goParallelRight();
                moveLeft(50);
                turnRight(90);
                goParallelLeft();
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
                //goParallelRight();
                moveLeft(50);
                turnRight(90);
                goParallelLeft();
                pickUpContainer();
                turnLeft(90);
                goParallelRight();
                //moveForward(300);
                currentStep = 60;
                break;

            // dritten Container finden
            case 60:
                turnLeft(90);
                goParallelRight();
                moveToRightWall(wandabstand);
                moveForwardParallelUntilContainer(wandabstand);
                currentStep = 70;
                break;

            // dritten Container aufnehmen
            case 70:
                //goParallelRight();
                moveLeft(100);
                turnRight(90);
                moveForward(130);
                goParallelLeft();
                pickUpContainer();
                currentStep = 80;
                break;

            // in Abladezone fahren
            case 80:
                moveToLeftWall(abladeZoneWandabstand);
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
                moveToLeftWall(300);
                turnRight(90);
                goParallelLeft();
                moveRight(500);
                moveToRightWall(wandabstandLadezone + 20);
                parkieren();
                isRunning = false;
                currentStep = 0;
                break;

            case 900:
                currentStep = 0;
                break;
        }
    }
    else {
        currentStep = 0;
        idleMotors();
        if(millis() - lastLogMessage > 2000){
        logMessage("EcoDrop is idle.");
        lastLogMessage = millis();
        stopMotors();
<<<<<<< HEAD
        //schliessen();
        staplerUnten();
        }
=======
        }
        if (isTestCase){
            //schliessen();
            staplerUnten();
        }
>>>>>>> b6a418797d792d19b4ff9897c68eb39898f97e11
    }    
}

