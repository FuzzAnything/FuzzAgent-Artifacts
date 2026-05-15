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
//<ID> 68
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 0
//<Quality> {"density":34,"unique_branches":{},"library_calls":["cJSON_ParseWithOpts","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_CreateBool","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_ReplaceItemInObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateBool","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_AddItemReferenceToObject","cJSON_AddTrueToObject","cJSON_Delete"],"critical_calls":["cJSON_ParseWithOpts","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_CreateBool","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_GetArraySize","cJSON_IsArray","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_ReplaceItemInObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_CreateBool","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_GetObjectItemCaseSensitive","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsNumber","cJSON_SetNumberHelper","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_AddItemReferenceToObject","cJSON_AddTrueToObject","cJSON_Delete"],"visited":0}
/**/


// cjson headers would be included here

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Step 1: Parse JSON from input data using cJSON_ParseWithOpts
    char *json_str = static_cast<char*>(malloc(size + 1));
    if (!json_str) {
	return 0;
	}
	
    
    memcpy(json_str, data, size);
    json_str[size] = '\0';  // Ensure null termination
    
    const char *parse_end = nullptr;
    cJSON *parsed_json = cJSON_ParseWithOpts(json_str, &parse_end, true);
    free(json_str);
    
    if (!parsed_json) {
        return 0;
    }

    // Step 2: Use various APIs to manipulate the parsed JSON
    
    // Get a number value from the JSON (if available)
    cJSON *number_item = cJSON_GetObjectItemCaseSensitive(parsed_json, "cachePagesTrack");
    if (cJSON_IsNumber(number_item)) {
        double num_value = cJSON_GetNumberValue(number_item);
        // Use the value in some way (e.g., as a condition)
        if (num_value > 0) {
            // Create a boolean item based on the number value
            cJSON *bool_item = cJSON_CreateBool(num_value > 50);
            
            // Replace an existing item with this boolean item
            // Try to replace "useJSP" field if it exists
            cJSON *web_app = cJSON_GetObjectItemCaseSensitive(parsed_json, "web-app");
            if (cJSON_IsObject(web_app)) {
                cJSON *servlet = cJSON_GetObjectItemCaseSensitive(web_app, "servlet");
                if (cJSON_IsArray(servlet) && cJSON_GetArraySize(servlet) > 0) {
                    cJSON *first_servlet = cJSON_GetArrayItem(servlet, 0);
                    if (cJSON_IsObject(first_servlet)) {
                        cJSON *init_param = cJSON_GetObjectItemCaseSensitive(first_servlet, "init-param");
                        if (cJSON_IsObject(init_param)) {
                            // Replace useJSP with the boolean item
                            cJSON_ReplaceItemInObject(init_param, "useJSP", bool_item);
                        }
                    }
                }
                
                // Also try case-sensitive replacement on a different field
                cJSON *servlet_mapping = cJSON_GetObjectItemCaseSensitive(web_app, "servlet-mapping");
                if (cJSON_IsObject(servlet_mapping)) {
                    cJSON *new_item = cJSON_CreateBool(true);
                    cJSON_ReplaceItemInObjectCaseSensitive(servlet_mapping, "cofaxCDS", new_item);
                }
                
                // Use cJSON_SetNumberHelper to modify a number field
                cJSON *adminGroupID = cJSON_GetObjectItemCaseSensitive(web_app, "servlet");
                if (cJSON_IsArray(adminGroupID)) {
                    if (cJSON_GetArraySize(adminGroupID) > 4) {
                        cJSON *last_servlet = cJSON_GetArrayItem(adminGroupID, 4);
                        if (cJSON_IsObject(last_servlet)) {
                            cJSON *init_param = cJSON_GetObjectItemCaseSensitive(last_servlet, "init-param");
                            if (cJSON_IsObject(init_param)) {
                                cJSON *adminGroup = cJSON_GetObjectItemCaseSensitive(init_param, "adminGroupID");
                                if (cJSON_IsNumber(adminGroup)) {
                                    double new_num = cJSON_SetNumberHelper(adminGroup, 999.0);
                                    (void)new_num; // Suppress unused warning
                                }
                            }
                        }
                    }
                }
                
                // Add a reference to an object
                cJSON *taglib = cJSON_GetObjectItemCaseSensitive(web_app, "taglib");
                if (cJSON_IsObject(taglib)) {
                    cJSON_AddItemReferenceToObject(web_app, "taglibReference", taglib);
                }
                
                // Add true to an object
                cJSON_AddTrueToObject(web_app, "fuzzerModified");
            }
        }
    }
    
    // Clean up
    cJSON_Delete(parsed_json);
    
    return 0;
}