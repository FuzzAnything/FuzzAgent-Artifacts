// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzgetc at gzread.c:473:13 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzread at gzread.c:396:13 in zlib.h
// gzungetc at gzread.c:505:13 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static gzFile open_gzfile(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return NULL;
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    return gzopen("./dummy_file", "rb");
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    gzFile file = open_gzfile(Data, Size);
    if (!file) return 0;
    
    unsigned char buf[4096];
    int len = Size > sizeof(buf) ? sizeof(buf) : Size;
    
    int ret;
    
    ret = gzrewind(file);
    if (ret == 0) {
        ret = gzread(file, buf, len);
    }
    
    gzclearerr(file);
    
    int c = gzgetc_(file);
    if (c != -1) {
        gzungetc(c, file);
        c = gzgetc(file);
    }
    
    ret = gzread(file, buf, len);
    
    while ((c = gzgetc(file)) != -1) {
        if (c == 0) break;
    }
    
    gzclearerr(file);
    gzrewind(file);
    
    ret = gzread(file, buf, len);
    if (ret > 0) {
        gzungetc(buf[0], file);
        c = gzgetc_(file);
    }
    
    gzclose(file);
    remove("./dummy_file");
    return 0;
}