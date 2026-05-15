// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzwrite at gzwrite.c:255:13 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
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

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Write data to dummy file for potential use
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }
    
    // Open gzip file for writing
    gzFile file = gzopen("./dummy_file", "wb");
    if (file == NULL) return 0;
    
    // Write data in chunks to explore different states
    size_t total_written = 0;
    while (total_written < Size) {
        size_t remaining = Size - total_written;
        unsigned chunk_size = (remaining > 1024) ? 1024 : (unsigned)remaining;
        
        // Ensure chunk_size fits in int (gzwrite takes unsigned but returns int)
        if (chunk_size > INT_MAX) chunk_size = INT_MAX;
        
        int written = gzwrite(file, (voidpc)(Data + total_written), chunk_size);
        if (written <= 0) break;
        total_written += written;
        
        // Occasionally check for errors during writing
        if (total_written % 4096 == 0) {
            int errnum = 0;
            const char *errmsg = gzerror(file, &errnum);
            (void)errmsg; // Use errmsg to avoid unused variable warning
        }
    }
    
    // Final error check
    int errnum = 0;
    const char *errmsg = gzerror(file, &errnum);
    (void)errmsg;
    
    // Close the file
    int close_result = gzclose(file);
    (void)close_result;
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}