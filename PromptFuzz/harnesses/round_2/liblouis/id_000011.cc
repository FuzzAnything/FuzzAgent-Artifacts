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
//<ID> 170
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 5
//<Quality> {"density":2,"unique_branches":{"lou_translatePrehyphenated":[[1416,6,1416,26,0,0,4,0],[1417,7,1417,28,0,0,4,1],[1418,7,1418,23,0,0,4,1],[1424,6,1425,42,0,0,4,1],[1443,6,1443,28,0,0,4,1]]},"library_calls":["lou_setDataPath","lou_checkTable","lou_registerTableResolver","lou_compileString","lou_getEmphClasses","lou_translateString","lou_translatePrehyphenated","lou_freeEmphClasses","lou_freeTableInfo","lou_free"],"critical_calls":["lou_setDataPath","lou_checkTable","lou_registerTableResolver","lou_compileString","lou_getEmphClasses","lou_translateString","lou_translatePrehyphenated","lou_freeEmphClasses","lou_freeTableInfo","lou_free"],"visited":0}
/**/


// Custom table resolver function
static char ** id_000011_fuzz_resolver(const char *table_list, const char *base) {
    // Simple resolver that just returns NULL
    return NULL;
}

// Global variable to track allocated resources
static char * id_000011_global_data_path = nullptr;
static const char ** id_000011_global_emph_classes = nullptr;
static char * id_000011_global_table_info = nullptr;

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        // Too small for meaningful fuzzing
        return 0;
    }

    // 1. Set data path
    char data_path[] = "/tmp/fuzz_data_path";
     id_000011_global_data_path = lou_setDataPath(data_path);
    
    // 2. Check table with first portion of data
    // We'll use the first min(size/2, 256) bytes as table name
    size_t table_name_len = (size / 2 > 256) ? 256 : size / 2;
    char *table_name = (char *)malloc(table_name_len + 1);
    if (!table_name) {
	return 0;
	}
	
    
    memcpy(table_name, data, table_name_len);
    table_name[table_name_len] = '\0';
    
    int check_result = lou_checkTable(table_name);
    free(table_name);
    
    // 3. Register table resolver
    lou_registerTableResolver( id_000011_fuzz_resolver);
    
    // 4. Compile a table string from remaining data
    size_t compile_size = size - table_name_len;
    if (compile_size > 0) {
        char *compile_string = (char *)malloc(compile_size + 1);
        if (compile_string) {
            memcpy(compile_string, data + table_name_len, compile_size);
            compile_string[compile_size] = '\0';
            lou_compileString("test_table", compile_string);
            free(compile_string);
        }
    }
    
    // 5. Get emphasis classes
     id_000011_global_emph_classes = lou_getEmphClasses("en-us-g2.ctb");
    
    // 6. Prepare input buffer for translation
    size_t translate_input_len = (size > 1024) ? 1024 : size;
    widechar *inbuf = (widechar *)malloc(translate_input_len * sizeof(widechar));
    int inlen = translate_input_len / 2; // Convert bytes to widechars
    
    if (inbuf) {
        // Convert bytes to widechars (simple copy, assuming 2-byte widechars)
        for (int i = 0; i < inlen && i * 2 + 1 < (int)translate_input_len; i++) {
            inbuf[i] = (data[i * 2] << 8) | data[i * 2 + 1];
        }
        
        // Allocate output buffers
        int outlen = inlen * 2; // Conservative estimate
        widechar *outbuf = (widechar *)malloc(outlen * sizeof(widechar));
        formtype *typeform = (formtype *)calloc(inlen, sizeof(formtype));
        char *spacing = (char *)calloc(inlen, sizeof(char));
        
        // Allocate additional buffers for translatePrehyphenated
        int *outputPos = (int *)calloc(outlen, sizeof(int));
        int *inputPos = (int *)calloc(inlen, sizeof(int));
        int cursorPos = 0;
        char *inputHyphens = (char *)calloc(inlen, sizeof(char));
        char *outputHyphens = (char *)calloc(outlen, sizeof(char));
        
        if (outbuf && typeform && spacing && outputPos && inputPos && 
            inputHyphens && outputHyphens) {
            
            // 7. Call translateString
            int translate_result = lou_translateString(
                "en-us-g2.ctb", 
                inbuf, 
                &inlen, 
                outbuf, 
                &outlen, 
                typeform, 
                spacing, 
                0
            );
            
            // 8. Reset lengths for prehyphenated translation
            inlen = translate_input_len / 2;
            outlen = inlen * 2;
            
            // Initialize some hyphens for testing
            for (int i = 0; i < inlen; i++) {
                inputHyphens[i] = (i % 3 == 0) ? 1 : 0;
            }
            
            // 9. Call translatePrehyphenated
            int prehyphenated_result = lou_translatePrehyphenated(
                "en-us-g2.ctb",
                inbuf,
                &inlen,
                outbuf,
                &outlen,
                typeform,
                spacing,
                outputPos,
                inputPos,
                &cursorPos,
                inputHyphens,
                outputHyphens,
                0
            );
        }
        
        // 10. Clean up translation buffers
        free(inbuf);
        free(outbuf);
        free(typeform);
        free(spacing);
        free(outputPos);
        free(inputPos);
        free(inputHyphens);
        free(outputHyphens);
    }
    
    // 11. Clean up global resources
    if ( id_000011_global_emph_classes) {
        lou_freeEmphClasses( id_000011_global_emph_classes);
         id_000011_global_emph_classes = nullptr;
    }
    
    if ( id_000011_global_table_info) {
        lou_freeTableInfo( id_000011_global_table_info);
         id_000011_global_table_info = nullptr;
    }
    
    // 12. Free liblouis internal resources
    lou_free();
    
    return 0;
}