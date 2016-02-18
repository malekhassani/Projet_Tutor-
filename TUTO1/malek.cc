
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
      go=false;
 PRO_BOT128_INIT();
  AbsDelay(100);

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
         AbsDelay(1000);



  if(SOUND_LEVEL()>650){
       Msg_WriteText("hellooo");
              Msg_WriteChar(13);
              AbsDelay(100);



    AbsDelay(100);
            go=true;
   count++;
   }
         if(((count % 2)== 0)& go){
               DRIVE_ON();
              FLL_OFF();
              FLR_ON();

              DRIVE_OFF();
              AbsDelay(1000);
              go=false;
         }else{
             if(((count % 2)!= 0)){
             DRIVE_ON();
             FLR_ON();
             FLL_ON();


             DRIVE(1,1);


             MOTOR_DIR(1,1);

             MOTOR_POWER(240,240);
              if(go){
              AbsDelay(1000);
              go=false;
              }

              }
         }

 }

}

