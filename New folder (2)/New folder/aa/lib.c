  
#define timer3ms 59536
#define timer2ms 63536
unsigned char Timeslot;
unsigned char Timeslot2;

#define kaka1_1   PORTL |= (1 << 2) //PORTL.2 logika 1(HIGH)
#define kaka2_1   PORTL |= (1 << 3) //PORTL.3 logika 1(HIGH)
#define kaka3_1   PORTL |= (1 << 4)
#define kaka4_1   PORTL |= (1 << 5)
#define kaka5_1   PORTL |= (1 << 6)
#define kaka6_1   PORTL |= (1 << 7)

#define kaka1_0   PORTL &= ~(1 << 2) //PORTL.2 logika 0(LOW)
#define kaka2_0   PORTL &= ~(1 << 3) //PORTL.3 logika 0(LOW)
#define kaka3_0   PORTL &= ~(1 << 4)
#define kaka4_0   PORTL &= ~(1 << 5)
#define kaka5_0   PORTL &= ~(1 << 6)
#define kaka6_0   PORTL &= ~(1 << 7)

#define taka1   PORTF.0
#define taka2   PORTF.1
#define taka3   PORTF.2
#define pala1   PORTF.3

#define taki1   PORTF.7
#define taki2   PORTF.6
#define taki3   PORTF.5
#define pala2   PORTF.4 

#define kaki1   PORTC.7
#define kaki2   PORTC.6
#define kaki3   PORTA.6
#define kaki4   PORTA.7
#define kaki5   PORTA.4
#define kaki6   PORTA.5 
                   
interrupt [TIM1_OVF] void timer1_ovf_isr(void);
interrupt [TIM1_COMPA] void timer1_compa_isr(void);
interrupt [TIM1_COMPB] void timer1_compb_isr(void);
interrupt [TIM3_OVF] void timer3_ovf_isr(void);
interrupt [TIM3_COMPA] void timer3_compa_isr(void);
interrupt [TIM3_COMPB] void timer3_compb_isr(void);
interrupt [USART0_RXC] void usart0_rx_isr(void);  
interrupt [TIM2_OVF] void timer2_ovf_isr(void); 
void init();

interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
    TCNT2=240; 
    counterTG++;  
    if(counterDelay>0)
        counterDelay--;
}
   
void init()
{   
    PORTA=0x00;
    DDRA=0xFF;

    PORTB=0x00;
    DDRB=0xFF;

    PORTC=0x00;
    DDRC=0xFF;

    PORTD=0x00;
    DDRD=0xF0;

    PORTE=0x00;
    DDRE=0xFF;

    PORTF=0x00;
    DDRF=0b11111111;

    PORTG=0x00;
    DDRG=0xFF;

    PORTH=0x00;
    DDRH=0xFF;

    PORTJ=0x00;
    DDRJ=0xFF;

    PORTK=0x00;
    DDRK=0xFF;

    PORTL=0x00;
    DDRL=0b11111111;

    TCCR1A=0x00;
    TCCR1B=0x02;             
    TCCR2A=0x00;
    TCCR2B=0x07;
    TCCR3A=0x00;
    TCCR3B=0x02;

        
    UCSR0A=0x00;
    UCSR0B=0x98;
    UCSR0C=0x06;
    UBRR0H=0x00;
    UBRR0L=0x67;

    ACSR=0x80;
    ADCSRB=0x00;
    DIDR1=0x00;         
                
    TIMSK1=0x07;
    TIMSK2=0x01;
    TIMSK3=0x07;

    rad=57.272727;      
    L1Kuadrat = L1 * L1;
    L2Kuadrat = L2 * L2;
    L3Kuadrat = L3 * L3;
    L4Kuadrat = L4 * L4;
           
    for (countNo = 0; countNo < 2; countNo++) {
      Xset[countNo] = initPositionX; Yset[countNo] = initPositionY; Zset[countNo] = initPositionZ;
    }
      
    X[0]=0; Y[0]=0; Z[0]=0;
    X[1]=0; Y[1]=0; Z[1]=0;
    
    for(index=0;index<20;index++)
    {
        servoInitError[index]=eServoInitError[index];
        printf("A%d\n",index); 
        printf("B%d\n",servoInitError[index]); 
        delay_ms(50);
    }      
    
}

