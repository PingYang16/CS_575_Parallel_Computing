#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int     NowYear;      // 2022 - 2027
int     NowMonth;     // 0 - 11

float   NowPrecip;    // inches of rain per month
float   NowTemp;      // temperature this month
float   NowHeight;    // grain height in inches
int     NowNumDeer;   // number of deer in the current population
int     NowNumWolf;  // number of my agent in the current population

const float GRAIN_GROWS_PER_MONTH =          9.0;
const float ONE_DEER_EATS_PER_MONTH =        1.0;

const float AVG_PRECIP_PER_MONTH =           7.0;    // average
const float AMP_PRECIP_PER_MONTH =           6.0;    // plus or minus
const float RANDOM_PRECIP =                  2.0;    // plus or minus noise

const float AVG_TEMP =                       60.0;    // average
const float AMP_TEMP =                       20.0;    // plus or minus
const float RANDOM_TEMP =                    10.0;    // plus or minus noise

const float MIDTEMP =                        40.0;
const float MIDPRECIP =                      10.0;

unsigned int seed = 0;

float
Ranf( unsigned int *seedp, float low, float high )
{
    float r = (float) rand_r( seedp );    // 0 - RAND_MAX
    return( low + r * ( high - low ) / (float)RAND_MAX );
}

int
Ranf( unsigned int *seedp, int ilow, int ihigh )
{
    float low = (float)ilow;
    float high = (float)ihigh + 0.9999f;
    
    return (int)( Ranf(seedp, low, high) );
}

float
SQR ( float x )
{
    return x*x;
}

void
Watcher()
{
    while( NowYear < 2028 )
    {
        // do nothing
        #pragma omp barrier
        
        // do nothing
        #pragma omp barrier
        
        // write out the now state of data
        float TempDegreeC = (5./9.) * (NowTemp - 32);
        float PrecipCm = NowPrecip * 2.54;
        
        printf("%d, %2d, %6.2f, %6.2f, %6.2f, %2d, %2d\n", NowYear, NowMonth+1, TempDegreeC, PrecipCm, NowHeight*2.54, NowNumDeer, NowNumWolf);
        
        // advance time and re-compute all environmental variables
        float ang = ( 30.*(float)NowMonth + 15. ) * ( M_PI / 180. );
        
        float temp = AVG_TEMP - AMP_TEMP * cos( ang );
        NowTemp = temp + Ranf( &seed, -RANDOM_PRECIP, RANDOM_PRECIP );
        
        float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
        NowPrecip = precip + Ranf( &seed, -RANDOM_PRECIP, RANDOM_PRECIP );
        if( NowPrecip < 0. )
            NowPrecip = 0.;
        
        NowMonth++;
        if( NowMonth > 11 )
        {
            NowYear++;
            NowMonth = 0;
        }
        
        #pragma omp barrier
    }
}

void
Deer()
{
    while( NowYear < 2028 )
    {
        int nextNumDeer = NowNumDeer;
        int carryingCapacity = (int)( NowHeight );
        
        if( nextNumDeer * 1/2 < NowNumWolf)
            nextNumDeer--;
        
        if( nextNumDeer < carryingCapacity )
            nextNumDeer++;
        else if( nextNumDeer > carryingCapacity )
                nextNumDeer--;
        if( nextNumDeer < 0 )
            nextNumDeer = 0;
        
        #pragma omp barrier
        
        NowNumDeer = nextNumDeer;
        
        #pragma omp barrier
        
        #pragma omp barrier
    }
}

void
Grain()
{
    while( NowYear < 2028 )
    {
        float nextHeight = NowHeight;
        
        float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );

        float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );
        
        nextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
        nextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;

        if( nextHeight < 0. )
            nextHeight = 0.;
        
        #pragma omp barrier
        
        NowHeight = nextHeight;
        
        #pragma omp barrier
        
        #pragma omp barrier
    }
}

void
Wolf()
{
    while( NowYear < 2028 )
    {
        int nextNumWolf = NowNumWolf;
        if( (nextNumWolf > 0) && (nextNumWolf < NowNumDeer * 1/10) )
        {
            nextNumWolf++;
        }
        else if( (nextNumWolf > NowNumDeer * 1/10) && (nextNumWolf < NowNumDeer * 2/5) )
        {
            nextNumWolf--;
        }
        else if( nextNumWolf > NowNumDeer * 1/2 )
        {
            nextNumWolf--;
        }
        else
        {
            nextNumWolf++;
        }
        
        if( nextNumWolf > 3 )
            nextNumWolf--;
        if( nextNumWolf < 0 )
            nextNumWolf = 0;
        
        #pragma omp barrier
        
        NowNumWolf = nextNumWolf;
        
        #pragma omp barrier
        
        #pragma omp barrier
    }
}


int
main( int argc, char *argv[] )
{
    // starting date and time;
    NowMonth =    0;
    NowYear  = 2022;
    
    // starting state
    NowNumDeer = 5;
    NowHeight  = 10;
    NowNumWolf = 2;
    
    float ang = ( 30.*(float)NowMonth + 15. ) * ( M_PI / 180. );
    
    float temp = AVG_TEMP - AMP_TEMP * cos( ang );
    NowTemp = temp + Ranf( &seed, -RANDOM_PRECIP, RANDOM_PRECIP );
    
    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
    NowPrecip = precip + Ranf( &seed, -RANDOM_PRECIP, RANDOM_PRECIP );
    if( NowPrecip < 0. )
        NowPrecip = 0.;
    
    omp_set_num_threads( 4 );
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            Deer();
        }
        #pragma omp section
        {
            Grain();
        }
        #pragma omp section
        {
            Watcher();
        }
        #pragma omp section
        {
            Wolf();
        }
    }
    
    return 0;
}
