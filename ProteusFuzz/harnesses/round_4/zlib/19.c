// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzputs at gzwrite.c:350:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzclose_w at gzwrite.c:667:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzsetparams at gzwrite.c:630:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzflush at gzwrite.c:603:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzputc at gzwrite.c:307:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static int fuzz_gzdirect(const uint8_t *Data, size_t Size) {
    gzFile file = gzopen("./dummy_file", "rb");
    if (!file) return 0;
    int result = gzdirect(file);
    gzclose(file);
    return result;
}

static int fuzz_gzputs(const uint8_t *Data, size_t Size) {
    gzFile file = gzopen("./dummy_file", "wb");
    if (!file) return 0;
    
    // Ensure null-termination for string
    char *str = (char *)malloc(Size + 1);
    if (!str) { gzclose(file); return 0; }
    memcpy(str, Data, Size);
    str[Size] = '\0';
    
    int result = gzputs(file, str);
    free(str);
    gzclose(file);
    return result;
}

static int fuzz_gzclose_w(const uint8_t *Data, size_t Size) {
    gzFile file = gzopen("./dummy_file", "wb");
    if (!file) return Z_STREAM_ERROR;
    
    // Write some data to ensure write mode is active
    if (Size > 0) {
        gzwrite(file, Data, Size);
    }
    
    int result = gzclose_w(file);
    return result;
}

static int fuzz_gzsetparams(const uint8_t *Data, size_t Size) {
    gzFile file = gzopen("./dummy_file", "wb");
    if (!file) return Z_STREAM_ERROR;
    
    int level = (Size > 0) ? (Data[0] % 10) : 6;
    int strategy = (Size > 1) ? (Data[1] % 5) : Z_DEFAULT_STRATEGY;
    
    int result = gzsetparams(file, level, strategy);
    gzclose(file);
    return result;
}

static int fuzz_gzflush(const uint8_t *Data, size_t Size) {
    gzFile file = gzopen("./dummy_file", "wb");
    if (!file) return Z_STREAM_ERROR;
    
    // Write some data first
    if (Size > 0) {
        gzwrite(file, Data, Size);
    }
    
    int flush_mode = (Size > 0) ? (Data[0] % (Z_FINISH + 1)) : Z_NO_FLUSH;
    int result = gzflush(file, flush_mode);
    gzclose(file);
    return result;
}

static int fuzz_gzputc(const uint8_t *Data, size_t Size) {
    gzFile file = gzopen("./dummy_file", "wb");
    if (!file) return -1;
    
    int c = (Size > 0) ? Data[0] : 'A';
    int result = gzputc(file, c);
    gzclose(file);
    return result;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    // Create dummy file if needed
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) fwrite(Data, 1, Size, f);
        fclose(f);
    }
    
    // Fuzz all target functions
    fuzz_gzdirect(Data, Size);
    fuzz_gzputs(Data, Size);
    fuzz_gzclose_w(Data, Size);
    fuzz_gzsetparams(Data, Size);
    fuzz_gzflush(Data, Size);
    fuzz_gzputc(Data, Size);
    
    return 0;
}