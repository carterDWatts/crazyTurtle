#include <SPI.h>
#include <DualVNH5019MotorShield.h>
#include <nRF24L01.h>
#include <RF24.h>


/*
 * Mini Pod Code
 * V 0.1
 * 
 * Carter Watts
 * 
 * Notes:
 */

//Comm
  RF24 radio (7, 8); // CE, CSN
  const byte addresses[][6] = {"00001", "00002"};
  String data ="";
  boolean dataOutB = false;
  String dataOut;
  int startInd, ind1, ind2, ind3, ind4, ind5, endInd;
  int runningB, x, y, z, f1, f2;
  String runningS, xS ,yS, zS, f1S, f2S;

//Motor
  DualVNH5019MotorShield md;
  //Which side of the robot the pod is - needed for arcade control
  const string LEFT = "left";
  const string RIGHT = "right";
  
  //String side = LEFT;
  //String side = RIGHT;
  
void setup() {
  
  Serial.begin(115200);

  //Radio
    radio.begin();
    radio.openWritingPipe(addresses[0]); // 00001
    radio.openReadingPipe(1, addresses[1]); // 00002
    radio.setPALevel(RF24_PA_MIN);
  //Motor Driver  
    md.init();
}

void loop(){

  //Control
  delay(5);
  radio.startListening();
    //Reading
    if(radio.available()){
      
      radio.read(&data, sizeof(data));
  
      //Indexing
        ind1 = data.indexOf('/');
        ind2 = data.indexOf('/',ind1+1);
        ind3 = data.indexOf('/',ind2+1);
        ind4 = data.indexOf('/',ind3+1);
        ind5 = data.indexOf('/',ind4+1);
        endInd = data.indexOf('>');
  
      //String Data
        runningS = data.substring(startInd+1,ind1);
        xS = data.substring(ind1+1,ind2);
        yS = data.substring(ind2+1,ind3);
        zS = data.substring(ind3+1,ind4);
        f1S = data.substring(ind4+1,endInd);
        f2S = data.substring(ind5+1,endInd);
      
      //Int data
        runningB = runningS.toInt();
        x = xS.toInt();
        y = yS.toInt();
        z = zS.toInt();
        f1 = f1S.toInt();
        f2 = f2S.toInt();

      //Control motors

        if(x < 30 && x > -30)
          x = 0;
        if(y < 30 && x > -30)
          y = 0;
      
        xIn = -x;
        yIn = -y;    
        //For left
        if(type == LEFT)
          //md.setSpeed(-y+x);
          md.setSpeeds(yIn-xIn);
  
        //For right
        if(side == RIGHT)
          //md.setSpeed(-y-x);
          md.setSpeeds(yIn+xIn);

    }

  //Response
    delay(5);
    radio.stopListening();
    if(dataOutB)
      radio.write(&dataOut, sizeof(dataOut));
  
}

void respond(String response){
  dataOutB = true;
  dataOut = response;

}

void stopIfFault(){
  
  if (md.getM1Fault()){
    Serial.println("M1 fault");
    while(1);
  }
  if (md.getM2Fault()){
    Serial.println("M2 fault");
    while(1);
  }
  
}
