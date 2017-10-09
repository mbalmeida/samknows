#include "HTTPStats.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void onSuccessCallback()
{
    //printf("onSuccessCallback\n");
}

void onFailCallback()
{
    //printf("onFailCallback\n");
}

int main(int argc, char** argv)
{
    HTTPStats* handle = HTTPStatsInit();
    if (!handle)
    {
        printf("Failed to initialise HTTPStats library!\n");
        return 1;
    }

    HTTPTestDescription testDescription;
    testDescription.iterations = 10;
    testDescription.onSuccess = onSuccessCallback;
    testDescription.onFailure = onFailCallback;
    testDescription.metricsMask =
            HTTPSTATS_NAMELOOKUP | HTTPSTATS_CONNECT_TIME
            | HTTPSTATS_STARTTRANSFER_TIME | HTTPSTATS_TOTAL_TIME;
    testDescription.url = "http://google.com";
    HTTPStatsResult ret = HTTPGetTest(handle, testDescription);

    double* storage = (double*)malloc(sizeof(double) * HTTPSTATS_MAX_METRICS);

    HTTPStatsMetrics(handle, ret, storage);

    printf("%f %f %f %f\n", storage[HTTPSTATS_NAMELOOKUP], storage[HTTPSTATS_CONNECT_TIME],
           storage[HTTPSTATS_STARTTRANSFER_TIME], storage[HTTPSTATS_TOTAL_TIME]);

    free (storage);
    HTTPStatsDestroy(handle);
    return 0;
}
