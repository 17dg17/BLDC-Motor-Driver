
# BLDC Motor Driver Circuit

This project contains the design and control software of a BLDC motor driver circuit. It uses STM8S microcontroller to provide PWM-based control of three-phase BLDC motors.

---

## Technologies Used

- STM8S Microcontroller  
- C Programming Language  
- PWM (Pulse Width Modulation)  
- Electronic circuit design (PCB and components)  

---

## Setup

1. Clone the project files to your computer:  
   ```bash
   git clone https://github.com/yourUsername/bldc-motor-driver.git
   ```
2. Open your STM8S programming environment (e.g. STM8Cube or STVD).  
3. Compile the firmware code in the `src` folder and upload it to the microcontroller.  
4. Assemble the BLDC motor driver circuit according to the hardware schematic.

---

## Usage

- After uploading the program to the microcontroller, connect the motor power supply.  
- Adjust PWM frequency and duty cycle values in the software as needed.  
- Use control commands to start the motor and adjust speed (e.g. via serial commands).

---

## Results

- PWM signals are clearly visible on the oscilloscope.  
- Motor direction and speed can be controlled precisely.  

![PWM Signal](docs/pwm_oscilloscope.png)  
*Oscilloscope view of PWM signal*

---

## Future Improvements

- Adding sensorless control algorithm  
- Improving fault detection and protection mechanisms  
- User interface or serial communication control  

---

## License

MIT License — see the LICENSE file for details.

