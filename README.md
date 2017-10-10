# Samknows Technical Test (C/C++)

The application demonstrates the use of a very simple library for calculating metrics related to HTTP requests to a webserver.

## Overview
The main idea is that 'HTTPStats' is a library (compiled as a shared library as well) with some 'abstract' functions i.e., functions that hide the main details of handling a HTTP connection. The underlying implementation (HTTPStatsCurlImpl.c) is an example of how one could use the library to log statistics about HTTP connections. It should be possible to select at link time a different library to handle the requests that doesn't use libcurl.

## Build mechanism
The application can be easily built using a combination of cmake and make
Example: cmake . && make
The commands above should generate an executable file named 'samknows'.

### External dependencies
1. libcurl
2. glibc

## Command-line arguments
1. -H "Header-name: Header-value": can be used multiple times, each time specifying an extra HTTP header to add to your request
2. -n <integer>: number of HTTP requests to make in total
3. -s <integer>: number of seconds between consecutive HTTP requests (defaults to 1)

## Usage
./samknows -n 10 -s 5 -H "name: value"

## Testing
Application was tested on MacOS High Sierra

### Other
My initial goal was to implement a more sophistated library that could:
1. Execute asynchronous tasks
2. Calculate a running median and output the results as the application executed (utils/heap.{c,h})
3. Allow other metrics that wouldn't just be "doubles". E.g.: Number of successful and unsuccessful connections
4. Allow more configuration options like: connection timeout, redirectation(on vs off), etc...
5. Set the target destination address as a command-line option
6. Allow library implementation to be loaded at run-time (e.g.: plug-ins).
7. Options are endless :)
