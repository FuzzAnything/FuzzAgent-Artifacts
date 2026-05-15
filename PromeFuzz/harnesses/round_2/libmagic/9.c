// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "magic.h"

static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void test_magic_check(const uint8_t *data, size_t size) {
    magic_t ms = magic_open(0);
    if (ms == NULL) {
        return;
    }
    
    write_dummy_file("./dummy_db", data, size);
    int ret = magic_check(ms, "./dummy_db");
    if (ret == -1) {
        const char *err = magic_error(ms);
        (void)err;
    }
    
    magic_close(ms);
    unlink("./dummy_db");
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    int flags = Data[0] % 16;
    const uint8_t *actual_data = Data + 1;
    size_t actual_size = Size - 1;
    
    magic_t ms = magic_open(flags);
    if (ms == NULL) {
        return 0;
    }
    
    int load_ret = magic_load(ms, NULL);
    if (load_ret == 0) {
        const char *file_result = magic_file(ms, "./dummy_file");
        (void)file_result;
        
        const char *buffer_result = magic_buffer(ms, actual_data, actual_size);
        (void)buffer_result;
    }
    
    const char *error_msg = magic_error(ms);
    (void)error_msg;
    
    magic_close(ms);
    
    test_magic_check(Data, Size);
    
    write_dummy_file("./dummy_file", actual_data, actual_size);
    unlink("./dummy_file");
    
    return 0;
}