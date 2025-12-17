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

void pixyMoveMiddle(){
    pixy.ccc.getBlocks();                                           // Roboter fährt nach rechts solange Objekt nicht im Bereich erscheint
    if(pixy.ccc.blocks[0].m_x > 160){
        while (pixy.ccc.blocks[0].m_x > 150){
            moveRight(1);
            logMessage("Roboter fährt nach rechts");
            pixy.ccc.getBlocks();
        }
        stopMotors();
    }
    else{    
        while (pixy.ccc.blocks[0].m_x < 150){
            moveLeft(1);
            logMessage("Roboter fährt nach links");
            pixy.ccc.getBlocks();
        }
        stopMotors();
    }
}


void pixyErrorObjects(){
    if(pixy.ccc.numBlocks > 1){                                 // Fehlermeldung falls mehrere Objekte erkannt wurden
    
        pixy.ccc.getBlocks();
        logMessage("Mehrere Objekte erkannt");
    }
}

void pixyLampeOn(){
    pixy.setLamp(1, 0);
}

void pixyLampeOff(){
    pixy.setLamp(0, 0);
}