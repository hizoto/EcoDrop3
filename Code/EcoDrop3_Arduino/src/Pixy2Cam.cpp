#include "Pixy2Cam.h"
#include <Pixy2.h>
#include "bewegung.h"
#include <Arduino.h>
#include "communication.h"

Pixy2 pixy;
uint16_t pixyHoehe = 208;
uint16_t pixyBreite = 316;


void pixySetup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");
  
  pixy.init();
}



void pixyMoveForward()
{
    pixy.ccc.getBlocks();
    if(pixy.ccc.blocks[0].m_y < pixyHoehe/2){
        while (pixy.ccc.blocks[0].m_y < pixyHoehe/2)
        {
        moveForward(1);
        Serial.println("Roboter bewegt sich vorwärts");
        }
        stopMotors();
        
    }

}

void pixyMoveBackwardUntilObject()
{
pixy.ccc.getBlocks();
    if(!pixy.ccc.numBlocks){
        while (!pixy.ccc.numBlocks)
        {
        moveBackward(1);
        Serial.println("Roboter bewegt sich rückwärts");
        }
        stopMotors();
        
    }

}

void pixyMoveRight()
{
pixy.ccc.getBlocks();
    if(pixy.ccc.blocks[0].m_x > pixyBreite/2){
        while (pixy.ccc.blocks[0].m_x > pixyBreite/2)
        {
        moveRight(1);
        Serial.println("Roboter fährt nach rechts");
        }
        stopMotors();
        
    }


}

void pixyMoveLeft()
{
pixy.ccc.getBlocks();
    if (pixy.ccc.blocks[0].m_x < pixyBreite/2){
        while (pixy.ccc.blocks[0].m_x < pixyBreite/2)
        {
        moveLeft(1);
        Serial.println("Roboter fährt nach links");
        }
        stopMotors();
        
    }
    

}

void pixyTestfunktion(){
    pixyMoveForward();
    pixyMoveBackwardUntilObject();
    pixyMoveLeft();
    pixyMoveRight();
}


