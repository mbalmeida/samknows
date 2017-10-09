#include "HTTPStats.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ITEMS 1024
#define MAX_TESTS 16

typedef struct
{
    double results[HTTPSTATS_MAX_METRICS][MAX_ITEMS];
    int measurements[HTTPSTATS_MAX_METRICS];
    unsigned requestedMetrics;
} HTTPStatsResult_;

typedef struct
{
    void* implementationHandle;
    HTTPStatsResult_ workArena[16];
    HTTPStatsResult nrCurrentResults;
} HTTPStatsContext;

HTTPStats* HTTPStatsInit()
{
    HTTPStatsContext* context = (HTTPStatsContext*)malloc(sizeof(HTTPStatsContext));
    context->implementationHandle = HTTPStatsInitImpl();

    return (HTTPStats*)context;
}

void HTTPStatsDestroy(HTTPStats* handle)
{
    if (handle)
    {
        HTTPStatsContext* ctx = (HTTPStatsContext*)handle;
        HTTPStatsDestroyImpl(ctx->implementationHandle);
        free(ctx);
    }
}

void HTTPStatsReset(HTTPStats* handle)
{
    if (handle)
    {
        HTTPStatsContext* ctx = (HTTPStatsContext*)handle;
        ctx->nrCurrentResults = 0;
        memset(&ctx->workArena, 0, sizeof(HTTPStatsResult_));
    }
}

int cmpfunc (const void * a, const void * b) {
    return (*(double*)a <= *(double*)b);
}

void HTTPStatsMetrics(HTTPStats* handle, HTTPStatsResult result,
                      double* computedValues)
{
    if (!handle)
        return;
    if (result < 0 || result >= MAX_TESTS)
        return;

    HTTPStatsContext* ctx = (HTTPStatsContext*)handle;
    // For each one of the requested metrics, compute the final result
    for (unsigned i = HTTPSTATS_MAX_METRICS >> 1; i != 0; i >>=1 )
    {
        if (!(i & ctx->workArena[result].requestedMetrics))
            continue;

        qsort(ctx->workArena[result].results[i], ctx->workArena[result].measurements[i], sizeof(double),
              cmpfunc);
        computedValues[i] = ctx->workArena[result].results[i][ctx->workArena[result].measurements[i] / 2];
    }
}

HTTPStatsResult HTTPGetTest(HTTPStats* handle, HTTPTestDescription desc)
{
    if (!handle)
        return -1;

    HTTPStatsContext* ctx = (HTTPStatsContext*)handle;

    double* tmpStorage;
    tmpStorage = (double*)malloc(sizeof(double) * HTTPSTATS_MAX_METRICS);
    HTTPStatsResult r = ctx->nrCurrentResults;
    ctx->nrCurrentResults = (ctx->nrCurrentResults + 1) % MAX_TESTS;
    ctx->workArena[r].requestedMetrics = desc.metricsMask;

    for (int it = 0; it < desc.iterations; it++)
    {
        int ret = HTTPGetTestImpl(ctx->implementationHandle, desc, tmpStorage, HTTPSTATS_MAX_METRICS);
        if (ret)
            continue;

        // Test completed successfully, copy results to our main storage
        for (unsigned i = HTTPSTATS_MAX_METRICS >> 1; i != 0; i >>= 1)
        {
            if (!(i & desc.metricsMask))
                continue;

            int idx = ctx->workArena[r].measurements[i];
            ctx->workArena[r].results[i][idx] = tmpStorage[i];

            // Update index
            idx += 1;
            if (idx > MAX_ITEMS)
                idx = MAX_ITEMS;
            ctx->workArena[r].measurements[i] = idx;
        }
        sleep(desc.secondsBwConsecutiveReqs);
    }

    free(tmpStorage);

    return r;
}