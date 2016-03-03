void main(void)
{

      PRO_BOT128_INIT();    //PRO-BOT128 Setup
      ACS_INIT(2);          //ACS setup / sensitivity 1 To 20 / 1=near / 20 =far
      AbsDelay(1000);       //Wait 1Sec.
      ENC_LED_ON();         //Encoder IR-LEDs "ON"
      DRIVE_INIT();
      DRIVE_ON();           //Motor "ON"
      while(1)                    //Endless Loop
      {
        //Drive forward
        if ((ACS_LEFT() == 1) && (ACS_RIGHT() == 1)){
        Msg_WriteText("avant");
        Msg_WriteChar(13);
       // AbsDelay(500);
        FLL_ON();            //Front LED left "OFF"
        FLR_ON();            //Front LED right "OFF"
        BLL_OFF();          //Front LED left "OFF"
        BLR_OFF();          //Front LED right "OFF"
        //DRIVE_FORWARD(10);   //Drive forward, speed 1 To 10 : Value = 7
        //GO_TURN(150,0,150);
        //DRIVE (gauche en byte, droite en byte)
        DRIVE(1,1);
        MOTOR_DIR (1,1);//le sens de marche
        MOTOR_POWER (250,250); //vitesse
        //DELAY_MS(150);      //Wait 150ms

        }

        //Drive backward
        if ((ACS_LEFT() == 0) && (ACS_RIGHT() == 0)){
        Msg_WriteText("ariere");
        Msg_WriteChar(13);
        //AbsDelay(500);
        BLL_ON();           //Front LED left "ON"
        BLR_ON();           //Front LED right "ON"
        FLL_OFF();          //Front LED left "OFF"
        FLR_OFF();
        //GO_TURN(-15,0,150); //Backward 15cm, Speed = 150
        DRIVE(0,0);
        MOTOR_DIR (0,0);//le sens de marche
        MOTOR_POWER (250,250); //vitesse
        DELAY_MS(2000);
        GO_TURN(0,60,150);  //Turn right, speed = 150


                                                   }

        //Turn left
        if ((ACS_LEFT() == 1) && (ACS_RIGHT() == 0)){
        Msg_WriteText("gauche");
        Msg_WriteChar(13);
        // AbsDelay(500);
        FLL_ON();          //Front LED left "OFF"
        FLR_OFF();
        BLL_OFF();          //Front LED left "OFF"
        BLR_OFF();           //Front LED right "ON"
        GO_TURN(0,-45,150); //Turn left, Speed = 150

                                                     }


        //Turn right
        if ((ACS_LEFT() == 0) && (ACS_RIGHT() == 1)){
        Msg_WriteText("droite");
        Msg_WriteChar(13);
        // AbsDelay(500);
        FLL_OFF();           //Front LED left "ON"
        FLR_ON();
        BLL_OFF();          //Front LED left "OFF"
        BLR_OFF();           //Front LED right "OFF"
        GO_TURN(0,45,150);  //Turn right, Speed = 150

                                                     }

    }

}


