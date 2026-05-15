// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzdirect at gzread.c:627:13 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzrewind at gzlib.c:346:13 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzgetc_ at gzread.c:500:13 in zlib.h
// gzeof at gzlib.c:498:13 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    gzFile file = gzopen("./dummy_file", "rb");
    if (!file) {
        remove_dummy_file();
        return 0;
    }
    
    int direct = gzdirect(file);
    (void)direct;
    
    unsigned char buf[4096];
    z_size_t items_read = gzfread(buf, 1, sizeof(buf), file);
    (void)items_read;
    
    int eof = gzeof(file);
    (void)eof;
    
    int rewind_result = gzrewind(file);
    (void)rewind_result;
    
    gzclearerr(file);
    
    int byte = gzgetc_(file);
    (void)byte;
    
    eof = gzeof(file);
    (void)eof;
    
    gzclose(file);
    remove_dummy_file();
    
    return 0;
}