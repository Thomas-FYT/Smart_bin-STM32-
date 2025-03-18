# Smart_bin-STM32-
Embedded device using CNN model and STM32 for rubbish classification

![image](https://github.com/user-attachments/assets/db4b598a-27d2-4d95-ae5d-68c90999a90e)

The system will be triggered by the infrared sensor. Then, the OV7725 camera will take the picture of the rubbish and the 
HC-05 bluetooth device will trnsfer the data from STM32 to our computer and later those data will be analysis by the CNN
model for classification. 

Below is the design of the CNN model:

![image](https://github.com/user-attachments/assets/c5d07238-c987-4b89-82d4-d8639c74e177)

Capacity checking and visualization:

![image](https://github.com/user-attachments/assets/6a05f2c1-c515-4900-b6a8-35d55f84889e)
The capacity of the rubbish bin will be checked by using the HC-SR04 ultrasonic distance sensor
