#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>

#define DEFAULT_TIME 300

#ifndef LOGIN_NAME_MAX
    #ifdef HOST_NAME_MAX
        #define LOGIN_NAME_MAX HOST_NAME_MAX
    #else
        #define LOGIN_NAME_MAX 128
    #endif
#endif

bool is_silent = false;
bool asking_help = false;
bool user_is_an_idiot = false;
int background = 0;
char *color = "\e[32m";
int time = DEFAULT_TIME;

void uptime(long uptime) {      // prints the uptime
    long days = uptime/86400;
    int hours = uptime/3600 - days*24;
    int mins = uptime/60 - days*1440 - hours*60;
    int sec = uptime - days*86400 - hours*3600 - mins*60;

    if(days) {
        printf("%ldd ", days);  // print the number of days passed if more than 0
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
        else if(!strcmp("-b", argv[i]) || !strcmp("--background", argv[i])) {
            background = i+1;
        }
    }

    if(user_is_an_idiot)
        return 1; // you fucking idiot

    if(asking_help) {
        printf("%suptime-record\e[0m - Keep track of your highest uptime!\n", color);
        printf("\n%sFLAGS\e[0m:\n", color);
        printf("\t%s-h\e[0m, %s--help\e[0m:\t  Print this help menu and exit\n", color, color);
        printf("\t%s-c\e[0m,%s --color\e[0m:\t  Change the output color (default: green)\n"
               "\t\t\t  [\e[30mblack\e[0m, \e[31mred\e[0m, \e[32mgreen\e[0m, \e[33myellow\e[0m, \e[34mblue\e[0m,"
               " \e[35mpurple\e[0m, \e[36mcyan\e[0m, \e[39mwhite\e[0m]\n", color, color);
        printf("\t%s-s\e[0m, %s--silent\e[0m\t  Don't print any output (update-only)\n", color, color);
        printf("\t%s-b\e[0m, %s--background\e[0m: Stay running at a set interval (default: 300)\n\t\t\t  [refresh seconds]\n", color, color);
        return 0;
    }

    char *home = getenv("HOME");
    // this will never need to run, right?
    if(!home) {
        fflush(stdout);
        fputs("\e[31m\e[1mERROR\e[0m:$HOME is not set, interrupting!\n", stderr);
        fflush(stderr);

        return 1;
    }
    if(!home[0]) {
        fflush(stdout);
        fputs("\e[31m\e[1mERROR\e[0m:$HOME is empty, interrupting!\n", stderr);
        fflush(stderr);

        return 1;
    }

    struct sysinfo info;
    sysinfo(&info);
    
    unsigned long current = info.uptime;
    char best[16];
    char path[LOGIN_NAME_MAX+48];
    char *xdg_data_home = getenv("XDG_DATA_HOME");

    if(xdg_data_home) {     // does it exist?
        if(xdg_data_home[0])
            snprintf(path, LOGIN_NAME_MAX+48, "%s/uptime-record", xdg_data_home);
        else
            snprintf(path, LOGIN_NAME_MAX+48, "%s/.local/share/uptime-record", home);
    } else
        snprintf(path, LOGIN_NAME_MAX+48, "%s/.local/share/uptime-record", home);

    if(background) {
        if(background < argc) {
            time = atoi(argv[background]) ? atoi(argv[background]) : DEFAULT_TIME;
        }
        struct sysinfo info;
        sysinfo(&info);

        FILE *fp = fopen(path, "r");
        if(!fp) {   // file didn't open correctly
            fprintf(stderr, "please create a file to store the uptime in (touch %s)", path);
            return -1;
        }
        fclose(fp);
            
        while(true) {
            sysinfo(&info);
            current = info.uptime;

            fp = fopen(path, "r+");
            fgets(best, 10, fp);
            rewind(fp);
                
            if(current > (unsigned long)atol(best)) {
                fprintf(fp, "%ld\n", current);
                if(!is_silent) printf("Updating %s to %ld from %s\n", path, current, best);
            }
            fclose(fp);
            sleep(time);
        }
    }

    FILE *fp = fopen(path, "r+");
    if(!fp) {// file didn't open correctly
        fprintf(stderr, "please create a file to store the uptime in (touch %s)", path);

        return -1;
    }
    
    fgets(best, 10, fp);
    rewind(fp);
    
    if(current > (unsigned long)atol(best)) {
        if(!is_silent) {
            printf("Current uptime: ");
            uptime(current);
            printf("\n%s\e[1mThis is your highest uptime!\e[0m\n\nPrevious highest: ", color);
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
