// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_checkTable at compileTranslationTable.c:5238:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_charToDots at lou_translateString.c:4173:1 in liblouis.h
// lou_dotsToChar at lou_translateString.c:4150:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_backTranslateString at lou_backTranslateString.c:152:1 in liblouis.h
#include "config.h"
#include "liblouis.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_LENGTH 1024

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write table data to dummy file
    write_dummy_file(Data, Size);
    
    // Use dummy file as table list
    const char *tableList = "./dummy_file";
    
    // Test lou_checkTable
    int check_result = lou_checkTable(tableList);
    
    // Test lou_getTable
    const void *table_ptr = lou_getTable(tableList);
    
    // Prepare buffers for translation functions
    widechar inbuf[MAX_LENGTH];
    widechar outbuf[MAX_LENGTH];
    int length = 0;
    
    // Determine length based on available data
    if (Size >= sizeof(widechar)) {
        length = (Size / sizeof(widechar)) % MAX_LENGTH;
        if (length == 0 && Size >= sizeof(widechar)) length = 1;
        
        // Fill input buffer with widechar data
        size_t bytes_to_copy = length * sizeof(widechar);
        if (bytes_to_copy > Size) bytes_to_copy = Size;
        memcpy(inbuf, Data, bytes_to_copy);
        
        // Fill remaining with zeros if needed
        if (bytes_to_copy < length * sizeof(widechar)) {
            memset((uint8_t*)inbuf + bytes_to_copy, 0, 
                   length * sizeof(widechar) - bytes_to_copy);
        }
    } else {
        length = 1;
        inbuf[0] = 0;
    }
    
    // Test lou_charToDots with different modes
    int mode = Data[0] % 4; // 0-3 modes
    lou_charToDots(tableList, inbuf, outbuf, length, mode);
    
    // Test with ucBrl mode (mode 1)
    lou_charToDots(tableList, inbuf, outbuf, length, 1);
    
    // Test lou_dotsToChar
    lou_dotsToChar(tableList, inbuf, outbuf, length, mode);
    
    // Test lou_translateString
    int inlen = length;
    int outlen = MAX_LENGTH;
    formtype typeform[MAX_LENGTH];
    char spacing[MAX_LENGTH + 1]; // +1 for null terminator
    
    // Randomly decide whether to use typeform and spacing
    int use_typeform = Data[0] & 0x01;
    int use_spacing = Data[0] & 0x02;
    
    lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen,
                       use_typeform ? typeform : NULL,
                       use_spacing ? spacing : NULL,
                       mode);
    
    // Test lou_backTranslateString
    inlen = length;
    outlen = MAX_LENGTH;
    
    lou_backTranslateString(tableList, inbuf, &inlen, outbuf, &outlen,
                           use_typeform ? typeform : NULL,
                           use_spacing ? spacing : NULL,
                           mode);
    
    // Test with different table list strings
    if (Size > 10) {
        // Create a table list from fuzzer data
        char dynamic_table[256];
        size_t table_len = Size % 256;
        if (table_len == 0) table_len = 1;
        memcpy(dynamic_table, Data, table_len);
        dynamic_table[table_len - 1] = '\0';
        
        lou_checkTable(dynamic_table);
        lou_getTable(dynamic_table);
        
        // Test with comma-separated table list
        if (table_len > 2) {
            dynamic_table[table_len/2] = ',';
            lou_checkTable(dynamic_table);
        }
    }
    
    // Test edge cases
    if (Size > 0) {
        // Test with NULL table list
        lou_checkTable(NULL);
        lou_getTable(NULL);
        
        // Test with empty string
        lou_checkTable("");
        lou_getTable("");
        
        // Test with zero length
        lou_charToDots(tableList, inbuf, outbuf, 0, mode);
        lou_dotsToChar(tableList, inbuf, outbuf, 0, mode);
        
        // Test with negative length (should be handled by library)
        lou_charToDots(tableList, inbuf, outbuf, -1, mode);
        lou_dotsToChar(tableList, inbuf, outbuf, -1, mode);
        
        // Test translation with zero lengths
        inlen = 0;
        outlen = 0;
        lou_translateString(tableList, inbuf, &inlen, outbuf, &outlen,
                           NULL, NULL, mode);
        
        inlen = 0;
        outlen = 0;
        lou_backTranslateString(tableList, inbuf, &inlen, outbuf, &outlen,
                               NULL, NULL, mode);
    }
    
    // Test with maximum length
    if (length > 0) {
        lou_charToDots(tableList, inbuf, outbuf, MAX_LENGTH, mode);
        lou_dotsToChar(tableList, inbuf, outbuf, MAX_LENGTH, mode);
    }
    
    return 0;
}