//Chatpad from Xbox 360 alternative firmware
//Vasco Santos 16/04/2021

//P18F26K20 set to 8MHZ Clock
//P16F883 set to 4MHZ Clock

//P18F26K20
#define T0_COUNTER (0xffff - 1000 - 12 )   // (8MHz / 4 ) / 2 => 1us ... 1000x => 1ms

//eeprom config addresses
#define CONFIG_LIGHTMODE 3
#define CONFIG_BAUDRATE 1

//suported BaudRates
#define BAUDRATE_300    1
#define BAUDRATE_1200   2
#define BAUDRATE_2400   3
#define BAUDRATE_4800   4
#define BAUDRATE_9600   5
#define BAUDRATE_19200  6
#define BAUDRATE_38400  7
#define BAUDRATE_57600  8
#define BAUDRATE_115200 9

//current version
#define VER "1.0"

const char scanMap[50] = {      // i could output directly idx,
  8,  9, 10, 40, 20, 30, 47,    // but maybe a translation map
 41, 42, 43, 39, 44, 38, 45,    // can prove useful in future.
 31, 32, 33, 35, 34, 36, 37,    // starts from 1...think 0
 21, 22, 23, 25, 24, 26, 27,    // should be reserved for
 11, 12, 13, 15, 14, 16, 17,    // future use
  1,  2,  3,  5,  4,  6,  7,
 28, 18, 19,  0,  0, 29, 46 };

const char serialMap[4][49] = { // CHARACTER MAPPING MATRIX for "pure serial mode"
// normal char press
{56,  57,  48,  13, 112,  44,   0, //8 9 0 enter p , orange bnt
 0,   0,   17,  46,  32, 109,  16,  //green bnt group bnt left . space m right
 0, 122, 120, 118,  99,  98, 110,  //shift z x v d b n
97, 115, 100, 103, 102, 104, 106,
113, 119, 101, 116, 114, 121, 117,
 49,  50,  51,  53,  52,  54,  55,
107, 105, 111,   0,   0, 108,   8},

// shifted
{56,  57,  48,  13,  80,  44,   0,
  0,   0,   0,  46,  32,  77,   0,
  0,  90,  88,  86,  67,  66,  78,
 65,  83,  68,  71,  70,  72,  74,
 81,  87,  69,  84,  82,  89,  85,
 49,  50,  51,  53,  52,  54,  55,
 75,  73,  79,   0,   0,  76,   8},

// green alt
{56,  57,  48,  13,  41,  58,   0,
  0,   0,   0,  63,  32,  62,   0,
  0,  96,  39,  45, 187, 124,  60, //0 `
126, 154, 123, 168, 125,  47,  39,
 33,  64, 128,  37,  35,  94,  38,
 49,  50,  51,  53,  52,  54,  55,
 91,  42,  40,   0,   0,  93,   8},

// red alt
{56,  57,  48,  13,  61,  59,   0,
  0,   0,   0, 191,  32, 181,   0,
 // 0, 230, 156,  95, 231,  43, 241, //0 with æ, œ
   0, 27, 9,  95, 231,  43, 241, //0 with ESC TAB
225, 223, 240, 165, 163,  92,  34,
161, 229, 232, 254,  36, 253, 249,  //¡
 49,  50,  51,  53,  52,  54,  55,
169, 236, 242,   0,   0, 248,   8}
};  ////////// END OF MAPPING MATRIX

    unsigned char sc = 0;
    unsigned char idx = 0;
    unsigned char keydata[7] = {0,0,0,0,0,0,0};
    unsigned char oldData[7] = {0,0,0,0,0,0,0};
    unsigned char justPressed[7] = {0,0,0,0,0,0,0};
    unsigned char AMask = 0;
    unsigned char BMask = 0;
    unsigned char newlyPressd = 0;
    unsigned char alts [] = {0, 1, 2, 2, 3, 3, 3, 3};
    unsigned char alt;
    unsigned char somethingChanged = 0;

   unsigned long cnt =0;
   unsigned long previousMillis = 0;
   int somepress = 0;
   
   int caps = 0;
   int orange = 0;
   
    char text[10];
    char XX2[4];
    int bklight = 0;
    int lightStrenght =0;

            
  unsigned long millis()//counter emulate of arduino
  {
       return cnt;
  }
  
  void interrupt()
  {
  
     if (INTCON.RBIF) { // SOMETHING CHANGED ON PORTB...
        //exitPowerSave(); // just wakeup and so some scanning...
        somepress = 1;
        INTCON.RBIF = 0; // clear intr flag
     }

      if(INTCON.TMR0IE && INTCON.TMR0IF) //Check if it is TMR0 Overflow ISR.
    {
      #ifdef P18F26K20
         TMR0H = T0_COUNTER >> 8;  // RE-Load Timer 0 counter - 1st TMR0H
         TMR0L = T0_COUNTER;       // RE-Load Timer 0 counter - 2nd TMR0L
      #else
       TMR1H = T0_COUNTER >> 8;
       TMR1L = T0_COUNTER;
      #endif
         INTCON.TMR0IF=0;

         cnt++;//counter millis()
     }
  }

