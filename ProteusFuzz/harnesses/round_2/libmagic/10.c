// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static int create_dummy_file(const uint8_t *data, size_t size) {
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

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    magic_t cookie = NULL;
    int ret;
    size_t param_val;
    const char *result;

    // 1. Prepare environment: Open magic database
    // Ensure we have enough data before accessing indices
    int flags = 0;
    if (Size >= 2) {
        flags = (Data[0] % 4) * MAGIC_MIME_TYPE + (Data[1] % 2) * MAGIC_ERROR;
    }
    
    cookie = magic_open(flags);
    if (cookie == NULL) {
        return 0;
    }

    // 2. Test magic_setparam with various parameters
    if (Size >= 5) {
        size_t test_val = (size_t)((Data[2] << 8) | Data[3]);
        int param_type = (Data[4] % 5);
        
        int params_to_try[] = {
            0, // MAGIC_PARAM_REGEX_MAX
            1, // MAGIC_PARAM_INDIR_MAX
            2, // MAGIC_PARAM_NAME_MAX
            3, // MAGIC_PARAM_ELF_SHNUM_MAX
            4  // MAGIC_PARAM_ELF_PHNUM_MAX
        };
        
        int param = (param_type < 5) ? params_to_try[param_type] : 0;
        ret = magic_setparam(cookie, param, &test_val);
        (void)ret;
    }

    // 3. Test magic_getparam
    if (Size >= 6) {
        int param = (Data[5] % 5);
        int params_to_try[] = {
            0, 1, 2, 3, 4
        };
        int get_param = (param < 5) ? params_to_try[param] : 0;
        ret = magic_getparam(cookie, get_param, &param_val);
        (void)ret;
        (void)param_val;
    }

    // 4. Test magic_compile and magic_load
    // Create a dummy file to load/compile
    if (Size > 0) {
        if (create_dummy_file(Data, Size) == 0) {
            // Try compile
            ret = magic_compile(cookie, "./dummy_file");
            (void)ret;
            
            // Try load
            ret = magic_load(cookie, "./dummy_file");
            (void)ret;
        }
    }

    // 5. Test magic_file
    // Use the dummy file we created
    result = magic_file(cookie, "./dummy_file");
    if (result != NULL) {
        (void)result;
    }

    // 6. Cleanup
    magic_close(cookie);
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}