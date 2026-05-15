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
#include <cJSON.h>
}
//<ID> 223
//<Prompt> []
/*<Combination>: [
*/
//<score> 46, nr_unique_branch: 1
//<Quality> {"density":46,"unique_branches":{"cJSON_CreateFloatArray":[[2694,14,2694,22,0,0,4,1]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_AddArrayToObject","cJSON_CreateArray","cJSON_CreateArray","cJSON_CreateArray","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_malloc","cJSON_malloc","cJSON_GetArrayItem","cJSON_GetNumberValue","cJSON_CreateFloatArray","cJSON_CreateDoubleArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_free","cJSON_free","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_CreateBool","cJSON_CreateBool","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_AddItemToObject","cJSON_AddStringToObject","cJSON_AddRawToObject","cJSON_GetObjectItem","cJSON_SetValuestring","cJSON_CreateArray","cJSON_CreateArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_DetachItemFromObject","cJSON_AddItemToArray","cJSON_Print","cJSON_PrintBuffered","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_free","cJSON_free"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_AddArrayToObject","cJSON_CreateArray","cJSON_CreateArray","cJSON_CreateArray","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_malloc","cJSON_malloc","cJSON_GetArrayItem","cJSON_GetNumberValue","cJSON_CreateFloatArray","cJSON_CreateDoubleArray","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_free","cJSON_free","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_CreateBool","cJSON_CreateBool","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_AddItemToObject","cJSON_AddStringToObject","cJSON_AddRawToObject","cJSON_GetObjectItem","cJSON_SetValuestring","cJSON_CreateArray","cJSON_CreateArray","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_DetachItemFromObject","cJSON_AddItemToArray","cJSON_Print","cJSON_PrintBuffered","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_free","cJSON_free"],"visited":1}
/*Looking at the input data (a 3x3 matrix) and the APIs provided, I'll create a fuzz driver that parses this matrix, creates various JSON structures using the specified APIs, and manipulates them.

*/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<226) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_15, fuzz_str_sz_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_17, fdp);
	//fuzzer shim end}




    // Early exit if we don't have enough data
    if (size < 9 * sizeof(double)) {
	return 0;
	}
	
    
    // Parse the input data as a JSON array (assuming it's the 3x3 matrix)
    cJSON *parsed = cJSON_ParseWithLength(reinterpret_cast<const char*>(data), size);
    if (!parsed) {
	return 0;
	}
	
    
    // 1. Create a root object
    cJSON *root = cJSON_CreateObject();
    
    // 2. Add an array to the object using cJSON_AddArrayToObject
    cJSON *mainArray = cJSON_AddArrayToObject(root, fuzz_str_1);
    
    // 3. Create separate arrays for different operations
    cJSON *floatArrayObj = cJSON_CreateArray();
    cJSON *doubleArrayObj = cJSON_CreateArray();
    cJSON *boolArrayObj = cJSON_CreateArray();
    
    // Extract data from parsed JSON if it's an array
    if (cJSON_IsArray(parsed)) {
        int arraySize = cJSON_GetArraySize(parsed);
        
        // Process each row
        for (int i = 0; i < arraySize && i < 3; i++) {
            cJSON *row = cJSON_GetArrayItem(parsed, fuzz_int32_t_2);
            
            if (cJSON_IsArray(row)) {
                int rowSize = cJSON_GetArraySize(row);
                
                // Create arrays for this row's data
                float *floatRow = (float*)cJSON_malloc(rowSize * sizeof(float));
                double *doubleRow = (double*)cJSON_malloc(rowSize * sizeof(double));
                
                for (int j = 0; j < rowSize && j < 3; j++) {
                    cJSON *element = cJSON_GetArrayItem(row, fuzz_int32_t_3);
                    
                    // Convert to appropriate types
                    if (element) {
                        double val = cJSON_GetNumberValue(element);
                        floatRow[j] = (float)val;
                        doubleRow[j] = val;
                    }
                }
                
                // Create arrays using the specialized functions
                cJSON *floatRowArray = cJSON_CreateFloatArray(floatRow, rowSize);
                cJSON *doubleRowArray = cJSON_CreateDoubleArray(doubleRow, rowSize);
                
                // Add arrays to their parent arrays
                cJSON_AddItemToArray(floatArrayObj, floatRowArray);
                cJSON_AddItemToArray(doubleArrayObj, doubleRowArray);
                
                // Clean up temporary arrays
                cJSON_free(floatRow);
                cJSON_free(doubleRow);
            }
        }
    }
    
    // 4. Add the arrays to the main object using cJSON_AddItemToObject
    cJSON_AddItemToObject(root, fuzz_str_4, floatArrayObj);
    cJSON_AddItemToObject(root, fuzz_str_5, doubleArrayObj);
    
    // 5. Create and add boolean values
    cJSON *boolTrue = cJSON_CreateBool(fuzz_int32_t_6);
    cJSON *boolFalse = cJSON_CreateBool(fuzz_int32_t_7);
    cJSON_AddItemToArray(boolArrayObj, boolTrue);
    cJSON_AddItemToArray(boolArrayObj, boolFalse);
    cJSON_AddItemToObject(root, fuzz_str_8, boolArrayObj);
    
    // 6. Add string and raw data using cJSON_AddStringToObject and cJSON_AddRawToObject
    cJSON_AddStringToObject(root, fuzz_str_9, fuzz_str_10);
    cJSON_AddRawToObject(root, fuzz_str_11, fuzz_str_12);
    
    // 7. Set value string on an object
    cJSON *stringItem = cJSON_GetObjectItem(root, fuzz_str_13);
    if (stringItem) {
        cJSON_SetValuestring(stringItem, fuzz_str_14);
    }
    
    // 8. Create additional arrays and add to main array
    cJSON *transformedArray = cJSON_CreateArray();
    cJSON *detachedArray = cJSON_CreateArray();
    
    // Add some values to transformed array
    for (int i = 0; i < 3; i++) {
        cJSON *num = cJSON_CreateNumber(i * 1.5);
        cJSON_AddItemToArray(transformedArray, num);
    }
    
    // Add transformed array to main array in object
    cJSON_AddItemToArray(mainArray, transformedArray);
    
    // 9. Detach an item from object
    cJSON *detached = cJSON_DetachItemFromObject(root, fuzz_str_15);
    if (detached) {
        cJSON_AddItemToArray(detachedArray, detached);
    }
    
    // 10. Print the JSON in different formats
    char *printedJson = cJSON_Print(root);
    char *bufferedJson = cJSON_PrintBuffered(root, fuzz_int32_t_16, fuzz_int32_t_17);
    
    // Write to files (as per instructions)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        if (printedJson) {
            fwrite(printedJson, strlen(printedJson), 1, out_file);
        }
        fwrite("\n---\n", 5, 1, out_file);
        if (bufferedJson) {
            fwrite(bufferedJson, strlen(bufferedJson), 1, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // 11. Test file operations using fmemopen (as per instructions)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        // Could use fd for operations if needed
        assert_file_closed(&in_file);;
    }
    
    // 12. Clean up all allocated resources
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_Delete(detachedArray);
    
    if (printedJson) cJSON_free(printedJson);
    if (bufferedJson) cJSON_free(bufferedJson);
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
}