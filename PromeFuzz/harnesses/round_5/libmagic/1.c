// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

static void fuzz_magic_list(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    magic_t cookie = magic_open(0);
    if (cookie == NULL) return;
    
    write_dummy_file(data, size);
    magic_list(cookie, "./dummy_file");
    magic_close(cookie);
    cleanup_dummy_file();
}

static void fuzz_magic_load(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    magic_t cookie = magic_open(0);
    if (cookie == NULL) return;
    
    write_dummy_file(data, size);
    magic_load(cookie, "./dummy_file");
    magic_close(cookie);
    cleanup_dummy_file();
}

static void fuzz_magic_check(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    magic_t cookie = magic_open(0);
    if (cookie == NULL) return;
    
    write_dummy_file(data, size);
    magic_check(cookie, "./dummy_file");
    magic_close(cookie);
    cleanup_dummy_file();
}

static void fuzz_magic_open(const uint8_t *data, size_t size) {
    if (size < sizeof(int)) return;
    
    int flags = *(int*)data;
    magic_t cookie = magic_open(flags);
    if (cookie != NULL) {
        magic_close(cookie);
    }
}

static void fuzz_magic_error(const uint8_t *data, size_t size) {
    magic_t cookie = magic_open(0);
    if (cookie == NULL) return;
    
    if (size > 0) {
        write_dummy_file(data, size);
        magic_load(cookie, "./dummy_file");
        cleanup_dummy_file();
    }
    
    magic_error(cookie);
    magic_close(cookie);
}

static void fuzz_magic_load_buffers(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    magic_t cookie = magic_open(0);
    if (cookie == NULL) return;
    
    size_t num_buffers = 1;
    void *buffers[1] = {(void*)data};
    size_t sizes[1] = {size};
    
    magic_load_buffers(cookie, buffers, sizes, num_buffers);
    magic_close(cookie);
}

int LLVMFuzzerTestOneInput_1(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    unsigned char selector = data[0] % 6;
    const uint8_t *fuzz_data = data + 1;
    size_t fuzz_size = size - 1;
    
    switch (selector) {
        case 0:
            fuzz_magic_list(fuzz_data, fuzz_size);
            break;
        case 1:
            fuzz_magic_load(fuzz_data, fuzz_size);
            break;
        case 2:
            fuzz_magic_check(fuzz_data, fuzz_size);
            break;
        case 3:
            fuzz_magic_open(fuzz_data, fuzz_size);
            break;
        case 4:
            fuzz_magic_error(fuzz_data, fuzz_size);
            break;
        case 5:
            fuzz_magic_load_buffers(fuzz_data, fuzz_size);
            break;
    }
    
    return 0;
}