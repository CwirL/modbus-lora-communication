 #include <LoRaWan.h>

 
char buffer[256];
int v1,v2,v3,i1,i2,i3,fp,freq,date;
int p1,p2,p3,p4,p5,p6,p7,p8,p9;
char inChar;
String data_raspi="";



void setup(void) { //Definición de parámetros de transmisión
    
    SerialUSB.begin(9600);
    while(!SerialUSB);
      lora.init();
    
      lora.setDeciveMode(LWABP);
      
      lora.setDataRate(DR0, EU868);
      lora.setAdaptiveDataRate(false);
      lora.setChannel(0, 868.1);
      lora.setChannel(1, 868.3);
      lora.setChannel(2, 868.5);
      lora.setChannel(3, 867.1);
      lora.setChannel(4, 867.3);
      lora.setChannel(5, 867.5);
      lora.setChannel(6, 867.7);
      lora.setChannel(7, 867.9);
      lora.setChannel(8, 868.8);
    
      lora.setDutyCycle(false);
      lora.setJoinDutyCycle(false);
    
      lora.setPower(14);
      
}



void loop(void)
{
  
  bool result=false;

  if (Serial1.available()){
//Lectura de caracteres   
   inChar = Serial1.read();
//Suma de caracteres en variable data_raspi
   
   data_raspi+=inChar;
   
//Imprime la variable con los caracteres acumulados hasta la "*"   
   if (inChar=='*'){
    //Busqueda de simbolos especiales para separar cada dato
      p1=data_raspi.indexOf("!");
      p2=data_raspi.indexOf("#");
      p3=data_raspi.indexOf("$");
      p4=data_raspi.indexOf("%");
      p5=data_raspi.indexOf("&");
      p6=data_raspi.indexOf("/");
      p7=data_raspi.indexOf("(");
      p8=data_raspi.indexOf(")");
      p9=data_raspi.indexOf("*");
      
    //Clasificación de los datos
      v1=atoi(data_raspi.substring(0,p1).c_str());
      v2=atoi(data_raspi.substring(p1+1,p2+1).c_str());
      v3=atoi(data_raspi.substring(p2+1,p3+1).c_str());
      fp=atoi(data_raspi.substring(p3+1,p4+1).c_str());
      i1=atoi(data_raspi.substring(p4+1,p5+1).c_str());
      i2=atoi(data_raspi.substring(p5+1,p6+1).c_str());
      i3=atoi(data_raspi.substring(p6+1,p7+1).c_str());
      freq=atoi(data_raspi.substring(p7+1,p8+1).c_str());
      date=atoi(data_raspi.substring(p8+1,p9+1).c_str());
    //Formación de la trama y codificación a bytes
        unsigned char data_transfer[9]={0,1,2,3,4,5,6,7,8};
            data_transfer[0]=v1;
            data_transfer[1]=v2;
            data_transfer[2]=v3;
            data_transfer[3]=fp;
            data_transfer[4]=i1;
            data_transfer[5]=i2;
            data_transfer[6]=i3;
            data_transfer[7]=freq;
            data_transfer[8]=date;
            
            //Envío de la trama
    result = lora.transferPacket(data_transfer,9);
//Borra la variable string para almacenar nuevos datos
    data_raspi=""; 
    SerialUSB.println();
   }
  }
  
}
  
      
