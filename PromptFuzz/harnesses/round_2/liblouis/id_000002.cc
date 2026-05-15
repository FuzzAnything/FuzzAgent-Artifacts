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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 4
//<Quality> {"density":12,"unique_branches":{"lou_checkTable":[[5255,6,5255,29,0,0,4,1]],"analyzeTable":[[702,7,702,23,0,0,4,0]],"lou_compileString":[[5450,6,5450,12,0,0,4,0]],"lou_getTableInfo":[[1152,21,1152,22,0,0,4,1]]},"library_calls":["lou_charSize","lou_checkTable","lou_checkTable","lou_getTableInfo","lou_freeTableInfo","lou_compileString","lou_freeTableInfo","lou_freeTableInfo","lou_freeTableInfo","lou_freeTableInfo","lou_translate","lou_backTranslate","lou_freeTableInfo"],"critical_calls":["lou_charSize","lou_checkTable","lou_checkTable","lou_getTableInfo","lou_compileString","lou_translate","lou_backTranslate","lou_freeTableInfo"],"visited":1}
/**/


// Include necessary headers from liblouis
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 2) {
        return 0;
    }

    // 1. Get character size
    int char_size = lou_charSize();
    if (char_size <= 0) {
        return 0;
    }

    // 2. Prepare table list - use a known valid table
    // We'll use a simple table for testing
    const char* table_list = "tests/tables/test.ctb";
    
    // 3. Check the table
    int table_check = lou_checkTable(table_list);
    if (table_check != 0) {
        // Table check failed, try alternative
        table_list = "en-us-g2.ctb";
        table_check = lou_checkTable(table_list);
        if (table_check != 0) {
            return 0;  // No valid table available
        }
    }

    // 4. Get table info
    char* table_info = lou_getTableInfo(table_list, "language");
    if (table_info) {
        // We got table info, free it later
    }

    // 5. Create input string from fuzzer data
    // We need to ensure we don't overflow and keep it as a valid C string
    size_t max_input_len = size < 256 ? size : 256;
    char* input_string = (char*)malloc(max_input_len + 1);
    if (!input_string) {
        if (table_info) lou_freeTableInfo(table_info);
        return 0;
    }
    
    // Copy data to input string, ensuring null termination
    memcpy(input_string, data, max_input_len);
    input_string[max_input_len] = '\0';

    // 6. Compile the string
    int compile_result = lou_compileString(table_list, input_string);
    
    // 7. Prepare for translation
    // Convert input to widechar format
    size_t widechar_len = max_input_len / sizeof(widechar);
    if (widechar_len < 1) {
        free(input_string);
        if (table_info) lou_freeTableInfo(table_info);
        return 0;
    }
    
    widechar* inbuf = (widechar*)malloc(widechar_len * sizeof(widechar));
    if (!inbuf) {
        free(input_string);
        if (table_info) lou_freeTableInfo(table_info);
        return 0;
    }
    
    // Copy data to widechar buffer
    memcpy(inbuf, data, widechar_len * sizeof(widechar));
    int inlen = (int)widechar_len;
    
    // Allocate output buffer
    int outlen = inlen * 2;  // Output might be larger
    widechar* outbuf = (widechar*)malloc(outlen * sizeof(widechar));
    if (!outbuf) {
        free(inbuf);
        free(input_string);
        if (table_info) lou_freeTableInfo(table_info);
        return 0;
    }
    
    // Allocate arrays for positions
    int* outputPos = (int*)malloc(outlen * sizeof(int));
    int* inputPos = (int*)malloc(inlen * sizeof(int));
    int* cursorPos = (int*)malloc(inlen * sizeof(int));
    formtype* typeform = (formtype*)malloc(inlen * sizeof(formtype));
    char* spacing = (char*)malloc(inlen * sizeof(char));
    
    if (!outputPos || !inputPos || !cursorPos || !typeform || !spacing) {
        // Clean up and return
        free(outputPos);
        free(inputPos);
        free(cursorPos);
        free(typeform);
        free(spacing);
        free(outbuf);
        free(inbuf);
        free(input_string);
        if (table_info) lou_freeTableInfo(table_info);
        return 0;
    }
    
    // Initialize arrays
    memset(typeform, 0, inlen * sizeof(formtype));
    memset(spacing, 0, inlen * sizeof(char));
    
    // 8. Perform forward translation
    int mode = 0;
    int translate_result = lou_translate(table_list, inbuf, &inlen, outbuf, 
                                         &outlen, typeform, spacing, 
                                         outputPos, inputPos, cursorPos, mode);
    
    if (translate_result > 0) {
        // 9. Prepare for back translation
        // Use output from forward translation as input for back translation
        int back_inlen = outlen;
        widechar* back_inbuf = (widechar*)malloc(back_inlen * sizeof(widechar));
        if (back_inbuf) {
            memcpy(back_inbuf, outbuf, back_inlen * sizeof(widechar));
            
            int back_outlen = inlen * 2;
            widechar* back_outbuf = (widechar*)malloc(back_outlen * sizeof(widechar));
            
            if (back_outbuf) {
                // Reset position arrays for back translation
                memset(outputPos, 0, back_outlen * sizeof(int));
                memset(inputPos, 0, back_inlen * sizeof(int));
                memset(cursorPos, 0, back_inlen * sizeof(int));
                memset(typeform, 0, back_inlen * sizeof(formtype));
                memset(spacing, 0, back_inlen * sizeof(char));
                
                // 10. Perform back translation
                int back_translate_result = lou_backTranslate(table_list, back_inbuf, 
                                                             &back_inlen, back_outbuf, 
                                                             &back_outlen, typeform, spacing,
                                                             outputPos, inputPos, cursorPos, mode);
                
                free(back_outbuf);
            }
            free(back_inbuf);
        }
    }
    
    // 11. Clean up all allocated resources
    free(outputPos);
    free(inputPos);
    free(cursorPos);
    free(typeform);
    free(spacing);
    free(outbuf);
    free(inbuf);
    free(input_string);
    
    if (table_info) {
        lou_freeTableInfo(table_info);
    }
    
    return 0;
}