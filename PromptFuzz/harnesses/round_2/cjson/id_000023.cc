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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 33, nr_unique_branch: 1
//<Quality> {"density":33,"unique_branches":{"cJSON_IsBool":[[3005,9,3005,21,0,0,4,0]]},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_IsObject","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsObject","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_Delete","cJSON_CreateObjectReference","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsBool","cJSON_IsTrue","cJSON_CreateArray","cJSON_Delete","cJSON_CreateNumber","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_GetArraySize","cJSON_CreateNumber","cJSON_ReplaceItemInArray","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_CreateNumber","cJSON_ReplaceItemViaPointer","cJSON_AddItemToObject","cJSON_AddNumberToObject","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateObjectReference","cJSON_GetObjectItemCaseSensitive","cJSON_IsBool","cJSON_IsTrue","cJSON_CreateArray","cJSON_CreateNumber","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_AddItemToArray","cJSON_GetArraySize","cJSON_CreateNumber","cJSON_ReplaceItemInArray","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_CreateNumber","cJSON_ReplaceItemViaPointer","cJSON_AddItemToObject","cJSON_AddNumberToObject","cJSON_AddItemToObject","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":1}
/*Here's a C++ fuzz driver that utilizes the specified cjson APIs to achieve parsing, modification, and validation of JSON data:

*/


// Define cJSON_bool if not already defined
#ifndef CJSON_H
typedef int cJSON_bool;
#endif

