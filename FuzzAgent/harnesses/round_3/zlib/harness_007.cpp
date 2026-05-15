#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "zlib.h"

namespace harness_007 {

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < 1) {
        return 0;
    }

    gzFile file;
    char fname[] = "/tmp/gzio_fuzz.XXXXXX";
    int fd = mkstemp(fname);
    if (fd < 0) {
        return 0;
    }
    close(fd);

    // Simple mode: just use "wb" for writing or "rb" for reading
    const char* mode = (data[0] & 1) ? "wb" : "rb";
    data++; size--;
    
    file = gzopen(fname, mode);
    if (!file) {
        unlink(fname);
        return 0;
    }

    // Perform a simple operation based on input
    if (size > 0) {
        uint8_t op = data[0] % 3;
        data++; size--;
        
        switch (op) {
            case 0: // Write some data
                if (size > 0) {
                    size_t write_len = size > 100 ? 100 : size;
                    gzwrite(file, data, write_len);
                }
                break;
            case 1: // Read (will likely get nothing since file is empty or newly written)
                {
                    char buf[100];
                    gzread(file, buf, sizeof(buf));
                }
                break;
            case 2: // Seek and tell
                gzseek(file, 0, SEEK_SET);
                gztell(file);
                break;
        }
    }

    // Close file
    gzclose(file);
    unlink(fname);
    
    return 0;
}

} // namespace harness_007
