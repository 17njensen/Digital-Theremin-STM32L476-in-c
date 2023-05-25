# Digital-Theremin-STM32L476-in-c
This is a Digital Theremin musical instrument for the microcontroller discovery board STM32L476 programmed in c.

This project was used as a final project for a microcontrollers course.

The device uses two ultrasonic distance sensors to grab input data from the user. One sensor detects distance to determine the output note for the speaker. 
The other sensor determines the amplitude, adjusting the output frequencies sound. The device takes in the distance data and converts the data to an analog signal,
which is then outputted to a connected speaker. This device has previously set frequencies to detect notes. 

Below is the final result:
https://www.youtube.com/watch?v=b5oFaTwS9fg&ab_channel=McClainJorgensen

### Digital design in comparison to a true theremin
![alt text](https://github.com/17njensen/Digital-Theremin-STM32L476-in-c/blob/main/image.PNG)
