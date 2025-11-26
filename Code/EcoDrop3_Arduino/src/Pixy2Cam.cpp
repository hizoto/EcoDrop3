#include "Pixy2Cam.h"
#include <Pixy2.h>
#include "bewegung.h"
#include <Arduino.h>
#include "communication.h"

Pixy2 pixy;
uint16_t pixyHoehe = 208;           // Definierte Höhe der Pixyanzeige
uint16_t pixyBreite = 316;          // Definierte Breite der Pixyanzeige


void pixySetup()
{  
  pixy.init();
}



void pixyMoveForward()
{
    pixy.ccc.getBlocks();
    if(pixy.ccc.blocks[0].m_y < pixyHoehe/2){                   // Wenn Endschalter verbaut dann IF Funktion raus und mit While solange Endschalter nicht betätigt
        while (pixy.ccc.blocks[0].m_y < pixyHoehe/2)
        {
        moveForward(1);
        Serial.println("Roboter bewegt sich vorwärts");
        pixy.ccc.getBlocks();
        }
        stopMotors();
        
    }

}
/*void pixyMoveForwardE()
{
    while(!endschalterContainer){                               // Funktion wenn Endschaöter verbaut ist
        moveForward(1);
        Serial.println("Roboter fährt zum Container");
    }
    stopMotors();
}*/

void pixyMoveBackwardUntilObject()
{
pixy.ccc.getBlocks();                                           // Roboter fährt Rückwärts solange kein Objekt erkannt wurde
    if(!pixy.ccc.numBlocks){
        while (!pixy.ccc.numBlocks)
        {
        moveBackward(1);
        Serial.println("Roboter bewegt sich rückwärts");
        pixy.ccc.getBlocks();
        }
        stopMotors();
        
    }

}

void pixyMoveRight()
{
pixy.ccc.getBlocks();                                           // Roboter fährt nach rechts solange Objekt nicht im Bereich erscheint
    if(pixy.ccc.blocks[0].m_x > 160){
        while (pixy.ccc.blocks[0].m_x > 160)
        {
        moveRight(1);
        Serial.println("Roboter fährt nach rechts");
        pixy.ccc.getBlocks();
        }
        stopMotors();
        
    }


}

void pixyMoveLeft()
{
pixy.ccc.getBlocks();                                           // Roboter fährt nach links solange Objekt nicht im Bereich erscheint
    if (pixy.ccc.blocks[0].m_x < 140){
        while (pixy.ccc.blocks[0].m_x < 140)
        {
        moveLeft(1);
        Serial.println("Roboter fährt nach links");
        pixy.ccc.getBlocks();
        }
        stopMotors();
        
    }
    

}

void pixyErrorObjects()
{
    if(pixy.ccc.numBlocks > 1){                                 // Fehlermeldung falls mehrere Objekte erkannt wurden
    
        pixy.ccc.getBlocks();
        Serial.println("Mehrere Objekte erkannt");
    }
}

void pixyTestfunktion(){
    
         pixyErrorObjects();
         pixyMoveForward();
         pixyMoveBackwardUntilObject();
         pixyMoveLeft();
         pixyMoveRight();
    
   
    

}


