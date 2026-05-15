// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "magic.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    magic_t ms = magic_open(0);
    if (!ms) return 0;
    
    write_dummy_file(Data, Size);
    
    int flags = magic_getflags(ms);
    
    int new_flags = (int)Data[0];
    magic_setflags(ms, new_flags);
    
    magic_load(ms, NULL);
    
    const char *result = magic_file(ms, "./dummy_file");
    (void)result;
    
    size_t param_value = (size_t)Data[0];
    int param_id = MAGIC_PARAM_BYTES_MAX;
    if (Size > 1) {
        param_id = (int)Data[1] % 10;
    }
    
    magic_setparam(ms, param_id, &param_value);
    
    size_t retrieved_value = 0;
    magic_getparam(ms, param_id, &retrieved_value);
    
    magic_close(ms);
    remove_dummy_file();
    
    return 0;
}