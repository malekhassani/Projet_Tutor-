
 /**
   \file malek.cc
   \brief Programme illustrant l'utilisation du capteur sonore pour faire marcher le robot PRO-BOT 128
   \author HASSANI MALEK & ADLI OUAHIB
   \version 0.1
   \author Email  : malek.1905@live.fr
   \date 12/2015
*/
 int count;

void main(void){
     count = 0;

 PRO_BOT128_INIT();
  AbsDelay(100);

    DRIVE_INIT();
  AbsDelay(100);

 while(1){


         SOUND_LEVEL();
    Msg_WriteText("LEVEL");
         Msg_WriteInt(SOUND_LEVEL());
         Msg_WriteChar(13);
         AbsDelay(100);



  if(SOUND_LEVEL()>650){
       Msg_WriteText("hellooo");
              Msg_WriteChar(13);
              AbsDelay(100);


    DRIVE_ON();
    AbsDelay(100);

   count++;
         if((count % 2)== 0){
              FLL_OFF();
              FLR_ON();

              DRIVE_OFF();
              AbsDelay(1000);
         }else{
             FLR_ON();
             FLL_ON();


             DRIVE(1,1);


             MOTOR_DIR(1,1);

             MOTOR_POWER(240,240);
              AbsDelay(1000);

         }
   }
 }

}