interrupt [USART0_RXC] void usart0_rx_isr(void)
{
  dataRx = UDR0;
  if (countRxProtokol == 2)
  {
    countRxProtokol = 0;
    countRx = 1;
  }
  switch (countRx)
  {
    case 0 :
      if (dataRx == 255)
      {
        countRxProtokol++;
      }
      break;

    case 1 :
      if (dataRx == 255)
        countRxProtokol++;
      else
        countRxProtokol = 0;
      dataMasuk[0] = dataRx;
      countRx++;
      break;

    case 2 :
      if (dataRx == 255)
        countRxProtokol++;
      else
        countRxProtokol = 0;
      dataMasuk[1] = dataRx;
      dataInt[0] = ((dataMasuk[0] & 0b00000011) << 8) + dataMasuk[1] ;
      if ((dataMasuk[0] & 0b00010000) >> 4)
        dataInt[0] *= -1;
      countRx++;
      break;

    case 3 :
      if (dataRx == 255)
        countRxProtokol++;
      else
        countRxProtokol = 0;
      dataMasuk[2] = dataRx;
      data[0] = dataMasuk[2] ;
      if ((dataMasuk[0] & 0b01000000) >> 6)
        data[0] *= -1;
      countRx++;
      break;
    case 4 :
      if (dataRx == 255)
        countRxProtokol++;
      else
        countRxProtokol = 0;
      dataMasuk[3] = dataRx;
      dataInt[1] = ((dataMasuk[0] & 0b00001100) << 6) + dataMasuk[3] ;
      if ((dataMasuk[0] & 0b00100000) >> 5)
        dataInt[1] *= -1;
      countRx ++;
      break;
    case 5 :
      if (dataRx == 255)
        countRxProtokol++;
      else
        countRxProtokol = 0;
      countRx = 0;
      break;
  }
}

interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
  switch (Timeslot)
  {
    case 0:
      kaka1_1;
      kaki1 = 1;
      OCR1AH = ((2 * (servoInitError[0] + servo[0])) + timer3ms) >> 8;
      OCR1AL = ((2 * (servoInitError[0] + servo[0])) + timer3ms) & 0xff;
      OCR1BH = ((6000-2 * (servoInitError[6] + servo[6])) + timer3ms) >> 8;
      OCR1BL = ((6000-2 * (servoInitError[6] + servo[6])) + timer3ms) & 0xff;
      TCNT1H = timer3ms >> 8;
      TCNT1L = timer3ms & 0xff;
      Timeslot = 1;
      break;

    case 1:
      kaka2_1;
      kaki2 = 1;
      OCR1AH = ((2 * (servoInitError[1] + servo[1])) + timer3ms) >> 8;
      OCR1AL = ((2 * (servoInitError[1] + servo[1])) + timer3ms) & 0xff;
      OCR1BH = ((6000-2 * (servoInitError[7] + servo[7])) + timer3ms) >> 8;
      OCR1BL = ((6000-2 * (servoInitError[7] + servo[7])) + timer3ms) & 0xff;
      TCNT1H = timer3ms >> 8;
      TCNT1L = timer3ms & 0xff;
      Timeslot = 2;
      break;

    case 2:
      kaka3_1;
      kaki3 = 1;
      OCR1AH = ((6000-2 * (servoInitError[2] + servo[2])) + timer3ms) >> 8;
      OCR1AL = ((6000-2 * (servoInitError[2] + servo[2])) + timer3ms) & 0xff;
      OCR1BH = ((2 * (servoInitError[8] + servo[8])) + timer3ms) >> 8;
      OCR1BL = ((2 * (servoInitError[8] + servo[8])) + timer3ms) & 0xff;
      TCNT1H = timer3ms >> 8;
      TCNT1L = timer3ms & 0xff;
      Timeslot = 3;
      break;

    case 3:
      kaka4_1;
      kaki4 = 1;
      OCR1AH = ((6000-2 * (servoInitError[3] + servo[3])) + timer3ms) >> 8;
      OCR1AL = ((6000-2 * (servoInitError[3] + servo[3])) + timer3ms) & 0xff;
      OCR1BH = ((2 * (servoInitError[9] + servo[9])) + timer3ms) >> 8;
      OCR1BL = ((2 * (servoInitError[9] + servo[9])) + timer3ms) & 0xff;
      TCNT1H = timer3ms >> 8;
      TCNT1L = timer3ms & 0xff;
      Timeslot = 4;
      break;

    case 4:
      kaka5_1;
      kaki5 = 1;
      OCR1AH = ((2 * (servoInitError[4] + servo[4])) + timer3ms) >> 8;
      OCR1AL = ((2 * (servoInitError[4] + servo[4])) + timer3ms) & 0xff;
      OCR1BH = ((6000-2 * (servoInitError[10] + servo[10])) + timer3ms) >> 8;
      OCR1BL = ((6000-2 * (servoInitError[10] + servo[10])) + timer3ms) & 0xff;
      TCNT1H = timer3ms >> 8;
      TCNT1L = timer3ms & 0xff;
      Timeslot = 5;
      break;

    case 5:
      kaka6_1;
      kaki6 = 1;
      OCR1AH = ((2 * (servoInitError[5] + servo[5])) + timer3ms) >> 8;
      OCR1AL = ((2 * (servoInitError[5] + servo[5])) + timer3ms) & 0xff;
      OCR1BH = ((2 * (servoInitError[11] + servo[11])) + timer3ms) >> 8;
      OCR1BL = ((2 * (servoInitError[11] + servo[11])) + timer3ms) & 0xff;
      TCNT1H = timer3ms >> 8;
      TCNT1L = timer3ms & 0xff;
      Timeslot = 6;
      break;

    case 6:
      TCNT1H = timer2ms >> 8;
      TCNT1L = timer2ms & 0xff;
      Timeslot = 0;
  }

}
                                            