int serialSend()
{
    unsigned char idx = 0;
    unsigned char sc = 0;
    int ret = 0;
       
    for (AMask=1; AMask<128; AMask<<=1){
      for (BMask=1; BMask<128; BMask<<=1){
        if (justPressed[sc] & BMask){
        
          if (orange == 1)
           {
              orange = 0;
             if (alt == 0 && idx == 14)//shift bnt
             {
                caps = !caps;
             }
           }
        
           if (alt == 0 && idx == 6){//orange bnt
             orange = 1;
            caps = 0;
           }
           
           if (caps == 1 && orange == 0)
              alt = alt+1;//enable caps lock with orange and shift
              
            //#ifdef P18F26K20
            //UART_Write(serialMap[alt][idx]);
            //#else
             UART1_Write(serialMap[alt][idx]);
            //#endif
            

              // char XX2[4];
            //IntToHex((int) idx,XX2);
          // string2hexString(justPressed[0],XX2);
           // UART_Write_text(XX2);
            
           ret = (int) idx;
           }
        idx++;
      }
      sc++;
    }
    alt = alts[keydata[2]&1|((keydata[1]&1)<<1)|((keydata[0]&64)>>4)]; // next scan can use alt retrieved here
    return ret;
  }

  void doScan(){
    unsigned char sc = 0;

    newlyPressd = 0;

    for (AMask=1; AMask<128; AMask<<=1){

       oldData[sc] = keydata[sc]; // save old data

       PORTA = ~AMask; // put a 0 on the line we want to inspect
       keydata[sc] = PORTB;  // read PORTB (pulled up)...if a line goes to 0, bingo!
       keydata[sc] = keydata[sc] | PORTB; // antibounce
       keydata[sc] = keydata[sc] | PORTB; // antibounce
       keydata[sc] = ~keydata[sc]; // return to positive logic

       justPressed[sc] = keydata[sc]&(~oldData[sc]);

       newlyPressd = newlyPressd | justPressed[sc];

       sc++;
    }
    }
     
