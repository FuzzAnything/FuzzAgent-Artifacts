// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_descriptor at magic.c:403:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    if (size > 0 && data != NULL) {
        if (write(fd, data, size) != (ssize_t)size) {
            close(fd);
            return -1;
        }
    }
    
    close(fd);
    return 0;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    magic_t cookie = NULL;
    const char *result = NULL;
    size_t param_val;
    int fd = -1;
    
    // Initialize magic handle
    cookie = magic_open(MAGIC_NONE);
    if (cookie == NULL) {
        return 0;
    }
    
    // Load magic database
    if (magic_load(cookie, NULL) != 0) {
        magic_close(cookie);
        return 0;
    }
    
    // 1. Test magic_buffer
    if (Size > 0) {
        result = magic_buffer(cookie, Data, Size);
        if (result != NULL) {
            (void)result;
        }
    }
    
    // 2. Test magic_setparam with various parameters
    // Using known constant values from libmagic source if macros are not exported
    // MAGIC_PARAM_INDIRECT_MAX = 0
    // MAGIC_PARAM_NAME_MAX = 1
    // MAGIC_PARAM_ELF_SHNUM_MAX = 2
    // MAGIC_PARAM_ELF_PHNUM_MAX = 3
    // MAGIC_PARAM_ELF_NOTES_MAX = 4
    // MAGIC_PARAM_REGEX_MAX = 5
    // MAGIC_PARAM_MAGWARN_MAX = 6
    // MAGIC_PARAM_BYTES_MAX = 7
    // MAGIC_PARAM_ENCODING_MAX = 8
    // MAGIC_PARAM_ELF_SHSIZE_MAX = 9
    
    param_val = (Size > 0) ? (size_t)Data[0] : 0;
    magic_setparam(cookie, 0, &param_val); // INDIRECT_MAX
    magic_setparam(cookie, 1, &param_val); // NAME_MAX
    magic_setparam(cookie, 2, &param_val); // ELF_SHNUM_MAX
    magic_setparam(cookie, 3, &param_val); // ELF_PHNUM_MAX
    magic_setparam(cookie, 4, &param_val); // ELF_NOTES_MAX
    magic_setparam(cookie, 5, &param_val); // REGEX_MAX
    magic_setparam(cookie, 6, &param_val); // MAGWARN_MAX
    magic_setparam(cookie, 7, &param_val); // BYTES_MAX
    magic_setparam(cookie, 8, &param_val); // ENCODING_MAX
    magic_setparam(cookie, 9, &param_val); // ELF_SHSIZE_MAX
    
    // 3. Test magic_setflags
    int flags = (Size > 1) ? (int)Data[1] : 0;
    magic_setflags(cookie, flags);
    
    // 4. Test magic_getparam
    magic_getparam(cookie, 0, &param_val); // INDIRECT_MAX
    magic_getparam(cookie, 1, &param_val); // NAME_MAX
    magic_getparam(cookie, 2, &param_val); // ELF_SHNUM_MAX
    magic_getparam(cookie, 3, &param_val); // ELF_PHNUM_MAX
    magic_getparam(cookie, 4, &param_val); // ELF_NOTES_MAX
    magic_getparam(cookie, 5, &param_val); // REGEX_MAX
    magic_getparam(cookie, 6, &param_val); // MAGWARN_MAX
    magic_getparam(cookie, 7, &param_val); // BYTES_MAX
    magic_getparam(cookie, 8, &param_val); // ENCODING_MAX
    magic_getparam(cookie, 9, &param_val); // ELF_SHSIZE_MAX
    
    // 5. Test magic_file and magic_descriptor with dummy file
    if (write_dummy_file(Data, Size) == 0) {
        result = magic_file(cookie, "./dummy_file");
        if (result != NULL) {
            (void)result;
        }
        
        fd = open("./dummy_file", O_RDONLY);
        if (fd >= 0) {
            result = magic_descriptor(cookie, fd);
            if (result != NULL) {
                (void)result;
            }
            close(fd);
        }
    }
    
    // Cleanup
    magic_close(cookie);
    
    return 0;
}