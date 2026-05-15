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
//<ID> 160
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 12
//<Quality> {"density":6,"unique_branches":{"lou_readCharFromFile":[[4338,6,4338,16,0,0,4,0],[4344,7,4344,45,0,0,4,0],[4344,7,4344,45,0,0,4,1],[4350,6,4350,21,0,0,4,1],[4355,6,4355,15,0,0,4,0],[4355,6,4355,15,0,0,4,1]],"getAChar":[[300,8,300,38,0,0,4,0],[300,42,300,72,0,0,4,0],[304,13,304,41,0,0,4,1],[304,45,304,73,0,0,4,1],[322,3,322,17,0,0,4,0],[324,8,324,18,0,0,4,1]]},"library_calls":["lou_setDataPath","lou_readCharFromFile","lou_readCharFromFile","lou_listTables","lou_indexTables","lou_compileString","lou_getTypeformForEmphClass","lou_charToDots","lou_backTranslateString","lou_hyphenate","lou_freeTableFiles","lou_free"],"critical_calls":["lou_setDataPath","lou_readCharFromFile","lou_readCharFromFile","lou_listTables","lou_indexTables","lou_compileString","lou_getTypeformForEmphClass","lou_charToDots","lou_backTranslateString","lou_hyphenate","lou_freeTableFiles","lou_free"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified liblouis APIs:

*/


// Include necessary headers (assuming these are available in liblouis)
extern "C" {
    typedef unsigned short widechar;
    typedef unsigned short formtype;
    
    // Function declarations from liblouis
    int lou_readCharFromFile(const char *fileName, int *mode);
    int lou_compileString(const char *tableList, const char *inString);
    formtype lou_getTypeformForEmphClass(const char *tableList, const char *emphClass);
    int lou_charToDots(const char *tableList, const widechar *inbuf, widechar *outbuf, int length, int mode);
    void lou_indexTables(const char **tables);
    int lou_backTranslateString(const char *tableList, const widechar *inbuf, int *inlen, widechar *outbuf, int *outlen, formtype *typeform, char *spacing, int mode);
    int lou_hyphenate(const char *tableList, const widechar *inbuf, int inlen, char *hyphens, int mode);
    char **lou_listTables();
    char *lou_setDataPath(const char *path);
    void lou_freeTableFiles(char **tables);
    void lou_free();
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<27) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful fuzzing
    
    // Set up data path
    char *old_path = lou_setDataPath(fuzz_str_1);
    
    // Create temporary file with input data
    FILE *temp_file = fmemopen((void *)data, size, "rb");
    if (!temp_file) {
	assert_file_closed(&temp_file);
	return 0;
	}
	
    
    // Use lou_readCharFromFile
    int mode = 0;
    int char_read = lou_readCharFromFile("input_file", &mode);
    
    // Write input to a temporary file for reading
    FILE *input_file = fopen("input_file", "wb");
    if (input_file) {
        fwrite(data, 1, size, input_file);
        assert_file_closed(&input_file);;
    }
    
    // Read again with the actual file
    char_read = lou_readCharFromFile("input_file", &mode);
    assert_file_closed(&temp_file);;
    
    // Get list of tables
    char **tables = lou_listTables();
    
    // Index tables if available
    if (tables) {
        lou_indexTables((const char **)tables);
    }
    
    // Create table list string from data
    std::string table_list;
    size_t table_list_len = size > 50 ? 50 : size;
    for (size_t i = 0; i < table_list_len && i < size; i++) {
        if (data[i] >= 32 && data[i] <= 126) {  // Printable ASCII
            table_list.push_back(data[i]);
        } else {
            table_list.push_back('a');  // Default char
        }
    }
    
    // Create input string for compilation
    std::string compile_string;
    size_t compile_start = table_list_len < size ? table_list_len : 0;
    size_t compile_len = size - compile_start > 50 ? 50 : size - compile_start;
    for (size_t i = 0; i < compile_len && (compile_start + i) < size; i++) {
        if (data[compile_start + i] >= 32 && data[compile_start + i] <= 126) {
            compile_string.push_back(data[compile_start + i]);
        } else {
            compile_string.push_back('b');  // Default char
        }
    }
    
    // Try to compile the string
    if (!table_list.empty() && !compile_string.empty()) {
        lou_compileString(table_list.c_str(), compile_string.c_str());
    }
    
    // Get typeform for emphasis class
    const char *emph_class = "emphasis";
    formtype typeform = lou_getTypeformForEmphClass(table_list.c_str(), fuzz_str_2);
    
    // Prepare input for charToDots
    std::vector<widechar> inbuf;
    size_t char_start = (table_list_len + compile_len) < size ? (table_list_len + compile_len) : 0;
    size_t char_len = size - char_start > 100 ? 100 : size - char_start;
    
    for (size_t i = 0; i < char_len && (char_start + i) < size; i++) {
        inbuf.push_back(static_cast<widechar>(data[char_start + i] % 256));
    }
    
    if (!inbuf.empty()) {
        std::vector<widechar> outbuf(inbuf.size() * 2);  // Allocate enough space
        lou_charToDots(table_list.c_str(), inbuf.data(), outbuf.data(), 
                      static_cast<int>(inbuf.size()), fuzz_int32_t_3);
    }
    
    // Try back translation
    if (!inbuf.empty()) {
        int inlen = static_cast<int>(inbuf.size());
        int outlen = inbuf.size() * 2;
        std::vector<widechar> outbuf(outlen);
        formtype back_typeform = 0;
        char spacing = 0;
        
        lou_backTranslateString(table_list.c_str(), inbuf.data(), &inlen, 
                               outbuf.data(), &outlen, &back_typeform, &spacing, fuzz_int32_t_4);
    }
    
    // Try hyphenation
    if (!inbuf.empty()) {
        std::vector<char> hyphens(inbuf.size());
        lou_hyphenate(table_list.c_str(), inbuf.data(), 
                     static_cast<int>(inbuf.size()), hyphens.data(), fuzz_int32_t_5);
    }
    
    // Clean up
    if (tables) {
        lou_freeTableFiles(tables);
    }
    
    lou_free();
    
    // Remove temporary file
    remove("input_file");
    
    assert_file_closed(&input_file);
	assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}