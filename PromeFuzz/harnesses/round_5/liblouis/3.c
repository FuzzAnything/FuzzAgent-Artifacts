// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_registerLogCallback at logging.c:86:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_free at compileTranslationTable.c:5363:1 in liblouis.h
#include "config.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Register log callback first (pass NULL to reset)
    lou_registerLogCallback(NULL);
    
    // Prepare table list string from fuzzer input
    char *tableList = NULL;
    if (Size > 0) {
        tableList = (char *)malloc(Size + 1);
        if (!tableList) return 0;
        memcpy(tableList, Data, Size);
        tableList[Size] = '\0';
    } else {
        // Use dummy table list if input is empty
        tableList = strdup("en-us-g2.ctb");
    }
    
    // Call lou_checkTable with the table list
    lou_checkTable(tableList);
    
    // First call to lou_free
    lou_free();
    
    // Second call to lou_free (as per specified order)
    lou_free();
    
    // Prepare parameters for lou_translateString
    int inlen = 0;
    int outlen = 0;
    widechar *inbuf = NULL;
    widechar *outbuf = NULL;
    formtype *typeform = NULL;
    char *spacing = NULL;
    int mode = 0;
    
    // Determine input length from fuzzer data
    if (Size > 0) {
        // Use up to 256 characters from input for translation
        inlen = (Size > 256) ? 256 : Size;
        inbuf = (widechar *)malloc(inlen * sizeof(widechar));
        if (inbuf) {
            // Convert bytes to widechars (simple zero-extension)
            for (int i = 0; i < inlen; i++) {
                inbuf[i] = (widechar)Data[i];
            }
            
            // Allocate output buffer (twice input size as reasonable guess)
            outlen = inlen * 2;
            outbuf = (widechar *)malloc(outlen * sizeof(widechar));
            
            // Optionally allocate typeform and spacing arrays
            if (Size % 2 == 0) {  // Use fuzzer input to decide
                typeform = (formtype *)malloc(inlen * sizeof(formtype));
                if (typeform) {
                    for (int i = 0; i < inlen; i++) {
                        typeform[i] = (formtype)(Data[i] % 16);  // Various form types
                    }
                }
                
                spacing = (char *)malloc((inlen + 1) * sizeof(char));
                if (spacing) {
                    memset(spacing, 0, (inlen + 1) * sizeof(char));
                }
            }
            
            // Set mode based on input
            mode = (Data[0] % 8);  // Various modes
            
            // Call lou_translateString
            lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen, 
                              typeform, spacing, mode);
        }
    }
    
    // Third call to lou_free
    lou_free();
    
    // Cleanup
    free(tableList);
    free(inbuf);
    free(outbuf);
    free(typeform);
    free(spacing);
    
    return 0;
}