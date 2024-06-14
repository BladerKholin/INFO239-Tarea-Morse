String texto[] = {"-","---","-...",".-",".-.."," "};
String palabra = "";

String decodifica[] = {".-", "A" , "-..." , "B" , "-.-." , "C",
                        "-..", "D" ,".","E","..-.","F",
                        "--.","G","....","H","..","I",
                        ".---","J", "-.-", "K", ".-..","L",
                        "--","M", "-.", "N", "---" , "O",
                        ".--.","P", "--.-", "Q", ".-." , "R",
                        "..." , "S" , "-" , "T" , "..-", "U",
                        "...-", "V", ".--", "W", "-..-", "X",
                        "-.--" , "Y", "--.." , "Z"
};


String letras[] = {"A","B","C","D","E","F","G","H","I","J","K",
                    "L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

String morse[] = {".-", "-...", "-.-.",
                        "-.." ,".","..-.",
                        "--.","....","..",
                        ".---", "-.-", ".-..",
                        "--", "-.", "---",
                        ".--.", "--.-", ".-." ,
                        "..." , "-" , "..-",
                        "...-", ".--", "-..-",
                        "-.--" , "--.." 
};


void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // Opcion 1
  /*
  for(int i = 0; i < 26;i+=2){
    if(texto == decodifica[i]){
      Serial.println(decodificar[i+1]);
      break;
      }
    }
		
  */
  // Opcion 2
  
  decodificar();
}


void decodificar (){
  for (int j = 0; j < 6 ; j++){
    for(int i = 0; i < 26;i++){
      if(texto[j] == morse[i]){
        //Serial.println(letras[i]);
        palabra += letras[i];
        break;
      }
    }
    if(texto[j] == " "){
        Serial.println(palabra);
        palabra = "";
      }
      delay(200);
    
  }
}
