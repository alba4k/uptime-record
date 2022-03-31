#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
char is_silent;
bool asking_help = 0;
char *color = "\e[32m";

void uptime(long uptime) {      // prints the uptime
    long days = uptime/86400;
    long hours = uptime/3600 - days*24;
    long mins = uptime/60 - days*1440 - hours*60;
    long sec = uptime - days*86400 - hours*3600 - mins*60;

    if(days) {
        printf("%ldd ", days);  // print the number of days passed if more than 0
    }
    if(hours) {
        printf("%ldh ", hours); // print the number of days passed if more than 0
    }
    if(mins) {
        printf("%ldm ", mins);  // print the number of minutes passed if more than 0
    }
    if(sec) {
        printf("%lds", sec);    // print the number of seconds passed if more than 0
    }
}


int main(const int argc, const char **argv) {
    for(int i = 1; i < argc; i++) {
        if(!strcmp("-s", argv[1]) || !strcmp("--silent", argv[1]))
            is_silent = 1;
        else if(!strcmp("-h", argv[1]) || !strcmp("--help", argv[1]))
            asking_help = 1;
        else if(!strcmp("-c", argv[1]) || !strcmp("--color", argv[1])) {
            if(argv[i+1]) {
                if(!strcmp(argv[i+1],"black")) {
                    color = "\e[30m";
                } else if(!strcmp(argv[i+1],"red")) {
                    color = "\e[31m";
                } else if(!strcmp(argv[i+1],"green")) {
                    color = "\e[32m";
                } else if(!strcmp(argv[i+1],"yellow")) {
                    color = "\e[33m";
                } else if(!strcmp(argv[i+1],"blue")) {
                    color = "\e[34m";
                } else if(!strcmp(argv[i+1],"pink")) {
                    color = "\e[35m";
                } else if(!strcmp(argv[i+1],"cyan")) {
                    color = "\e[36m";
                } else if(!strcmp(argv[i+1],"shell")) {
                    color = "";
                } else {
                    puts("ERROR: invalid color! Use --help for more info");
                    return 0;
                }
            } else {
                puts("ERROR: --color requires a color! Use --help for more info");
                return 1;
            }
        }
    }

    if(asking_help) {
        printf("Keep track of your highest uptime!", color);
        printf("\n%sFLAGS\e[0m:", color);
        printf("\t%s-h\e[0m, current%s--help\e[0m:\tPrint this help menu and exit", color, color);
        printf("\t%s-c\e[0m,%s --color\e[0m:\t Change the output color (default: cyan) [black, red, green, yellow, blue, pink, cyan, shell]\n", color, color);
        printf("\tcurrent%s-s\e[0m, current%s--silent\e[0m:\tDon't print any output", color, color);
        return 0;
    }
    struct sysinfo info;
    sysinfo(&info);

    long current = info.uptime;

    char path[61];
    snprintf(path, 61, "%s/.config/uptime-record", getenv("HOME"));

    FILE *fp = fopen(path, "r+");
    if(!fp) {return 69;} // file didn't open correctly
    
    char best[10];
    fgets(best, 10, fp);
    rewind(fp);
    
    if (current > atol(best)) {
        if(!is_silent) {
            printf("Current uptime: ");
            uptime(current);
            printf("\ncurrent%sThis is your highest uptime!\e[0m\n\nPrevious highest: ");
            uptime(atol(best));
        }

        fprintf(fp, "%ld\n", current);
    } else if(!is_silent) {
        printf("Current uptime: ");
        uptime(current);
        printf("\n\nHighest uptime: ");
        uptime(atol(best));
    }

    printf("\n");
    fclose(fp);
    return 0;
}