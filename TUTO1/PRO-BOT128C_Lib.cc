
   /**
   \file PRO-BOT128C_Lib.cc
   \brief la bibliotheque utiliser
   \author Conrad

*/

#define LF 0x0A
#define CR 0x0D
#define SPACE 0x20

#define SCL 24
#define SDA 25

#define ON 0
#define OFF 1
#define OUT 1
#define IN 0

#define FLL 19
#define FLR 18
#define BLL 17
#define BLR 16
#define LineLED 20
#define Button 36

#define Line_LED 20

#define Motor_Enable 15

#define Encoder_IR 8

#define PWM_IR 35
#define IR_left 27
#define IR_right 29
#define TSOP 26

//1. Oktave
#define Tone_C1 440
#define Tone_Cis1 413
#define Tone_D1 392
#define Tone_Dis1 371
#define Tone_E1 350
#define Tone_F1 330
#define Tone_Fis1 312
#define Tone_G1 294
#define Tone_Gis1 277
#define Tone_A1 262
#define Tone_Ais1 247
#define Tone_H1 234

//2. Oktave
#define Tone_C2 220
#define Tone_Cis2 208
#define Tone_D2 196
#define Tone_Dis2 185
#define Tone_E2 175
#define Tone_F2 165
#define Tone_Fis2 156
#define Tone_G2 147
#define Tone_Gis2 138
#define Tone_A2 131
#define Tone_Ais2 125
#define Tone_H2 117

#define UNIT_TICK 3
#define SPEED_INTEGRATOR 2

int ENCODER_RIGHT, ENCODER_LEFT;
int ENCODER_LEFT_CNT, ENCODER_RIGHT_CNT;
int ODO_LEFT, ODO_RIGHT;
int CNT1;
int SW2;

byte left_dir, right_dir;

int POWER_LEFT, POWER_RIGHT, MOTOR_L, MOTOR_R;
int ERROR, ERROR_LAST, Direction, SPEED;

#define Start_Power_Forward 240
#define I_up 2
#define I_down 2

//---[ System Init ]------------------------------------------------------------
 /**
   \fn   PRO_BOT128_INIT()
   \brief Initialise le hardware de base du PRO-BOT12, toujours commencer pars cette fonction
  */
void PRO_BOT128_INIT(void)
{
    SPI_Disable();

    //IR LED Encoder
    Port_DataDirBit(Encoder_IR,1);

    //Line LED
    Port_DataDirBit(Line_LED,PORT_OUT);

    //Status LEDs
    Port_DataDirBit(FLL,PORT_OUT);
    Port_DataDirBit(FLR,PORT_OUT);
    Port_DataDirBit(BLL,PORT_OUT);
    Port_DataDirBit(BLR,PORT_OUT);

    //ACS System
    Port_DataDirBit(IR_left,PORT_OUT);
    Port_DataDirBit(IR_right,PORT_OUT);
    Port_DataDirBit(TSOP,PORT_IN);

    //Radencoder Interrupts
    Irq_SetVect(INT_6,ENC_RIGHT_ISR);
    Irq_SetVect(INT_7,ENC_LEFT_ISR);

    Ext_IntEnable(6,1);
    Ext_IntEnable(7,1);

    //Button SW2 Interrupt
    Irq_SetVect(INT_4,BUTTON);
    Ext_IntEnable(4,2);

    //Timer 2 IRQ
    Irq_SetVect(INT_TIM2COMP,SYSTEM_CNT);

    //I2C BUS
    I2C_Init(I2C_100kHz);

    //Init
    FLL_OFF();
    FLR_OFF();
    BLL_OFF();
    BLR_OFF();
    LINE_LED_OFF();
    ENC_LED_OFF();
    DRIVE_INIT();

    //Button SW2 Init
    SW2=0;

}

//---[ Button SW2 ]-------------------------------------------------------------
byte BUTTON(void)
{
    if (SW2==0)
      {
       SW2=1;
      }
     else SW2=0;

    AbsDelay(100);
    Irq_GetCount(INT_4);
    return SW2;
}

