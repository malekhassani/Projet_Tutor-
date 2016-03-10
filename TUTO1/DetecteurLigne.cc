#define HIGHSPEED 255
#define MIDDLESPEED 220
#define LOWSPEED 100
#define WAIT1 50

int Line_left_value;
int Line_right_value;

void main(void)
{
    PRO_BOT128_INIT();
    ACS_INIT(2);
    AbsDelay(1000);
    ENC_LED_ON();
    Line_left_value = 0;
    Line_right_value = 0;
    LINE_LED_ON();
    DRIVE_ON();
    MOTOR_DIR(1,1);
    do
     {
     Line_left_value = READ_LINE_LEFT();
     Line_right_value = READ_LINE_RIGHT();
     if (Line_left_value > Line_right_value){
        if ((ACS_LEFT() == 1) && (ACS_RIGHT() == 1)){
             MOTOR_POWER(LOWSPEED, MIDDLESPEED );
             BLR_ON();
             DELAY_MS(WAIT1);
                                                    }
                                            }
        else{
             MOTOR_POWER(MIDDLESPEED, LOWSPEED);
             BLL_OFF();
             DELAY_MS(WAIT1);
            }

     }while (1);

}

