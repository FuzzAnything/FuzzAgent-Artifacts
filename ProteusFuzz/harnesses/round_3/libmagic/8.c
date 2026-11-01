// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_getparam at magic.c:656:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    magic_t m = NULL;
    size_t param_val;
    size_t set_param_val;
    void *buffers[10];
    size_t buffer_sizes[10];
    int num_buffers = 0;

    // 1. Prepare environment: Open magic handle
    // Try different flags to explore states
    int flags = MAGIC_NONE;
    if (Size > 0) flags = Data[0] & 0x0F; // Use input to vary flags slightly
    m = magic_open(flags);
    if (m == NULL) {
        return 0;
    }

    // 2. Fuzz magic_setflags
    // Use input data to generate flags
    int set_flags = MAGIC_NONE;
    if (Size > 1) set_flags = Data[1] & 0xFF;
    if (Size > 2) set_flags |= (Data[2] << 8);
    
    magic_setflags(m, set_flags);

    // 3. Fuzz magic_setparam
    // Try different parameter types based on input
    if (Size > 3) {
        int param_type = Data[3] % 10; // Limit to a small range of valid params
        int actual_param = 0;
        // Map to known MAGIC_PARAM constants available in libmagic
        // Using standard constants: MAGIC_PARAM_MAXINDIR, MAGIC_PARAM_NAME_MAX, etc.
        // If specific constants are missing in older versions, we use the raw integer values 
        // typically defined in magic.h for these parameters if macros are missing, 
        // but standard libmagic defines them as MAGIC_PARAM_MAXINDIR (0), MAGIC_PARAM_NAME_MAX (1), etc.
        // To be safe against missing macros in some builds, we can try to use the enum values directly if defined,
        // or fallback to known integer values if the macros are not present.
        // However, the error indicates the macros are missing. 
        // Standard libmagic.h defines:
        // #define MAGIC_PARAM_MAXINDIR 0
        // #define MAGIC_PARAM_NAME_MAX 1
        // #define MAGIC_PARAM_ELF_SHNUM_MAX 2
        // #define MAGIC_PARAM_ELF_PHNUM_MAX 3
        // #define MAGIC_PARAM_REGEX_MAX 4
        // #define MAGIC_PARAM_MAGWARN_MAX 5
        // #define MAGIC_PARAM_BYTES_MAX 6
        // #define MAGIC_PARAM_ENCODING_MAX 7
        // #define MAGIC_PARAM_ELF_SHSIZE_MAX 8
        
        switch (param_type % 9) {
            case 0: actual_param = 0; break; // MAGIC_PARAM_MAXINDIR
            case 1: actual_param = 1; break; // MAGIC_PARAM_NAME_MAX
            case 2: actual_param = 2; break; // MAGIC_PARAM_ELF_SHNUM_MAX
            case 3: actual_param = 3; break; // MAGIC_PARAM_ELF_PHNUM_MAX
            case 4: actual_param = 4; break; // MAGIC_PARAM_REGEX_MAX
            case 5: actual_param = 5; break; // MAGIC_PARAM_MAGWARN_MAX
            case 6: actual_param = 6; break; // MAGIC_PARAM_BYTES_MAX
            case 7: actual_param = 7; break; // MAGIC_PARAM_ENCODING_MAX
            case 8: actual_param = 8; break; // MAGIC_PARAM_ELF_SHSIZE_MAX
        }
        
        if (Size > 4) {
            set_param_val = Data[4];
            if (Size > 5) set_param_val |= ((size_t)Data[5] << 8);
            if (Size > 6) set_param_val |= ((size_t)Data[6] << 16);
            if (Size > 7) set_param_val |= ((size_t)Data[7] << 24);
        } else {
            set_param_val = 100;
        }
        
        magic_setparam(m, actual_param, &set_param_val);
    }

    // 4. Fuzz magic_getparam
    if (Size > 8) {
        int get_param_type = Data[8] % 9;
        int actual_get_param = 0;
        switch (get_param_type) {
            case 0: actual_get_param = 0; break; // MAGIC_PARAM_MAXINDIR
            case 1: actual_get_param = 1; break; // MAGIC_PARAM_NAME_MAX
            case 2: actual_get_param = 2; break; // MAGIC_PARAM_ELF_SHNUM_MAX
            case 3: actual_get_param = 3; break; // MAGIC_PARAM_ELF_PHNUM_MAX
            case 4: actual_get_param = 4; break; // MAGIC_PARAM_REGEX_MAX
            case 5: actual_get_param = 5; break; // MAGIC_PARAM_MAGWARN_MAX
            case 6: actual_get_param = 6; break; // MAGIC_PARAM_BYTES_MAX
            case 7: actual_get_param = 7; break; // MAGIC_PARAM_ENCODING_MAX
            case 8: actual_get_param = 8; break; // MAGIC_PARAM_ELF_SHSIZE_MAX
        }
        magic_getparam(m, actual_get_param, &param_val);
    }

    // 5. Fuzz magic_compile
    // Create a dummy file to compile
    if (Size > 0) {
        write_dummy_file(Data, Size);
        magic_compile(m, "./dummy_file");
    }

    // 6. Fuzz magic_load_buffers
    // Prepare buffers from input data
    if (Size > 0) {
        // Split input into multiple buffers for fuzzing
        int split_count = 1;
        if (Size > 10) split_count = (Data[0] % 5) + 1; // 1 to 5 buffers
        
        size_t chunk_size = Size / split_count;
        size_t offset = 0;
        
        for (int i = 0; i < split_count && i < 10; i++) {
            size_t current_size = (i == split_count - 1) ? (Size - offset) : chunk_size;
            if (current_size == 0) current_size = 1; // Avoid zero size if possible
            
            buffers[i] = (void*)(Data + offset);
            buffer_sizes[i] = current_size;
            offset += current_size;
            num_buffers++;
        }
        
        if (num_buffers > 0) {
            magic_load_buffers(m, buffers, buffer_sizes, num_buffers);
        }
    }

    // Cleanup
    magic_close(m);
    
    // Remove dummy file
    unlink("./dummy_file");

    return 0;
}