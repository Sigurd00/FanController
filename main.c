#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef linux
#include <wiringPi.h>
#include <unistd.h>
#define FAN_PIN 7 //https://pinout.xyz/pinout/pin7_gpio4#
#define PIN_ON 1
#define PIN_OFF 0
void run_fan_controller();
#else
#include <windows.h>
#endif

double get_system_temp();
double extract_temp_from_text(char* tempText);
double get_fake_temp();

int main(int argc, char *argv[]) {
    #ifdef linux
    pinMode(FAN_PIN, OUTPUT);
    run_fan_Controller();
    #else
    printf("FAKE TEMPERATURE = %lf", get_fake_temp());
    #endif
    return EXIT_SUCCESS;
}

void run_fan_Controller(){
    int _running = 1;
    double temperature = get_system_temp();
    printf("%lf", &temperature);
    while (_running) {
        if (temperature >= 60) {
            digitalWrite(FAN_PIN, PIN_ON);
            printf("FAN ON");
        }
        else if (temperature <= 40) {
            digitalWrite(FAN_PIN, PIN_OFF);
            printf("FAN OFF");
        }
        //Amount of seconds to sleep before reading temps again
        sleep(2);
    }
}

double get_system_temp(){
    FILE * fp;
    char path[1035];
    double temp;

    fp = popen("vcgencmd measure_temp", "r");
    if (fp == NULL)
        printf("There was an error executing the command");
    while (fgets(path, sizeof(path), fp) != NULL) {
        printf("%s", path);
    }
    pclose(fp);
    return extract_temp_from_text(path);
}

double get_fake_temp(){
    char *tempString = "temp=36.5'C";
    
    printf("USING FAKE TEMP\n");
    return extract_temp_from_text(tempString);
}

double extract_temp_from_text(char* tempString){
    int firstDigitPosition = 0, Cposition = 0, i = 0;
    char temp[6] = "";
    double temperature;
    while (Cposition == 0)
    {
        if (firstDigitPosition == 0 && isdigit(tempString[i])){
            firstDigitPosition = i;
        }

        else if (Cposition == 0 && tempString[i] == 'C'){
            Cposition = i;
        }
        i++;
    }
    strncpy(temp, tempString + firstDigitPosition, 
        Cposition - firstDigitPosition - 1);
    
    temp[Cposition - firstDigitPosition] = '\0';
    sscanf(temp, "%lf", &temperature);
    return temperature;
}
