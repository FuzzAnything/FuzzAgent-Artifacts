#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include "cjson/cJSON.h"

namespace harness_025 {
/*
 * Harness 025: cJSON_Compare comparison failure paths and edge cases targeting remaining 73 blocked branches
 * 
 * Focus: Specific comparison failure scenarios not covered by previous harnesses:
 * 1) Comparison of malformed/invalid JSON objects (type manipulation, null pointers)
 * 2) Comparison of objects with different memory allocation patterns (references vs owned)
 * 3) Comparison with case-sensitive vs case-insensitive mismatches
 * 4) Comparison of objects with circular references or self-referential structures
 * 5) Comparison with mixed escaped/non-escaped characters
 * 6) Comparison of objects with special floating point values (NaN, Inf)
 * 
 * Semantic diversity from existing harnesses:
 * - harness_023: Complex nested structures, same keys different values, different keys
 * - harness_024: Edge case key characters, null values, escaped chars, Unicode, mixed types  
 * - This harness: Reference objects, circular references, malformed types, special float values
 * 
 * Unique APIs to target: Reference creation functions for different memory patterns:
 * - cJSON_CreateStringReference (creates string that won't be freed by cJSON_Delete)
 * - cJSON_CreateObjectReference (references existing object)
 * - cJSON_CreateArrayReference (references existing array)
 * 
 * Blocked branches to target in cJSON_Compare:
 * - Line 3078: NULL pointer checks (a == NULL || b == NULL)
 * - Line 3078: Type mismatch ((a->type & 0xFF) != (b->type & 0xFF))
 * - Line 3097: Default case for invalid types (return false)
 * - Line 3115: compare_double() for special floating point values
 * - Line 3123: NULL valuestring checks for String/Raw types
 * - Line 3166: b_element == NULL (key missing in second object)
 * - Line 3171: !cJSON_Compare() for value comparison failure
 * - Line 3182: a_element == NULL (key missing in first object)
 * - Line 3187: !cJSON_Compare() for reverse comparison failure
 * - Line 3197: Default case for invalid types in object comparison
 */



// Helper to create malformed JSON objects by manipulating type fields
static cJSON* create_malformed_object(FuzzedDataProvider& fdp) {
    uint8_t malform_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (malform_type) {
        case 0: {
            // Object with manipulated type field (invalid type)
            cJSON* obj = cJSON_CreateObject();
            if (!obj) return NULL;
            
            // Add some valid content first
            cJSON_AddItemToObject(obj, "key1", cJSON_CreateNumber(42.5));
            cJSON_AddItemToObject(obj, "key2", cJSON_CreateString("test"));
            
            // Now set an invalid type (not a power of 2 or valid type)
            uint8_t invalid_type = fdp.ConsumeIntegral<uint8_t>() | 0x40; // Ensure not valid
            obj->type = invalid_type;
            
            return obj;
        }
        
        case 1: {
            // Object with NULL valuestring for String type
            cJSON* obj = cJSON_CreateString("test");
            if (!obj) return NULL;
            
            // Simulate malformed state: String type but NULL valuestring
            obj->valuestring = NULL;
            return obj;
        }
        
        case 2: {
            // Object with inconsistent type vs valuestring
            cJSON* obj = cJSON_CreateNumber(3.14);
            if (!obj) return NULL;
            
            // Make it appear as String type but keep numeric value
            obj->type = cJSON_String;
            return obj;
        }
        
        default: {
            // Object with partially corrupted child
            cJSON* obj = cJSON_CreateObject();
            if (!obj) return NULL;
            
            cJSON_AddItemToObject(obj, "valid", cJSON_CreateBool(1));
            
            // Create child with corrupted type
            cJSON* corrupted_child = (cJSON*)malloc(sizeof(cJSON));
            if (corrupted_child) {
                memset(corrupted_child, 0, sizeof(cJSON));
                corrupted_child->type = 0xFF; // Invalid type
                corrupted_child->string = NULL;
                corrupted_child->valuestring = NULL;
                corrupted_child->valueint = 0;
                corrupted_child->valuedouble = 0.0;
                corrupted_child->child = NULL;
                corrupted_child->next = NULL;
                corrupted_child->prev = NULL;
                
                // Don't add to object as child, leave it standalone for comparison
                free(corrupted_child);
            }
            
            return obj;
        }
    }
}

// Helper to create objects with reference memory patterns
static std::pair<cJSON*, cJSON*> create_objects_with_different_memory_patterns(FuzzedDataProvider& fdp) {
    // Create base objects
    cJSON* base_string = cJSON_CreateString("reference_string");
    cJSON* base_object = cJSON_CreateObject();
    cJSON* base_array = cJSON_CreateArray();
    
    if (!base_string || !base_object || !base_array) {
        if (base_string) cJSON_Delete(base_string);
        if (base_object) cJSON_Delete(base_object);
        if (base_array) cJSON_Delete(base_array);
        return {NULL, NULL};
    }
    
    // Add some content to base object and array
    cJSON_AddItemToObject(base_object, "nested", cJSON_CreateNumber(42.5));
    cJSON_AddItemToArray(base_array, cJSON_CreateString("array_item"));
    
    uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (pattern_type) {
        case 0: {
            // Compare reference string vs owned string
            cJSON* ref_string = cJSON_CreateStringReference("reference_string");
            return {base_string, ref_string};
        }
        
        case 1: {
            // Compare reference object vs owned object
            cJSON* ref_object = cJSON_CreateObjectReference(base_object);
            return {base_object, ref_object};
        }
        
        case 2: {
            // Compare reference array vs owned array
            cJSON* ref_array = cJSON_CreateArrayReference(base_array);
            return {base_array, ref_array};
        }
        
        default: {
            // Mix of references and owned objects in nested structure
            cJSON* obj1 = cJSON_CreateObject();
            cJSON* obj2 = cJSON_CreateObject();
            
            if (!obj1 || !obj2) {
                if (obj1) cJSON_Delete(obj1);
                if (obj2) cJSON_Delete(obj2);
                return {NULL, NULL};
            }
            
            // Add reference to obj1, owned copy to obj2
            cJSON_AddItemReferenceToObject(obj1, "ref_item", base_string);
            cJSON_AddItemToObject(obj2, "ref_item", cJSON_CreateString("reference_string"));
            
            // Add owned object to obj1, reference to obj2
            cJSON_AddItemToObject(obj1, "owned_item", cJSON_CreateNumber(3.14));
            cJSON_AddItemReferenceToObject(obj2, "owned_item", base_object);
            
            return {obj1, obj2};
        }
    }
}

// Helper to create circular/self-referential structures
static cJSON* create_circular_structure(FuzzedDataProvider& fdp) {
    uint8_t circular_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    switch (circular_type) {
        case 0: {
            // Self-referential object (object contains itself)
            cJSON* obj = cJSON_CreateObject();
            if (!obj) return NULL;
            
            // Add regular items
            cJSON_AddItemToObject(obj, "number", cJSON_CreateNumber(42.5));
            cJSON_AddItemToObject(obj, "string", cJSON_CreateString("test"));
            
            // Try to create self-reference (won't actually work due to type system)
            // but we can create a reference that points to similar structure
            cJSON* similar_obj = cJSON_CreateObject();
            if (similar_obj) {
                cJSON_AddItemToObject(similar_obj, "similar", cJSON_CreateNumber(42.5));
                cJSON_AddItemReferenceToObject(obj, "self_ref", similar_obj);
                // Note: This isn't truly circular but tests reference patterns
                // similar_obj is now referenced by obj, don't delete it separately
            }
            
            return obj;
        }
        
        case 1: {
            // Mutual reference between two objects - create owned copies instead of references
            cJSON* obj1 = cJSON_CreateObject();
            cJSON* obj2 = cJSON_CreateObject();
            
            if (!obj1 || !obj2) {
                if (obj1) cJSON_Delete(obj1);
                if (obj2) cJSON_Delete(obj2);
                return NULL;
            }
            
            cJSON_AddItemToObject(obj1, "name", cJSON_CreateString("object1"));
            cJSON_AddItemToObject(obj2, "name", cJSON_CreateString("object2"));
            
            // Instead of creating circular references (which cause cleanup issues),
            // create owned copies of similar objects
            cJSON* copy_for_obj1 = cJSON_CreateObject();
            cJSON* copy_for_obj2 = cJSON_CreateObject();
            
            if (copy_for_obj1 && copy_for_obj2) {
                cJSON_AddItemToObject(copy_for_obj1, "copy_key", cJSON_CreateNumber(1.0));
                cJSON_AddItemToObject(copy_for_obj2, "copy_key", cJSON_CreateNumber(1.0));
                
                cJSON_AddItemToObject(obj1, "other_copy", copy_for_obj1);
                cJSON_AddItemToObject(obj2, "other_copy", copy_for_obj2);
            } else {
                if (copy_for_obj1) cJSON_Delete(copy_for_obj1);
                if (copy_for_obj2) cJSON_Delete(copy_for_obj2);
            }
            
            // Clean up obj2 and return obj1 (obj2 is independent, not referenced)
            cJSON_Delete(obj2);
            return obj1;
        }
        
        default: {
            // Create array with owned items (not self-referential to avoid cleanup issues)
            cJSON* arr = cJSON_CreateArray();
            if (!arr) return NULL;
            
            // Add some items
            cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
            cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));
            
            // Instead of self-reference, add a reference to another array
            cJSON* other_arr = cJSON_CreateArray();
            if (other_arr) {
                cJSON_AddItemToArray(other_arr, cJSON_CreateNumber(3.0));
                cJSON_AddItemReferenceToArray(arr, other_arr);
                // other_arr is now referenced by arr, don't delete separately
            }
            
            return arr;
        }
    }
}

