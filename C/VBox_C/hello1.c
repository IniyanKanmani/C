#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main() {

    int file;
    //char fname[25];
    char buffer[100];
    char rbuffer[1000];
    int count = 0;

    //printf("Enter Filename : ");
    //scanf("%s", fname);
    file = open("random_text.txt", O_RDONLY, 0777);

    if (file < 0) {

        write(1, "Oops Error1\n", strlen("Oops Error1\n"));
        exit(1);
        
    }

/*
    int x;
    x = read(file, buffer, 100);

    if (x < 0) {

        write(1, "Oops Error2\n", strlen("Oops Error2\n"));
        exit(1);

    } else {

        write(1, buffer, strlen(buffer));

    }

    write(1, "\n", strlen("\n"));
*/

    int z;
    z = read(file, rbuffer, 1);

    while(z > 0) {

        count++;
        printf("%.*s", z, rbuffer);
        z = read(file, rbuffer, 1);

    }

    printf("%d\n", count);

    int y;
    y = close(file);

    if (y < 0) {

        write(1, "Oops Error4\n", strlen("Oops Error4\n"));
        exit(1);

    }

    /*
    int readc;
    readc = read(file, rbuffer, 1000);

    if (readc < 0) {

        write(1, "Oops Error3\n", strlen("Oops Error3\n"));
        exit(1);

    } else {

        write(1, rbuffer, strlen(rbuffer));

    }
    */

    //int rm;
    //rm = remove("myfile.txt");

    //if (rm < 0) {

        //write(1, "Oops Error3\n", strlen("OOps Error3\n"));

    //}

    return 0;

}