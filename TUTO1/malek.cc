
 /**
   \file malek.cc
   \brief Programme illustrant l'utilisation du capteur sonore pour faire marcher le robot PRO-BOT 128
   \author HASSANI MALEK & ADLI OUAHIB
   \version 0.1
   \author Email  : malek.1905@live.fr
   \date 12/2015
*/
 int count;
 byte go;
void main(void){
     count = 0;

 PRO_BOT128_INIT();
  AbsDelay(100);
        DRIVE_ON();
    DRIVE_INIT();
  AbsDelay(100);

 while(1){



    Msg_WriteText("LEVEL");
         Msg_WriteInt(SOUND_LEVEL());
         Msg_WriteChar(13);
         AbsDelay(100);

         Msg_WriteText("lumier_left");
         Msg_WriteInt(LDR_LEFT());
         Msg_WriteChar(13);
         AbsDelay(100);
         Msg_WriteText("lumiere_right");
         Msg_WriteInt(LDR_RIGHT());
         Msg_WriteChar(13);
         AbsDelay(100);



  if(SOUND_LEVEL()>800){
       Msg_WriteText("hellooo");
              Msg_WriteChar(13);
              AbsDelay(100);



    AbsDelay(100);

   count++;
   }
        if(((count % 2)== 0)){
               DRIVE_ON();
              FLL_OFF();
              FLR_ON();

              MOTOR_STOP();
              AbsDelay(100);

         }
             if(((count % 2)!= 0)){
            DRIVE_ON();
             FLR_ON();
             FLL_ON();


             DRIVE(1,1);


             MOTOR_DIR(1,1);

             MOTOR_POWER(240,240);

              AbsDelay(100);


             }


 }

}
