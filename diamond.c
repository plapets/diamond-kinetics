
/*  file:   diamond.c
 *  date:   2019-09-05
 *          2019-09-06
 *  author: Peter Lapets
 *
 *  description:
 *  This file implements the four problem functions, as well as some helper
 *  functions.
 *
 *  It also contains an array which associates each column with the approp-
 *  riate function to call in order to compare the data.
 */

#include "diamond.h"

/* Associate each SWING_DATA_COLUMN with a Comparator.
 * This way we don't have to care about the type of any column data as long
 * as the table format is correct.  We can add new columns without changing
 * algorithms as long as we add the appropriate Comparator.
 */
static Comparator SwingDataComparator[SWD_N_COLUMNS] = {
    ComparatorInt,      /* timestamp    */
    
    ComparatorDouble,   /* accel_x      */
    ComparatorDouble,   /* accel_y      */
    ComparatorDouble,   /* accel_z      */

    ComparatorDouble,   /* omega_x      */
    ComparatorDouble,   /* omega_y      */
    ComparatorDouble,   /* omega_z      */
};

int ComparatorInt   ( void * item1, int index1, void * item2, int index2 )
{
    int diff;
    int result;

    diff = ((int *) item1)[index1] - ((int *) item2)[index2];

    if      (diff < 0)  result = LESS;
    else if (diff > 0)  result = GREATER;
    else                result = EQUAL;

    return (result);
}

int ComparatorDouble( void * item1, int index1, void * item2, int index2 )
{
    double diff;
    int result;

    diff = ((double *) item1)[index1] - ((double *) item2)[index2];

    if (fabs(diff) < EPSILON)   result = EQUAL;
    else if (diff < 0)          result = LESS;
    else                        result = GREATER;

    return (result);
}


int searchContinuityAboveValue      (   struct SWING_DATA *     data,
                                        enum SWING_DATA_COLUMN  dataColumn,
                                        int                     indexBegin,
                                        int                     indexEnd,
                                        void *                  threshold,
                                        int                     winLength   )
{
    int foundIndex  = E_CONTINUITY_NOT_FOUND;
    int matchLength = 0;
    int currentIndex;

    assert(NULL != data->columns[dataColumn]);

    assert(indexBegin >= 0);
    assert(indexEnd < data->length);

    for (currentIndex = indexBegin; currentIndex <= indexEnd; ++currentIndex)
    {
        if (GREATER == SwingDataComparator[dataColumn](data->columns[dataColumn], currentIndex, threshold, 0))
        {
            matchLength++;
        }
        else
        {
            matchLength = 0;
        }

        if (matchLength >= winLength)
        {
            foundIndex = currentIndex - matchLength + 1;
            break;
        }
    }

    return (foundIndex);
}

int backSearchContinuityWithinRange (   struct SWING_DATA *     data,
                                        enum SWING_DATA_COLUMN  dataColumn,
                                        int                     indexBegin,
                                        int                     indexEnd,
                                        void *                  thresholdLo,
                                        void *                  thresholdHi,
                                        int                     winLength   )
{
    int foundIndex  = E_CONTINUITY_NOT_FOUND;
    int matchLength = 0;
    int currentIndex;

    assert(NULL != data->columns[dataColumn]);

    assert(indexEnd >= 0);
    assert(indexBegin < data->length);

    for (currentIndex = indexBegin; currentIndex >= indexEnd; --currentIndex)
    {
        if  (
                (GREATER == SwingDataComparator[dataColumn](data->columns[dataColumn], currentIndex, thresholdLo, 0))
            &&  (   LESS == SwingDataComparator[dataColumn](data->columns[dataColumn], currentIndex, thresholdHi, 0))
            )
        {
            matchLength++;
        }
        else
        {
            matchLength = 0;
        }

        if (matchLength >= winLength)
        {
            foundIndex = currentIndex + matchLength - 1;
            break;
        }
    }

    return (foundIndex);
}

int searchContinuityAboveValueTwoSignals(
                                        struct SWING_DATA *     data,
                                        enum SWING_DATA_COLUMN  dataColumn1,
                                        enum SWING_DATA_COLUMN  dataColumn2,
                                        int                     indexBegin,
                                        int                     indexEnd,
                                        void *                  threshold1,
                                        void *                  threshold2,
                                        int                     winLength   )
{
    int foundIndex  = E_CONTINUITY_NOT_FOUND;
    int matchLength = 0;
    int currentIndex;

    assert(NULL != data->columns[dataColumn1]);
    assert(NULL != data->columns[dataColumn2]);

    assert(indexBegin >= 0);
    assert(indexEnd < data->length);

    for (currentIndex = indexBegin; currentIndex <= indexEnd; ++currentIndex)
    {
        if  (
                (GREATER == SwingDataComparator[dataColumn1](data->columns[dataColumn1], currentIndex, threshold1, 0))
            &&  (GREATER == SwingDataComparator[dataColumn2](data->columns[dataColumn2], currentIndex, threshold2, 0))
            )
        {
            matchLength++;
        }
        else
        {
            matchLength = 0;
        }

        if (matchLength >= winLength)
        {
            foundIndex = currentIndex - matchLength + 1;
            break;
        }
    }

    return (foundIndex);
}

struct SWING_RANGE_LIST * searchMultiContinuityWithinRange(
                                        struct SWING_DATA *     data,
                                        enum SWING_DATA_COLUMN  dataColumn,
                                        int                     indexBegin,
                                        int                     indexEnd,
                                        void *                  thresholdLo,
                                        void *                  thresholdHi,
                                        int                     winLength   )
{
    struct SWING_RANGE_LIST * head = NULL;
    struct SWING_RANGE_LIST * tail = NULL;

    int matchLength = 0;
    int currentIndex;

    assert(NULL != data->columns[dataColumn]);
    
    assert(indexBegin >= 0);
    assert(indexEnd < data->length);

    for (currentIndex = indexBegin; currentIndex <= indexEnd; ++currentIndex)
    {
        if  (
                (GREATER == SwingDataComparator[dataColumn](data->columns[dataColumn], currentIndex, thresholdLo, 0))
            &&  (   LESS == SwingDataComparator[dataColumn](data->columns[dataColumn], currentIndex, thresholdHi, 0))
            )
        {
            matchLength++;
        }
        else
        {
            matchLength = 0;
        }

        if (matchLength >= winLength)
        {
            if (NULL == head)
            {
                head = malloc(sizeof(struct SWING_RANGE_LIST));
                tail = head;
            }
            else
            {
                tail->next = malloc(sizeof(struct SWING_RANGE_LIST));
                tail = tail->next;
            }

            tail->indexBegin = currentIndex - matchLength + 1;
            tail->indexEnd = currentIndex;
            tail->next = NULL;

            matchLength = 0;
        }
    }

    return (head);
}

