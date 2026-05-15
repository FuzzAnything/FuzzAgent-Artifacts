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
//<ID> 198
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["lou_registerTableResolver","lou_readCharFromFile","lou_getTableInfo","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_hyphenate","lou_translate","lou_backTranslate","lou_backTranslateString","lou_freeTableInfo","lou_getTableInfo","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_hyphenate","lou_translate","lou_backTranslate","lou_backTranslateString","lou_freeTableInfo","lou_free"],"critical_calls":["lou_registerTableResolver","lou_readCharFromFile","lou_getTableInfo","lou_getEmphClasses","lou_getTypeformForEmphClass","lou_freeEmphClasses","lou_hyphenate","lou_translate","lou_backTranslate","lou_backTranslateString","lou_freeTableInfo","lou_free"],"visited":0}
/**/


// Include liblouis headers
extern "C" {
    // Basic types from liblouis
    typedef unsigned short widechar;
    typedef unsigned short formtype;
    
    // Function declarations for the APIs we'll use
    void lou_registerTableResolver(char **(*)(const char *, const char *));
    int lou_translate(const char *, const widechar *, int *, widechar *, int *,
                     formtype *, char *, int *, int *, int *, int);
    int lou_backTranslate(const char *, const widechar *, int *, widechar *, int *,
                         formtype *, char *, int *, int *, int *, int);
    formtype lou_getTypeformForEmphClass(const char *, const char *);
    const char **lou_getEmphClasses(const char *);
    char *lou_getTableInfo(const char *, const char *);
    int lou_hyphenate(const char *, const widechar *, int, char *, int);
    int lou_backTranslateString(const char *, const widechar *, int *, widechar *, int *,
                               formtype *, char *, int);
    int lou_readCharFromFile(const char *, int *);
    
    // Other liblouis functions we might use
    int lou_checkTable(const char *);
    void lou_freeTableInfo(char *);
    void lou_freeEmphClasses(const char **);
    void lou_free();
}