interrupt [TIM1_COMPA] void timer1_compa_isr(void)
{
switch (Timeslot)
    {
    case 0:
    break;
    
    case 1:
            kaka1_0;
    break;
    
    case 2:
            kaka2_0;
    break;
    
    case 3:
            kaka3_0;
    break;
    
    case 4:
            kaka4_0;
    break;
    
    case 5:
            kaka5_0;
    break;
    
    case 6:
            kaka6_0;
    break;
    }

}
                                           
interrupt [TIM1_COMPB] void timer1_compb_isr(void)
{
switch (Timeslot)
    {
    case 0:
    break;
    
    case 1:
            kaki1 = 0;
    break;
    
    case 2:
            kaki2 = 0;
    break;
    
    case 3:
            kaki3 = 0;
    break;
    
    case 4:
            kaki4 = 0;
    break;
    
    case 5:
            kaki5 = 0;
    break;
    
    case 6:
            kaki6 = 0;
    break;
    }


}

interrupt [TIM3_OVF] void timer3_ovf_isr(void)
{
  switch (Timeslot2)
  {
    case 0:
      taka1 = 1;
      taki1 = 1;
      OCR3AH = ((2 * (servoInitError[12] + servo[12])) + timer3ms) >> 8;
      OCR3AL = ((2 * (servoInitError[12] + servo[12])) + timer3ms) & 0xff;
      OCR3BH = ((2 * (servoInitError[15] + servo[15])) + timer3ms) >> 8;
      OCR3BL = ((2 * (servoInitError[15] + servo[15])) + timer3ms) & 0xff;
      TCNT3H = timer3ms >> 8;
      TCNT3L = timer3ms & 0xff;
      Timeslot2 = 1;
      break;

    case 1:
      taka2 = 1;
      taki2 = 1;
      OCR3AH = ((2 * (servoInitError[13] + servo[13])) + timer3ms) >> 8;
      OCR3AL = ((2 * (servoInitError[13] + servo[13])) + timer3ms) & 0xff;
      OCR3BH = ((2 * (servoInitError[16] + servo[16])) + timer3ms) >> 8;
      OCR3BL = ((2 * (servoInitError[16] + servo[16])) + timer3ms) & 0xff;
      TCNT3H = timer3ms >> 8;
      TCNT3L = timer3ms & 0xff;
      Timeslot2 = 2;
      break;

    case 2:
      taka3 = 1;
      taki3 = 1;
      OCR3AH = ((2 * (servoInitError[14] + servo[14])) + timer3ms) >> 8;
      OCR3AL = ((2 * (servoInitError[14] + servo[14])) + timer3ms) & 0xff;
      OCR3BH = ((2 * (servoInitError[17] + servo[17])) + timer3ms) >> 8;
      OCR3BL = ((2 * (servoInitError[17] + servo[17])) + timer3ms) & 0xff;
      TCNT3H = timer3ms >> 8;
      TCNT3L = timer3ms & 0xff;
      Timeslot2 = 3;
      break;

    case 3:
      pala1 = 1;
      pala2 = 1;
      OCR3AH = ((2 * (servoInitError[18] + servo[18])) + timer3ms) >> 8;
      OCR3AL = ((2 * (servoInitError[18] + servo[18])) + timer3ms) & 0xff;
      OCR3BH = ((2 * (servoInitError[19] + servo[19])) + timer3ms) >> 8;
      OCR3BL = ((2 * (servoInitError[19] + servo[19])) + timer3ms) & 0xff;
      TCNT3H = timer3ms >> 8;
      TCNT3L = timer3ms & 0xff;
      Timeslot2 = 4;
      break;

    case 4:

      TCNT3H = timer3ms >> 8;
      TCNT3L = timer3ms & 0xff;
      Timeslot2 = 5;
      break;

    case 5:

      TCNT3H = timer3ms >> 8;
      TCNT3L = timer3ms & 0xff;
      Timeslot2 = 6;
      break;

    case 6:
      TCNT3H = timer3ms >> 8;
      TCNT3L = timer3ms & 0xff;
      Timeslot2 = 0;
  }

}
                                               
// Timer3 output compare A interrupt service routine
interrupt [TIM3_COMPA] void timer3_compa_isr(void)
{
switch (Timeslot2)
    {
    case 0:
    break;
    
    case 1:
            taka1 = 0;
    break;
    
    case 2:
            taka2 = 0;
    break;
    
    case 3:
            taka3 = 0;
    break;
    
    case 4:
            pala1 = 0;
    break;
    
    case 5:
            
    break;
    
    case 6:
            
    break;
    }

}
                                               
// Timer3 output compare B interrupt service routine
interrupt [TIM3_COMPB] void timer3_compb_isr(void)
{
switch (Timeslot2)
    {
    case 0:
    break;
    
    case 1:
            taki1 = 0;
    break;
    
    case 2:
            taki2 = 0;
    break;
    
    case 3:
            taki3 = 0;
    break;
    
    case 4:
            pala2 = 0;
    break;
    
    case 5:
            
    break;
    
    case 6:
            
    break;
    }
}