//---[ System Counter 10ms ]----------------------------------------------------
void SYSTEM_CNT(void)
{
    int irqcnt_sys;
    CNT1++;

    if (CNT1>=10) //100ms
      {
        CNT1=0;
        ENCODER_LEFT=ENCODER_LEFT_CNT;
        ENCODER_RIGHT=ENCODER_RIGHT_CNT;
        ENCODER_LEFT_CNT=0;
        ENCODER_RIGHT_CNT=0;

       //Forward syncron drive
       if (Direction==1 && SPEED>0)
         {

           //*********************************************************
           //*** I Simple controls for the wheel constant velocity ***
           //*********************************************************

          //Who is faster?
          if (ENCODER_LEFT<SPEED)
            {
             POWER_LEFT=POWER_LEFT+I_up;
            }

          if (ENCODER_LEFT>SPEED)
            {
             POWER_LEFT=POWER_LEFT-I_down;
            }

          if (ENCODER_RIGHT<SPEED)
            {
             POWER_RIGHT=POWER_RIGHT+I_up;
            }

          if (ENCODER_RIGHT>SPEED)
            {
             POWER_RIGHT=POWER_RIGHT-I_down;
            }

          //Error between the two wheels calculate
          ERROR=(ENCODER_LEFT-ENCODER_RIGHT);
          ERROR=ERROR+ERROR_LAST;

          if (ERROR>128)
            {
             ERROR=128;
            }

          if (ERROR<-128)
            {
             ERROR=-128;
            }

          MOTOR_L=Start_Power_Forward+POWER_LEFT-ERROR;
          MOTOR_R=Start_Power_Forward+POWER_RIGHT+ERROR;

          //Limit
          if (MOTOR_L>255)
            {
             MOTOR_L=255;
            }

          if (MOTOR_R>255)
            {
             MOTOR_R=255;
            }

          DRIVE(MOTOR_L,MOTOR_R);
          ERROR_LAST=ERROR;

        }
   }

   irqcnt_sys=Irq_GetCount(INT_TIM2COMP);
}

//---[ Status LED "FRONT LED LEFT" ON ]-----------------------------------------
void FLL_ON(void)
{
    Port_WriteBit(FLL,PORT_ON);
}

//---[ Status LED "FRONT LED LEFT" OFF ]----------------------------------------
void FLL_OFF(void)
{
    Port_WriteBit(FLL,PORT_OFF);
}

//---[ Status LED "FRONT LED RIGHT" ON ]----------------------------------------
void FLR_ON(void)
{
    Port_WriteBit(FLR,PORT_ON);
}

//---[ Status LED "FRONT LED RIGHT" OFF ]---------------------------------------
void FLR_OFF(void)
{
    Port_WriteBit(FLR,PORT_OFF);
}

//---[ Status LED "BACK LED LEFT" ON ]------------------------------------------
void BLL_ON(void)
{
    Port_WriteBit(BLL,PORT_ON);
}

//---[ Status LED "BACK LED LEFT" OFF ]-----------------------------------------
void BLL_OFF(void)
{
    Port_WriteBit(BLL,PORT_OFF);
}

//---[ Status LED "BACK LED RIGHT" ON ]-----------------------------------------
void BLR_ON(void)
{
    Port_WriteBit(BLR,PORT_ON);
}

//---[ Status LED "BACK LED RIGHT" OFF ]----------------------------------------
void BLR_OFF(void)
{
    Port_WriteBit(BLR,PORT_OFF);
}

//---[ Interrupt service routine stimulus right wheel ]-------------------------
void ENC_RIGHT_ISR(void)
{
    int irqreq_r;
    ENCODER_RIGHT_CNT++;             //Increment counter right wheel
    ODO_RIGHT++;
    irqreq_r=Irq_GetCount(INT_6);    //Interrupt Request Counter = 0
}

