// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "magic.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    static const char *dummy_filename = "./dummy_file";
    
    // Write fuzzer data to dummy file
    FILE *fp = fopen(dummy_filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Try different flags to explore more states
    int flags[] = {0, MAGIC_MIME, MAGIC_MIME_TYPE, MAGIC_CONTINUE, 
                   MAGIC_CHECK, MAGIC_PRESERVE_ATIME, MAGIC_RAW, 
                   MAGIC_ERROR, MAGIC_NO_CHECK_COMPRESS, MAGIC_NO_CHECK_TAR,
                   MAGIC_NO_CHECK_SOFT, MAGIC_NO_CHECK_APPTYPE,
                   MAGIC_NO_CHECK_ELF, MAGIC_NO_CHECK_TEXT,
                   MAGIC_NO_CHECK_CSV, MAGIC_NO_CHECK_CDF,
                   MAGIC_NO_CHECK_TOKENS, MAGIC_NO_CHECK_JSON};
    
    for (size_t i = 0; i < sizeof(flags)/sizeof(flags[0]); i++) {
        // Open magic handle
        magic_t ms = magic_open(flags[i]);
        if (ms == NULL) {
            continue;
        }
        
        // Load magic database
        int load_result = magic_load(ms, NULL);
        
        // Check for load errors
        const char *load_error = magic_error(ms);
        (void)load_error; // Use variable to avoid unused warning
        
        // Identify file type
        const char *file_type = magic_file(ms, dummy_filename);
        (void)file_type; // Use variable to avoid unused warning
        
        // Check for file identification errors
        const char *file_error = magic_error(ms);
        (void)file_error; // Use variable to avoid unused warning
        
        // Close magic handle
        magic_close(ms);
    }
    
    // Clean up dummy file
    unlink(dummy_filename);
    
    return 0;
}