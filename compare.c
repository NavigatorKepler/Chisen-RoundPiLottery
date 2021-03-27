#define READSIZE         32*1024*1024
#define WINDOWSIZE       9
#define UIDWIDTH         1000
#define UIDLENGTH        32
#define PROGRESSGAP      1000
// UIDWIDTH是总uid数, UIDLENGTH是每行uid最大长度

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

const char* PiPath="Pi - Dec - Chudnovsky.txt";
const char* RepostPath="reposts.txt";
const char* LogPath="log.txt";

size_t global_pointer=0;
uint64_t global_counter=0;
size_t  epoch_pointer=0;
uint64_t*        uidarray;
uint64_t        __tempuid;
uint64_t     __uidcount=0;
uint64_t    __readcount=0;
uint8_t*     __readresult;
size_t         __readstat;
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

    FILE* fp_log = fopen(LogPath, "w+");
    if (fp_log == NULL) {
        printf("Cannot open %s.", LogPath);
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
    printf("PROGRESSGAP: %d\n\n", PROGRESSGAP);

    while (1) {
        __readstat = fread(&FileBuffer_T[WINDOWSIZE], sizeof(char), READSIZE, fp_pi);
        __readcount++;
        if ((__readstat < READSIZE)&&(__readstat > 0)) {
            printf("Process Tailing.      \n");
        } else if (__readstat == 0) {
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
                if (global_pointer % PROGRESSGAP == 0) {
                    printf("Read: %d, Current: %09d, Pointer: %d              \r", __readcount, __tempuid, global_pointer);
                }
                for (int i=0;i<__uidcount;i++){
                    if (uidarray[i] == __tempuid) {
                        fprintf(fp_log, "Hit %d: %9d, Pointer: %d\n", global_counter, __tempuid, global_pointer-1);
                        fflush(fp_log);
                        printf("Hit %d: %9d, Pointer: %d              \n", global_counter, __tempuid, global_pointer-1);
                        global_counter++;
                        break;
                    }
                }
                epoch_pointer++;
                global_pointer++;

                if ((__readstat < READSIZE)&&(epoch_pointer>=__readstat)) {
                    break;
                }
            }
        }
        __firstrun=0;
        epoch_pointer=WINDOWSIZE;
        while (epoch_pointer < READSIZE) {
            memset(Window,0,WINDOWSIZE+1);
            memcpy(Window, &FileBuffer_T[epoch_pointer], WINDOWSIZE);
            sscanf(Window, "%d", &__tempuid);
            if (global_pointer % PROGRESSGAP == 0) {
                printf("Read: %d, Current: %09d, Pointer: %d              \r", __readcount, __tempuid, global_pointer);
            }
            for (int i=0;i<__uidcount;i++){
                if (uidarray[i] == __tempuid) {
                    fprintf(fp_log, "Hit %d: %9d, Pointer: %d\n", global_counter, __tempuid, global_pointer-1);
                    fflush(fp_log);
                    printf("Hit %d: %9d, Pointer: %d              \n", global_counter, __tempuid, global_pointer-1);
                    global_counter++;
                    break;
                }
            }
            epoch_pointer++;
            global_pointer++;

            if ((__readstat < READSIZE)&&(epoch_pointer>=__readstat)) {
                break;
            }
        }
    }


    printf("Process Finished.      \n");
    fclose(fp_log);
    fclose(fp_pi);
    fclose(fp_uid);
    return 0;
}
