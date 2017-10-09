#include "HTTPStats.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 1024

typedef struct
{
    double results[HTTPSTATS_MAX_METRICS][MAX_ITEMS];
    int measurements[HTTPSTATS_MAX_METRICS];
    unsigned requestedMetrics;
} HTTPStatsResult_;

typedef struct
{
    void* implementationHandle;
    HTTPStatsResult_ workArena;
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

    HTTPStatsContext* ctx = (HTTPStatsContext*)handle;
    // For each one of the requested metrics, compute the final result
    for (unsigned i = HTTPSTATS_MAX_METRICS >> 1; i != 0; i >>=1 )
    {
        if (!(i & ctx->workArena.requestedMetrics))
            continue;

        qsort(ctx->workArena.results[i], ctx->workArena.measurements[i], sizeof(double),
              cmpfunc);
        computedValues[i] = ctx->workArena.results[i][ctx->workArena.measurements[i] / 2];
    }
}

HTTPStatsResult HTTPGetTest(HTTPStats* handle, HTTPTestDescription desc)
{
    if (!handle)
        return -1;

    HTTPStatsContext* ctx = (HTTPStatsContext*)handle;

    double* tmpStorage;
    tmpStorage = (double*)malloc(sizeof(double) * HTTPSTATS_MAX_METRICS);
    ctx->workArena.requestedMetrics = desc.metricsMask;

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

            int idx = ctx->workArena.measurements[i];
            ctx->workArena.results[i][idx] = tmpStorage[i];

            // Update index
            idx += 1;
            if (idx > MAX_ITEMS)
                idx = MAX_ITEMS;
            ctx->workArena.measurements[i] = idx;
        }
    }

    free(tmpStorage);

    return 0;
}