#include "Pixy2Cam.h"
#include <Pixy2.h>
#include "bewegung.h"
#include <Arduino.h>
#include "communication.h"

Pixy2 pixy;
uint16_t pixyHoehe = 208;           // Definierte Höhe der Pixyanzeige
uint16_t pixyBreite = 316;          // Definierte Breite der Pixyanzeige
int endschalterVorne = 41;

void pixySetup(){
    pinMode(endschalterVorne, INPUT_PULLUP);
    pixy.init();
}



void pixyMoveForward(){
    bool endschalterVorneGetriggert = digitalRead(endschalterVorne);
    logMessage("Roboter bewegt sich vorwärts");                 // Roboter fährt vorwärts solange der Endschalter nicht betätigt wurde
    while(!endschalterVorneGetriggert){
        moveForward(1);
        endschalterVorneGetriggert = digitalRead(endschalterVorne);
    }
    stopMotors();
}

void pixyMoveForwardUntilObject(){
pixy.ccc.getBlocks();                                           // Roboter fährt Rückwärts solange kein Objekt erkannt wurde
    if(!pixy.ccc.numBlocks){
        while (!pixy.ccc.numBlocks)
        {
        moveForward(1);
        pixy.ccc.getBlocks();
        }
        logMessage("Container erkannt");
        stopMotors();
        
    }

}

void pixyMoveMiddle(int ziel){
    int tolerance = 5;
    pixy.ccc.getBlocks();
    do {
        // Auswerten, in welche Richtung korrigiert werden muss
        if(pixy.ccc.blocks[0].m_x < ziel){
            logMessage("Position nach rechts korrigieren...");
            while (pixy.ccc.blocks[0].m_x < ziel){
                moveRight(1);
                pixy.ccc.getBlocks();
                stopMotors();
            }
        }
        else{
            logMessage("Position nach links korrigieren...");
            while (pixy.ccc.blocks[0].m_x > ziel){
                moveLeft(1);
                pixy.ccc.getBlocks();
                stopMotors();
            }
        }
    }while(pixy.ccc.blocks[0].m_x > ziel + tolerance || pixy.ccc.blocks[0].m_x < ziel - tolerance);
}


void pixyErrorObjects(){
    if(pixy.ccc.numBlocks > 1){                                 // Fehlermeldung falls mehrere Objekte erkannt wurden
    
        pixy.ccc.getBlocks();
        logMessage("Mehrere Objekte erkannt");
    }
}

void pixyLampeOn(){
    pixy.setLamp(1, 0);
    delay(200);
}

void pixyLampeOff(){
    pixy.setLamp(0, 0);
}