void Timer0_CTL()
{
#ifdef P18F26K20

     T0CON.T08BIT=0; //O FOR 16 BIT 1 FOR 8 BIT
     T0CON.TMR0ON=1;
     INTCON.TMR0IE=1;//Enable timer0 Interrupt.

    //while(INTCON.TMR0IF==0);// WAIT FOR TIMER0 TF0 TO ROLL OVER

    //INTCON.TMR0IF==0;// CLEAR TF0

    T0CON.T0CS = 0;         // Internal clock => Crystal/4
  T0CON.PSA = 0;          // Prescaler ON
  T0CON.T0PS2 = 0;        // Prescaler = 000 => 1:2
  T0CON.T0PS1 = 0;
  T0CON.T0PS0 = 0;

  // Load Timer 0 counter
  TMR0H = T0_COUNTER >> 8;  // Load Timer 0 counter - 1st TMR0H
  TMR0L = T0_COUNTER;       // Load Timer 0 counter - 2nd TMR0L
       #else
       //pic16f883

 // T1CON.T18BIT=0;
  T1CON.TMR1ON=1;
  INTCON.TMR0IE=1;//Enable timer0 Interrupt.
  T1CON.T0CS = 0;         // Internal clock => Crystal/4
  T1CON.PSA = 0;          // Prescaler ON
  T1CON.T1CKPS1 = 0;  // Prescaler = 000 => 1:2
  T1CON.T1CKPS0 = 0;
  
  // Load Timer 0 counter
  TMR1H = T0_COUNTER >> 8;  // Load Timer 0 counter - 1st TMR0H
  TMR1L = T0_COUNTER;       // Load Timer 0 counter - 2nd TMR0L
       #endif

}
void blink_setting()
{
        PORTC.RC1     = 0;
        PORTC.RC2     = 0;
        Delay_ms(500);
        PORTC.RC1     = 1;
        PORTC.RC2     = 1;
        Delay_ms(500);
}

  void RST_PIC(){
  #ifdef P18F26K20
  asm{reset;}
  #else
  //P16F883
  asm{CLRWDT;}
  #endif
  }
  
