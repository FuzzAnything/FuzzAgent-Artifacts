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
//<ID> 256
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["lou_setDataPath","lou_logFile","lou_logEnd","lou_logEnd","lou_checkTable","lou_compileString","lou_getTableInfo","lou_freeTableInfo","lou_translate","lou_backTranslate","lou_hyphenate","lou_translateString","lou_backTranslateString","lou_logEnd","lou_free"],"critical_calls":["lou_setDataPath","lou_logFile","lou_checkTable","lou_compileString","lou_getTableInfo","lou_freeTableInfo","lou_translate","lou_backTranslate","lou_hyphenate","lou_translateString","lou_backTranslateString","lou_logEnd","lou_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<306) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeCharArray(char, fuzz_char_array_3, fuzz_char_array_size_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	
		const char * fuzzer_var_lou_compileString_5_0 = fuzz_char_array_3;//fuzzer shim end}




    // Initialize liblouis
    lou_setDataPath(fuzz_str_1);
    lou_logFile(fuzz_str_2);
    
    // Create input file from fuzzed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        lou_logEnd();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read table list from input (first null-terminated string)
    char tableList[256] = {0};
    if (fgets(tableList, sizeof(tableList), in_file) == NULL) {
        assert_file_closed(&in_file);;
        lou_logEnd();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Remove newline if present
    tableList[strcspn(tableList, "\n")] = 0;
    
    // Check if table is valid
    if (lou_checkTable(tableList) != 0) {
        // Table not valid, try to compile it
        rewind(in_file);
        fseek(in_file, 0, SEEK_END);
        long tableSize = ftell(in_file);
        rewind(in_file);
        
        if (tableSize > 0) {
            char *tableContent = (char *)malloc(tableSize + 1);
            if (tableContent) {
                fread(tableContent, 1, tableSize, in_file);
                tableContent[tableSize] = 0;
                lou_compileString(fuzzer_var_lou_compileString_5_0, tableContent);
                free(tableContent);
            }
        }
    }
    
    // Get table info
    char *info = lou_getTableInfo(tableList, fuzz_str_4);
    if (info) {
        lou_freeTableInfo(info);
    }
    
    // Prepare translation buffers
    rewind(in_file);
    fseek(in_file, 0, SEEK_END);
    long remaining = ftell(in_file);
    rewind(in_file);
    
    // Skip table list string
    fgets(tableList, sizeof(tableList), in_file);
    remaining -= ftell(in_file);
    
    if (remaining > 0) {
        // Allocate buffers
        size_t maxChars = remaining / sizeof(widechar);
        if (maxChars > 0) {
            widechar *inbuf = (widechar *)malloc(maxChars * sizeof(widechar));
            if (inbuf) {
                // Read widechar data
                size_t readCount = fread(inbuf, sizeof(widechar), maxChars, in_file);
                int inlen = static_cast<int>(readCount);
                
                if (inlen > 0) {
                    // Output buffers
                    int outlen = inlen * 4;  // Allow expansion
                    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
                    formtype *typeform = (formtype *)malloc(inlen * sizeof(formtype));
                    char *spacing = (char *)malloc(inlen);
                    
                    // Position arrays
                    int *outputPos = (int *)malloc(outlen * sizeof(int));
                    int *inputPos = (int *)malloc(inlen * sizeof(int));
                    int *cursorPos = (int *)malloc(sizeof(int));
                    
                    if (outbuf && typeform && spacing && outputPos && inputPos && cursorPos) {
                        // Initialize arrays
                        memset(typeform, 0, inlen * sizeof(formtype));
                        memset(spacing, 0, inlen);
                        cursorPos[0] = 0;
                        
                        int mode = 0;
                        
                        // 1. Translate
                        int trans_inlen = inlen;
                        int trans_outlen = outlen;
                        lou_translate(tableList, inbuf, &trans_inlen, outbuf, &trans_outlen,
                                     typeform, spacing, outputPos, inputPos, cursorPos, fuzz_int32_t_5);
                        
                        // 2. Back translate
                        int back_inlen = trans_outlen;
                        int back_outlen = inlen;
                        widechar *back_outbuf = (widechar *)malloc(back_outlen * sizeof(widechar));
                        if (back_outbuf) {
                            lou_backTranslate(tableList, outbuf, &back_inlen, back_outbuf, &back_outlen,
                                            typeform, spacing, outputPos, inputPos, cursorPos, fuzz_int32_t_6);
                            free(back_outbuf);
                        }
                        
                        // 3. Hyphenate
                        char *hyphens = (char *)malloc(inlen);
                        if (hyphens) {
                            lou_hyphenate(tableList, inbuf, inlen, hyphens, fuzz_int32_t_7);
                            free(hyphens);
                        }
                        
                        // 4. Translate string (simpler version)
                        int str_inlen = inlen;
                        int str_outlen = outlen;
                        lou_translateString(tableList, inbuf, &str_inlen, outbuf, &str_outlen,
                                          typeform, spacing, fuzz_int32_t_8);
                        
                        // 5. Back translate string
                        int backstr_inlen = str_outlen;
                        int backstr_outlen = inlen;
                        lou_backTranslateString(tableList, outbuf, &backstr_inlen, back_outbuf, &backstr_outlen,
                                               typeform, spacing, fuzz_int32_t_9);
                    }
                    
                    // Free allocated memory
                    free(outbuf);
                    free(typeform);
                    free(spacing);
                    free(outputPos);
                    free(inputPos);
                    free(cursorPos);
                }
                
                free(inbuf);
            }
        }
    }
    
    // Cleanup
    assert_file_closed(&in_file);;
    lou_logEnd();
    lou_free();
    
    assert_file_closed(&in_file);
	return 0;
}