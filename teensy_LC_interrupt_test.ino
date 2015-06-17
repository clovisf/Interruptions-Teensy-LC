/* teensy_LC_interrupt_test.ino
 *
 * Authored by Clovis Fritzen in 06/17/2015 , clovisf AT gmail DOT com 
 * based on code from Shawn Hymel (created on May 17, 2015)
 *
 * Manually set up an interrupt service routine to toggle an LED
 * on pin 13. The interrupt routine is then disabled and enabled
 * every second, meaning the LED will flash for a second and then
 * be stuck on or off for a second.
 *
 * Setting pin 13 and toggling the LED is done with the Arduino
 * functions pinMode() and digitalWrite(). This is to show how
 * to create a custom ISR.
 *
 * NVIC + ISR vectors: https://github.com/PaulStoffregen/cores/blob/master/teensy3/kinetis.h
 * Cortex-M0+ Generic User Guide: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0662b/index.html
 * My post on PJRC: https://forum.pjrc.com/threads/28572-Can-t-get-__disable_irq()-to-work-on-Teensy-LC
 */

//const uint8_t LED_PIN = 17;
//volatile uint8_t led = 0;
const int pinoAzul = 17;
const int pinoAmarelo = 15;
const int pinoVerde = 5;
const int pinoVermelho = 9;
const int pinoLed = 11;

volatile int azul = 0;
volatile int amarelo = 0;
volatile int vermelho = 0;
volatile int verde = 0;
volatile int led = 0;
volatile String stringTempo;

int amareloAnterior;
int tempoAmarelo;
int azulAnterior;
int tempoAzul;
int verdeAnterior;
int tempoVerde;
int vermelhoAnterior;
int tempoVermelho;
int ledAnterior;
int tempoL  ed;

int counterAzul = 0;
int counterAmarelo = 0;
int counterVerde = 0;
int counterVermelho = 0;
int tempo =0;

void setup() {
  
  // Set up the LED and set it low initially
  pinMode(pinoAzul, OUTPUT);
  pinMode(pinoAmarelo, OUTPUT);
  pinMode(pinoVerde, OUTPUT);
  pinMode(pinoVermelho, OUTPUT);
  pinMode(pinoLed, OUTPUT);
  
  // The order of setting the TPMx_SC, TPMx_CNT, and TPMx_MOD
  // seems to matter. You must clear _SC, set _CNT to 0, set _MOD
  // to the desired value, then you can set the bit fields in _SC.
  
  // Clear TPM0_SC register (p. 572)
  FTM0_SC = 0;
  
  // Reset the TPM0_CNT counter (p. 574)
  FTM0_CNT = 0;
  
  // Set overflow value (modulo) (p.574)
  FTM0_MOD = 0xFFFF;
  
  // Set TPM0_SC register (p. 572)
  // Bits | Va1ue | Description
  //  8   |    0  | DMA: Disable DMA
  //  7   |    1  | TOF: Clear Timer Overflow Flag
  //  6   |    1  | TOIE: Enable Timer Overflow Interrupt
  //  5   |    0  | CPWMS: TPM in up counting mode
  // 4-3  |   01  | CMOD: Counter incrememnts every TPM clock
  // 2-0  |  111  | PS: Prescale = 128
  FTM0_SC = 0b011001000;
  
  // if 2-0 = 111, time: 175mS.
  // if 2-0 = 110, time: 88mS. 
  // if 2-0 = 101, time: 43,7mS.
  // if 2-0 = 000, time: 1,37mS. 
  
  // Nested Vector Interrupt Controller (NVIC) (p. 57)
  // Also: Chapter 4.2 of the Generic User Guide  
  // Our FTM0 interrupt number is 17 (as per kinetis.h). We can
  // use that to set up our interrupt vector and priority.
 
  // Set the urgency of the interrupt. Lower numbers mean higher
  // urgency (they will happen first). Acceptable values are
  // 0, 64, 128, and 192. Default is 128. We set the priority
  // (2nd byte) in the register for the FTM0 interrupt (&E000_E410) 
  // to 64.
  NVIC_SET_PRIORITY(IRQ_FTM0, 64);
  
  // Enable the interrupt vector. In this case, we want to execute
  // the ISR (named "ftm0_isr()" for Teensy) every time TPM0 
  // overflows. We set bit 17 of &E000_E100.
  NVIC_ENABLE_IRQ(IRQ_FTM0);
  // Same as: NVIC_ISER0 |= (1 << 17);
  
  Serial.begin(115200);
}

void loop() {
  
  delay (200);
  led = ~led;
  digitalWrite(pinoLed, led);
  tempoLed= (micros()-ledAnterior);
  ledAnterior= micros();
  Serial.print("Tempo led:");
  Serial.println(tempoLed);  
  
}

// "ftm0_isr" is an interrupt vector defined for the Teensy
void ftm0_isr(void) {
  
  // First, we need to clear the timer flag so that the interrupt
  // will happen again. We can take care of the rest of the 
  // interrupt after that.
  
  // Write a 1 to the TOF bit to clear the timer overflow flag
  FTM0_SC |= (1 << 7);
  
  // Our interrupt routine: Toggle the LED
  tempo= micros();
  counterAzul ++;
  counterAmarelo ++;
  counterVerde ++;
  counterVermelho ++;
  
  if(counterAmarelo > 600) {
      counterAmarelo = 0;
      amarelo = ~amarelo;
      tempoAmarelo= (micros()-amareloAnterior);
      amareloAnterior= micros();
            
  digitalWrite(pinoAmarelo, amarelo);
  Serial.print("tempo Amarelo:");
  Serial.println(tempoAmarelo);
  } 
  if( counterVerde > 200) {
        counterVerde = 0;
     verde = ~verde;
     tempoVerde= (micros()-verdeAnterior);
      verdeAnterior= micros();
      
  digitalWrite(pinoVerde, verde);
 Serial.print("tempo Verde:");
  Serial.println(tempoVerde); 
  } 
  if(counterVermelho > 400) {
      counterVermelho = 0;
    vermelho = ~vermelho;
    tempoVermelho= (micros()-vermelhoAnterior);
      vermelhoAnterior= micros();
      
  digitalWrite(pinoVermelho, vermelho);
  Serial.print("tempo Vermelho:");
  Serial.println(tempoVermelho);
  }
  if (counterAzul > 731){ // 0.999424 seconds = 731, 9,5704mS = 7
    counterAzul = 0;
    
    azul = ~azul;
    tempoAzul= (micros()-azulAnterior);
      azulAnterior= micros();
     
  digitalWrite(pinoAzul, azul);
  
  Serial.print("tempo Azul:");
  Serial.println(tempoAzul);
  } 
}
