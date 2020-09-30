# GPIO-PWM-Cooler
Service for control cooler by GPIO PWM on RaspberryPi BananaPi OrangePi
1. for compiling use CORRECT wiringPi lib (Orange wiring need build link - https://github.com/orangepi-xunlong/wiringOP)
2. g++ -Ofast -Wall gpio_pwm_cooler.cpp -lwiringPi -lcrypt -lrt -lpthread -o gpio_pwm_cooler 
3. cp service file to /etc/systemd/system
4. enable daemon

TODO: 
add /etc/gpio_pwn_cooler.conf 
generate deb package