// Helper to create strings with mixed escaped/non-escaped characters
static cJSON* create_string_with_mixed_escapes(FuzzedDataProvider& fdp) {
    uint8_t escape_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (escape_type) {
        case 0: {
            // String with standard escapes
            std::string escaped = "Line1\\nLine2\\tTab\\\"Quote\\\\Backslash";
            return cJSON_CreateString(escaped.c_str());
        }
        
        case 1: {
            // String with mixed escaped and literal versions
            std::string mixed = "Mix: \\n newline vs \n actual newline";
            return cJSON_CreateString(mixed.c_str());
        }
        
        case 2: {
            // String with Unicode escapes
            std::string unicode = "Unicode: \\u03B1\\u03B2\\u03B3 (αβγ)";
            return cJSON_CreateString(unicode.c_str());
        }
        
        default: {
            // String with control characters and escapes
            std::string control = "Control:\\x01\\x02\\x03 and tab:\t and return:\r";
            return cJSON_CreateString(control.c_str());
        }
    }
}

// Helper to create numbers with special floating point values
static cJSON* create_number_with_special_value(FuzzedDataProvider& fdp) {
    uint8_t special_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (special_type) {
        case 0:
            return cJSON_CreateNumber(std::numeric_limits<double>::quiet_NaN());
        case 1:
            return cJSON_CreateNumber(std::numeric_limits<double>::signaling_NaN());
        case 2:
            return cJSON_CreateNumber(std::numeric_limits<double>::infinity());
        case 3:
            return cJSON_CreateNumber(-std::numeric_limits<double>::infinity());
        case 4: {
            // Very small number near epsilon
            double epsilon = std::numeric_limits<double>::epsilon();
            return cJSON_CreateNumber(epsilon);
        }
        default: {
            // Very large number
            double large = std::numeric_limits<double>::max();
            return cJSON_CreateNumber(large * fdp.ConsumeFloatingPoint<double>());
        }
    }
}

