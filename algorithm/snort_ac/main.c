#include <stdio.h>
#include "acsmx2.h"
#include "acsm_debug.h"

int main(int argc, char** argv)
{
    printf(" Test DFA algorithm\n");
    char* p = "hello";
    ACSM_STRUCT2 * acsm;
    acsm = acsmNew2(NULL, NULL, NULL);

    if(!acsm)
    {
        printf("acsm-no memory\n");
        exit(0);
    }

    acsm->acsmFormat = ACF_FULL;
    int nocase = 1;
    int s_verbose = 1;

    acsm->acsmFSA = FSA_DFA;

    acsmAddPattern2(acsm, (unsigned char*)p, strlen(p), nocase, 1, 0, 0, (void*)p, 0);
    if(s_verbose) printf("Patterns added \n");

    Print_DFA(acsm);

    acsmCompile2(acsm, NULL, NULL);

    Write_DFA(acsm, "acsmx2-snort.dfa");

    if(s_verbose) printf("Patterns compiled -- writtern to file.\n");

    acsmPrintInfo2(acsm);

    //acsm->acsmMatchList = &acsm->acsmPatterns;
    char text[512] = "world hello";
    int start_state = 0;
    int result = acsmSearch2(acsm, (unsigned char *)text, strlen(text), MatchFound, (void *)0, &start_state);
    printf(" acsm search result:%d, text:%s\n", result, text);

    acsmFree2(acsm);
    printf("normal pgm end\n");

    return 0;
}
