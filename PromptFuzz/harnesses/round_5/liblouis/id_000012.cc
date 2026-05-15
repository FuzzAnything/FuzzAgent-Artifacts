#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <liblouis/liblouis.h>
}
//<ID> 290
//<Prompt> []
/*<Combination>: [
*/
//<score> 3.3333333, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"lou_translatePrehyphenated":[[1418,7,1418,23,0,0,4,1]]},"library_calls":["lou_findTables","lou_freeTableFiles","lou_getEmphClasses","lou_freeEmphClasses","lou_translateString","lou_translate","lou_translatePrehyphenated","lou_getTableInfo","lou_freeTableInfo","lou_free"],"critical_calls":["lou_findTables","lou_freeTableFiles","lou_getEmphClasses","lou_freeEmphClasses","lou_translateString","lou_translate","lou_translatePrehyphenated","lou_freeTableInfo","lou_getTableInfo","lou_free"],"visited":2}
/**/


extern "C" {
}

// Custom type declarations from liblouis
typedef unsigned short formtype;
typedef unsigned short widechar;

// Helper function to convert raw bytes to widechar
static widechar*  id_000012_bytes_to_widechars(const uint8_t* data, size_t size, size_t* out_len) {
    if (size < sizeof(widechar)) {
        *out_len = 0;
        return nullptr;
    }
    
    size_t max_chars = size / sizeof(widechar);
    widechar* wchars = new widechar[max_chars];
    
    // Copy bytes to widechar array
    memcpy(wchars, data, max_chars * sizeof(widechar));
    *out_len = max_chars;
    
    return wchars;
}

// Helper function to create a null-terminated string from data
static char*  id_000012_create_string_from_data(const uint8_t* data, size_t size, size_t max_len) {
    if (size == 0) return nullptr;
    
    size_t len = (size < max_len) ? size : max_len;
    char* str = new char[len + 1];
    memcpy(str, data, len);
    str[len] = '\0';
    
    return str;
}

// Helper function to extract table list string
static const char*  id_000012_get_table_list(const uint8_t* data, size_t size) {
    // Use first part of data as table list
    size_t table_len = (size > 100) ? 100 : size;
    char* table_list = new char[table_len + 1];
    memcpy(table_list, data, table_len);
    table_list[table_len] = '\0';
    
    // Ensure it's a valid table list string
    for (size_t i = 0; i < table_len; i++) {
        if (table_list[i] == '\0') {
            table_list[i] = ',';
        }
    }
    
    return table_list;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data
    
    // 1. Get table list from data
    const char* table_list =  id_000012_get_table_list(data, size);
    size_t data_offset = strlen(table_list);
    
    // 2. Use lou_findTables to find tables based on query
    // Use part of data as query
    size_t query_len = (size - data_offset > 50) ? 50 : (size - data_offset);
    char* query =  id_000012_create_string_from_data(data + data_offset, query_len, 50);
    if (query) {
        char** found_tables = lou_findTables(query);
        if (found_tables) {
            lou_freeTableFiles(found_tables);
        }
        delete[] query;
    }
    data_offset += query_len;
    
    // 3. Use lou_getEmphClasses to get emphasis classes
    const char** emph_classes = lou_getEmphClasses(table_list);
    if (emph_classes) {
        lou_freeEmphClasses(emph_classes);
    }
    
    // 4. Convert remaining data to widechar for translation
    size_t widechar_len = 0;
    widechar* inbuf =  id_000012_bytes_to_widechars(data + data_offset, size - data_offset, &widechar_len);
    if (!inbuf || widechar_len == 0) {
        delete[] table_list;
        if (inbuf) delete[] inbuf;
        return 0;
    }
    
    // Prepare buffers for translation
    int inlen = static_cast<int>(widechar_len);
    int outlen = inlen * 2;  // Output buffer size
    widechar* outbuf = new widechar[outlen];
    formtype* typeform = new formtype[inlen];
    char* spacing = new char[inlen];
    
    // Initialize arrays
    memset(typeform, 0, inlen * sizeof(formtype));
    memset(spacing, 0, inlen);
    memset(outbuf, 0, outlen * sizeof(widechar));
    
    // 5. Use lou_translateString
    int mode = 0;
    int translate_result = lou_translateString(table_list, inbuf, &inlen, outbuf, &outlen, 
                                               typeform, spacing, mode);
    
    // 6. Use lou_translate with position arrays
    int* outputPos = new int[outlen];
    int* inputPos = new int[inlen];
    int* cursorPos = new int[inlen];
    memset(outputPos, 0, outlen * sizeof(int));
    memset(inputPos, 0, inlen * sizeof(int));
    memset(cursorPos, 0, inlen * sizeof(int));
    
    // Reset lengths for second translation
    inlen = static_cast<int>(widechar_len);
    outlen = inlen * 2;
    memset(outbuf, 0, outlen * sizeof(widechar));
    
    translate_result = lou_translate(table_list, inbuf, &inlen, outbuf, &outlen,
                                     typeform, spacing, outputPos, inputPos, cursorPos, mode);
    
    // 7. Use lou_translatePrehyphenated with hyphen arrays
    char* inputHyphens = new char[inlen];
    char* outputHyphens = new char[outlen];
    memset(inputHyphens, 0, inlen);
    memset(outputHyphens, 0, outlen);
    
    // Initialize some hyphens for testing
    for (int i = 0; i < inlen && i < 10; i++) {
        inputHyphens[i] = (i % 2 == 0) ? '1' : '0';
    }
    
    // Reset lengths for third translation
    inlen = static_cast<int>(widechar_len);
    outlen = inlen * 2;
    memset(outbuf, 0, outlen * sizeof(widechar));
    
    translate_result = lou_translatePrehyphenated(table_list, inbuf, &inlen, outbuf, &outlen,
                                                  typeform, spacing, outputPos, inputPos, 
                                                  cursorPos, inputHyphens, outputHyphens, mode);
    
    // 8. Use lou_getTableInfo with various keys
    const char* keys[] = {"locale", "author", "description", "copyright", "notes"};
    for (const char* key : keys) {
        char* table_info = lou_getTableInfo(table_list, key);
        if (table_info) {
            lou_freeTableInfo(table_info);
        }
    }
    
    // 9. Clean up all allocated resources
    delete[] table_list;
    delete[] inbuf;
    delete[] outbuf;
    delete[] typeform;
    delete[] spacing;
    delete[] outputPos;
    delete[] inputPos;
    delete[] cursorPos;
    delete[] inputHyphens;
    delete[] outputHyphens;
    
    // 10. Free liblouis resources
    lou_free();
    
    return 0;
}