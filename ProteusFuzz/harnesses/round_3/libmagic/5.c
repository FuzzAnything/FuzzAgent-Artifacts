// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    if (size > 0) {
        ssize_t written = write(fd, data, size);
        if (written != (ssize_t)size) {
            close(fd);
            return -1;
        }
    }
    
    close(fd);
    return 0;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test magic_open
    magic_t mag = magic_open(MAGIC_MIME_TYPE | MAGIC_ERROR);
    if (mag == NULL) {
        return 0;
    }

    // Test magic_check with a separate magic_set
    magic_t mag_check = magic_open(MAGIC_MIME_TYPE | MAGIC_ERROR);
    if (mag_check != NULL) {
        // Create a dummy magic file for testing magic_check
        int fd = open("./dummy_magic", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd >= 0) {
            const char *dummy_magic_content = "0 byte 0x89\n";
            write(fd, dummy_magic_content, strlen(dummy_magic_content));
            close(fd);
            
            // Test magic_check
            magic_check(mag_check, "./dummy_magic");
            
            // Clean up dummy magic file
            unlink("./dummy_magic");
        }
        magic_close(mag_check);
    }

    // Test magic_load
    if (magic_load(mag, NULL) != -1) {
        // This should fail, but we continue to test other paths
    }

    // Test magic_load_buffers
    void *buffers[2];
    size_t sizes[2];
    buffers[0] = (void *)Data;
    sizes[0] = Size;
    buffers[1] = NULL;
    sizes[1] = 0;
    magic_load_buffers(mag, buffers, sizes, 2);

    // Test magic_compile
    if (magic_compile(mag, NULL) != -1) {
        // This should fail, but we continue to test other paths
    }

    // Test magic_file
    if (write_dummy_file(Data, Size) == 0) {
        const char *result = magic_file(mag, "./dummy_file");
        if (result != NULL) {
            // Use result to avoid unused variable warning
            (void)strlen(result);
        }
        unlink("./dummy_file");
    }

    // Test with NULL handle to ensure error handling
    magic_check(NULL, "./dummy_magic");
    magic_load(NULL, NULL);
    magic_load_buffers(NULL, NULL, NULL, 0);
    magic_compile(NULL, NULL);
    magic_file(NULL, "./dummy_file");

    // Cleanup
    magic_close(mag);

    return 0;
}