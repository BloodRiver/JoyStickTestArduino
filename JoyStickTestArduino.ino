// define joystick axis pins
#define VRx        A0
#define VRy        A1
#define SW         A2

// define LED pins
#define RED_LED    2
#define BLUE_LED1  3
#define WHITE_LED  4
#define BLUE_LED2  6
#define YELLOW_LED 12

// assigning numerical values to certain words that will be used to refer to
// LEDs corresponding to the direction of the joystick's movement
// Example digitalWrite(LED_array[UP], HIGH); This will turn on an LED
// that is meant to turn on when the joystick is moved UP.
#define UP        0
#define DOWN      1
#define LEFT      2
#define RIGHT     3
#define BUTTON    4

// putting all LEDs in an array so that later on, if new LEDs are added, or
// some LEDs are removed we don't need to add or delete pinMode(LED_PIN) in
// the setup function. Happens automatically due to forloop
int LED_array[] = {BLUE_LED1, WHITE_LED, BLUE_LED2, RED_LED, YELLOW_LED};


// these are constant because their values will not change throughout the code

const int response_delay = 5;    // response delay of joystick, in ms
const int lower_analog_bound = 0;  // the lowest possible analog value from analogRead()
const int upper_analog_bound = 1023; // the highest possible analog value from analogRead()

// constants to be used to scale down the analog value to smaller values in order to reduce
// joystick sensitivity
const int range_lower_bound = 0;  // the lowest value we want to see for the joystick movement
const int range_upper_bound = 12;  // the highest range of movement for X or Y axis of the joystick
const int threshold = range_upper_bound / 4;  // this value defines the limit of analog movement by the joystick before a response is triggered (example turning on an LED)
const int joystick_center_position = range_upper_bound / 2;    // resting position value. The joystick is centered


/**
  @param this_axis - the analog pin number from which the joystick's axis will be read
 
  @returns distance_moved_by_joystick - the distance of the joystick from its center
*/
int readAxis(int this_axis)
{
  // read the analog input from one of the analog pins given by this_axis
  int axis_value_reading = analogRead(this_axis);

  // scale down the value received from analogRead to reduce sensitivity of joystick
  axis_value_reading = map(axis_value_reading, lower_analog_bound, upper_analog_bound, range_lower_bound, range_upper_bound);  

  int distance_moved_by_joystick = axis_value_reading - joystick_center_position;  // input reading - center position of joystick

  // ignoring the positive/negative value of distance, if the distance moved by joystick is less than the minimum limit
  // for triggering a response, then we set the distance to 0. Later in the code, if the distance == 0, then no response (no LED will be turned on)
  if (abs(distance_moved_by_joystick) < threshold)
  {
    distance_moved_by_joystick = 0;    
  }

  // return the distance for this_axis (could be either positive or negative)
  return distance_moved_by_joystick;
}


/**
  @param switch_pin - the analog pin using which the joystick switch's value is going to be read

  @returns bool - if the switch is pressed, return true, else return false
*/
bool readSwitch(int switch_pin)
{
  int joystick_switch_reading = analogRead(switch_pin);
  
  if (joystick_switch_reading == 0)
  {
    return true;
  }
  return false;
}

void setup()
{
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT);

  // loop through each LEDs in the LED array and set their pinModes to OUTPUT
  for (int i=0; i<5; i++)
  {
    pinMode(LED_array[i], OUTPUT);
  }

  // Begin serial communication at 9600 baud rate
  Serial.begin(9600);
}


void loop()
{
  // use the user-defined function readAxis to get a value for the joystick input
  // for both axes  
  int x_reading = readAxis(VRx);
  int y_reading = readAxis(VRy);

  bool switch_is_pressed = readSwitch(SW);

  // int sw = analogRead(SW);

  // Serial.print("sw: ");
  // Serial.println(sw);

  // Serial.print("x_reading: ");
  // Serial.println(x_reading);
  
  // Serial.print("y_reading: ");
  // Serial.println(y_reading);


  // if joystick moves to the right
  if (x_reading > 0)
  {
    digitalWrite(LED_array[RIGHT], HIGH);        
  }
  // else if joystick moves to the left
  else if (x_reading < 0)
  {
    digitalWrite(LED_array[LEFT], HIGH);
  }
  // else if joystick is horizontally centered (might not be vertically centered though)
  else
  {
    digitalWrite(LED_array[LEFT], LOW);
    digitalWrite(LED_array[RIGHT], LOW);
  }
  
  // if joystick moves upward
  if (y_reading > 0)
  {
    digitalWrite(LED_array[UP], HIGH);        
  }
  // else if joystick moves downward
  else if (y_reading < 0)
  {
    digitalWrite(LED_array[DOWN], HIGH);
  }
  // else if joystick is vertically centered (might not be horizontally centered though)
  else
  {
    digitalWrite(LED_array[UP], LOW);
    digitalWrite(LED_array[DOWN], LOW);
  }


  // self-explanatory
  if (switch_is_pressed)
  {
    digitalWrite(LED_array[BUTTON], HIGH);
  }
  else
  {
    digitalWrite(LED_array[BUTTON], LOW);
  }

  // wait for given response delay duration in order to avoid chaos
  delay(response_delay);
}








