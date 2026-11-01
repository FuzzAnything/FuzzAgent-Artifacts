// This fuzz driver is generated for library liblouis, aiming to fuzz the following functions:
// lou_indexTables at metadata.c:945:1 in liblouis.h
// lou_translateString at lou_translateString.c:1128:1 in liblouis.h
// lou_translatePrehyphenated at lou_translateString.c:1410:1 in liblouis.h
// lou_getEmphClasses at compileTranslationTable.c:5070:1 in liblouis.h
// lou_translate at lou_translateString.c:1135:1 in liblouis.h
// lou_getTable at compileTranslationTable.c:5118:1 in liblouis.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <liblouis.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void safe_free_emph_classes(char const **classes) {
    if (classes) {
        size_t i = 0;
        while (classes[i] != NULL) {
            free((void *)classes[i]);
            i++;
        }
        free((void *)classes);
    }
}

static int is_valid_table_name(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    // Simple heuristic: ensure it looks like a table name (no nulls in middle, printable)
    for (size_t i = 0; i < size; i++) {
        if (data[i] == 0) return 0;
        if (data[i] < 32 || data[i] > 126) return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare environment
    // We will use a subset of data for table names and the rest for input text.
    // We need to ensure table names are valid strings.
    
    // Try to extract a table name from the beginning of the input
    // If the input doesn't look like a table name, we default to a known safe table or skip.
    // For fuzzing, we often want to try random strings as table names to trigger parsing errors.
    
    // Let's split the input: first N bytes as table name, rest as text.
    // We'll try a few splits or just use the whole thing as table name if short.
    
    // Define buffers
    const size_t MAX_BUF = 4096;
    widechar inbuf[MAX_BUF];
    widechar outbuf[MAX_BUF];
    formtype typeform[MAX_BUF];
    char spacing[MAX_BUF];
    int outputPos[MAX_BUF];
    int inputPos[MAX_BUF];
    int cursorPos = 0;
    char inputHyphens[MAX_BUF];
    char outputHyphens[MAX_BUF];
    
    int inlen = 0;
    int outlen = MAX_BUF;
    
    // Initialize buffers
    memset(inbuf, 0, sizeof(inbuf));
    memset(outbuf, 0, sizeof(outbuf));
    memset(typeform, 0, sizeof(typeform));
    memset(spacing, 0, sizeof(spacing));
    memset(outputPos, 0, sizeof(outputPos));
    memset(inputPos, 0, sizeof(inputPos));
    memset(inputHyphens, 0, sizeof(inputHyphens));
    memset(outputHyphens, 0, sizeof(outputHyphens));

    // Create a table name string from input
    // We limit the table name length to avoid huge allocations or issues
    size_t table_name_len = Size;
    if (table_name_len > 256) table_name_len = 256;
    
    char *table_name = malloc(table_name_len + 1);
    if (!table_name) return 0;
    memcpy(table_name, Data, table_name_len);
    table_name[table_name_len] = '\0';

    // Prepare input text (widechar) from the rest of the data or the same data
    // We'll use the data as is, but ensure it's valid UTF-16-ish or just raw bytes cast to widechar
    // liblouis expects widechar (unsigned short). We'll just cast the bytes.
    size_t text_len = Size;
    if (text_len > MAX_BUF - 1) text_len = MAX_BUF - 1;
    
    for (size_t i = 0; i < text_len; i++) {
        inbuf[i] = (widechar)Data[i];
    }
    inlen = text_len;

    // Prepare typeform and spacing arrays if needed
    // For fuzzing, we can pass NULL or valid arrays. Let's try both strategies based on data pattern.
    formtype *tf_ptr = NULL;
    char *sp_ptr = NULL;
    
    if (Size > 10) {
        // Use part of data to fill typeform and spacing
        for (int i = 0; i < inlen; i++) {
            typeform[i] = (formtype)(Data[(i + 5) % Size]);
            spacing[i] = (char)(Data[(i + 10) % Size]);
        }
        tf_ptr = typeform;
        sp_ptr = spacing;
    }

    // Strategy 1: lou_indexTables
    // Requires NULL-terminated array of table names
    const char *tables[2];
    tables[0] = table_name;
    tables[1] = NULL;
    
    // Call lou_indexTables
    lou_indexTables(tables);

    // Strategy 2: lou_translateString
    // Try with valid table name or random string
    int mode = (Size % 4); // Vary mode
    int ret = lou_translateString(table_name, inbuf, &inlen, outbuf, &outlen, tf_ptr, sp_ptr, mode);
    // Check return, but no specific cleanup needed for this function other than buffers

    // Strategy 3: lou_translatePrehyphenated
    // Prepare hyphen arrays
    for (int i = 0; i < inlen; i++) {
        inputHyphens[i] = (char)(Data[(i + 15) % Size]);
        outputHyphens[i] = 0;
        outputPos[i] = i; // Initial guess
        inputPos[i] = i;
    }
    ret = lou_translatePrehyphenated(table_name, inbuf, &inlen, outbuf, &outlen, tf_ptr, sp_ptr, 
                                     outputPos, inputPos, &cursorPos, inputHyphens, outputHyphens, mode);

    // Strategy 4: lou_getEmphClasses
    char const **emph_classes = lou_getEmphClasses(table_name);
    // Cleanup emph_classes
    safe_free_emph_classes(emph_classes);

    // Strategy 5: lou_translate
    // Re-initialize lengths for this call
    int inlen2 = inlen;
    int outlen2 = MAX_BUF;
    int cursorPos2 = (Size % inlen2);
    ret = lou_translate(table_name, inbuf, &inlen2, outbuf, &outlen2, tf_ptr, sp_ptr, 
                        outputPos, inputPos, &cursorPos2, mode);

    // Strategy 6: lou_getTable
    const void *table = lou_getTable(table_name);
    // No cleanup needed for table pointer as it's managed internally or const

    // Cleanup
    free(table_name);

    return 0;
}