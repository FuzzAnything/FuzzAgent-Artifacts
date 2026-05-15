// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
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
#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    magic_t ms = magic_open(0);
    if (!ms) return 0;
    
    // Use first byte to determine operation mix
    uint8_t op_selector = Data[0];
    size_t data_offset = 1;
    
    // Write data to dummy file for magic_file operations
    if (Size > data_offset) {
        write_dummy_file(Data + data_offset, Size - data_offset);
    }
    
    // Try loading magic database (use default)
    int load_ret = magic_load(ms, NULL);
    
    // Test magic_getflags
    int flags = magic_getflags(ms);
    
    // Test magic_setflags with fuzzed data
    if (Size > data_offset + sizeof(int)) {
        int new_flags;
        memcpy(&new_flags, Data + data_offset, sizeof(int));
        magic_setflags(ms, new_flags);
        data_offset += sizeof(int);
    }
    
    // Test magic_file with dummy file
    if (Size > data_offset) {
        const char *result = magic_file(ms, "./dummy_file");
        (void)result; // Use result to avoid unused variable warning
    }
    
    // Test magic_setparam with fuzzed parameters
    if (Size > data_offset + sizeof(int) + sizeof(size_t)) {
        int param_id;
        size_t param_value;
        
        memcpy(&param_id, Data + data_offset, sizeof(int));
        data_offset += sizeof(int);
        memcpy(&param_value, Data + data_offset, sizeof(size_t));
        data_offset += sizeof(size_t);
        
        // Try common parameter IDs
        int param_ids[] = {MAGIC_PARAM_INDIR_MAX, MAGIC_PARAM_NAME_MAX, 
                          MAGIC_PARAM_ELF_PHNUM_MAX, MAGIC_PARAM_ELF_SHNUM_MAX,
                          MAGIC_PARAM_ELF_NOTES_MAX, MAGIC_PARAM_REGEX_MAX,
                          MAGIC_PARAM_BYTES_MAX};
        
        int selected_param = param_ids[param_id % (sizeof(param_ids)/sizeof(param_ids[0]))];
        magic_setparam(ms, selected_param, &param_value);
    }
    
    // Test magic_getparam
    if (Size > data_offset + sizeof(int)) {
        int getparam_id;
        size_t getparam_value;
        
        memcpy(&getparam_id, Data + data_offset, sizeof(int));
        data_offset += sizeof(int);
        
        int param_ids[] = {MAGIC_PARAM_INDIR_MAX, MAGIC_PARAM_NAME_MAX, 
                          MAGIC_PARAM_ELF_PHNUM_MAX, MAGIC_PARAM_ELF_SHNUM_MAX,
                          MAGIC_PARAM_ELF_NOTES_MAX, MAGIC_PARAM_REGEX_MAX,
                          MAGIC_PARAM_BYTES_MAX};
        
        int selected_param = param_ids[getparam_id % (sizeof(param_ids)/sizeof(param_ids[0]))];
        magic_getparam(ms, selected_param, &getparam_value);
    }
    
    // Clean up
    magic_close(ms);
    unlink("./dummy_file");
    
    return 0;
}