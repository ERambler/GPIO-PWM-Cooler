#include <iostream>
#include <fstream>
#include <wiringPi.h>
#include <softPwm.h>
#include <unistd.h>
#include <string.h>
//load defaults
int PIN      	= 	7;
int RANGE_MAX	= 	100;
int RANGE_MIN	=	70;
int TEMPERATURE_MIN =  	30;
int TEMPERATURE_MAX =  	60;

float RANGE_COEF= (RANGE_MAX - RANGE_MIN)/(TEMPERATURE_MAX-TEMPERATURE_MIN);

using namespace std;

// Удаление пробелов в строке
void delSpaces(char *str) 
{
for (int i = 0, j = 0; str[i];(str[j++] = str[i] != ' ' ? str[i] : (j--, str[j]), i++, (!str[i] ? (str[j] = 0) : 0)));
}
// Функция преобразования строки в целое число
int StrToInt(char *s)
{
  int temp = 0, i = 0, sign = 0; // число
  if (s[i] == '-')  {sign = 1;i++;}
  while (s[i] >= 0x30 && s[i] <= 0x39)
  { temp = temp + (s[i] & 0x0F);
    temp = temp * 10;
    i++;}
  temp = temp / 10;
  if (sign == 1)
    temp = -temp;
  return(temp);
}
//Беру температуру из файла
static int getTemperature() {
  static fstream myfile;
  int temperature = 0;
  myfile.open("/sys/devices/virtual/thermal/thermal_zone0/temp", ios_base::in);
  myfile >> temperature;
  myfile.close();
//  cout << temperature/1000;
  return temperature/1000;
}
static int getRPM(int T) {
	int rpm;
	if (T < TEMPERATURE_MIN) {return 0;}
	rpm = RANGE_COEF*(T-TEMPERATURE_MIN)+RANGE_MIN;
	if (rpm > RANGE_MAX) {rpm = RANGE_MAX;}
return rpm;
}
//Забираем конфигурационный файл
void config_parser ()
{
    char buff[256];
    char *parameter;   // буфер промежуточного хранения считываемого из файла текста
    ifstream fin("/etc/gpio_pwn_cooler.conf"); // открыли файл для чтения
 
    while (fin.getline(buff, 256)) // считали строку из файла
    {
    	//cout << buff << " | " << strlen (buff);
    	delSpaces (buff); //cout << " del spaces";
    	parameter = strtok (buff,"="); //cout << " strtok";
    	//cout << parameter << " |" << sizeof buff << endl;
    	if ((strcmp (parameter,"PIN"))==0) PIN = StrToInt(strtok (NULL,"="));
 	if ((strcmp (parameter,"RANGE_MAX"))==0) RANGE_MAX = StrToInt(strtok (NULL,"="));
    	if ((strcmp (parameter,"RANGE_MIN"))==0) RANGE_MIN = StrToInt(strtok (NULL,"="));
    	if ((strcmp (parameter,"TEMPERATURE_MIN"))==0) TEMPERATURE_MIN = StrToInt(strtok (NULL,"="));
    	if ((strcmp (parameter,"TEMPERATURE_MAX"))==0) TEMPERATURE_MAX = StrToInt(strtok (NULL,"="));

    }
    fin.close(); // закрываем файл
    float RANGE_COEF= (RANGE_MAX - RANGE_MIN)/(TEMPERATURE_MAX-TEMPERATURE_MIN);
}

int main(int argc, char* argv[]) {
	config_parser ();
  int temperature, rpm;
  try {
    if (wiringPiSetup() == 0) {
      softPwmCreate(PIN, 0, RANGE_MAX);
      while (1) 
        {
        temperature = getTemperature();
        rpm = getRPM(temperature);
        //cout << "RPM on " << rpm << "% " << "T = " << temperature << "\n";
        softPwmWrite(PIN, rpm);
        //softPwmStop(PIN);
        usleep(1000 * 5000);
      	}
      }
  } catch (exception& e) {
    cerr << e.what() << endl;
  }
  return 0;
}
