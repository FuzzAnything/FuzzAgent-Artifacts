// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// gzopen at gzlib.c:288:16 in zlib.h
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzfwrite at gzwrite.c:280:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
// gzopen at gzlib.c:288:16 in zlib.h
// gzopen64 at gzlib.c:293:16 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzerror at gzlib.c:513:22 in zlib.h
// gzclearerr at gzlib.c:531:14 in zlib.h
// gzfread at gzread.c:439:18 in zlib.h
// gzclose at gzclose.c:11:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create a dummy file with input data to test gzopen/gzopen64
    create_dummy_file(Data, Size);

    gzFile file_read = NULL;
    gzFile file_write = NULL;
    uint8_t *read_buf = NULL;
    uint8_t *write_buf = NULL;
    z_size_t items_read, items_written;
    int errnum;
    const char *err_str;

    // Allocate buffers with a safe minimum size
    size_t buf_size = Size > 1024 ? 1024 : Size;
    if (buf_size < 1) buf_size = 1;
    
    read_buf = (uint8_t *)malloc(buf_size);
    write_buf = (uint8_t *)malloc(buf_size);
    if (!read_buf || !write_buf) {
        free(read_buf);
        free(write_buf);
        return 0;
    }

    // Initialize write buffer with input data to ensure valid content
    memset(write_buf, 0, buf_size);
    size_t copy_len = (Size < buf_size) ? Size : buf_size;
    memcpy(write_buf, Data, copy_len);

    // Test gzopen and gzopen64 for reading
    file_read = gzopen("./dummy_file", "rb");
    if (!file_read) {
        file_read = gzopen64("./dummy_file", "rb");
    }

    if (file_read) {
        // Test gzfread
        z_size_t item_size = (Data[0] % 100) + 1;
        z_size_t nitems = (Data[1] % 100) + 1;
        
        // Prevent overflow and ensure buffer fits
        if (item_size > 0 && nitems > 0) {
            // Check for potential overflow before multiplication
            if (item_size > buf_size / nitems) {
                nitems = buf_size / item_size;
            }
            if (nitems == 0) {
                // If nitems becomes 0, we cannot read anything safely with current item_size
                // Set nitems to 1 only if item_size fits, otherwise skip
                if (item_size > buf_size) {
                    nitems = 0;
                } else {
                    nitems = 1;
                }
            }
        } else {
            item_size = 1;
            nitems = 1;
        }

        if (nitems > 0) {
            items_read = gzfread(read_buf, item_size, nitems, file_read);
        }

        // Test gzerror
        err_str = gzerror(file_read, &errnum);
        (void)err_str;

        // Test gzclearerr
        gzclearerr(file_read);

        // Test gzfread again after clearing error
        if (nitems > 0) {
            items_read = gzfread(read_buf, item_size, nitems, file_read);
        }

        gzclose(file_read);
        file_read = NULL;
    }

    // Test gzopen and gzopen64 for writing
    file_write = gzopen("./dummy_file", "wb");
    if (!file_write) {
        file_write = gzopen64("./dummy_file", "wb");
    }

    if (file_write) {
        // Test gzfwrite
        z_size_t item_size = (Data[0] % 100) + 1;
        z_size_t nitems = (Data[1] % 100) + 1;
        
        // Prevent overflow and ensure buffer fits
        if (item_size > 0 && nitems > 0) {
            // Check for potential overflow before multiplication
            if (item_size > buf_size / nitems) {
                nitems = buf_size / item_size;
            }
            if (nitems == 0) {
                // If nitems becomes 0, we cannot write anything safely with current item_size
                // Set nitems to 1 only if item_size fits, otherwise skip
                if (item_size > buf_size) {
                    nitems = 0;
                } else {
                    nitems = 1;
                }
            }
        } else {
            item_size = 1;
            nitems = 1;
        }

        if (nitems > 0) {
            items_written = gzfwrite(write_buf, item_size, nitems, file_write);
        }

        // Test gzerror
        err_str = gzerror(file_write, &errnum);
        (void)err_str;

        // Test gzclearerr
        gzclearerr(file_write);

        // Test gzfwrite again after clearing error
        if (nitems > 0) {
            items_written = gzfwrite(write_buf, item_size, nitems, file_write);
        }

        gzclose(file_write);
        file_write = NULL;
    }

    // Cleanup
    free(read_buf);
    free(write_buf);

    return 0;
}