// Include necessary headers for cJSON
extern "C" {
    const char *cJSON_Version();
    cJSON *cJSON_Parse(const char * value);
    cJSON *cJSON_ParseWithLength(const char * value, size_t buffer_length);
    cJSON *cJSON_ParseWithOpts(const char * value, const char ** return_parse_end, cJSON_bool require_null_terminated);
    cJSON *cJSON_ParseWithLengthOpts(const char * value, size_t buffer_length, const char ** return_parse_end, cJSON_bool require_null_terminated);
    char *cJSON_Print(const cJSON * item);
    char *cJSON_PrintUnformatted(const cJSON * item);
    char *cJSON_PrintBuffered(const cJSON * item, int prebuffer, cJSON_bool fmt);
    cJSON_bool cJSON_PrintPreallocated(cJSON * item, char * buffer, const int length, const cJSON_bool format);
    void cJSON_Delete(cJSON * item);
    int cJSON_GetArraySize(const cJSON * array);
    cJSON *cJSON_GetArrayItem(const cJSON * array, int index);
    cJSON *cJSON_GetObjectItem(const cJSON *const object, const char *const string);
    cJSON *cJSON_GetObjectItemCaseSensitive(const cJSON *const object, const char *const string);
    cJSON_bool cJSON_HasObjectItem(const cJSON * object, const char * string);
    char *cJSON_GetStringValue(const cJSON *const item);
    double cJSON_GetNumberValue(const cJSON *const item);
    cJSON_bool cJSON_IsInvalid(const cJSON *const item);
    cJSON_bool cJSON_IsFalse(const cJSON *const item);
    cJSON_bool cJSON_IsTrue(const cJSON *const item);
    cJSON_bool cJSON_IsBool(const cJSON *const item);
    cJSON_bool cJSON_IsNull(const cJSON *const item);
    cJSON_bool cJSON_IsNumber(const cJSON *const item);
    cJSON_bool cJSON_IsString(const cJSON *const item);
    cJSON_bool cJSON_IsArray(const cJSON *const item);
    cJSON_bool cJSON_IsObject(const cJSON *const item);
    cJSON_bool cJSON_IsRaw(const cJSON *const item);
    cJSON *cJSON_CreateNull();
    cJSON *cJSON_CreateTrue();
    cJSON *cJSON_CreateFalse();
    cJSON *cJSON_CreateBool(cJSON_bool boolean);
    cJSON *cJSON_CreateNumber(double num);
    cJSON *cJSON_CreateString(const char * string);
    cJSON *cJSON_CreateRaw(const char * raw);
    cJSON *cJSON_CreateArray();
    cJSON *cJSON_CreateObject();
    cJSON *cJSON_CreateStringReference(const char * string);
    cJSON *cJSON_CreateObjectReference(const cJSON * child);
    cJSON *cJSON_CreateArrayReference(const cJSON * child);
    cJSON *cJSON_CreateIntArray(const int * numbers, int count);
    cJSON *cJSON_CreateFloatArray(const float * numbers, int count);
    cJSON *cJSON_CreateDoubleArray(const double * numbers, int count);
    cJSON *cJSON_CreateStringArray(const char *const * strings, int count);
    cJSON_bool cJSON_AddItemToArray(cJSON * array, cJSON * item);
    cJSON_bool cJSON_AddItemToObject(cJSON * object, const char * string, cJSON * item);
    cJSON_bool cJSON_AddItemToObjectCS(cJSON * object, const char * string, cJSON * item);
    cJSON_bool cJSON_AddItemReferenceToArray(cJSON * array, cJSON * item);
    cJSON_bool cJSON_AddItemReferenceToObject(cJSON * object, const char * string, cJSON * item);
    cJSON *cJSON_DetachItemViaPointer(cJSON * parent, cJSON *const item);
    cJSON *cJSON_DetachItemFromArray(cJSON * array, int which);
    void cJSON_DeleteItemFromArray(cJSON * array, int which);
    cJSON *cJSON_DetachItemFromObject(cJSON * object, const char * string);
    cJSON *cJSON_DetachItemFromObjectCaseSensitive(cJSON * object, const char * string);
    void cJSON_DeleteItemFromObject(cJSON * object, const char * string);
    void cJSON_DeleteItemFromObjectCaseSensitive(cJSON * object, const char * string);
    cJSON_bool cJSON_InsertItemInArray(cJSON * array, int which, cJSON * newitem);
    cJSON_bool cJSON_ReplaceItemViaPointer(cJSON *const parent, cJSON *const item, cJSON * replacement);
    cJSON_bool cJSON_ReplaceItemInArray(cJSON * array, int which, cJSON * newitem);
    cJSON_bool cJSON_ReplaceItemInObject(cJSON * object, const char * string, cJSON * newitem);
    cJSON_bool cJSON_ReplaceItemInObjectCaseSensitive(cJSON * object, const char * string, cJSON * newitem);
    cJSON *cJSON_Duplicate(const cJSON * item, cJSON_bool recurse);
    cJSON_bool cJSON_Compare(const cJSON *const a, const cJSON *const b, const cJSON_bool case_sensitive);
    void cJSON_Minify(char * json);
    cJSON *cJSON_AddNullToObject(cJSON *const object, const char *const name);
    cJSON *cJSON_AddTrueToObject(cJSON *const object, const char *const name);
    cJSON *cJSON_AddFalseToObject(cJSON *const object, const char *const name);
    cJSON *cJSON_AddBoolToObject(cJSON *const object, const char *const name, const cJSON_bool boolean);
    cJSON *cJSON_AddNumberToObject(cJSON *const object, const char *const name, const double number);
    cJSON *cJSON_AddStringToObject(cJSON *const object, const char *const name, const char *const string);
    cJSON *cJSON_AddRawToObject(cJSON *const object, const char *const name, const char *const raw);
    cJSON *cJSON_AddObjectToObject(cJSON *const object, const char *const name);
    cJSON *cJSON_AddArrayToObject(cJSON *const object, const char *const name);
    double cJSON_SetNumberHelper(cJSON * object, double number);
    char *cJSON_SetValuestring(cJSON * object, const char * valuestring);
    void *cJSON_malloc(size_t size);
    void cJSON_free(void * object);
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Event: Parse JSON, validate structure, modify values, and output result
    
    // 1. Parse the input data using cJSON_ParseWithLengthOpts
    const char *parse_end = nullptr;
    cJSON *root = cJSON_ParseWithLengthOpts(reinterpret_cast<const char*>(data), size, &parse_end, false);
    
    if (!root) {
        return 0; // Failed to parse
    }
    
    // 2. Check if root is an object
    cJSON_bool is_object = cJSON_IsObject(root);
    
    if (!is_object) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 3. Get object item using case-insensitive method
    cJSON *widget = cJSON_GetObjectItem(root, "widget");
    
    if (!widget || !cJSON_IsObject(widget)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 4. Get object item using case-sensitive method
    cJSON *window = cJSON_GetObjectItemCaseSensitive(widget, "window");
    
    if (!window || !cJSON_IsObject(window)) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 5. Create a reference to the window object
    cJSON *window_ref = cJSON_CreateObjectReference(window);
    
    if (!window_ref) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 6. Get and check boolean values
    cJSON *debug_item = cJSON_GetObjectItemCaseSensitive(widget, "debug");
    cJSON_bool is_bool = cJSON_IsBool(debug_item);
    
    if (debug_item && is_bool) {
        cJSON_bool is_true = cJSON_IsTrue(debug_item);
        // Use is_true as needed
        (void)is_true;
    }
    
    // 7. Create a new array
    cJSON *new_array = cJSON_CreateArray();
    
    if (!new_array) {
        cJSON_Delete(root);
        return 0;
    }
    
    // 8. Create and add numbers to array
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(99.9);
    
    if (num1 && num2) {
        cJSON_bool added = cJSON_AddItemToArray(new_array, num1);
        (void)added; // Use added as needed
        
        added = cJSON_AddItemToArray(new_array, num2);
        (void)added;
        
        // 9. Get array size
        int array_size = cJSON_GetArraySize(new_array);
        
        if (array_size > 0) {
            // 10. Replace item in array
            cJSON *replacement = cJSON_CreateNumber(123.45);
            if (replacement) {
                cJSON_bool replaced = cJSON_ReplaceItemInArray(new_array, 0, replacement);
                (void)replaced;
            }
        }
    }
    
    // 11. Check if certain items are numbers
    cJSON *width_item = cJSON_GetObjectItemCaseSensitive(window, "width");
    cJSON_bool is_number = cJSON_IsNumber(width_item);
    
    if (is_number && width_item) {
        // 12. Replace item via pointer
        cJSON *new_width = cJSON_CreateNumber(600.0);
        if (new_width) {
            cJSON_bool replaced = cJSON_ReplaceItemViaPointer(window, width_item, new_width);
            (void)replaced;
        }
    }
    
    // 13. Add array to widget object
    cJSON_bool added_to_object = cJSON_AddItemToObject(widget, "new_array", new_array);
    (void)added_to_object;
    
    // 14. Add number to object using helper function
    cJSON *added_num = cJSON_AddNumberToObject(widget, "extra_number", 777.0);
    (void)added_num;
    
    // 15. Add window reference to root
    cJSON_bool added_ref = cJSON_AddItemToObject(root, "window_ref", window_ref);
    (void)added_ref;
    
    // 16. Print the modified JSON
    char *json_str = cJSON_Print(root);
    
    if (json_str) {
        // Write to output file if needed
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(json_str, 1, strlen(json_str), out_file);
            assert_file_closed(&out_file);;
        }
        
        // Use file descriptor if needed
        if (out_file) {
            int fd = fuzz_fileno(out_file);
            (void)fd; // Use fd as needed
        }
        
        cJSON_free(json_str);
    }
    
    // 17. Clean up
    cJSON_Delete(root);
    
    return 0;
}