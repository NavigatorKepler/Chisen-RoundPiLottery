#define READSIZE     50000000
#define WINDOWSIZE          9
#define UIDWIDTH         1000
#define UIDLENGTH          32
// UIDWIDTH是总uid数, UIDLENGTH是每行uid最大长度

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

const char* PiPath="Pi - Dec - Chudnovsky.txt";
const char* RepostPath="reposts.txt";

uint64_t global_pointer=0;
uint64_t global_counter=0;
uint32_t  epoch_pointer=0;
uint64_t*        uidarray;
uint64_t        __tempuid;
uint64_t     __uidcount=0;
uint8_t*     __readresult;
uint8_t      __firstrun=1;

char FileBuffer_T[READSIZE+WINDOWSIZE+1];
char Window[WINDOWSIZE+1];

int main() {
    FILE* fp_pi = fopen(PiPath, "r");
    if (fp_pi == NULL) {
        printf("Cannot find %s.", PiPath);
        return -1;
    }

    FILE* fp_uid = fopen(RepostPath, "r");
    if (fp_uid == NULL) {
        printf("Cannot find %s.", RepostPath);
        return -1;
    }

    uidarray=(uint64_t*)malloc(sizeof(uint64_t)*(UIDWIDTH+1));
    while (fgets(FileBuffer_T, UIDLENGTH, fp_uid) != NULL) {
        sscanf(FileBuffer_T, "%d", &__tempuid);
        uidarray[__uidcount]=__tempuid;
        __uidcount++;
    }

    printf("Load UID: %d\n", __uidcount);
    printf("WindowSize: %d\n", WINDOWSIZE);
    printf("READSIZE: %d\n", READSIZE);

    while (1) {
        __readresult = fgets(&FileBuffer_T[WINDOWSIZE], READSIZE, fp_pi);
        if (__readresult == NULL) {
            printf("Process Finished.\n");
            break;
        }

        // 先处理越界的情况
        if (__firstrun == 0) {
            epoch_pointer=0;
            memcpy(FileBuffer_T, &FileBuffer_T[READSIZE], WINDOWSIZE);
            while (epoch_pointer < WINDOWSIZE) {
                memset(Window,0,WINDOWSIZE+1);
                memcpy(Window, &FileBuffer_T[epoch_pointer], WINDOWSIZE);
                sscanf(Window, "%d", &__tempuid);
                printf("Current: %010d, Pointer: %d|\t\r", __tempuid, global_pointer);
                for (int i=0;i<__uidcount;i++){
                    if (uidarray[i] == __tempuid) {
                        printf("Hit %d: %d, Pointer:%d|\t\n", global_counter, __tempuid, global_pointer-1);
                        global_counter++;
                        break;
                    }
                }
                epoch_pointer++;
                global_pointer++;
            }
        }
        __firstrun=0;
        epoch_pointer=WINDOWSIZE;
        while (epoch_pointer < READSIZE) {
            memset(Window,0,WINDOWSIZE+1);
            memcpy(Window, &FileBuffer_T[epoch_pointer], WINDOWSIZE);
            sscanf(Window, "%d", &__tempuid);
            printf("Current: %010d, Pointer: %d|\t\r", __tempuid, global_pointer);
            for (int i=0;i<__uidcount;i++){
                if (uidarray[i] == __tempuid) {
                    printf("Hit %d: %d, Pointer:%d|\t\n", global_counter, __tempuid, global_pointer-1);
                    global_counter++;
                    break;
                }
            }
            epoch_pointer++;
            global_pointer++;
        }
    }


    fclose(fp_pi);
    fclose(fp_uid);
    return 0;
}
