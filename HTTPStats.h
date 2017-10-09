#ifndef HTTPSTATS_H
#define HTTPSTATS_H

// Move these horrible defines somewhere else (central location)
#define MAX_H_OPTIONS 256

typedef enum
{
    HTTPSTATS_NAMELOOKUP = 1,
    HTTPSTATS_CONNECT_TIME = 2,
    HTTPSTATS_STARTTRANSFER_TIME = 4,
    HTTPSTATS_TOTAL_TIME = 8,
    HTTPSTATS_MAX_METRICS = 32
} Metrics;

struct HTTPTestDescription_
{
    unsigned metricsMask;
    const char* url;
    int iterations;
    int secondsBwConsecutiveReqs;
    char** hValueList;

    // Callbacks
    void (*onSuccess)();
    void (*onFailure)();
};

typedef struct HTTPTestDescription_ HTTPTestDescription;

typedef void HTTPStats;

typedef int HTTPStatsResult;

/**
 * Function must be defined by clients of this API.
 * It should return a handle to it (it can be NULL if handle isn't required).
 * @return Handle to actual implementation
 */
void* HTTPStatsInitImpl();

/**
 * Function must be defined by clients of this API.
 * It receives a handle to the underlying impl. so that it can clean up
 * its resources
 * @param handle
 */
void HTTPStatsDestroyImpl(void* handle);

int HTTPGetTestImpl(void* handle, HTTPTestDescription desc, double* store, int length);

/**
 *
 *
 * @return
 */
HTTPStats* HTTPStatsInit();

/**
 *
 *
 * @param handle
 */
void HTTPStatsDestroy(HTTPStats* handle);

/**
 *
 *
 * @param handle
 * @param desc
 * @return
 */
HTTPStatsResult HTTPGetTest(HTTPStats* handle, HTTPTestDescription desc);

/**
 *
 *
 * @param handle
 */
void HTTPStatsReset(HTTPStats* handle);


void HTTPStatsMetrics(HTTPStats* handle, HTTPStatsResult result,
                      double* computedValues);

#endif //HTTPSTATS_H
