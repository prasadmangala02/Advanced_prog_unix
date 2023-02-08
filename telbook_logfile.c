// Name - Mangala Prasad Sahu
// Regno - 22560
// Class - I Mtech CS

// Compile using - gcc -o tel telbook.c
// Run using - ./tel

// A program that provides the following functionalities

// 1. Creating a telbook file with user specified name and size. The size of each file entry is 40bytes
// 2. On user providing index number, the program shows the entry corresponding to that index number.
// 3. On user providing index number and entry (name + telephone number) the program writes the entry at the specified index in the telbook.
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "apue.h"

int main(int argc, char *argv[]){

    
    int fd,lfd, size, opt, index, sizeNew;
    char buf1[] = "Hello World";
    char *resbuf;
    char filename[20] ;
    char logfile_name[20] = "logfile";
    char name[20], number[20];
    char entry[40];
    printf("Enter the file name: ");
    scanf("%s", filename);
    printf("Enter the file size: ");
    scanf("%d", &size);
    // printf("size : %d", size);

    sizeNew = size;
    size *= 40;
    printf("size : %d", size);

    // Structure for logfile entry
    // struct logBookEntry{
    //     char time[30];
    //     char gap[2];
    //     char action[10];
    // };

    struct logBookEntry{
        char time[24];
        char action[6];
    };

    if((fd = open(filename, O_RDWR | O_CREAT | O_EXCL, FILE_MODE)) == -1)
        {
            printf("\nCreat Error 1");
            if(errno == EEXIST){
                printf("\nFile exists!!!");
                if((fd = open(filename, O_RDWR, FILE_MODE)) == -1){
                    printf("\nCreate Error 2");
                }
            }
        }

    // Logbook implementation
    if((lfd = open(logfile_name, O_WRONLY | O_CREAT | O_APPEND | O_EXCL, FILE_MODE) ) == -1){
        printf("\nlogfile create error");
        if(errno == EEXIST){
            printf("\nfile exists");
            if((lfd = open(logfile_name, O_WRONLY | O_APPEND, FILE_MODE)) == -1){
                    printf("\nCreate Error 2");
            }
        }
    }



    else {
        lseek(fd, size, SEEK_END);
        write(fd, "\0", 1);

    }

    // If the file is empty
    //  if(lseek(fd, 0, SEEK_SET) == lseek(fd, 0, SEEK_END)){
    //         lseek(fd, size, SEEK_END);
    //         write(fd, "\0", 1);
    //     }
    // printf("size:%d, %d", size, fd);

    // if (lseek(fd, size, SEEK_SET) == -1)
    //     printf("\nlseek Error 1 ");
    
    printf("\nWelcome to Telbook");

    printf("\nEnter 1 for creating an entry in the phone book ");
    printf("\nEnter 2 for searching an entry in the phone book ");
    printf("\nEnter 3 for exit");
    printf("\n >>>");

    // Take the choice from the user
    scanf("%d", &opt);
    while(1){


    switch(opt){
        ///////////////////////////// Creating an entry in the telbook/////////////////////////////////////
        case 1:
        printf("\nInsert a phone number entry >>>");
        printf("\nEnter an index number >>>");
        scanf("%d", &index);

        if(index > sizeNew){
            printf("\nIndex out of range!!! (While creating)\n");
            break;
        }

        printf("\nEnter the name >>>");
        scanf("%s", name);
        printf("\nEnter the phone number >>>");
        scanf("%s", number);

        if (lseek(fd, (index - 1)*40, SEEK_SET) == -1)
        printf("\nlseek Error 1 ");

        if (write(fd, name, 20) == -1)
        printf("\nwrite Error 1");

        if (write(fd, number, 20) == -1)
        printf("\nwrite Error 2");

        if (lseek(fd, 0, SEEK_SET) == -1)
        printf("\nlseek Error 2");

        struct logBookEntry l;

        time_t t = time(NULL);
        // time(&t);
        struct tm tm = *localtime(&t);
        
        // printf("time: %s", asctime(&tm));
        // struct logBookEntry l = {asctime(&tm), '|', "write"};
        // printf("time: %s", l.time);

        strcpy(l.time, asctime(&tm));
        strcpy(l.action, "read");
        // if(write(lfd, &l, sizeof(struct logBookEntry)) == -1)
        dprintf(lfd, "%s : %s : %s : %s : %s\n ",l.time, l.action, filename, name, number);
        // printf("time: %s", l.time);

        break;

        case 2:

        ///////////////////////////// Searching for an entry in the telbook/////////////////////////////////////

        // If the file doesn't contain any entries
        if(lseek(fd, 0, SEEK_SET) == lseek(fd, 0, SEEK_END)){
            printf("\ntelBook is empty!!!");
            break;
        }

        printf ("\nSearch an entry in the Telbook using index number...");
        printf("\nEnter the index number >>>");
        scanf("%d", &index);

        if(index > sizeNew){
            printf("\nIndex out of range!!! (While searching)\n");
            break;
        }
        
        
        if (lseek(fd, 0, SEEK_SET) == -1)
        printf("\nlseek Error 3");

        if (lseek(fd, (index - 1)*40, SEEK_SET) == -1)
        printf("\nlseek Error 4");

        if (read(fd, name, 20) == -1)
        printf("\nread Error 1");

        if (read(fd, number, 20) == -1)
        printf("\nread Error 2");

        printf("\nName : %s", name);
        printf("\nNumber : %s", number);

        break;


        ///////////////////////////// Exit code /////////////////////////////////////

        case 3:
        printf("\nExiting");
        return 0; 

        default:
        printf("\nInvalid choice");
        return 0;
        

    }

    printf("\nEnter 1 for creating an entry in the phone book ");
    printf("\nEnter 2 for searching an entry in the phone book ");
    printf("\nEnter 3 for exit");
    printf("\n >>>");
    scanf("%d", &opt);

    }

    close(fd);
    exit(0);
}