// Helper to test case-sensitive vs case-insensitive mismatches
static std::pair<cJSON*, cJSON*> create_case_mismatch_objects(FuzzedDataProvider& fdp) {
    cJSON* obj1 = cJSON_CreateObject();
    cJSON* obj2 = cJSON_CreateObject();
    
    if (!obj1 || !obj2) {
        if (obj1) cJSON_Delete(obj1);
        if (obj2) cJSON_Delete(obj2);
        return {NULL, NULL};
    }
    
    uint8_t mismatch_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    switch (mismatch_type) {
        case 0: {
            // Same keys different case (e.g., "key" vs "KEY")
            std::string key_lower = "testkey";
            std::string key_upper = "TESTKEY";
            
            cJSON_AddItemToObject(obj1, key_lower.c_str(), cJSON_CreateNumber(1.0));
            cJSON_AddItemToObject(obj2, key_upper.c_str(), cJSON_CreateNumber(1.0));
            
            // Add another identical key for both
            cJSON_AddItemToObject(obj1, "common", cJSON_CreateString("same"));
            cJSON_AddItemToObject(obj2, "common", cJSON_CreateString("same"));
            
            break;
        }
        
        case 1: {
            // Mixed case in same object vs different case in other
            cJSON_AddItemToObject(obj1, "MixedCaseKey", cJSON_CreateNumber(2.0));
            cJSON_AddItemToObject(obj2, "mixedcasekey", cJSON_CreateNumber(2.0));
            
            cJSON_AddItemToObject(obj1, "Another_Key", cJSON_CreateString("test"));
            cJSON_AddItemToObject(obj2, "ANOTHER_KEY", cJSON_CreateString("test"));
            
            break;
        }
        
        default: {
            // Unicode case differences (if applicable)
            cJSON_AddItemToObject(obj1, "straße", cJSON_CreateNumber(3.0));  // German sharp s
            cJSON_AddItemToObject(obj2, "STRASSE", cJSON_CreateNumber(3.0));  // Traditional uppercase
            
            cJSON_AddItemToObject(obj1, "ιωάννης", cJSON_CreateString("greek"));
            cJSON_AddItemToObject(obj2, "ΙΩΑΝΝΗΣ", cJSON_CreateString("greek"));
            
            break;
        }
    }
    
    return {obj1, obj2};
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 4) return 0;  // Minimum size for basic operations
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation type to determine which test scenario to run
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 7;
    
    switch (operation) {
        case 0: {
            // Test 1: Malformed/invalid JSON objects
            cJSON* malformed1 = create_malformed_object(fdp);
            cJSON* malformed2 = create_malformed_object(fdp);
            
            if (malformed1 && malformed2) {
                // Test both case-sensitive and case-insensitive
                cJSON_Compare(malformed1, malformed2, fdp.ConsumeBool());
                cJSON_Compare(malformed2, malformed1, fdp.ConsumeBool());
            }
            
            if (malformed1) cJSON_Delete(malformed1);
            if (malformed2) cJSON_Delete(malformed2);
            break;
        }
        
        case 1: {
            // Test 2: Different memory allocation patterns (references vs owned)
            auto [obj1, obj2] = create_objects_with_different_memory_patterns(fdp);
            
            if (obj1 && obj2) {
                cJSON_Compare(obj1, obj2, fdp.ConsumeBool());
                cJSON_Compare(obj2, obj1, fdp.ConsumeBool());
                
                // Also compare references with themselves
                cJSON_Compare(obj1, obj1, fdp.ConsumeBool());
                cJSON_Compare(obj2, obj2, fdp.ConsumeBool());
            }
            
            if (obj1) cJSON_Delete(obj1);
            if (obj2) cJSON_Delete(obj2);
            break;
        }
        
        case 2: {
            // Test 3: Case-sensitive vs case-insensitive mismatches
            auto [case_obj1, case_obj2] = create_case_mismatch_objects(fdp);
            
            if (case_obj1 && case_obj2) {
                // Test both case-sensitive (false) and case-insensitive (true)
                cJSON_bool case_sensitive = fdp.ConsumeBool();
                cJSON_bool case_insensitive = !case_sensitive;
                
                cJSON_Compare(case_obj1, case_obj2, case_sensitive);
                cJSON_Compare(case_obj1, case_obj2, case_insensitive);
                cJSON_Compare(case_obj2, case_obj1, case_sensitive);
                cJSON_Compare(case_obj2, case_obj1, case_insensitive);
            }
            
            if (case_obj1) cJSON_Delete(case_obj1);
            if (case_obj2) cJSON_Delete(case_obj2);
            break;
        }
        
        case 3: {
            // Test 4: Circular/self-referential structures
            cJSON* circular1 = create_circular_structure(fdp);
            cJSON* circular2 = create_circular_structure(fdp);
            
            if (circular1 && circular2) {
                cJSON_Compare(circular1, circular2, fdp.ConsumeBool());
                cJSON_Compare(circular1, circular1, fdp.ConsumeBool());  // Self-comparison
                cJSON_Compare(circular2, circular2, fdp.ConsumeBool());  // Self-comparison
            }
            
            if (circular1) cJSON_Delete(circular1);
            if (circular2) cJSON_Delete(circular2);
            break;
        }
        
        case 4: {
            // Test 5: Mixed escaped/non-escaped characters
            cJSON* escaped1 = create_string_with_mixed_escapes(fdp);
            cJSON* escaped2 = create_string_with_mixed_escapes(fdp);
            
            if (escaped1 && escaped2) {
                cJSON_Compare(escaped1, escaped2, fdp.ConsumeBool());
                
                // Also create Raw JSON with same content for comparison
                if (escaped1->valuestring) {
                    cJSON* raw1 = cJSON_CreateRaw(escaped1->valuestring);
                    if (raw1) {
                        cJSON_Compare(escaped1, raw1, fdp.ConsumeBool());
                        cJSON_Delete(raw1);
                    }
                }
            }
            
            if (escaped1) cJSON_Delete(escaped1);
            if (escaped2) cJSON_Delete(escaped2);
            break;
        }
        
        case 5: {
            // Test 6: Special floating point values
            cJSON* special1 = create_number_with_special_value(fdp);
            cJSON* special2 = create_number_with_special_value(fdp);
            
            if (special1 && special2) {
                cJSON_Compare(special1, special2, fdp.ConsumeBool());
                
                // Also compare with regular numbers
                cJSON* regular = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                if (regular) {
                    cJSON_Compare(special1, regular, fdp.ConsumeBool());
                    cJSON_Compare(regular, special2, fdp.ConsumeBool());
                    cJSON_Delete(regular);
                }
            }
            
            if (special1) cJSON_Delete(special1);
            if (special2) cJSON_Delete(special2);
            break;
        }
        
        default: {
            // Combined test: Mix multiple failure scenarios
            cJSON* complex1 = cJSON_CreateObject();
            cJSON* complex2 = cJSON_CreateObject();
            
            if (complex1 && complex2) {
                // Add various elements to test combined scenarios
                
                // Add malformed child
                cJSON* malformed_child = create_malformed_object(fdp);
                if (malformed_child) {
                    cJSON_AddItemToObject(complex1, "malformed", malformed_child);
                }
                
                // Add reference vs owned string
                cJSON* ref_string = cJSON_CreateStringReference("test_string");
                cJSON* owned_string = cJSON_CreateString("test_string");
                if (ref_string && owned_string) {
                    cJSON_AddItemReferenceToObject(complex1, "str_ref", ref_string);
                    cJSON_AddItemToObject(complex2, "str_owned", owned_string);
                }
                
                // Add case-mismatch keys
                cJSON_AddItemToObject(complex1, "CaseKey", cJSON_CreateNumber(42.5));
                cJSON_AddItemToObject(complex2, "CASEKEY", cJSON_CreateNumber(42.5));
                
                // Add special floating point values
                cJSON* special_num = create_number_with_special_value(fdp);
                if (special_num) {
                    cJSON_AddItemToObject(complex1, "special_float", special_num);
                    // Add slightly different special value to complex2
                    cJSON* special_num2 = create_number_with_special_value(fdp);
                    if (special_num2) {
                        cJSON_AddItemToObject(complex2, "special_float", special_num2);
                    }
                }
                
                // Test comparison with both case-sensitive and case-insensitive
                cJSON_Compare(complex1, complex2, false);  // case-sensitive
                cJSON_Compare(complex1, complex2, true);   // case-insensitive
                cJSON_Compare(complex2, complex1, false);
                cJSON_Compare(complex2, complex1, true);
                // Clean up (parent objects will handle child cleanup)
                // DON'T delete ref_string or owned_string here - they're owned by parent objects
                // When complex1 and complex2 are deleted, they will handle cleanup
            }
            
            if (complex1) cJSON_Delete(complex1);
            if (complex2) cJSON_Delete(complex2);
            break;
        }
    }
    
    return 0;
}

} // namespace harness_025
