// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_compileString at compileTranslationTable.c:5430:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
#define HAVE_CONFIG_H
#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "liblouis.h"

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Use first byte to select which function to test
    uint8_t function_selector = Data[0] % 6;
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;

    // Extract tableList string (null-terminated)
    size_t tableList_len = remaining_size;
    if (tableList_len > 255) tableList_len = 255;
    char tableList[256];
    memcpy(tableList, remaining_data, tableList_len);
    tableList[tableList_len] = '\0';

    // Extract inString for functions that need it
    const char *inString = "";
    if (remaining_size > tableList_len + 1) {
        inString = (const char *)(remaining_data + tableList_len);
    }

    // For widechar functions, prepare buffers
    #define MAX_WIDECHAR_LEN 100
    widechar inbuf[MAX_WIDECHAR_LEN];
    widechar outbuf[MAX_WIDECHAR_LEN];
    int length = 0;
    
    // Prepare widechar input buffer from remaining data
    if (remaining_size > 0) {
        size_t widechar_len = remaining_size / sizeof(widechar);
        if (widechar_len > MAX_WIDECHAR_LEN) widechar_len = MAX_WIDECHAR_LEN;
        length = (int)widechar_len;
        memcpy(inbuf, remaining_data, length * sizeof(widechar));
    }

    // Prepare additional buffers for translateString
    formtype typeform[MAX_WIDECHAR_LEN];
    char spacing[MAX_WIDECHAR_LEN + 1];
    int inlen = length;
    int outlen = MAX_WIDECHAR_LEN;

    // Clear spacing array
    memset(spacing, 0, sizeof(spacing));

    // Random mode selection
    int mode = (remaining_size > 0) ? (remaining_data[0] % 4) : 0;

    switch (function_selector) {
        case 0:
            // lou_checkTable
            lou_checkTable(tableList);
            break;
            
        case 1:
            // lou_compileString
            // First try to get a valid table
            if (lou_getTable(tableList) != NULL) {
                lou_compileString(tableList, inString);
            }
            break;
            
        case 2:
            // lou_charToDots
            if (lou_getTable(tableList) != NULL) {
                lou_charToDots(tableList, inbuf, outbuf, length, mode);
            }
            break;
            
        case 3:
            // lou_getTable
            lou_getTable(tableList);
            break;
            
        case 4:
            // lou_dotsToChar
            if (lou_getTable(tableList) != NULL) {
                lou_dotsToChar(tableList, inbuf, outbuf, length, mode);
            }
            break;
            
        case 5:
            // lou_translateString
            if (lou_getTable(tableList) != NULL) {
                // Randomly decide whether to use typeform and spacing
                uint8_t use_typeform = (remaining_size > 1) ? (remaining_data[1] % 2) : 0;
                uint8_t use_spacing = (remaining_size > 2) ? (remaining_data[2] % 2) : 0;
                
                lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen,
                                   use_typeform ? typeform : NULL,
                                   use_spacing ? spacing : NULL,
                                   mode);
            }
            break;
    }

    return 0;
}