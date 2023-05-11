/* Receiver code for the Arduino Radio control with PWM output

    Module // Arduino UNO
    
    GND    ->   GND
    Vcc    ->   3.3V
    CE     ->   D9
    CSN    ->   D10
    CLK    ->   D13
    MOSI   ->   D11
    MISO   ->   D12

*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

//Define widths
int pwm_width_2 = 0;
int pwm_width_3 = 0;
int pwm_width_4 = 0;
int pwm_width_5 = 0;

Servo PWM2;
Servo PWM3;
Servo PWM4;
Servo PWM5;

                    //We could use up to 32 channels
struct MyData {
byte throttle;      //We define each byte of data input, in this case just 6 channels
byte yaw;
byte pitch;
byte roll;
// byte AUX1;
// byte AUX2;
};
MyData data;


const uint64_t pipeIn = 0xE8E8F0F0E1LL;     //Remember that this code is the same as in the transmitter
RF24 radio(9, 10); 



void resetData()
{
//We define the inicial value of each data input
//3 potenciometers will be in the middle position so 127 is the middle from 254
data.throttle = 0;
data.yaw = 127;
data.pitch = 127;
data.roll = 127;
// data.AUX1 = 0;
// data.AUX2 = 0;
}

/**************************************************/

void setup()
{
  //Set the pins for each PWM signal
  PWM2.attach(2);
  PWM3.attach(3);
  PWM4.attach(4);
  PWM5.attach(5);

  //Configure the NRF24 module
  Serial.begin(9600);
  resetData();
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);  
  radio.openReadingPipe(1,pipeIn);
  
  //we start the radio comunication
  radio.startListening();
}

/**************************************************/

unsigned long lastRecvTime = 0;

void recvData()
{
while ( radio.available() ) {
radio.read(&data, sizeof(MyData));
lastRecvTime = millis(); //here we receive the data
}
}

/**************************************************/

void loop()
{
recvData();
unsigned long now = millis();
//Here we check if we've lost signal, if we did we reset the values 
if ( now - lastRecvTime > 1000 ) {
// Signal lost?
resetData();
}

pwm_width_2 = map(data.throttle, 0, 255, 1000, 2000);     //PWM value on digital pin D2
pwm_width_3 = map(data.yaw,      0, 255, 1000, 2000);     //PWM value on digital pin D3
pwm_width_4 = map(data.pitch,    0, 255, 1000, 2000);     //PWM value on digital pin D4
pwm_width_5 = map(data.roll,     0, 255, 1000, 2000);     //PWM value on digital pin D5

  Serial.print("Throttle: ");
  Serial.println(pwm_width_2);
  Serial.print("Yaw: ");
  Serial.println(pwm_width_3);
  Serial.print("Pitch: ");
  Serial.println(pwm_width_4);
  Serial.print("Roll: ");
  Serial.println(pwm_width_5);
  delay(200);  // delay
//Now we write the PWM signal using the servo function
PWM2.writeMicroseconds(pwm_width_2);
PWM3.writeMicroseconds(pwm_width_3);
PWM4.writeMicroseconds(pwm_width_4);
PWM5.writeMicroseconds(pwm_width_5);
PWM6.writeMicroseconds(pwm_width_6);
PWM7.writeMicroseconds(pwm_width_7);


}//Loop end
/**************************************************/

