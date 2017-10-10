#include "HTTPStats.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void onSuccessCallback()
{
    //printf("onSuccessCallback\n");
}

void onFailCallback()
{
    //printf("onFailCallback\n");
}


#define MAX_H_OPTIONS 256
#define DEFAULT_INTERVAL 1
#define DEFAULT_ITERATIONS 1

int main(int argc, char** argv)
{
    char *nvalue = NULL;
    char *svalue = NULL;
    int c;

    // Simple list for handling multiple -H command-line options
    int hValueCount = 0;
    char* hValueList[MAX_H_OPTIONS];
    memset(hValueList, 0, sizeof(hValueList));

    int nIntValue = DEFAULT_ITERATIONS, sIntValue = DEFAULT_INTERVAL;


    while ((c = getopt (argc, argv, "H:n:s:")) != -1)
    {
        switch(c)
        {
            case 'H':
                hValueList[hValueCount++ % MAX_H_OPTIONS] = strdup(optarg);
                break;
            case 'n':
                nvalue = strdup(optarg);
                break;
            case 's':
                svalue = strdup(optarg);
                break;
            default:
                break;
        }
    }

    // Convert command-line options to actual values
    if (nvalue)
        nIntValue = atoi(nvalue);
    if (svalue)
        sIntValue = atoi(svalue);


    HTTPStats* handle = HTTPStatsInit();
    if (!handle)
    {
        printf("Failed to initialise HTTPStats library!\n");
        return 1;
    }

    HTTPTestDescription testDescription;
    testDescription.iterations = nIntValue;
    testDescription.secondsBwConsecutiveReqs = sIntValue;
    testDescription.hValueList = hValueList;
    testDescription.onSuccess = onSuccessCallback;
    testDescription.onFailure = onFailCallback;
    testDescription.metricsMask =
            HTTPSTATS_NAMELOOKUP | HTTPSTATS_CONNECT_TIME
            | HTTPSTATS_STARTTRANSFER_TIME | HTTPSTATS_TOTAL_TIME;
    testDescription.url = "http://www.google.com/";
    HTTPStatsResult ret = HTTPGetTest(handle, testDescription);

    double* storage = (double*)malloc(sizeof(double) * HTTPSTATS_MAX_METRICS);

    char* ipAddr;
    long retCode;
    HTTPStatsMetrics(handle, ret, storage, &ipAddr, &retCode);

    printf("SKTEST;<%s>;<%ld>;<%f>;<%f>;<%f>;<%f>\n", ipAddr, retCode, storage[HTTPSTATS_NAMELOOKUP], storage[HTTPSTATS_CONNECT_TIME],
           storage[HTTPSTATS_STARTTRANSFER_TIME], storage[HTTPSTATS_TOTAL_TIME]);

    free (storage);
    HTTPStatsDestroy(handle);

    for (int i = 0; i < MAX_H_OPTIONS; ++i )
    {
        if (hValueList[i])
            free(hValueList[i]);
    }
    if (nvalue)
        free(nvalue);
    if (svalue)
        free(svalue);

    return 0;
}
