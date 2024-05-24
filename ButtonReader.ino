#include <Time.h>

int buttonPin = 9;
int pointTime = 200;
int timeDelta = 50;

bool presionado = false;
int startPressed, endPressed, startSpace, endSpace = 0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if (digitalRead(9) == HIGH){
    startSpace, endSpace = 0;
    startPressed = millis();
    while (digitalRead(9) == HIGH);
    endPressed = millis();
    int time = endPressed - startPressed;
    if (time >= pointTime - timeDelta && time<=pointTime+ timeDelta){
      Serial.print(".");
    }
    else if(time >= 3*(pointTime - timeDelta) && time <= 3*(pointTime + timeDelta)){
      Serial.print("_");
    }
  }else{
    startPressed, endPressed = 0;
    startSpace = millis();
    while (digitalRead(9) == LOW);
    endSpace = millis();
    int time = endSpace - startSpace;
    if (time >= pointTime - timeDelta && time<=pointTime+ timeDelta){
      Serial.print("");
    }
    else if(time >= 3*(pointTime - timeDelta) && time <= 3*(pointTime + timeDelta)){
      Serial.print(" ");
    }
    
  }
}