
 /**
   \file malek.cc
   \brief Programme illustrant l'utilisation des capteurs lumineux pour faire marcher le robot PRO-BOT 128
   \author HASSANI MALEK & ADLI OUAHIB
   \version 0.1
   \author Email  : malek.1905@live.fr
   \date 02/2016
*/

void main(void){

    PRO_BOT128_INIT();      //PRO-BOT128 System Setup
    DRIVE_INIT();           //Drive setup
    ENC_LED_ON();           //Encoder IR-LEDs "ON"
    DRIVE_ON();             //Drive "ON"

    do                      //Endless Loop
     {
       Start:               //Label Start

       //Drive forward 30cm, Speed = 150 (1 To 255)
       // GO_TURN(10,0,255);

       /****************************************
        *** LDR check where it is brighter?  ***
        ***************************************/
        Msg_WriteText("capteur droit");
        Msg_WriteWord(LDR_RIGHT());
        Msg_WriteChar(13);

        Msg_WriteText("capteur gauche");
        Msg_WriteWord(LDR_LEFT());
         Msg_WriteChar(13);
         AbsDelay(500);
       //The left is brighter than it is right...
        if ((LDR_RIGHT()|LDR_LEFT())< 80)
         {
       if (LDR_RIGHT() > LDR_LEFT())
         {

             GO_TURN(0,-35,250);
             GO_TURN(20,0,255);
             goto Start;

          }


       //Right, it is brighter than the left...
       if (LDR_LEFT() > LDR_RIGHT())
         {

             GO_TURN(0,35,250);
              GO_TURN(20,0,255);
               goto Start;

          }
        }


     } while (1);

}
