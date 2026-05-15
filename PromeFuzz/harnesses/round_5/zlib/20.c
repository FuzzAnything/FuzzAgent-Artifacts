// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzsetparams at gzwrite.c:630:13 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzclose_w at gzwrite.c:667:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zlib.h"

static gzFile open_dummy_file(const char *mode) {
    return gzopen("./dummy_file", mode);
}

static void cleanup_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    /* Prepare dummy file for writing */
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    /* Open gzip file for writing */
    gzFile file = open_dummy_file("wb");
    if (!file) {
        cleanup_file();
        return 0;
    }
    
    /* Use first few bytes to derive parameters */
    int level = Data[0] % 10;               /* 0-9 compression level */
    int strategy = Data[1] % 5;             /* 0-4 strategy values */
    int flush = Data[2] % 7;                /* 0-6 flush modes */
    int byte_to_write = Data[3];            /* Byte for gzputc */
    
    /* 1. Test gzsetparams */
    int ret = gzsetparams(file, level, strategy);
    if (ret != Z_OK && ret != Z_STREAM_ERROR) {
        /* Unexpected error - continue anyway */
    }
    
    /* 2. Test gzprintf with various format strings */
    const char *formats[] = {
        "%s", "%.*s", "%d %s", "%x", "test: %s"
    };
    size_t format_idx = Size % 5;
    size_t str_len = Size > 100 ? 100 : Size;
    char *temp_str = (char*)malloc(str_len + 1);
    if (temp_str) {
        memcpy(temp_str, Data, str_len);
        temp_str[str_len] = '\0';
        gzprintf(file, formats[format_idx], temp_str);
        free(temp_str);
    }
    
    /* 3. Test gzputc */
    gzputc(file, byte_to_write);
    
    /* 4. Test gzflush */
    int flush_modes[] = {Z_NO_FLUSH, Z_PARTIAL_FLUSH, Z_SYNC_FLUSH,
                         Z_FULL_FLUSH, Z_FINISH, Z_BLOCK, Z_TREES};
    gzflush(file, flush_modes[flush % 7]);
    
    /* 5. Test gzeof (should return 0 for writing) */
    int eof_result = gzeof(file);
    (void)eof_result; /* Suppress unused warning */
    
    /* 6. Test gzclose_w */
    gzclose_w(file);
    
    /* Cleanup */
    cleanup_file();
    
    return 0;
}