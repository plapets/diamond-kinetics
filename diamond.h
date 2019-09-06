#ifndef DIAMOND_H
#define DIAMOND_H

/*  file:   diamond.h
 *  date:   2019-09-05
 *          2019-09-06
 *  author: Peter Lapets
 *
 *  description:
 *  This file declares the interface to the four problem functions, as well
 *  as declaring various data structures for passing swing data or returning
 *  found ranges:
 *
 *  enum SWING_DATA_COLUMN  used to select columns for processing
 *  struct SWING_DATA       stores data length and pointers to column data
 *  struct SWING_RANGE_LIST singly-linked list for returning ranges of data 
 */

#include <math.h>
#include <assert.h>
#include <stdlib.h>

#define E_CONTINUITY_NOT_FOUND  (-1)

enum SWING_DATA_COLUMN
{
    SWD_TIMESTAMP = 0,
    
    SWD_ACCEL_X,
    SWD_ACCEL_Y,
    SWD_ACCEL_Z,

    SWD_OMEGA_X,
    SWD_OMEGA_Y,
    SWD_OMEGA_Z,

    SWD_N_COLUMNS
};

struct SWING_DATA
{
    int length;

    void * columns[SWD_N_COLUMNS];
};

/* Singly-linked list holds all ranges returned from
 * searchMultiContinuityWithinRange().
 */
struct SWING_RANGE_LIST
{
    int indexBegin;
    int indexEnd;

    struct SWING_RANGE_LIST * next;
};


/* Standard comparator interface for each table column. */
#define LESS    (-1)
#define EQUAL   ( 0)
#define GREATER ( 1)

#define EPSILON 0.0001

typedef int (* Comparator) (void * item1, int index1,
                            void * item2, int index2 );

/* note:    it's a matter of debate whether to
 *          make these "static" in the .c file...
 */
int ComparatorInt   ( void * item1, int index1, void * item2, int index2 );
int ComparatorDouble( void * item1, int index1, void * item2, int index2 );


/* Requested challenge functions. */
int searchContinuityAboveValue      (   struct SWING_DATA *     data,
                                        enum SWING_DATA_COLUMN  dataColumn,
                                        int                     indexBegin,
                                        int                     indexEnd,
                                        void *                  threshold,
                                        int                     winLength   );

int backSearchContinuityWithinRange (   struct SWING_DATA *     data,
                                        enum SWING_DATA_COLUMN  dataColumn,
                                        int                     indexBegin,
                                        int                     indexEnd,
                                        void *                  thresholdLo,
                                        void *                  thresholdHi,
                                        int                     winLength   );

int searchContinuityAboveValueTwoSignals(
                                        struct SWING_DATA *     data,
                                        enum SWING_DATA_COLUMN  dataColumn1,
                                        enum SWING_DATA_COLUMN  dataColumn2,
                                        int                     indexBegin,
                                        int                     indexEnd,
                                        void *                  threshold1,
                                        void *                  threshold2,
                                        int                     winLength   );

struct SWING_RANGE_LIST * searchMultiContinuityWithinRange(
                                        struct SWING_DATA *     data,
                                        enum SWING_DATA_COLUMN  dataColumn,
                                        int                     indexBegin,
                                        int                     indexEnd,
                                        void *                  thresholdLo,
                                        void *                  thresholdHi,
                                        int                     winLength   );

#endif /* DIAMOND_H */

