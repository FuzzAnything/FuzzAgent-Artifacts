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
//<ID> 71
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["lou_charSize","lou_getDataPath","lou_setDataPath","lou_findTables","lou_indexTables","lou_translatePrehyphenated","lou_backTranslateString","lou_freeTableFiles","lou_free"],"critical_calls":["lou_charSize","lou_getDataPath","lou_setDataPath","lou_findTables","lou_indexTables","lou_translatePrehyphenated","lou_backTranslateString","lou_freeTableFiles","lou_free"],"visited":0}
/**/


// Include necessary headers for liblouis
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Initialize variables
    const char *tableList = nullptr;
    char **tables = nullptr;
    char *dataPath = nullptr;
    int result = 0;
    
    // Use lou_charSize() at least once
    int charSize = lou_charSize();
    
    // Get current data path
    dataPath = lou_getDataPath();
    
    // Set data path (use existing or default)
    if (dataPath) {
        char *newPath = lou_setDataPath(dataPath);
        if (newPath) free(newPath);
    }
    
    // Find tables using a query - use a fixed query for simplicity
    tables = lou_findTables("en");
    
    // Index the tables if found
    if (tables) {
        lou_indexTables((const char **)tables);
        
        // Use the first table if available
        if (tables[0]) {
            tableList = tables[0];
            
            // Prepare buffers for translation
            // We'll use part of the input data as widechar buffer
            if (size >= sizeof(widechar) * 2) {
                // Calculate buffer sizes
                int inlen = static_cast<int>(size / sizeof(widechar));
                int outlen = inlen * 2;  // Allow for expansion
                
                // Allocate buffers
                widechar *inbuf = (widechar *)data;
                widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
                formtype *typeform = (formtype *)calloc(inlen, sizeof(formtype));
                char *spacing = (char *)calloc(inlen, sizeof(char));
                int *outputPos = (int *)calloc(inlen, sizeof(int));
                int *inputPos = (int *)calloc(inlen, sizeof(int));
                int *cursorPos = (int *)calloc(inlen, sizeof(int));
                char *inputHyphens = (char *)calloc(inlen, sizeof(char));
                char *outputHyphens = (char *)calloc(outlen, sizeof(char));
                
                if (outbuf && typeform && spacing && outputPos && inputPos && 
                    cursorPos && inputHyphens && outputHyphens) {
                    
                    // Initialize some values for safety
                    int mode = 0;
                    int actualInlen = inlen;
                    int actualOutlen = outlen;
                    
                    // Call translatePrehyphenated
                    result = lou_translatePrehyphenated(
                        tableList,
                        inbuf,
                        &actualInlen,
                        outbuf,
                        &actualOutlen,
                        typeform,
                        spacing,
                        outputPos,
                        inputPos,
                        cursorPos,
                        inputHyphens,
                        outputHyphens,
                        fuzz_int32_t_1
                    );
                    
                    // If translation succeeded, try back translation
                    if (result > 0 && actualOutlen > 0) {
                        int backInlen = actualOutlen;
                        int backOutlen = inlen;
                        widechar *backOutbuf = (widechar *)malloc(backOutlen * sizeof(widechar));
                        
                        if (backOutbuf) {
                            // Call backTranslateString
                            lou_backTranslateString(
                                tableList,
                                outbuf,
                                &backInlen,
                                backOutbuf,
                                &backOutlen,
                                typeform,
                                spacing,
                                fuzz_int32_t_2
                            );
                            
                            free(backOutbuf);
                        }
                    }
                }
                
                // Clean up allocated buffers
                if (outbuf) free(outbuf);
                if (typeform) free(typeform);
                if (spacing) free(spacing);
                if (outputPos) free(outputPos);
                if (inputPos) free(inputPos);
                if (cursorPos) free(cursorPos);
                if (inputHyphens) free(inputHyphens);
                if (outputHyphens) free(outputHyphens);
            }
        }
    }
    
    // Clean up resources
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    // Call lou_free to ensure cleanup of internal resources
    lou_free();
    
    return 0;
}