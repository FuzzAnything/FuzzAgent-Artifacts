// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_HasObjectItem at cJSON.c:1993:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cJSON.h"

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated copy for functions that require it
    char *null_terminated_data = malloc(Size + 1);
    if (null_terminated_data == NULL) {
        return 0;
    }
    memcpy(null_terminated_data, Data, Size);
    null_terminated_data[Size] = '\0';

    // Test cJSON_Parse
    cJSON *json1 = cJSON_Parse(null_terminated_data);
    if (json1 != NULL) {
        // Test cJSON_HasObjectItem on parsed JSON
        if (json1->type == cJSON_Object) {
            // Try with different keys to explore more states
            cJSON_HasObjectItem(json1, "key");
            cJSON_HasObjectItem(json1, "name");
            cJSON_HasObjectItem(json1, "value");
            cJSON_HasObjectItem(json1, "");
            
            // Also try with a key from the input data (truncated to reasonable length)
            char key[32] = {0};
            size_t key_len = Size < 31 ? Size : 31;
            memcpy(key, Data, key_len);
            key[key_len] = '\0';
            cJSON_HasObjectItem(json1, key);
        }
        cJSON_Delete(json1);
    } else {
        // On parse failure, get error pointer
        const char *error_ptr = cJSON_GetErrorPtr();
        (void)error_ptr; // Use error_ptr to avoid unused variable warning
    }

    // Test cJSON_ParseWithLength
    cJSON *json2 = cJSON_ParseWithLength((const char*)Data, Size);
    if (json2 != NULL) {
        cJSON_Delete(json2);
    }

    // Test cJSON_ParseWithOpts with different options
    const char *parse_end1 = NULL;
    cJSON *json3 = cJSON_ParseWithOpts(null_terminated_data, &parse_end1, 0);
    if (json3 != NULL) {
        cJSON_Delete(json3);
    }

    const char *parse_end2 = NULL;
    cJSON *json4 = cJSON_ParseWithOpts(null_terminated_data, &parse_end2, 1);
    if (json4 != NULL) {
        cJSON_Delete(json4);
    }

    // Test cJSON_ParseWithLengthOpts with different options
    const char *parse_end3 = NULL;
    cJSON *json5 = cJSON_ParseWithLengthOpts((const char*)Data, Size, &parse_end3, 0);
    if (json5 != NULL) {
        cJSON_Delete(json5);
    }

    const char *parse_end4 = NULL;
    cJSON *json6 = cJSON_ParseWithLengthOpts(null_terminated_data, Size + 1, &parse_end4, 1);
    if (json6 != NULL) {
        cJSON_Delete(json6);
    }

    // Also test with the original buffer but require null termination (should fail)
    const char *parse_end5 = NULL;
    cJSON *json7 = cJSON_ParseWithLengthOpts((const char*)Data, Size, &parse_end5, 1);
    if (json7 != NULL) {
        cJSON_Delete(json7);
    }

    free(null_terminated_data);
    return 0;
}