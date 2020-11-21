#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {

    printf("Hello World!\n");

    write(1, "Hello World\n", strlen("Hello World\n"));

    return 0;

}