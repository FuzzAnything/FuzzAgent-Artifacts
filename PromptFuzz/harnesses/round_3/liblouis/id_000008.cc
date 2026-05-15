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
//<ID> 130
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 2
//<Quality> {"density":2,"unique_branches":{"lou_dotsToChar":[[4160,6,4160,19,0,0,4,0]],"_lou_backTranslate":[[217,6,217,19,0,0,4,0]]},"library_calls":["lou_getDataPath","lou_registerTableResolver","lou_registerLogCallback","lou_compileString","lou_indexTables","lou_getTypeformForEmphClass","lou_backTranslate","lou_hyphenate","lou_dotsToChar","lou_free"],"critical_calls":["lou_getDataPath","lou_registerTableResolver","lou_registerLogCallback","lou_compileString","lou_indexTables","lou_getTypeformForEmphClass","lou_backTranslate","lou_hyphenate","lou_dotsToChar","lou_free"],"visited":0}
/**/


// Include necessary headers for liblouis

// Custom table resolver function
static char ** id_000008_custom_table_resolver(const char *table, const char *base) {
    // Simple resolver that returns NULL for simplicity
    (void)table;
    (void)base;
    return NULL;
}

// Custom log callback function
static void  id_000008_custom_log_callback(logLevels level, const char *message) {
    (void)level;
    (void)message;
    // Do nothing in fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<75) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    // Early return if input is too small for basic operations
    if (size < sizeof(widechar) * 2) {
        return 0;
    }

    // 1. Get data path
    char *data_path = lou_getDataPath();
    if (data_path) {
        free(data_path);
    }

    // 2. Register table resolver
    lou_registerTableResolver( id_000008_custom_table_resolver);

    // 3. Register log callback
    lou_registerLogCallback( id_000008_custom_log_callback);

    // 4. Create a simple braille table string from input data
    // Use first 256 bytes for table compilation, or less if input is smaller
    size_t table_str_size = size > 256 ? 256 : size;
    char *table_str = (char *)malloc(table_str_size + 1);
    if (!table_str) {
        return 0;
    }
    memcpy(table_str, data, table_str_size);
    table_str[table_str_size] = '\0';
    
    // Compile the table string
    int compile_result = lou_compileString("test_table", table_str);
    free(table_str);
    
    // 5. Index tables
    const char *tables[] = {"test_table", NULL};
    lou_indexTables(tables);

    // 6. Get typeform for emphasis class
    // Use a fixed emphasis class name
    formtype typeform = lou_getTypeformForEmphClass(fuzz_str_1, fuzz_str_2);
    (void)typeform; // Mark as used

    // 7. Prepare input buffer for various operations
    // Use remaining data for widechar operations
    size_t widechar_data_size = size - table_str_size;
    if (widechar_data_size < sizeof(widechar) * 2) {
        return 0;
    }
    
    // Cast remaining data to widechar array
    const widechar *inbuf = (const widechar *)(data + table_str_size);
    int inlen = widechar_data_size / sizeof(widechar);
    
    // Ensure inlen is reasonable
    if (inlen <= 0) {
        return 0;
    }

    // 8. Perform back translation
    int outlen = inlen * 2; // Allocate enough space
    widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
    if (!outbuf) {
        return 0;
    }
    
    char *spacing = (char *)malloc(inlen);
    int *outputPos = (int *)malloc(inlen * sizeof(int));
    int *inputPos = (int *)malloc(inlen * sizeof(int));
    int *cursorPos = (int *)malloc(sizeof(int));
    
    if (!spacing || !outputPos || !inputPos || !cursorPos) {
        free(outbuf);
        free(spacing);
        free(outputPos);
        free(inputPos);
        free(cursorPos);
        return 0;
    }
    
    int backtranslate_result = lou_backTranslate(
        fuzz_str_3,
        inbuf,
        &inlen,
        outbuf,
        &outlen,
        NULL, // typeform
        spacing,
        outputPos,
        inputPos,
        cursorPos,
        fuzz_int32_t_4 // mode
    );
    
    // 9. Perform hyphenation on the original input
    char *hyphens = (char *)malloc(inlen);
    if (hyphens) {
        int hyphenate_result = lou_hyphenate(
            fuzz_str_5,
            inbuf,
            inlen,
            hyphens,
            fuzz_int32_t_6 // mode
        );
        free(hyphens);
    }

    // 10. Perform dotsToChar conversion
    // Use a portion of the input for this operation
    int dots_length = inlen > 10 ? 10 : inlen;
    widechar *dots_outbuf = (widechar *)malloc(dots_length * sizeof(widechar));
    if (dots_outbuf) {
        // Create a mutable copy for dotsToChar
        widechar *mutable_inbuf = (widechar *)malloc(dots_length * sizeof(widechar));
        if (mutable_inbuf) {
            memcpy(mutable_inbuf, inbuf, dots_length * sizeof(widechar));
            int dots_result = lou_dotsToChar(
                fuzz_str_7,
                mutable_inbuf,
                dots_outbuf,
                dots_length,
                fuzz_int32_t_8 // mode
            );
            free(mutable_inbuf);
        }
        free(dots_outbuf);
    }

    // Cleanup
    free(outbuf);
    free(spacing);
    free(outputPos);
    free(inputPos);
    free(cursorPos);

    // 11. Free liblouis resources
    lou_free();

    return 0;
}