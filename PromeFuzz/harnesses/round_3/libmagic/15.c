// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_descriptor at magic.c:403:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
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
#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void write_magic_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_magic", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to choose different code paths
    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    // Create main magic handle
    magic_t ms = magic_open(MAGIC_NONE);
    if (!ms) return 0;
    
    // Test magic_load with various inputs
    if (selector & 0x01) {
        magic_load(ms, NULL);  // Default database
    } else {
        write_magic_file(fuzz_data, fuzz_size);
        magic_load(ms, "./dummy_magic");
    }
    
    // Test magic_file
    write_dummy_file(fuzz_data, fuzz_size);
    const char *result = magic_file(ms, "./dummy_file");
    if (result) {
        // Use result to avoid optimization
        volatile const char *tmp = result;
        (void)tmp;
    }
    
    // Test magic_descriptor
    int fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        const char *desc_result = magic_descriptor(ms, fd);
        if (desc_result) {
            volatile const char *tmp = desc_result;
            (void)tmp;
        }
        close(fd);
    }
    
    // Test magic_errno
    int err = magic_errno(ms);
    volatile int tmp_err = err;
    (void)tmp_err;
    
    // Test magic_check with separate handle
    magic_t ms_check = magic_open(MAGIC_NONE);
    if (ms_check) {
        write_magic_file(fuzz_data, fuzz_size);
        int check_result = magic_check(ms_check, "./dummy_magic");
        volatile int tmp_check = check_result;
        (void)tmp_check;
        magic_close(ms_check);
    }
    
    // Test magic_list with separate handle
    magic_t ms_list = magic_open(MAGIC_NONE);
    if (ms_list) {
        write_magic_file(fuzz_data, fuzz_size);
        int list_result = magic_list(ms_list, "./dummy_magic");
        volatile int tmp_list = list_result;
        (void)tmp_list;
        magic_close(ms_list);
    }
    
    // Cleanup
    magic_close(ms);
    unlink("./dummy_file");
    unlink("./dummy_magic");
    
    return 0;
}