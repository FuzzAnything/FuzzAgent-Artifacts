// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_pushheader_bynum at http2.c:657:7 in multi.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_pushheader_bynum at http2.c:657:7 in multi.h
// curl_free at escape.c:189:6 in curl.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_pushheader_bynum at http2.c:657:7 in multi.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/mprintf.h>

static void safe_free(char *ptr) {
    if (ptr) {
        curl_free(ptr);
    }
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 
     * Test 1: curl_version
     * Returns a static string, no cleanup needed.
     */
    char *ver = curl_version();
    if (ver) {
        (void)ver; 
    }

    /* 
     * Test 2: curl_getenv
     * Retrieves environment variable.
     */
    if (Size > 0) {
        size_t env_len = (Size < 256) ? Size : 256;
        char *env_name = malloc(env_len + 1);
        if (env_name) {
            memcpy(env_name, Data, env_len);
            env_name[env_len] = '\0';
            
            /* Replace null bytes to ensure valid string */
            for (size_t i = 0; i < env_len; i++) {
                if (env_name[i] == '\0') env_name[i] = ' ';
            }

            char *env_val = curl_getenv(env_name);
            if (env_val) {
                (void)strlen(env_val);
                curl_free(env_val);
            }
            free(env_name);
        }
    }

    /* 
     * Test 3: curl_maprintf
     * Formats string and allocates memory.
     */
    if (Size > 0 && Size < 1024) {
        char *fmt_buf = malloc(Size + 1);
        if (fmt_buf) {
            memcpy(fmt_buf, Data, Size);
            fmt_buf[Size] = '\0';
            
            /* Safe usage: fixed format, input as data */
            char *result = curl_maprintf("FuzzInput: %.*s", (int)Size, Data);
            if (result) {
                safe_free(result);
            }

            /* Risky usage: input as format string with dummy arg */
            if (Size < 100) {
                char *result2 = curl_maprintf(fmt_buf, 0xDEADBEEF);
                if (result2) {
                    safe_free(result2);
                }
            }
            free(fmt_buf);
        }
    }

    /* 
     * Test 4: curl_msnprintf
     * Formats string into a fixed buffer.
     */
    if (Size > 0) {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        
        /* Safe usage */
        int ret = curl_msnprintf(buffer, sizeof(buffer), "Data: %.*s", (int)Size, Data);
        (void)ret;

        /* Risky usage */
        if (Size < 100) {
            char *fmt_buf = malloc(Size + 1);
            if (fmt_buf) {
                memcpy(fmt_buf, Data, Size);
                fmt_buf[Size] = '\0';
                ret = curl_msnprintf(buffer, sizeof(buffer), fmt_buf, 0x12345678);
                (void)ret;
                free(fmt_buf);
            }
        }
    }

    /* 
     * Test 5 & 6: curl_pushheader_bynum and curl_pushheader_byname
     * struct curl_pushheaders is opaque (forward declaration only).
     * We cannot instantiate it directly. We test with NULL and a pointer
     * to uninitialized memory to trigger error handling paths.
     */
    {
        /* Test with NULL */
        char *h1 = curl_pushheader_bynum(NULL, 0);
        char *h2 = curl_pushheader_byname(NULL, "Content-Type");
        (void)h1;
        (void)h2;

        /* 
         * Create a dummy pointer to uninitialized stack memory.
         * We cannot define the struct, so we use a raw buffer and cast it.
         * This tests if the function validates the pointer before dereferencing.
         */
        char dummy_buffer[256];
        memset(dummy_buffer, 0, sizeof(dummy_buffer));
        struct curl_pushheaders *dummy_ptr = (struct curl_pushheaders *)dummy_buffer;

        char *h3 = curl_pushheader_bynum(dummy_ptr, 0);
        char *h4 = curl_pushheader_byname(dummy_ptr, "Content-Type");
        (void)h3;
        (void)h4;

        /* Test with varying index and name from input */
        if (Size > 0) {
            size_t idx = (size_t)Data[0] % 100;
            char *h5 = curl_pushheader_bynum(dummy_ptr, idx);
            (void)h5;

            /* Construct a name from input */
            char name[64];
            size_t name_len = (Size < 63) ? Size : 63;
            memcpy(name, Data, name_len);
            name[name_len] = '\0';
            
            char *h6 = curl_pushheader_byname(dummy_ptr, name);
            (void)h6;
        }
    }

    return 0;
}