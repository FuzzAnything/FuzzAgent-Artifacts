// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzdopen at gzlib.c:298:16 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzprintf at gzwrite.c:487:15 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Write data to dummy file for file-based operations */
    write_dummy_file(Data, Size);
    
    /* Test gzopen */
    gzFile file1 = gzopen("./dummy_file", "rb");
    if (file1) {
        gzclose(file1);
    }
    
    /* Test gzopen64 */
    gzFile file2 = gzopen64("./dummy_file", "rb");
    if (file2) {
        gzclose(file2);
    }
    
    /* Test gzdopen */
    int fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        gzFile file3 = gzdopen(fd, "rb");
        if (file3) {
            gzclose(file3);
        } else {
            close(fd);
        }
    }
    
    /* Test gzprintf, gzfwrite, and gzfread with a writeable file */
    gzFile file4 = gzopen("./dummy_file", "wb");
    if (file4) {
        /* Use a small portion of data for format string if available */
        size_t fmt_len = Size > 10 ? 10 : Size;
        char fmt[11];
        memcpy(fmt, Data, fmt_len);
        fmt[fmt_len] = '\0';
        
        /* Test gzprintf with simple format */
        gzprintf(file4, "Test: %s\n", fmt);
        
        /* Test gzfwrite */
        size_t write_size = Size > 100 ? 100 : Size;
        z_size_t written = gzfwrite(Data, 1, write_size, file4);
        (void)written; /* Ignore result */
        
        gzclose(file4);
    }
    
    /* Test gzfread with a readable file */
    gzFile file5 = gzopen("./dummy_file", "rb");
    if (file5) {
        char buf[1024];
        z_size_t read = gzfread(buf, 1, sizeof(buf), file5);
        (void)read; /* Ignore result */
        gzclose(file5);
    }
    
    /* Clean up */
    unlink("./dummy_file");
    
    return 0;
}