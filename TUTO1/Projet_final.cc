
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
 int Line_left_value;
int Line_right_value;
 //thread 1 stop
 #thread 1,128,10
 //thread 2 marche
 #thread 2,128,10
 //thread 0 obstacle
 #thread 0,128,10
  //thread 3 lignes
 #thread 3,128,10
 //thread 4 lumineux
 #thread 4,128,10




 void marche(void){

       Msg_WriteText("marche");
        Msg_WriteChar(13) ;
       DRIVE_ON();
       FLR_ON();
       FLL_ON();
       DRIVE(1,1);
       MOTOR_DIR(1,1);
       MOTOR_POWER(240,240);
       AbsDelay(100);

 }

 void stop(void){

      Msg_WriteText("stop");
        Msg_WriteChar(13);
      DRIVE_ON();
      FLL_OFF();
      FLR_ON();
      MOTOR_STOP();
      AbsDelay(100);
 }
 void capteurs_obstacle(void){
  Msg_WriteText("capteurs_obstacle");
        Msg_WriteChar(13) ;

 ACS_INIT(2);
   ENC_LED_ON();         //Encoder IR-LEDs "ON"
    do
      {

        if ((ACS_LEFT() == 1) && (ACS_RIGHT() == 1)){
        Forward();
        Msg_WriteText("avant");
        Msg_WriteChar(13);
        AbsDelay(500);
        }
        if ((ACS_LEFT() == 0) && (ACS_RIGHT() == 0)){
        Backward();
        Msg_WriteText("ariere");
        Msg_WriteChar(13);
        AbsDelay(500);
       }
        if ((ACS_LEFT() == 1) && (ACS_RIGHT() == 0)){
        Turn_Left();
        Msg_WriteText("gauche");
        Msg_WriteChar(13);
        AbsDelay(500);
       }
        if ((ACS_LEFT() == 0) && (ACS_RIGHT() == 1)){
        Turn_Right();
        Msg_WriteText("droite");
        Msg_WriteChar(13);
        AbsDelay(500);
         }

    } while (1);
 }




 void Forward(void)      //Marche Avant
{
    FLL_ON();
    FLR_ON();
    BLL_OFF();
    BLR_OFF();
    DRIVE_FORWARD(7);
    DELAY_MS(150);
}

void Backward(void)     //Marche Arriere
{
    BLL_ON();
    BLR_ON();
    FLL_OFF();
    FLR_OFF();
    GO_TURN(-15,0,150);
    GO_TURN(0,60,150);
}

void Turn_Left(void)    //Tourner à gauche
{
    FLL_ON();
    FLR_OFF();
    BLL_OFF();
    BLR_OFF();
    GO_TURN(0,-45,150);
}

void Turn_Right(void)   //Tourner à droite
{
    FLL_OFF();
    FLR_ON();
    BLL_OFF();
    BLR_OFF();
    GO_TURN(0,45,150);
}
 void capteurs_lignes(void){
 Msg_WriteText("capteurs_lignes");
        Msg_WriteChar(13) ;
     //PRO_BOT128_INIT();
    Line_left_value = 0;
    Line_right_value = 0;
    LINE_LED_ON();
    DRIVE_ON();
    MOTOR_DIR(1,1);

    do
     {
     Line_left_value = READ_LINE_LEFT();
     Line_right_value = READ_LINE_RIGHT();


      if (Line_left_value > Line_right_value)
        {
        MOTOR_POWER(120, 180);
        BLR_ON();
        DELAY_MS(50);
        }
        else
        {
        MOTOR_POWER(180, 120);
        BLL_OFF();
        DELAY_MS(50);
        }

     }while (1);

 }


 void  capteurs_lumineux (void){

 Msg_WriteText("capteurs_lignes");

 ENC_LED_ON();           //Encoder IR-LEDs "ON"

    do
     {
       Start:

        Msg_WriteText("Capteur droit");
        Msg_WriteWord(LDR_RIGHT());
        Msg_WriteChar(13);
        Msg_WriteText("Capteur gauche");
        Msg_WriteWord(LDR_LEFT());
        Msg_WriteChar(13);
         AbsDelay(500);

        if ((LDR_RIGHT()|LDR_LEFT())< 80)
         {
       if (LDR_RIGHT() > LDR_LEFT())
         {

             GO_TURN(0,-35,250);
             GO_TURN(20,0,255);
             goto Start;

          }

       if (LDR_LEFT() > LDR_RIGHT())
         {

             GO_TURN(0,35,250);
              GO_TURN(20,0,255);
               goto Start;

          }
        }


     } while (1);
 }




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

        /* Msg_WriteText("lumier_left");
         Msg_WriteInt(LDR_LEFT());
         Msg_WriteChar(13);
         AbsDelay(100);
         Msg_WriteText("lumiere_right");
         Msg_WriteInt(LDR_RIGHT());
         Msg_WriteChar(13);
         AbsDelay(100);  */



  if(SOUND_LEVEL()>700){
        Msg_WriteText("hellooo");
        Msg_WriteChar(13);
        //BEEP(262, 10) ;
        AbsDelay(100);
        AbsDelay(100);

   count++;
   if(((count % 2)== 0)){
        Thread_Start(1,capteurs_lumineux);
        Thread_Wait(2,2);
               //stop();

         }
            if(((count % 2)!= 0)){
                Thread_Start(2,capteurs_lignes);
                Thread_Wait(1,1);
                 //marche();

             }
   }



 }

}
