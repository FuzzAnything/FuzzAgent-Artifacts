// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_error at magic.c:569:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write fuzzer input to dummy file
    write_dummy_file(Data, Size);

    // Create main magic handle
    magic_t ms = magic_open(0);
    if (!ms) {
        remove_dummy_file();
        return 0;
    }

    // Create separate handle for magic_check
    magic_t ms_check = magic_open(0);
    if (!ms_check) {
        magic_close(ms);
        remove_dummy_file();
        return 0;
    }

    // Use first byte to determine which functions to call
    uint8_t selector = Data[0];
    
    // Try different function combinations based on selector
    switch (selector % 8) {
        case 0:
            // Test magic_file and magic_error
            magic_load(ms, NULL);
            const char *result = magic_file(ms, "./dummy_file");
            if (!result) {
                magic_error(ms);
            }
            break;
            
        case 1:
            // Test magic_buffer and magic_error
            magic_load(ms, NULL);
            result = magic_buffer(ms, Data, Size);
            if (!result) {
                magic_error(ms);
            }
            break;
            
        case 2:
            // Test magic_check with dummy file as database
            magic_check(ms_check, "./dummy_file");
            magic_error(ms_check);
            break;
            
        case 3:
            // Test magic_load with dummy file as database
            magic_load(ms, "./dummy_file");
            magic_error(ms);
            break;
            
        case 4:
            // Test magic_load_buffers
            {
                void *buffers[1] = {(void *)Data};
                size_t sizes[1] = {Size};
                magic_load_buffers(ms, buffers, sizes, 1);
                magic_error(ms);
            }
            break;
            
        case 5:
            // Test multiple functions in sequence
            magic_load(ms, NULL);
            result = magic_file(ms, "./dummy_file");
            if (!result) {
                magic_error(ms);
            }
            result = magic_buffer(ms, Data, Size);
            if (!result) {
                magic_error(ms);
            }
            break;
            
        case 6:
            // Test with different flags
            magic_close(ms);
            ms = magic_open(MAGIC_CONTINUE | MAGIC_ERROR);
            if (ms) {
                magic_load(ms, NULL);
                result = magic_buffer(ms, Data, Size);
                magic_error(ms);
            }
            break;
            
        case 7:
            // Test with NULL filename for magic_file
            magic_load(ms, NULL);
            // Note: magic_file with NULL reads from stdin, which we won't feed
            // So we'll just call magic_error to check state
            magic_error(ms);
            break;
    }

    // Clean up
    magic_close(ms);
    magic_close(ms_check);
    remove_dummy_file();
    
    return 0;
}