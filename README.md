# GPIO-PWM-Cooler
Service for control cooler by GPIO PWN on RaspberryPi BananaPi OrangePi
1. for compiling use CORRECT wiringPi lib (Orange wiring need build)
2. g++ -Ofast -Wall gpio_pwm_cooler.cpp -lwiringPi -lcrypt -lrt -lpthread -o gpio_pwm_cooler 
3. cp service file to /etc/systemd/system
4. enable demon

TODO: 
add /etc/gpio_pwn_cooler.conf 
generate deb package