void main() {
int bla = 0;
int getbaud = 0;

  OSCCON = 0x67;                    // 01100111 - 0110 stands for 4Mhz internal clock
  
  getbaud = EEprom_read(CONFIG_BAUDRATE);
  
   switch(getbaud){
       case BAUDRATE_300:    UART1_Init(300);
          break;
       case BAUDRATE_1200:  UART1_Init(1200);
          break;
       case BAUDRATE_2400:  UART1_Init(2400);
          break;
       case BAUDRATE_4800:  UART1_Init(4800);
          break;
       case BAUDRATE_9600:  UART1_Init(9600);
          break;
       case BAUDRATE_19200:  UART1_Init(19200);
          break;
       case BAUDRATE_38400:  UART1_Init(38400);
          break;
       case BAUDRATE_57600:  UART1_Init(57600);
          break;
       case BAUDRATE_115200: UART1_Init(115200);
          break;
       default:
              EEprom_write(CONFIG_BAUDRATE, BAUDRATE_4800); // if no config found, set a default 4800
              UART1_Init(4800);
              break;
    }
    

  //UART1_Write_Text("\r\n#Starting...\n\r");
  
  ANSEL  = 0;                       // Configure AN pins as digital I/O
  ANSELH = 0;
  
  PORTA  = 0;                       // init port A
  TRISA  = 0;                       // 1 = input
  
  PORTB  = 0;                  // initialize PORTB
  TRISB  = 255;                // designate PORTB as all input

  PORTC  = 255;                // initialize PORTC
  TRISC  = 0;                // designate PORTB 0-7 as output

  INTCON.RBIE = 0; // disables PORTB on-change interrupt
  INTCON.RBIF = 0; // clear PORTB mismatch values

  IOCB = 127; // pin 0-6 of PORTB configured to raise interrupt when changed
  
  //PIE1 = 32; // RCIE = 1; enables EUSART on-receive interrupt
    
  INTCON.GIE = 1; // Global Interrupt Enable/disable
    
    #ifdef P18F26K20
    INTCON2.RBPU = 0;    //pull-ups enabled on PORTB.
     #else
     OPTION_REG  = 0; //pull-ups enabled on PORTB.
    #endif
       
       //   T0CS_bit = 0;
  
      PORTC = 255; //all off

      PORTC.RC1     = 0;//ON right
      PORTC.RC2     = 0;//ON left
      PORTC.RC3     = 0;//ON  Verde
      PORTC.RC4     = 0;//ON  Laranja
      Delay_ms(50);
      PORTC = 255; //all off
      
       Timer0_CTL();//get millis()
          
         PWM1_Init(50000);
         PWM1_Set_Duty(255);
         PWM1_Start();
         
         PWM2_Init(50000);
         PWM2_Set_Duty(255);
         PWM2_Start();

        if (EEprom_read(CONFIG_LIGHTMODE) == 1)//read backlight state from eeprom
            bklight = 1;
            
       Delay_ms(500);
       
         doScan(); //just needed to clean some inicial serial garbage and read/write values
          if (justPressed[5]&1){ // 1 pressed at start - change baudrate serial port, one at time
             int setb = EEprom_read(CONFIG_BAUDRATE)+1;
             if (setb == 10)
                setb = 0;
             EEprom_write(CONFIG_BAUDRATE,setb);
             blink_setting();
             Delay_ms(1000);
                RST_PIC();
          }
          
          if (justPressed[5]&2){ // 2 pressed at start write about
            UART1_Write_Text("\r\n* Starting Chatpad...");
            UART1_Write_Text(VER);
            UART1_Write_Text(" VS2021 *\r\n");
          }
          
          if (justPressed[5]&4){ // 3
          }
          if (justPressed[5]&16){ // 4
          }
          if (justPressed[5]&8){ // 5
          }
          if (justPressed[5]&32){ // 6
          }
          if (justPressed[5]&64){ // 7
          }
          if (justPressed[0]&1){ // 8
          }
          if (justPressed[0]&2){ // 9
          }
          if (justPressed[0]&4){ // 0 -reset to default
             EEprom_write(CONFIG_BAUDRATE, BAUDRATE_4800);
             EEprom_write(CONFIG_LIGHTMODE, 0);
             blink_setting();
             RST_PIC();
          }
          
  do {

   unsigned long currentMillis = millis();

    if (bklight == 1)//fade only if enabled
    {
      if (currentMillis - previousMillis >= 500) {
         previousMillis = currentMillis;

               lightStrenght=lightStrenght+10;

                  if (lightStrenght > 255)//all lights off
                  {
                    lightStrenght = 255;
                    PORTC.RC3     = 1;//OFF
                    PORTC.RC4     = 1;//OFF
                  }
                  
                  PWM1_Set_Duty(lightStrenght);
                  PWM2_Set_Duty(lightStrenght);

      // sprintf(text,"%lu\r\n",millis());

        //UART_Write_text(text);
      }
   }
   
  doScan();

  bla = serialSend();
  
  if (newlyPressd){ // 1

  //UART_Write_text("KEY:")  ;
                 if( bklight == 1)//backlight reset on key press
                 {
                      PWM1_Set_Duty(0);
                      PWM2_Set_Duty(0);
                      lightStrenght = 0;
                 }
                 IntToHex(bla,XX2);
                     
          // sprintf(XX,"%d %d %d %d %d %d",justPressed[0],justPressed[1],justPressed[2],justPressed[3],justPressed[4],justPressed[5]);
           //sprintf(XX2,"%d",justPressed[0]);
           //strcpy(XX2,(int)justPressed[0]);

           //IntToStr((int)justPressed[0],XX2);

          // string2hexString(justPressed[0],XX2);
            ///UART_Write_text(XX2);
            ///UART_Write_text("\r\n");

                             

             if ( strcmp(XX2,"0008") == 0)
           { //chat group button press
              if ( bklight == 0)
              {   
                   EEprom_write(CONFIG_LIGHTMODE, 1);
               //UART_Write_text("Light ON\r\n");
                    bklight = 1;
                    //PORTC = 0;
                      PORTC.RC1     = 0;//ON right
                      PORTC.RC2     = 0;//ON left
                      
                }else{
                      //UART_Write_text("Light OFF\r\n");
                      EEprom_write(CONFIG_LIGHTMODE, 0);
                      PORTC = 255;
                      bklight = 0;
                      PWM1_Set_Duty(255);
                      PWM2_Set_Duty(255);

                }
           }
              if ( strcmp(XX2,"0007") == 0 &&  bklight == 1) //green alt
            {
               PORTC.RC3     = 0;//ON
             }else{
               PORTC.RC3     = 1;//OFF
            }
           if ( strcmp(XX2,"0006") == 0 &&  bklight == 1) //orangealt
            {
               PORTC.RC4     = 0;//ON
             }else{
               PORTC.RC4     = 1;//OFF
            }
  }
                  
    //Delay_80us(); // without this delay, pic hangs on sleep...

  } while (1);                   // endless loop

}