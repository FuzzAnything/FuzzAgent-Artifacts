// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzsetparams at gzwrite.c:630:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzclose_w at gzwrite.c:667:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Create dummy file for reading if needed */
    write_dummy_file(Data, Size);
    
    /* Open a gzFile for writing */
    gzFile file = gzopen("./dummy_file", "wb");
    if (file == NULL) {
        return 0;
    }
    
    /* Test gzputc with first byte */
    int byte = Data[0];
    int result = gzputc(file, byte);
    (void)result; /* Suppress unused warning */
    
    /* Test gzputs if we have enough data for a string */
    if (Size > 1) {
        /* Create null-terminated string from input */
        char *str = (char *)malloc(Size);
        if (str) {
            memcpy(str, Data, Size);
            str[Size-1] = '\0';
            gzputs(file, str);
            free(str);
        }
    }
    
    /* Test gzsetparams with valid parameters */
    int level = Data[0] % 10; /* 0-9 compression levels */
    int strategy = Data[0] % 4; /* 0-3 for common strategies */
    gzsetparams(file, level, strategy);
    
    /* Test gzflush with different flush modes */
    int flush_mode = Data[0] % 5; /* 0-4 for Z_NO_FLUSH to Z_FINISH */
    gzflush(file, flush_mode);
    
    /* Test gzclose_w (for writing) */
    gzclose_w(file);
    
    /* Reopen file for reading to test gzclose (general) */
    file = gzopen("./dummy_file", "rb");
    if (file) {
        gzclose(file);
    }
    
    return 0;
}