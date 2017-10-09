#include "HTTPStats.h"

#include <curl/curl.h>
#include <stdlib.h>

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    // Do nothing
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

void* HTTPStatsInitImpl()
{
    CURL* curl = curl_easy_init();
    return curl;
}

void HTTPStatsDestroyImpl(void* handle)
{
    CURL* curl = (CURL*)handle;
    if (!curl)
        return;
    curl_easy_cleanup(curl);
}

int HTTPGetTestImpl(void* handle, HTTPTestDescription desc, double* store, int length)
{
    CURL* curl = (CURL*)handle;
    if (!curl)
        return 1;

    curl_easy_setopt(curl, CURLOPT_URL, desc.url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK)
    {
        if (desc.onSuccess)
            desc.onSuccess();

        if (desc.metricsMask & HTTPSTATS_NAMELOOKUP)
        {
            double val;
            curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &val);
            store[HTTPSTATS_NAMELOOKUP] = val;
        }
        if (desc.metricsMask & HTTPSTATS_CONNECT_TIME)
        {
            double val;
            curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &val);
            store[HTTPSTATS_CONNECT_TIME] = val;
        }
        if (desc.metricsMask & HTTPSTATS_STARTTRANSFER_TIME)
        {
            double val;
            curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME, &val);
            store[HTTPSTATS_STARTTRANSFER_TIME] = val;
        }
        if (desc.metricsMask & HTTPSTATS_TOTAL_TIME)
        {
            double val;
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &val);
            store[HTTPSTATS_TOTAL_TIME] = val;
        }

    }
    else
    {
        if (desc.onFailure)
            desc.onFailure();
    }

    curl_easy_reset(curl);

    return 0;
}