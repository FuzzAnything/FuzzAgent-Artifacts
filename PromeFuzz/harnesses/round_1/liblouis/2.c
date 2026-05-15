// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

static void logCallback(logLevels level, const char *message) {
    // Empty callback to suppress logging during fuzzing
    (void)level;
    (void)message;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Ensure minimum input size
    if (Size < 1) return 0;
    
    // Prepare table list string (null-terminated)
    size_t table_len = Size > 100 ? 100 : Size;
    char *table_list = malloc(table_len + 1);
    if (!table_list) return 0;
    memcpy(table_list, Data, table_len);
    table_list[table_len] = '\0';
    
    // Register log callback
    lou_registerLogCallback(logCallback);
    
    // Check table validity
    lou_checkTable(table_list);
    
    // Free library memory twice as specified
    lou_free();
    lou_free();
    
    // Prepare input buffer for translation
    size_t inbuf_len = (Size - table_len) / sizeof(widechar);
    if (inbuf_len > 0 && inbuf_len < 1000) {
        widechar *inbuf = malloc(inbuf_len * sizeof(widechar));
        if (inbuf) {
            // Copy remaining data as wide characters
            memcpy(inbuf, Data + table_len, inbuf_len * sizeof(widechar));
            
            int inlen = (int)inbuf_len;
            int outlen = inlen * 10; // Reasonable output buffer size
            widechar *outbuf = malloc(outlen * sizeof(widechar));
            
            // Prepare optional arrays based on input data
            formtype *typeform = NULL;
            char *spacing = NULL;
            
            // Use first byte to decide whether to allocate optional arrays
            if (Data[0] & 0x01) {
                typeform = malloc(inlen * sizeof(formtype));
                if (typeform) {
                    memset(typeform, 0, inlen * sizeof(formtype));
                }
            }
            
            if (Data[0] & 0x02) {
                spacing = malloc((inlen + 1) * sizeof(char));
                if (spacing) {
                    memset(spacing, 0, (inlen + 1) * sizeof(char));
                }
            }
            
            // Mode from input data (mask to reasonable values)
            int mode = Data[0] & 0x0F;
            
            if (outbuf) {
                // Perform translation
                lou_translateString(table_list, inbuf, &inlen, outbuf, &outlen, 
                                   typeform, spacing, mode);
                free(outbuf);
            }
            
            // Cleanup optional arrays
            free(typeform);
            free(spacing);
            free(inbuf);
        }
    }
    
    // Final cleanup
    lou_free();
    free(table_list);
    
    return 0;
}