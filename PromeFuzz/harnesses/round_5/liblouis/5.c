// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
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

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // 1. Register log callback
    lou_registerLogCallback(logCallback);
    
    // 2. Prepare table list from fuzz data
    // Use first part of data as table list, ensure null termination
    size_t table_len = Size > 100 ? 100 : Size;
    char *tableList = (char *)malloc(table_len + 1);
    if (!tableList) return 0;
    
    memcpy(tableList, Data, table_len);
    tableList[table_len] = '\0';
    
    // 3. Check table validity
    lou_checkTable(tableList);
    
    // 4. Call lou_free twice as specified
    lou_free();
    lou_free();
    
    // 5. Prepare input buffer for back translation
    // Use remaining data for input buffer, convert to widechar
    size_t remaining = Size - table_len;
    if (remaining < sizeof(widechar)) {
        free(tableList);
        return 0;
    }
    
    size_t widechar_count = remaining / sizeof(widechar);
    widechar *inbuf = (widechar *)malloc(widechar_count * sizeof(widechar));
    if (!inbuf) {
        free(tableList);
        return 0;
    }
    
    memcpy(inbuf, Data + table_len, widechar_count * sizeof(widechar));
    int inlen = (int)widechar_count;
    
    // 6. Prepare output buffer and related arrays
    int outlen = 4096;  // Reasonable initial size
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    if (!outbuf) {
        free(inbuf);
        free(tableList);
        return 0;
    }
    
    // Allocate typeform and spacing based on fuzz data characteristics
    formtype *typeform = NULL;
    char *spacing = NULL;
    
    // Use data pattern to decide whether to allocate typeform/spacing
    // This helps explore different code paths
    if (Size > 0 && (Data[0] & 0x01)) {
        typeform = (formtype *)malloc(outlen * sizeof(formtype));
        if (typeform) {
            memset(typeform, 0, outlen * sizeof(formtype));
        }
    }
    
    if (Size > 0 && (Data[0] & 0x02)) {
        spacing = (char *)malloc(outlen * sizeof(char));
        if (spacing) {
            memset(spacing, 0, outlen * sizeof(char));
        }
    }
    
    // 7. Perform back translation
    lou_backTranslateString(tableList, inbuf, &inlen, outbuf, &outlen, 
                           typeform, spacing, 0);
    
    // 8. Cleanup
    free(tableList);
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    
    // 9. Final free
    lou_free();
    
    return 0;
}