//---[ Interrupt service routine stimulus leftt wheel ]-------------------------
void ENC_LEFT_ISR(void)
{
    int irqreq_l;
    ENCODER_LEFT_CNT++;               //Increment counter left wheel
    ODO_LEFT++;
    irqreq_l=Irq_GetCount(INT_7);     //Interrupt Request Counter = 0
}

//---[ Odometer Reset ]---------------------------------------------------------
void ODO_RESET(void)
{
    ODO_LEFT=0;
    ODO_RIGHT=0;
}

//---[ IR Encoder LED ON ]------------------------------------------------------
void ENC_LED_ON(void)
{
    Port_WriteBit(Encoder_IR,PORT_OFF);  //PORT_OFF = IR -> Ein
}

//---[ IR Encoder LED OFF ]-----------------------------------------------------
void ENC_LED_OFF(void)
{
    Port_WriteBit(Encoder_IR,PORT_ON);
}

//---[ BEEP ]-------------------------------------------------------------------
void BEEP(word Tone, word Periode)
{
    Timer_T0FRQ(Tone,PS_64);
    AbsDelay(Periode);
    Timer_T0Stop();
}

//---[ Sound level ]------------------------------------------------------------

            /**
          \fn SOUND_LEVEL()
           \brief recupérer le niveau du bruit dans l'envirenement
           \return une valeur entre 0 à 1023
         */

word SOUND_LEVEL(void)
{
    word Level;
    Level=GET_ADC(3);
    return Level;
}

//---[ LDR level left ]---------------------------------------------------------
word LDR_LEFT(void)
{
    word Level;
    Level=GET_ADC(4);
    return Level;
}

//---[ LDR level right ]--------------------------------------------------------
word LDR_RIGHT(void)
{
    word Level;
    Level=GET_ADC(5);
    return Level;
}

//---[ Line Sensor left read ]--------------------------------------------------
word READ_LINE_LEFT(void)
{
    word Level;
    Level=GET_ADC(1);
    return Level;
}

//---[ Line Sensor right read ]-------------------------------------------------
word READ_LINE_RIGHT(void)
{
    word Level;
    Level=GET_ADC(2);
    return Level;
}

//---[ Line Sensor LED ON ]-----------------------------------------------------
void LINE_LED_ON(void)
{
    Port_WriteBit(Line_LED,PORT_ON);
}

//---[ Line Sensor LED OFF ]----------------------------------------------------
void LINE_LED_OFF(void)
{
    Port_WriteBit(Line_LED,PORT_OFF);
}

//---[ Akku Voltage ]-----------------------------------------------------------
float AKKU_SPG(void)
{
    float Volt, Ref;
    word ADC_RAW;               //Variable for ADC values from 0 To 1023
    Ref=2.56/1023.0;            //Resolution calculate 5mV per ADC-Step
    ADC_RAW=GET_ADC(0);         //From ADC(0) collect data
    Volt=ADC_RAW*Ref;           //Calculate Volts
    Volt=Volt*2.08;             //Voltage divider ratio multiply
    return Volt;
}

//---[ ADC Channel read ]-------------------------------------------------------
word GET_ADC(byte Channel)
{
    //Uref 2,56V internal ref.
    ADC_Set(ADC_VREF_BG,Channel);
    return ADC_Read();
}

//---[ Drive Setup ]------------------------------------------------------------

        /**
          \fn DRIVE_INIT()
           \brief Initialise le moteur
         */

void DRIVE_INIT(void)
{
    Port_DataDirBit(Motor_Enable,PORT_OUT);  //Port Enable Motor = Output
    DRIVE_OFF();
    Timer_T1PWMX(256,128,128,PS_8);          //Config for channel A und B Timer1
    Timer_T1PWA(128);                        //Pulse/break = 50/50 channel-A
    Timer_T1PWB(128);                        //Pulse/break = 50/50 channel-B
}

//---[ Drive ON ]---------------------------------------------------------------

       /**
          \fn DRIVE_ON()
           \brief Activer les moteurs
         */
void DRIVE_ON(void)
{
    Port_WriteBit(Motor_Enable,1);           //Port = Low
}