// Custom table resolver that returns hardcoded table data
char ** id_000016_customTableResolver(const char *table, const char *base) {
    // Return a simple hardcoded table
    static char *tableData[] = {
        (char *)"table test\n"
        "include testBase.ctb\n"
        "nobreak",
        NULL
    };
    return tableData;
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<264) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_15, fuzz_str_sz_15, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_16, fuzz_str_sz_16, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_17, fuzz_str_sz_17, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_18, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_19, fuzz_str_sz_19, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_20, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_21, fuzz_str_sz_21, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_22, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_23, fuzz_str_sz_23, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_24, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 2) {
	return 0;
	}
	
    
    // Write input data to a file for lou_readCharFromFile
    FILE *input_file = fopen("input_file", "wb");
    if (!input_file) {
	assert_file_closed(&input_file);
	return 0;
	}
	
    fwrite(data, 1, size, input_file);
    assert_file_closed(&input_file);;
    
    // Register custom table resolver
    lou_registerTableResolver( id_000016_customTableResolver);
    
    // Try to read a character from the file we created
    int mode = 0;
    int charResult = lou_readCharFromFile("input_file", &mode);
    
    // Parse the input data for our test
    // First part: table list (null-terminated string)
    size_t tableListLen = 0;
    while (tableListLen < size && data[tableListLen] != 0) {
        tableListLen++;
    }
    if (tableListLen >= size) {
        // No null terminator found, use a default table
        const char *tableList = "en-us-g2.ctb";
        
        // Use a small amount of data for wide character input
        if (size < 4) {
            remove("input_file");
            assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
        }
        
        // Convert data to wide characters
        int inlen = (size < 100) ? size / 2 : 50;
        widechar *inbuf = new widechar[inlen];
        for (int i = 0; i < inlen; i++) {
            inbuf[i] = (i * 2 + 1 < size) ? 
                       (data[i * 2] << 8) | data[i * 2 + 1] : 'A' + i;
        }
        
        // Prepare buffers for translation
        int outlen = inlen * 2;
        widechar *outbuf = new widechar[outlen];
        int actual_outlen = outlen;
        
        formtype *typeform = new formtype[inlen];
        char *spacing = new char[inlen];
        int *outputPos = new int[outlen];
        int *inputPos = new int[inlen];
        int *cursorPos = new int[1];
        cursorPos[0] = 0;
        
        // Initialize arrays
        for (int i = 0; i < inlen; i++) {
            typeform[i] = 0;
            spacing[i] = 0;
            inputPos[i] = i;
        }
        for (int i = 0; i < outlen; i++) {
            outputPos[i] = i;
        }
        
        // Try to get table info
        char *tableInfo = lou_getTableInfo(fuzz_str_1, fuzz_str_2);
        
        // Try to get emphasis classes
        const char **emphClasses = lou_getEmphClasses(fuzz_str_3);
        if (emphClasses) {
            // Get typeform for first emphasis class if available
            if (emphClasses[0]) {
                formtype typeformEmph = lou_getTypeformForEmphClass(fuzz_str_4, emphClasses[0]);
                // Apply to first few characters
                for (int i = 0; i < 3 && i < inlen; i++) {
                    typeform[i] = typeformEmph;
                }
            }
            lou_freeEmphClasses(emphClasses);
        }
        
        // Perform hyphenation
        char *hyphens = new char[inlen];
        int hyphenResult = lou_hyphenate(fuzz_str_5, inbuf, inlen, hyphens, fuzz_int32_t_6);
        
        // Perform translation
        int actual_inlen = inlen;
        int translateResult = lou_translate(fuzz_str_7, inbuf, &actual_inlen, outbuf,
                                           &actual_outlen, typeform, spacing,
                                           outputPos, inputPos, cursorPos, fuzz_int32_t_8);
        
        // Perform back translation
        int back_inlen = actual_outlen;
        int back_outlen = inlen * 2;
        widechar *back_outbuf = new widechar[back_outlen];
        int backResult = lou_backTranslate(fuzz_str_9, outbuf, &back_inlen, back_outbuf,
                                          &back_outlen, typeform, spacing,
                                          outputPos, inputPos, cursorPos, fuzz_int32_t_10);
        
        // Also try backTranslateString
        int backstr_inlen = inlen;
        int backstr_outlen = inlen * 2;
        widechar *backstr_outbuf = new widechar[backstr_outlen];
        int backstrResult = lou_backTranslateString(fuzz_str_11, inbuf, &backstr_inlen,
                                                   backstr_outbuf, &backstr_outlen,
                                                   typeform, spacing, fuzz_int32_t_12);
        
        // Clean up
        delete[] inbuf;
        delete[] outbuf;
        delete[] typeform;
        delete[] spacing;
        delete[] outputPos;
        delete[] inputPos;
        delete[] cursorPos;
        delete[] hyphens;
        delete[] back_outbuf;
        delete[] backstr_outbuf;
        if (tableInfo) lou_freeTableInfo(tableInfo);
    } else {
        // We have a null-terminated table list in the data
        const char *tableList = reinterpret_cast<const char*>(data);
        
        // Use remaining data for wide character input
        size_t wideDataStart = tableListLen + 1;
        if (wideDataStart >= size) {
            remove("input_file");
            assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
        }
        
        size_t wideDataSize = size - wideDataStart;
        int inlen = (wideDataSize < 200) ? wideDataSize / 2 : 100;
        widechar *inbuf = new widechar[inlen];
        
        for (int i = 0; i < inlen; i++) {
            size_t offset = wideDataStart + i * 2;
            if (offset + 1 < size) {
                inbuf[i] = (data[offset] << 8) | data[offset + 1];
            } else {
                inbuf[i] = 'A' + i;
            }
        }
        
        // Prepare buffers
        int outlen = inlen * 2;
        widechar *outbuf = new widechar[outlen];
        int actual_outlen = outlen;
        
        formtype *typeform = new formtype[inlen];
        char *spacing = new char[inlen];
        int *outputPos = new int[outlen];
        int *inputPos = new int[inlen];
        int *cursorPos = new int[1];
        cursorPos[0] = 0;
        
        for (int i = 0; i < inlen; i++) {
            typeform[i] = 0;
            spacing[i] = 0;
            inputPos[i] = i;
        }
        for (int i = 0; i < outlen; i++) {
            outputPos[i] = i;
        }
        
        // Get table info
        char *tableInfo = lou_getTableInfo(fuzz_str_13, fuzz_str_14);
        
        // Get emphasis classes
        const char **emphClasses = lou_getEmphClasses(fuzz_str_15);
        if (emphClasses && emphClasses[0]) {
            formtype typeformEmph = lou_getTypeformForEmphClass(fuzz_str_16, emphClasses[0]);
            for (int i = 0; i < 3 && i < inlen; i++) {
                typeform[i] = typeformEmph;
            }
            lou_freeEmphClasses(emphClasses);
        }
        
        // Hyphenate
        char *hyphens = new char[inlen];
        lou_hyphenate(fuzz_str_17, inbuf, inlen, hyphens, fuzz_int32_t_18);
        
        // Translate
        int actual_inlen = inlen;
        lou_translate(fuzz_str_19, inbuf, &actual_inlen, outbuf, &actual_outlen,
                     typeform, spacing, outputPos, inputPos, cursorPos, fuzz_int32_t_20);
        
        // Back translate
        int back_inlen = actual_outlen;
        int back_outlen = inlen * 2;
        widechar *back_outbuf = new widechar[back_outlen];
        lou_backTranslate(fuzz_str_21, outbuf, &back_inlen, back_outbuf, &back_outlen,
                         typeform, spacing, outputPos, inputPos, cursorPos, fuzz_int32_t_22);
        
        // Back translate string
        int backstr_inlen = inlen;
        int backstr_outlen = inlen * 2;
        widechar *backstr_outbuf = new widechar[backstr_outlen];
        lou_backTranslateString(fuzz_str_23, inbuf, &backstr_inlen, backstr_outbuf,
                               &backstr_outlen, typeform, spacing, fuzz_int32_t_24);
        
        // Clean up
        delete[] inbuf;
        delete[] outbuf;
        delete[] typeform;
        delete[] spacing;
        delete[] outputPos;
        delete[] inputPos;
        delete[] cursorPos;
        delete[] hyphens;
        delete[] back_outbuf;
        delete[] backstr_outbuf;
        if (tableInfo) lou_freeTableInfo(tableInfo);
    }
    
    // Clean up liblouis resources
    lou_free();
    
    // Remove temporary file
    remove("input_file");
    
    assert_file_closed(&input_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}