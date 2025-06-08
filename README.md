
# BLDC Motor Driver Circuit with STM32 Discovery

This project demonstrates a BLDC motor driver circuit controlled by an STM32 Discovery board. The firmware uses timers and PWM signals to drive a three-phase BLDC motor.



## Technologies Used

- STM32 Discovery Board (e.g. STM32F4 series)  
- C Programming Language  
- PWM (Pulse Width Modulation)  
- Timer peripherals for motor control  
- Electronic circuit design (PCB and components)  


## Setup

1. Clone the project to your local machine:  
   ```bash
   git clone https://github.com/yourUsername/bldc-motor-driver.git
   ```
2. Open your STM32 IDE (e.g. STM32CubeIDE or Keil).  
3. Compile the firmware located in the `src` folder and flash it to the STM32 Discovery board.  
4. Assemble the BLDC motor driver circuit as per the hardware schematic.


## Usage

- Connect the motor power supply and the STM32 Discovery board to the driver circuit.  
- The firmware generates PWM signals via timers to control motor speed and direction.  
- Adjust PWM parameters in the code to change motor speed.  


## Results

- PWM signals can be observed on an oscilloscope confirming correct timer operation.  
- Motor speed and direction are controllable with the provided firmware.


## Future Improvements

- Implement sensorless BLDC control algorithms  
- Add fault detection and protection features  
- Integrate communication protocols for remote control  


## License

MIT License — see the LICENSE file for details.
