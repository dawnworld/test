#include <stdio.h>
#include "acsm_debug.h"

static void Print_DFA_MatchList( ACSM_STRUCT2 * acsm, int state )
{
     ACSM_PATTERN2 * mlist;

     for (mlist = acsm->acsmMatchList[state];
          mlist;
          mlist = mlist->next)
     {
        printf("%.*s ", mlist->n, mlist->patrn);
     }
}

/*
*  Write a state table to disk
*/
void Write_DFA(ACSM_STRUCT2 * acsm, char * f)
{
  int  k,i;
  acstate_t * p, n, fmt, index, nb, bmatch;
  acstate_t ** NextState = acsm->acsmNextState;
  FILE * fp;

  printf("Dump DFA - %d active states\n",acsm->acsmNumStates);

  fp = fopen(f,"wb");
  if(!fp)
   {
     printf("WARNING: could not write dfa to file - %s.\n",f);
     return;
   }

  fwrite( &acsm->acsmNumStates, 4, 1, fp);

  for(k=0;k<acsm->acsmNumStates;k++)
  {
    p   = NextState[k];

    if( !p ) continue;

    fmt = *p++;

    bmatch = *p++;

    fwrite( &fmt,    sizeof(acstate_t), 1, fp);
    fwrite( &bmatch, sizeof(acstate_t), 1, fp);

    if( fmt ==ACF_SPARSE )
    {
       n = *p++;
       fwrite( &n,     sizeof(acstate_t), 1, fp);
       fwrite(  p, n*2*sizeof(acstate_t), 1, fp);
    }
    else if( fmt ==ACF_BANDED )
    {
       n = *p++;
       fwrite( &n,     sizeof(acstate_t), 1, fp);

       index = *p++;
       fwrite( &index, sizeof(acstate_t), 1, fp);

       fwrite(  p, sizeof(acstate_t), n, fp);
    }
    else if( fmt ==ACF_SPARSEBANDS )
    {
       nb    = *p++;
       fwrite( &nb,    sizeof(acstate_t), 1, fp);
       for(i=0;i<nb;i++)
       {
         n     = *p++;
         fwrite( &n,    sizeof(acstate_t), 1, fp);

         index = *p++;
         fwrite( &index,sizeof(acstate_t), 1, fp);

         fwrite( p,     sizeof(acstate_t), 1, fp);
       }
    }
    else if( fmt == ACF_FULL )
    {
      fwrite( p,  sizeof(acstate_t), acsm->acsmAlphabetSize,  fp);
    }

    Print_DFA_MatchList( acsm, k);

  }

  fclose(fp);
}

void Print_DFA(ACSM_STRUCT2 * acsm)
{
  int  k,i;
  acstate_t * p, state, n, fmt, index, nb;
  acstate_t ** NextState = acsm->acsmNextState;

  printf("Print DFA - %d active states\n",acsm->acsmNumStates);

  for(k=0;k<acsm->acsmNumStates;k++)
  {
    p   = NextState[k];

    if( !p ) continue;

    fmt = *p++;

    printf("state %3d, fmt=%d: ",k,fmt);

    if( fmt ==ACF_SPARSE )
    {
       n = *p++;
       for( ; n>0; n--, p+=2 )
       {
         if( isascii((int)p[0]) && isprint((int)p[0]) )
         printf("%3c->%-5d\t",p[0],p[1]);
         else
         printf("%3d->%-5d\t",p[0],p[1]);
      }
    }
    else if( fmt ==ACF_BANDED )
    {

       n = *p++;
       index = *p++;

       for( ; n>0; n--, p++ )
       {
         if( isascii((int)p[0]) && isprint((int)p[0]) )
         printf("%3c->%-5d\t",index++,p[0]);
         else
         printf("%3d->%-5d\t",index++,p[0]);
      }
    }
    else if( fmt ==ACF_SPARSEBANDS )
    {
       nb    = *p++;
       for(i=0;(acstate_t)i<nb;i++)
       {
         n     = *p++;
         index = *p++;
         for( ; n>0; n--, p++ )
         {
           if( isascii((int)index) && isprint((int)index) )
           printf("%3c->%-5d\t",index++,p[0]);
           else
           printf("%3d->%-5d\t",index++,p[0]);
         }
       }
    }
    else if( fmt == ACF_FULL )
    {

      for( i=0; i<acsm->acsmAlphabetSize; i++ )
      {
         state = p[i];

         if( state != 0 && state != ACSM_FAIL_STATE2 )
         {
           if( isascii(i) && isprint(i) )
             printf("%3c->%-5d\t",i,state);
           else
             printf("%3d->%-5d\t",i,state);
         }
      }
    }

    Print_DFA_MatchList( acsm, k);

    printf("\n");
  }
}

/*
*    A Match is found
*/
int MatchFound (void* id, void * tree, int index, void *data, void * neg_list)
{
    printf("Found the string\n");
    fprintf (stdout, "%s\n", (char *) id);
    return 1;
}
