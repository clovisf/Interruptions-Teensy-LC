# Interruptions-Teensy-LC
A sketch for timer interruptions test of Teensy LC 

This code tests the timer interruption for Teensy LC, by blinking 5 leds at different rates:
- 1 LED uses delay and goes inside the main() loop (pin 11)
- 4 LED's respect the interruption and are controlled by individual counters inside it (pins 5, 9, 15 and 17).  

Based on code from ShawnHymel >> http://shawnhymel.com/661/learning-the-teensy-lc-interrupt-service-routines/
