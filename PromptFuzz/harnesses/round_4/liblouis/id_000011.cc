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
//<ID> 177
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 2
//<Quality> {"density":5,"unique_branches":{"compileTable":[[5058,9,5058,56,0,0,4,0]],"_lou_getALine":[[352,19,352,33,0,0,4,1]]},"library_calls":["lou_getDataPath","lou_readCharFromFile","lou_checkTable","lou_getTable","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_hyphenate","lou_backTranslateString","lou_free"],"critical_calls":["lou_getDataPath","lou_readCharFromFile","lou_checkTable","lou_getTable","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_hyphenate","lou_backTranslateString","lou_free"],"visited":0}
/**/


// Include liblouis headers
extern "C" {
    // Declare the liblouis API functions we'll use
    int lou_checkTable(const char * tableList);
    formtype lou_getTypeformForEmphClass(const char * tableList, const char * emphClass);
    const void *lou_getTable(const char * tableList);
    int lou_backTranslateString(const char * tableList, const widechar * inbuf, int * inlen, widechar * outbuf, int * outlen, formtype * typeform, char * spacing, int mode);
    const char **lou_getEmphClasses(const char * tableList);
    int lou_hyphenate(const char * tableList, const widechar * inbuf, int inlen, char * hyphens, int mode);
    int lou_readCharFromFile(const char * fileName, int * mode);
    char *lou_getDataPath();
    void lou_freeEmphClasses(const char **);
    void lou_free();
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early exit if data is too small for basic operations
    if (size < 10) {
	return 0;
	}
	

    // 1. Create a FILE* from input data for lou_readCharFromFile
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 2. Get data path
    char *data_path = lou_getDataPath();
    (void)data_path; // Use it to avoid unused variable warning

    // 3. Read first character from file
    int mode = 0;
    int char_result = lou_readCharFromFile("input_file", &mode);
    (void)char_result;

    // 4. Create a temporary table list from input data
    // Use first 64 bytes or less for table list
    size_t table_list_len = size > 64 ? 64 : size;
    std::vector<char> table_list(table_list_len + 1);
    memcpy(table_list.data(), data, table_list_len);
    table_list[table_list_len] = '\0';

    // 5. Check table
    int check_result = lou_checkTable(table_list.data());
    (void)check_result;

    // 6. Get table pointer
    const void *table_ptr = lou_getTable(table_list.data());
    (void)table_ptr;

    // 7. Get emphasis classes
    const char **emph_classes = lou_getEmphClasses(table_list.data());
    if (emph_classes && emph_classes[0]) {
        // 8. Get typeform for first emphasis class
        formtype typeform = lou_getTypeformForEmphClass(table_list.data(), emph_classes[0]);
        (void)typeform;
        
        // Free emphasis classes
        lou_freeEmphClasses(emph_classes);
    }

    // 9. Prepare data for hyphenation and back-translation
    // Use remaining data for widechar buffer
    if (size > table_list_len + 1) {
        size_t widechar_data_size = size - (table_list_len + 1);
        // Ensure we have at least some data
        if (widechar_data_size >= sizeof(widechar)) {
            // Convert bytes to widechars (simplistic approach)
            size_t widechar_count = widechar_data_size / sizeof(widechar);
            std::vector<widechar> inbuf(widechar_count);
            memcpy(inbuf.data(), data + table_list_len + 1, widechar_count * sizeof(widechar));
            
            int inlen = static_cast<int>(widechar_count);
            
            // 10. Hyphenate
            std::vector<char> hyphens(widechar_count);
            int hyphen_result = lou_hyphenate(table_list.data(), inbuf.data(), inlen, hyphens.data(), fuzz_int32_t_1);
            (void)hyphen_result;

            // 11. Back translate
            std::vector<widechar> outbuf(widechar_count * 2); // Allocate extra space
            int outlen = static_cast<int>(outbuf.size());
            formtype back_typeform = 0;
            char spacing = 0;
            
            int back_result = lou_backTranslateString(
                table_list.data(), 
                inbuf.data(), 
                &inlen, 
                outbuf.data(), 
                &outlen, 
                &back_typeform, 
                &spacing, 
                fuzz_int32_t_2
            );
            (void)back_result;
        }
    }

    // 12. Clean up resources
    assert_file_closed(&in_file);;
    lou_free();
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}