//---[ Drive OFF ]--------------------------------------------------------------
/**
          \fn DRIVE_OFF()
           \brief Arrêter les moteurs
         */
void DRIVE_OFF(void)
{
    Port_WriteBit(Motor_Enable,0);           //Port = High +5V
}

//---[ Motor power, left : right ]----------------------------------------------

          /**
          \fn DRIVE()
           \brief choisir le sens de rotation des moteurs
           \param type:byte, Valeurs entre 1 à 128 = recul, Valeurs entre 128 à 255 = marche avant, Valeur 128 = arrêt moteur

         */
void DRIVE(byte left, byte right)
{
    Timer_T1PWA(left);
    Timer_T1PWB(right);
}

//---[ Motor power left And right ]---------------------------------------------

/**
          \fn MOTOR_POWER()
           \brief choisir la Vitesse du moteur
           \param type:byte, Valeurs entre 1 à 255

         */
void MOTOR_POWER(byte left, byte right)
{
    Direction=255;
    MOTOR_L=128;
    MOTOR_R=128;
    ERROR=0;
    ERROR_LAST=0;

    //Left motor 0 To 255
    if (left_dir==1)
      {
       left=left/2;
       if (left>=128)
         {
           left=128;
         }
         Timer_T1PWA(128+left);
       }

    if (left_dir==0)
      {
       left=left/2;
       if (left>=128)
         {
           left=128;
         }
         Timer_T1PWA(128-left);
       }

    //break
    if (left_dir==255)
      {
       Timer_T1PWA(128);
      }

    //Right motor 0 To 255
    if (right_dir==1)
      {
       right=right/2;
       if (right>=128)
         {
          right=128;
         }
         Timer_T1PWB(128+right);
      }

    if (right_dir==0)
      {
       right=right/2;
       if (right>=128)
         {
          right=128;
         }
         Timer_T1PWB(128-right);
      }

    //break
    if (right_dir==255)
      {
       Timer_T1PWB(128);
      }
   }

//---[ Motor direction 1=FWD ; 0=RWD ; 255=break ]------------------------------

 /**
          \fn MOTOR_DIR()
           \brief choisir la derection de robot
           \param type:boolean, Valeurs 0 = recul, Valeurs 1 = marche avant

         */

void MOTOR_DIR(byte left, byte right)
{
    left_dir=left;
    right_dir=right;
}

//---[ Motor break ]------------------------------------------------------------
void MOTOR_STOP(void)
{
    Timer_T1PWA(128);
    Timer_T1PWB(128);
}

//---[ Drive regulated forward ]------------------------------------------------
void DRIVE_FORWARD(byte speed)
{
    //Speed 1 To 10
    Direction=1;
    SPEED=speed;
}

//---[ GO_TURN(Distance "cm", Degree "Grad", Speed 1 To 255) ]------------------
void GO_TURN(int distance, int degree, byte speed)
{
    int encoder_cnt, drive_cnt, diff, speed_left, speed_right;

    //Distance: postiv=go forward / negativ=go backward / zero=use degree for turn
    //Degree  : postiv=turn right / negativ=turn left
    //Speed   : sets motorspeed

    drive_cnt=0;
    encoder_cnt=0;
    diff=0;
    speed_left=speed;
    speed_right=speed;

    //Stop the robot
    MOTOR_STOP();

    //if distance is Not zero, Then take this value To go ...
    if (distance!=0)
      {
        //Calculate ticks "cm"
        encoder_cnt=ABS_INT((distance-3))*UNIT_TICK;
        if (distance<0)
          {
           //Backward
           MOTOR_DIR(0,0);
          } //Forward
         else MOTOR_DIR(1,1);
       }
      else
       {
        //calculate degree
        encoder_cnt=ABS_INT(degree)*10;
        encoder_cnt=(encoder_cnt/37)-3;
        if (degree<0)
          {
           //Turn left
           MOTOR_DIR(0,1);
          } //Turn right
         else MOTOR_DIR(1,0);
        }

    //Reset encoder
    ODO_RESET();

    //Now move the bot
    MOTOR_POWER(speed_left,speed_right);

    do
      {
        drive_cnt=drive_cnt+((ODO_LEFT+ODO_RIGHT)/2);
        diff=ENCODER_LEFT-ENCODER_RIGHT;

        if (diff>0)
          {
           //Left faster than right
          if ((speed_left>speed) | (speed_right>244))
            {
             speed_left=speed_left-SPEED_INTEGRATOR;
            }
          else speed_right=speed_right+SPEED_INTEGRATOR;
          }

        if (diff<0)
          {
           //Right faster than left
          if ((speed_right>speed) | (speed_left>244))
            {
             speed_right=speed_right-SPEED_INTEGRATOR;
            }
          else speed_left=speed_left+SPEED_INTEGRATOR;
          }

        //Reset encoder
        ODO_RESET();

        //Set Motorpower
        MOTOR_POWER(speed_left,speed_right);

        //Speed regulator delay
        DELAY_MS(25);

    } while (drive_cnt<encoder_cnt);


    //Stop motors And jump back after litle delay
    MOTOR_STOP();
    DELAY_MS(500);
}


