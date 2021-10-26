#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    FILE * fp;
    char path[1035];

    fp = popen("vcgencmd measure_temp", "r");
    if (fp == NULL)
        printf("There was an error executing the command");
    while (fgets(path, sizeof(path), fp) != NULL)
        printf("%s", path);
    pclose(fp)

    return EXIT_SUCCESS;
}