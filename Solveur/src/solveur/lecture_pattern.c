#include "lecture_pattern.h"

int nbrgood(char* motif){
    int i;
    count = 0;
    for (i = 0; i < strlen(motif); i++)
    {
        if(str[i]==ch){
            count++;
        }
    }
    return count;    
}