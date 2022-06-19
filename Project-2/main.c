#include <stdio.h>

#define R_MAX 2147483647
static long long unsigned int SEED = 0x1;
void srnd(int seed){
    SEED=seed;
}
int rnd(int  from, int to){
    SEED = SEED * 16807 % R_MAX;
    return from + (int) SEED % (to - from + 1);
}
int main() {
    int seed,s,t,n,f,swap;
    int N[106];
    int Nmin=10,Nmax=100;

    scanf("%d %d %d", &s, &t, &n);
    srnd(seed);

    if(seed>0 && s >= Nmin && s <= Nmax && (t + n) <= s / 2){
        int M1[t];
        int M2[n];
        for(int i=0; i < t; i++)
        {
            M1[i]=0;
        }
        for(int i=0; i < n; i++)
        {
            M2[i]=0;
        }
        printf("BLOCK:");

        //ukladanie do pola//
        int pozicia;
        for(int i=0; i < s + 6; i++)
        {
            N[i]=0;
        }
        for(int i=0; i < t; i++)
        {
            M1[i]=rnd(1, s - 1);
            if(i==0)
            {
                pozicia=M1[i];
                N[pozicia]=25;
            }
            else if(i>0)
            {
                for(int j=0;j<i;j++)
                {
                    if(M1[i]!=M1[j])
                    {
                        pozicia=M1[i];
                        N[pozicia]=25;
                    }
                    else{
                        i--;
                        break;
                    }
                }
            }
        }
        //vypis pola
        while(1){
            swap=0;
            for(int i=0; i < t - 1; i++)
            {
                if(M1[i]>M1[i+1])
                {
                    f=M1[i];
                    M1[i]=M1[i+1];
                    M1[i+1]=f;
                    swap=1;
                }
            }
            if(swap==0){
                break;
            }
        }
        printf("\n");
        for(int i=0; i < t; i++){
            printf("%d ",M1[i]);
        }

        //BOOST//
        printf("\n");
        printf("BOOST:");

        for(int i=0; i < n; i++)
        {
            M2[i]=rnd(1, s - 1);

            for(int j=0; j < t; j++)
            {
                if(M2[i]!=M1[j])
                {
                    pozicia=M2[i];
                    N[pozicia]=25;
                    for(int k=0;k<i;k++)
                    {
                        if(M2[i]!=M2[k])
                        {
                            pozicia=M2[i];
                            N[pozicia]=26;
                        }
                        else
                        {
                            i--;
                            break;
                        }
                    }
                }
                else
                {
                    pozicia=M2[i];
                    N[pozicia]=25;
                    i--;
                    break;
                }
            }
        }
        //vypis pola
        while(1){
            swap=0;
            for(int i=0; i < n - 1; i++)
            {
                if(M2[i]>M2[i+1])
                {
                    f=M2[i];
                    M2[i]=M2[i+1];
                    M2[i+1]=f;
                    swap=1;
                }
            }
            if(swap==0){
                break;
            }
        }
        printf("\n");
        for(int i=0; i < n; i++){
            printf("%d ",M2[i]);
        }
        /*for (int i = 0; i < s; i++) {
            printf("%d=%d \s\s", i,N[i]);
        }
        printf("\s");*/
        //závod
        int hod_kockou1,hod_kockou2;
        int sucet_pozicii=0,hrac1=1,hrac2=2,cislo_tahu=1,d=0;
        int pozicia1=-1,pht[106],nova_pht[106],boost1=0,bht[106],novy_bht[106];
        int pozicia2=-1,pot[106],nova_pot[106],boost2=0,bot[106],novy_bot[106];
        int spolu=0;
        int i=0;
        while(sucet_pozicii < s)
        {

            while(pozicia1 < s)
            {
                hod_kockou1=rnd(1,6);
                hod_kockou2=rnd(1,6);
                pozicia1=hod_kockou1+hod_kockou2;
            }
            while(pozicia2 < s)
            {
                hod_kockou1=rnd(1,6);
                hod_kockou2=rnd(1,6);
                pozicia2=hod_kockou1+hod_kockou2;
            }
            sucet_pozicii=pozicia1+pozicia2;
            printf("[%d %d] [%d %d] [%d %d] [%d %d]\n",cislo_tahu,hrac1,pht[pozicia1],bht[boost1],hod_kockou1,hod_kockou2,nova_pht[pozicia1],novy_bht[boost1]);
        }
        if(pozicia1!=0)
        {
            hod_kockou1=rnd(1,6);
            hod_kockou2=rnd(1,6);
            spolu=hod_kockou1+hod_kockou2;
            if(spolu>7)
            {
                pozicia1=(spolu-7);
            }
        }

    }
    else{
        return 1;
    }

    return 0;
}
