#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include "simplehttp.h"

static const char uri_chars[256] = {
    0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 1, 0, 0, 1,   1, 1, 1, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 0, 0, 1, 0, 0,
    /* 64 */
    1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 0, 0, 0, 0, 1,
    0, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 0, 0, 0, 1, 0,
    /* 128 */
    0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
    /* 192 */
    0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
};

int int_cmp(const void *a, const void *b) 
{
    const uint64_t *ia = (const uint64_t *)a;
    const uint64_t *ib = (const uint64_t *)b;
    
    return *ia  - *ib;
}

uint64_t ninety_five_percent(int64_t *int_array, int length)
{
    uint64_t value;
    int64_t *sorted_requests;
    int index_of_95;
    
    sorted_requests = calloc(length, sizeof(int64_t));
    memcpy(sorted_requests, int_array, length * sizeof(int64_t));
    qsort(sorted_requests, length, sizeof(int64_t), int_cmp);
    index_of_95 = (int)ceil(((95.0 / 100.0) * length) + 0.5);
    if (index_of_95 >= length) {
        index_of_95 = length - 1;
    }
    value = sorted_requests[index_of_95];
    free(sorted_requests);
    
    return value;
}

int simplehttp_parse_url(char *endpoint, size_t endpoint_len, char **address, int *port, char **path)
{
    // parse out address, port, path
    char *tmp = NULL;
    char *tmp_port = NULL;
    char *tmp_pointer;
    size_t address_len;
    size_t path_len;
    
    // http://0/
    if (endpoint_len < 9) {
        return 0;
    }
    
    // find the first /
    *address = (char *)strchr(endpoint, '/'); // http:/
    if (!*address) {
        return 0;
    }
    
    // move past the two slashes
    *address = *address + 2;
    
    // check for the colon specifying a port
    tmp_pointer = (char *)strchr(*address, ':');
    *path = (char *)strchr(*address, '/');
    
    if (!*path) {
        return 0;
    }
    
    if (tmp_pointer) {
        address_len = tmp_pointer - *address;
        tmp_port = *address + address_len + 1;
        **path = '\0';
        *port = atoi(tmp_port);
        **path = '/';
    } else {
        address_len = *path - *address;
        *port = 80;
    }
    
    path_len = (endpoint + endpoint_len) - *path;
    DUPE_N_TERMINATE(*address, address_len, tmp);
    DUPE_N_TERMINATE(*path, path_len, tmp);
    
    return 1;
}


// libevent's built in encoder does not encode spaces, this does
char *simplehttp_encode_uri(const char *uri)
{
    char *buf, *cur;
    char *p;
    
    // allocate 3x the memory
    buf = malloc(strlen(uri) * 3 + 1);
    cur = buf;
    for (p = (char *)uri; *p != '\0'; p++) {
        if (uri_chars[(unsigned char)(*p)]) {
            *cur++ = *p;
        } else {
            sprintf(cur, "%%%02X", (unsigned char)(*p));
            cur += 3;
        }
    }
    *cur = '\0';
    
    return buf;
}
