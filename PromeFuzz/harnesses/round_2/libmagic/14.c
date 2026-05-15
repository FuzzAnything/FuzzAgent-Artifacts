// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize libmagic handle
    magic_t ms = magic_open(0);
    if (ms == NULL) return 0;
    
    // Try to load default magic database
    if (magic_load(ms, NULL) == -1) {
        magic_close(ms);
        return 0;
    }
    
    // Use first byte to determine which functions to call
    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    // Test magic_getparam
    if ((selector & 0x01) && fuzz_size >= sizeof(int)) {
        int param_type = *(int*)fuzz_data % 10; // Use modulo to get valid-ish param
        size_t param_value = 0;
        magic_getparam(ms, param_type, &param_value);
    }
    
    // Test magic_file
    if ((selector & 0x02) && fuzz_size > 0) {
        write_dummy_file(fuzz_data, fuzz_size);
        const char *result = magic_file(ms, "./dummy_file");
        (void)result; // Use result to avoid unused variable warning
        remove_dummy_file();
    }
    
    // Test magic_getflags
    if (selector & 0x04) {
        int flags = magic_getflags(ms);
        (void)flags;
    }
    
    // Test magic_error
    if (selector & 0x08) {
        const char *error = magic_error(ms);
        (void)error;
    }
    
    // Test magic_setparam
    if ((selector & 0x10) && fuzz_size >= sizeof(int) + sizeof(size_t)) {
        int param_type = *(int*)fuzz_data % 10;
        size_t param_value = *(size_t*)(fuzz_data + sizeof(int));
        magic_setparam(ms, param_type, &param_value);
    }
    
    // Test magic_setflags
    if ((selector & 0x20) && fuzz_size >= sizeof(int)) {
        int flags = *(int*)fuzz_data;
        magic_setflags(ms, flags);
    }
    
    // Clean up
    magic_close(ms);
    
    return 0;
}