
void main(void)
{

    PRO_BOT128_INIT();    //PRO-BOT128 Setup
    ACS_INIT(2);          //ACS setup / sensitivity 1 To 20 / 1=near / 20 =far

    AbsDelay(1000);       //Wait 1Sec.

    ENC_LED_ON();         //Encoder IR-LEDs "ON"
    DRIVE_ON();           //Motor "ON"

    do                    //Endless Loop
      {
        //Drive behaviour
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


void Forward(void)      //Drive forward
{
    FLL_ON();          //Front LED left "OFF"
    FLR_ON();          //Front LED right "OFF"
    BLL_OFF();          //Front LED left "OFF"
    BLR_OFF();          //Front LED right "OFF"
    DRIVE_FORWARD(7);   //Drive forward, speed 1 To 10 : Value = 7
    DELAY_MS(150);      //Wait 150ms
}

void Backward(void)     //Drive backward
{
    BLL_ON();           //Front LED left "ON"
    BLR_ON();           //Front LED right "ON"
    FLL_OFF();          //Front LED left "OFF"
    FLR_OFF();
    GO_TURN(-15,0,150); //Backward 15cm, Speed = 150
    GO_TURN(0,60,150);  //Turn right, speed = 150
}

void Turn_Left(void)    //Turn left
{
    FLL_ON();          //Front LED left "OFF"
    FLR_OFF();
    BLL_OFF();          //Front LED left "OFF"
    BLR_OFF();           //Front LED right "ON"
    GO_TURN(0,-45,150); //Turn left, Speed = 150

}

void Turn_Right(void)   //Turn right
{
    FLL_OFF();           //Front LED left "ON"
    FLR_ON();
    BLL_OFF();          //Front LED left "OFF"
    BLR_OFF();           //Front LED right "OFF"
    GO_TURN(0,45,150);  //Turn right, Speed = 150
}
