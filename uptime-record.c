#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char is_silent;

void uptime(long uptime) {         // prints the uptime
    long days = uptime/86400;
    long hours = uptime/3600 - days*24;
    long mins = uptime/60 - days*1440 - hours*60;
    long sec = uptime - days*86400 - hours*3600 - mins*60;

    if(days) {
        printf("%ldd ", days);     // print the number of days passed if more than 0
    }
    if(hours) {
        printf("%ldh ", hours);       // print the number of days passed if more than 0
    }
    if(mins) {
        printf("%ldm ", mins);        // print the number of minutes passed if more than 0
    }
    if(sec) {
        printf("%lds", sec);         // print the number of seconds passed if more than 0
    }
}


int main(const int argc, const char **argv) {
    for(int i = 1; i < argc; i++) {
        if(!strcmp("-h", argv[1]) || !strcmp("--help", argv[1])) {
            puts("Keep track of your highest uptime!");
            puts("\n\e[32mFLAGS\e[0m:");
            puts("\t\e[32m-h\e[0m, \e[32m--help\e[0m:\tPrint this help menu and exit");
            puts("\t\e[32m-s\e[0m, \e[32m--silent\e[0m:\tDon't print any output");
            return 0;
        } else if(!strcmp("-s", argv[1]) || !strcmp("--silent", argv[1])) is_silent = 1;
    }

    struct sysinfo info;
    sysinfo(&info);

    long current = info.uptime;

    FILE *fp = fopen("/home/alba4k/.uptimeRecord", "r+");
    if(!fp) {return 69;} // file didn't open correctly
    
    char best[10];
    fgets(best, 10, fp);
    rewind(fp);
    
    if (current > atol(best)) {
        if(!is_silent) {
            printf("Current uptime: ");
            uptime(current);
            printf("\n\e[32mThis is your highest uptime!\e[0m\n\nPrevious highest: ");
            uptime(atol(best));
        }

        fprintf(fp, "%ld\n", current);
    } else if(!is_silent) {
        printf("Current uptime: ");
        uptime(current);
        printf("\n\nHighest uptime: ");
        uptime(atol(best));
    }

    fclose(fp);
    return 0;
}