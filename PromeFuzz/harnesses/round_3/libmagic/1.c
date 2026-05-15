// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "magic.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    write_dummy_file(Data, Size);
    
    // Create magic cookie
    magic_t ms = magic_open(0);
    if (ms == NULL) {
        remove_dummy_file();
        return 0;
    }
    
    // Load magic database
    int load_result = magic_load(ms, NULL);
    
    // Check for errors after load
    const char *error1 = magic_error(ms);
    
    // Identify file type of dummy file
    const char *result = magic_file(ms, "./dummy_file");
    
    // Check for errors after file identification
    const char *error2 = magic_error(ms);
    
    // Clean up
    magic_close(ms);
    remove_dummy_file();
    
    return 0;
}