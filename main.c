
/*  file:   main.c
 *  date:   2019-09-05
 *          2019-09-06
 *  author: Peter Lapets
 *
 *  description:
 *  This file contains code which tests the four assigned problem functions
 *  and demonstrates their use.
 */

#include <stdio.h>

#include "diamond.h"

/* obviously nonsense, just for testing. */
static int      G_DATA_TSTAMP[]     = {   0,   0,   5,   4,   0,   0,   5,   5,    5,   4,    0,   5 };
static double   G_DATA_ACCEL_X[]    = { 0.0, 1.1, 1.2, 0.0, 0.3, 0.5, 1.1, 1.3, -1.4, 1.1, -0.2, 0.0 };

static double   G_DATA_OMEGA_X[]    = { 0.0, 1.0, 2.1, 3.0, 4.0, 5.0, 4.0, 3.0,  2.0, 1.0,  0.0, 0.0 };
static double   G_DATA_OMEGA_Y[]    = { 0.0, 1.1, 8.2, 0.0, 7.3, 7.5, 7.1, 7.3,  7.4, 1.1, -0.2, 0.0 };

/* refer to diamond.h */
static struct SWING_DATA G_DATA = {
    12,
    {
        G_DATA_TSTAMP,
        G_DATA_ACCEL_X,
        NULL,
        NULL,
        G_DATA_OMEGA_X,
        G_DATA_OMEGA_Y,
        NULL
    }
};

void test_searchContinuityAboveValue ( void )
{
    int thres_i = 3;
    double thres_d = 0.0;

    assert( 2 == searchContinuityAboveValue(&G_DATA, SWD_TIMESTAMP, 0, 11, &thres_i, 1) );
    assert( 2 == searchContinuityAboveValue(&G_DATA, SWD_TIMESTAMP, 0, 11, &thres_i, 2) );
    assert( 6 == searchContinuityAboveValue(&G_DATA, SWD_TIMESTAMP, 0, 11, &thres_i, 3) );
    assert( 6 == searchContinuityAboveValue(&G_DATA, SWD_TIMESTAMP, 0, 11, &thres_i, 4) );
    assert(-1 == searchContinuityAboveValue(&G_DATA, SWD_TIMESTAMP, 0, 11, &thres_i, 5) );

    assert( 1 == searchContinuityAboveValue(&G_DATA, SWD_ACCEL_X, 0, 11, &thres_d, 1) );
    assert( 1 == searchContinuityAboveValue(&G_DATA, SWD_ACCEL_X, 0, 11, &thres_d, 2) );
    assert( 4 == searchContinuityAboveValue(&G_DATA, SWD_ACCEL_X, 0, 11, &thres_d, 3) );
    assert( 4 == searchContinuityAboveValue(&G_DATA, SWD_ACCEL_X, 0, 11, &thres_d, 4) );
    assert(-1 == searchContinuityAboveValue(&G_DATA, SWD_ACCEL_X, 0, 11, &thres_d, 5) );
}

void test_backSearchContinuityWithinRange ( void )
{
    int thres_i_lo = 4;
    int thres_i_hi = 6;
    
    double thres_d_lo = 0.0;
    double thres_d_hi = 1.4;

    assert(11 == backSearchContinuityWithinRange(&G_DATA, SWD_TIMESTAMP, 11, 0, &thres_i_lo, &thres_i_hi, 1) );
    assert( 8 == backSearchContinuityWithinRange(&G_DATA, SWD_TIMESTAMP, 11, 0, &thres_i_lo, &thres_i_hi, 2) );
    assert( 8 == backSearchContinuityWithinRange(&G_DATA, SWD_TIMESTAMP, 11, 0, &thres_i_lo, &thres_i_hi, 3) );
    thres_i_lo = 3;
    assert( 9 == backSearchContinuityWithinRange(&G_DATA, SWD_TIMESTAMP, 11, 0, &thres_i_lo, &thres_i_hi, 4) );
    assert(-1 == backSearchContinuityWithinRange(&G_DATA, SWD_TIMESTAMP, 11, 0, &thres_i_lo, &thres_i_hi, 5) );

    assert( 9 == backSearchContinuityWithinRange(&G_DATA, SWD_ACCEL_X, 11, 0, &thres_d_lo, &thres_d_hi, 1) );
    assert( 7 == backSearchContinuityWithinRange(&G_DATA, SWD_ACCEL_X, 11, 0, &thres_d_lo, &thres_d_hi, 2) );
    assert( 7 == backSearchContinuityWithinRange(&G_DATA, SWD_ACCEL_X, 11, 0, &thres_d_lo, &thres_d_hi, 3) );
    assert( 7 == backSearchContinuityWithinRange(&G_DATA, SWD_ACCEL_X, 11, 0, &thres_d_lo, &thres_d_hi, 4) );
    assert(-1 == backSearchContinuityWithinRange(&G_DATA, SWD_ACCEL_X, 11, 0, &thres_d_lo, &thres_d_hi, 5) );
}

void test_searchContinuityAboveValueTwoSignals ( void )
{
    double thres_d_1 = 2.0;
    double thres_d_2 = 7.0;

    assert( 2 == searchContinuityAboveValueTwoSignals(&G_DATA, SWD_OMEGA_X, SWD_OMEGA_Y, 0, 11, &thres_d_1, &thres_d_2, 1));
    assert( 4 == searchContinuityAboveValueTwoSignals(&G_DATA, SWD_OMEGA_X, SWD_OMEGA_Y, 0, 11, &thres_d_1, &thres_d_2, 2));
    assert( 4 == searchContinuityAboveValueTwoSignals(&G_DATA, SWD_OMEGA_X, SWD_OMEGA_Y, 0, 11, &thres_d_1, &thres_d_2, 3));
    assert( 4 == searchContinuityAboveValueTwoSignals(&G_DATA, SWD_OMEGA_X, SWD_OMEGA_Y, 0, 11, &thres_d_1, &thres_d_2, 4));
    assert(-1 == searchContinuityAboveValueTwoSignals(&G_DATA, SWD_OMEGA_X, SWD_OMEGA_Y, 0, 11, &thres_d_1, &thres_d_2, 5));
}

void test_searchMultiContinuityWithinRange ( void )
{
    struct SWING_RANGE_LIST * sw_ranges;
    struct SWING_RANGE_LIST * temp;
    int thres_i_lo = 3;
    int thres_i_hi = 6;
    
    sw_ranges = searchMultiContinuityWithinRange(&G_DATA, SWD_TIMESTAMP, 0, 11, &thres_i_lo, &thres_i_hi, 1);

    printf("searchMultiContinuityWithinRange: easier to hand-check this one.\n");
    while (NULL != sw_ranges)
    {
        printf("found from %d-%d\n", sw_ranges->indexBegin, sw_ranges->indexEnd);
        temp = sw_ranges;
        sw_ranges = sw_ranges->next;

        free(temp);
    }
}


int main ( void )
{
    test_searchContinuityAboveValue();
    test_backSearchContinuityWithinRange();
    test_searchContinuityAboveValueTwoSignals();
    printf("Passed all assert() tests!\n\n");

    test_searchMultiContinuityWithinRange();

    return (0);
}

