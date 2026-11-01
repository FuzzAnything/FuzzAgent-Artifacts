// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Fuzz curl_multi_get_offt
    curl_off_t offt_value;
    if (Size >= 1) {
        CURLMinfo_offt info = (CURLMinfo_offt)(Data[0]);
        curl_multi_get_offt(multi_handle, info, &offt_value);
    }

    // Fuzz curl_multi_strerror
    if (Size >= 2) {
        CURLMcode error_code = (CURLMcode)(Data[1] & 0xFF);
        const char *error_str = curl_multi_strerror(error_code);
        (void)error_str; // Use the result to avoid unused variable warning
    }

    // Fuzz curl_multi_timeout
    long milliseconds;
    if (Size >= 3) {
        curl_multi_timeout(multi_handle, &milliseconds);
    }

    // Fuzz curl_multi_wakeup
    if (Size >= 4) {
        curl_multi_wakeup(multi_handle);
    }

    // Create a dummy file if needed
    if (Size >= 5) {
        write_dummy_file("./dummy_file", Data + 4, Size - 4);
    }

    // Cleanup
    curl_multi_cleanup(multi_handle);

    return 0;
}