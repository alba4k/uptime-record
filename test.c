#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("%s/.config\n", getenv("HOME"));
}