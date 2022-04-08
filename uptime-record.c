#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool is_silent = false;
bool asking_help = false;
bool user_is_an_idiot = false;
char *color = "\e[32m";

void uptime(long uptime) {      // prints the uptime
    long days = uptime/86400;
    int hours = uptime/3600 - days*24;
    int mins = uptime/60 - days*1440 - hours*60;
    int sec = uptime - days*86400 - hours*3600 - mins*60;

    if(days) {
        printf("%dd ", days);  // print the number of days passed if more than 0
    }
    if(hours) {
        printf("%dh ", hours); // print the number of days passed if more than 0
    }
    if(mins) {
        printf("%dm ", mins);  // print the number of minutes passed if more than 0
    }
    if(sec) {
        printf("%ds", sec);    // print the number of seconds passed if more than 0
    }
}


int main(const int argc, const char **argv) {
    for(int i = 1; i < argc; i++) {
        if(!strcmp("-s", argv[i]) || !strcmp("--silent", argv[i]))
            is_silent = true;
        else if(!strcmp("-h", argv[i]) || !strcmp("--help", argv[i]))
            asking_help = true;
        else if(!strcmp("-c", argv[i]) || !strcmp("--color", argv[i])) {
            if(argv[i+1]) {
                char * colors[8][2] = {
                    {"black", "\e[30m"},
                    {"red", "\e[31m"},
                    {"green", "\e[32m"},
                    {"yellow", "\e[33m"},
                    {"blue", "\e[34m"},
                    {"purple", "\e[35m"},
                    {"cyan", "\e[36m"},
                    {"shell", "\e[30m"},
                };

                for (int j = 0; j < 8; ++j) {
                    if (!strcmp(argv[i+1], colors[j][0])) {
                        color = colors[j][1];
                        goto color;
                    }
                }

                fputs("\e[31m\e[1mERROR\e[0m: invalid color! Use --help for more info\n", stderr);
                user_is_an_idiot = true;

                color: ;
            } else {
                fputs("\e[31m\e[1mERROR\e[0m: --color requires a color! Use --help for more info\n", stderr);
                user_is_an_idiot = true;
            }
        }
    }

    if(user_is_an_idiot) return 1; // you fucking idiot

    if(asking_help) {
        printf("Keep track of your highest uptime!\n", color);
        printf("\n%sFLAGS\e[0m:\n", color);
        printf("\t%s-h\e[0m, %s--help\e[0m:\tPrint this help menu and exit\n", color, color);
        printf("\t%s-c\e[0m,%s --color\e[0m:\t Change the output color (default: green) [black, red, green, yellow, blue, purple, cyan, shell]\n", color, color);
        printf("\t%s-s\e[0m, %s--silent\e[0m:\tDon't print any output\n", color, color);
        return 0;
    }
    struct sysinfo info;
    sysinfo(&info);

    unsigned long current = info.uptime;

    char path[64];
    snprintf(path, 64, "%s/.config/uptime-record", getenv("HOME"));

    FILE *fp = fopen(path, "r+");
    if(!fp) {return 69;} // file didn't open correctly
    
    char best[10];
    fgets(best, 10, fp);
    rewind(fp);
    
    if (current > atol(best)) {
        if(!is_silent) {
            printf("Current uptime: ");
            uptime(current);
            printf("\n%sThis is your highest uptime!\e[0m\n\nPrevious highest: ", color);
            uptime(atol(best));
        }

        fprintf(fp, "%ld\n", current);
    } else if(!is_silent) {
        printf("Current uptime: ");
        uptime(current);
        printf("\n\n%sHighest uptime\e[0m: ", color);
        uptime(atol(best));
    }

    printf("\n");
    fclose(fp);
    return 0;
}
