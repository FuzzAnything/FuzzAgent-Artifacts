// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_getpath at magic.c:254:1 in magic.h
// magic_version at magic.c:607:1 in magic.h
// magic_version at magic.c:607:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // 1. Prepare environment and invoke functions in specified order

    // magic_getpath: Resolve path (no handle needed)
    const char *path = magic_getpath(NULL, 0);
    (void)path; // Suppress unused warning

    // magic_version: Get version (no handle needed)
    int v1 = magic_version();
    (void)v1;

    // magic_version: Call again as per requirement
    int v2 = magic_version();
    (void)v2;

    // magic_open: Create handle for main operations
    // Use 0 as flags since MAGIC_SILENT is not defined in the provided context
    magic_t mh = magic_open(0);
    if (mh == NULL) {
        return 0;
    }

    // magic_check: Requires a NEW magic_set structure
    magic_t check_handle = magic_open(0);
    if (check_handle != NULL) {
        // Use a dummy file for checking
        const char *dummy_file = "./dummy_file";
        // Write input data to dummy file if not empty
        if (Size > 0) {
            FILE *f = fopen(dummy_file, "wb");
            if (f) {
                fwrite(Data, 1, Size, f);
                fclose(f);
            }
        } else {
            // Create empty file if input is empty
            FILE *f = fopen(dummy_file, "w");
            if (f) fclose(f);
        }
        
        int check_res = magic_check(check_handle, dummy_file);
        (void)check_res;
        magic_close(check_handle);
    }

    // magic_compile: Compile rules on the main handle
    const char *dummy_file = "./dummy_file";
    int compile_res = magic_compile(mh, dummy_file);
    (void)compile_res;

    // magic_list: Requires a NEW magic_set structure
    magic_t list_handle = magic_open(0);
    if (list_handle != NULL) {
        int list_res = magic_list(list_handle, dummy_file);
        (void)list_res;
        magic_close(list_handle);
    }

    // magic_error: Retrieve error message
    const char *err = magic_error(mh);
    (void)err;

    // 2. Cleanup
    magic_close(mh);

    return 0;
}