//---[ ACS System ]-------------------------------------------------------------
void ACS_INIT(byte sensitivity)
{
    //Calculating the pulse width modulation
    //Timer_T3PWM(Par1,Par2,PS);
    //Period=Par1*PS/FOSC (51*8/14,7456MHz=27,66 µs)  = 36Khz
    //Pulse=Par2*PS/FOSC (25*8/14,7456MHz=13,56 µs) On Time

    //Timer_T3PWM(word period,word PW0,byte PS)   '36Khz
    Timer_T3PWM(51,sensitivity,PS_8);   //with Par1, Par2 can reach altered!
                                        //Responding To the ACS must be sensitive To these parameters are screwed!
}

//---[ Read ACS Sensor right ]--------------------------------------------------
byte ACS_RIGHT(void)
{
    byte ACS_Right;
    Port_WriteBit(IR_left,PORT_OFF);
    Port_WriteBit(IR_right,PORT_ON);
    AbsDelay(5);
    ACS_Right=Port_ReadBit(TSOP);
    Port_WriteBit(IR_right,PORT_OFF);
    return ACS_Right;
}

//---[ Read ACS Sensor left ]---------------------------------------------------
byte ACS_LEFT(void)
{
    byte ACS_Left;
    Port_WriteBit(IR_right,PORT_OFF);
    Port_WriteBit(IR_left,PORT_ON);
    AbsDelay(5);
    ACS_Left=Port_ReadBit(TSOP);
    Port_WriteBit(IR_left,PORT_OFF);
    return ACS_Left;
}

//---[ Delay_ms ]---------------------------------------------------------------
void DELAY_MS(int time)
{
    int x;
    for(x=0;x<=time;x++)
      {
       AbsDelay(1);
      }
}

//---[ Grad To Rad ]------------------------------------------------------------
float GRAD_To_RAD(float Val)
{
    float X;
    //Calculate Grad To Rad
    X=Val*(PI/180.0);
    return X;
}

//---[ Rad To Grad ]------------------------------------------------------------
float RAD_To_GRAD(float Val)
{
    float X;
    //Calculate Rad To Grad
    X=Val*(180.0/PI);
    return X;
}

//---[ Make 2Byte To word ]-----------------------------------------------------
word MAKE_INT(byte a, byte b)
{
    word val;
    val=(a*256)+b;
    return val;
}

//---[ Absolte Integer ]--------------------------------------------------------
int ABS_INT(int Val)
{
   int X_Save;
   if (Val<-0)
     {
      X_Save=Val;
      Val=X_Save-Val;
      Val=Val-X_Save;
      return Val;
      }
     else return Val;
}

//---[ Absolte Single ]---------------------------------------------------------
float ABS_SINGLE(float Val)
{
   float X_Save;
   if (Val<-0.0)
     {
      X_Save=Val;
      Val=X_Save-Val;
      Val=Val-X_Save;
      return Val;
     }
     else return Val;
}


