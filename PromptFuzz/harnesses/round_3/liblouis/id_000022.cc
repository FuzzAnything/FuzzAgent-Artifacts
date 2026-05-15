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
//<ID> 246
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 18
//<Quality> {"density":2,"unique_branches":{"getAChar":[[300,8,300,38,0,0,4,0],[300,42,300,72,0,0,4,0],[300,42,300,72,0,0,4,1],[302,13,302,43,0,0,4,0],[302,47,302,77,0,0,4,0],[302,47,302,77,0,0,4,1],[304,13,304,41,0,0,4,1],[304,45,304,73,0,0,4,1],[322,3,322,17,0,0,4,0],[324,8,324,18,0,0,4,1],[328,3,328,20,0,0,4,0],[330,8,330,18,0,0,4,1]],"lou_readCharFromFile":[[4338,6,4338,16,0,0,4,0],[4344,7,4344,45,0,0,4,0],[4344,7,4344,45,0,0,4,1],[4350,6,4350,21,0,0,4,1],[4355,6,4355,15,0,0,4,0],[4355,6,4355,15,0,0,4,1]]},"library_calls":["lou_getDataPath","lou_indexTables","lou_readCharFromFile","lou_readCharFromFile","lou_compileString","lou_getTable","lou_translateString","lou_backTranslateString","lou_free"],"critical_calls":["lou_getDataPath","lou_indexTables","lou_readCharFromFile","lou_readCharFromFile","lou_compileString","lou_getTable","lou_translateString","lou_backTranslateString","lou_free"],"visited":0}
/*Here's a fuzzer that uses the specified liblouis APIs to test back-translation functionality:

*/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<21) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early return if input is too small for meaningful testing
    if (size < 10) {
	return 0;
	}
	
    
    // 1. Use lou_getDataPath to get current data path
    char *data_path = lou_getDataPath();
    
    // 2. Use lou_indexTables - create a list of tables to index
    const char *tables[] = {"en-us-g2.ctb", "en-ueb-g2.ctb", NULL};
    lou_indexTables(tables);
    
    // 3. Create a temporary file with the input data for lou_readCharFromFile
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 4. Use lou_readCharFromFile to read first character and mode
    int mode = 0;
    int char_result = lou_readCharFromFile("input_file", &mode);
    
    // Write input data to actual file for lou_readCharFromFile
    FILE *tmp_file = fopen("input_file", "wb");
    if (tmp_file) {
        fwrite(data, 1, size, tmp_file);
        assert_file_closed(&tmp_file);;
        // Now read from the actual file
        char_result = lou_readCharFromFile("input_file", &mode);
    }
    
    // 5. Use lou_compileString to compile a simple table
    // Create a simple table definition from input data
    // We'll use a small portion of data to avoid issues
    size_t compile_size = size > 100 ? 100 : size;
    char *table_def = (char *)malloc(compile_size + 1);
    if (!table_def) {
        assert_file_closed(&in_file);;
        assert_file_closed(&tmp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    memcpy(table_def, data, compile_size);
    table_def[compile_size] = '\0';
    
    int compile_result = lou_compileString("test_table", table_def);
    free(table_def);
    
    // 6. Use lou_getTable to retrieve the compiled table
    const void *table_ptr = lou_getTable(fuzz_str_1);
    
    // Use a fallback table if compilation failed
    const char *table_to_use = (compile_result == 0) ? "test_table" : "en-us-g2.ctb";
    
    // 7. Convert input data to widechar for liblouis functions
    // Use first half of data for forward translation
    size_t half_size = size / 2;
    if (half_size < 4) {
        assert_file_closed(&in_file);;
        assert_file_closed(&tmp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Allocate buffers
    widechar *inbuf = (widechar *)malloc(half_size * sizeof(widechar));
    widechar *outbuf = (widechar *)malloc(half_size * sizeof(widechar));
    widechar *back_outbuf = (widechar *)malloc(half_size * sizeof(widechar));
    
    if (!inbuf || !outbuf || !back_outbuf) {
        free(inbuf);
        free(outbuf);
        free(back_outbuf);
        assert_file_closed(&in_file);;
        assert_file_closed(&tmp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Convert input bytes to widechars (simple conversion)
    size_t wchar_count = half_size / sizeof(widechar);
    memcpy(inbuf, data, wchar_count * sizeof(widechar));
    
    // 8. Use lou_translateString for forward translation
    int inlen = wchar_count;
    int outlen = half_size;
    formtype *typeform = (formtype *)calloc(wchar_count, sizeof(formtype));
    char *spacing = (char *)calloc(wchar_count, sizeof(char));
    
    int translate_result = lou_translateString(
        table_to_use, 
        inbuf, 
        &inlen, 
        outbuf, 
        &outlen, 
        typeform, 
        spacing, 
        fuzz_int32_t_2
    );
    
    // 9. Use lou_backTranslateString for back-translation
    // Only if forward translation succeeded
    if (translate_result > 0) {
        int back_inlen = outlen;
        int back_outlen = half_size;
        
        int back_translate_result = lou_backTranslateString(
            table_to_use,
            outbuf,
            &back_inlen,
            back_outbuf,
            &back_outlen,
            typeform,
            spacing,
            fuzz_int32_t_3
        );
    }
    
    // 10. Clean up
    free(inbuf);
    free(outbuf);
    free(back_outbuf);
    free(typeform);
    free(spacing);
    assert_file_closed(&in_file);;
    
    // Remove temporary file
    remove("input_file");
    
    // 11. Call lou_free to clean up liblouis resources
    lou_free();
    
    assert_file_closed(&tmp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}