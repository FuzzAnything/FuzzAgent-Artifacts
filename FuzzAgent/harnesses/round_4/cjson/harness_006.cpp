#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_006 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for comprehensive comparison testing
    // Need enough data for: operation type, JSON types, multiple values, strings, etc.
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Determine the comparison scenario to test
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 8;

    // Create two JSON items to compare
    cJSON* item1 = NULL;
    cJSON* item2 = NULL;
    
    // Determine whether to test case sensitivity
    bool case_sensitive = fdp.ConsumeBool();

    switch (scenario) {
        case 0:
            // Test 1: Compare same type items (could be equal or different)
            if (fdp.remaining_bytes() > 2) {
                uint8_t item_type = fdp.ConsumeIntegral<uint8_t>() % 7;
                
                // Create first item
                switch (item_type) {
                    case 0:
                        item1 = cJSON_CreateNull();
                        break;
                    case 1:
                        item1 = cJSON_CreateBool(fdp.ConsumeBool());
                        break;
                    case 2:
                        item1 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                        break;
                    case 3:
                        if (fdp.remaining_bytes() > 0) {
                            size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                            if (fdp.remaining_bytes() >= str_len) {
                                std::string str_val = fdp.ConsumeBytesAsString(str_len);
                                item1 = cJSON_CreateString(str_val.c_str());
                            }
                        }
                        break;
                    case 4:
                        if (fdp.remaining_bytes() > 0) {
                            size_t raw_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                            if (fdp.remaining_bytes() >= raw_len) {
                                std::string raw_val = fdp.ConsumeBytesAsString(raw_len);
                                item1 = cJSON_CreateRaw(raw_val.c_str());
                            }
                        }
                        break;
                    case 5:
                        item1 = cJSON_CreateArray();
                        if (item1 != NULL && fdp.remaining_bytes() > 0) {
                            int array_size = fdp.ConsumeIntegralInRange<int>(0, 3);
                            for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                                uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                                cJSON* elem = NULL;
                                
                                switch (elem_type) {
                                    case 0:
                                        elem = cJSON_CreateNull();
                                        break;
                                    case 1:
                                        elem = cJSON_CreateBool(fdp.ConsumeBool());
                                        break;
                                    case 2:
                                        elem = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                                        break;
                                    case 3:
                                        if (fdp.remaining_bytes() > 0) {
                                            size_t elem_str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                                            if (fdp.remaining_bytes() >= elem_str_len) {
                                                std::string elem_str = fdp.ConsumeBytesAsString(elem_str_len);
                                                elem = cJSON_CreateString(elem_str.c_str());
                                            }
                                        }
                                        break;
                                }
                                
                                if (elem != NULL) {
                                    cJSON_AddItemToArray(item1, elem);
                                }
                            }
                        }
                        break;
                    case 6:
                        item1 = cJSON_CreateObject();
                        if (item1 != NULL && fdp.remaining_bytes() > 0) {
                            int obj_size = fdp.ConsumeIntegralInRange<int>(0, 3);
                            for (int i = 0; i < obj_size && fdp.remaining_bytes() > 0; i++) {
                                size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                                if (fdp.remaining_bytes() >= key_len) {
                                    std::string key = fdp.ConsumeBytesAsString(key_len);
                                    
                                    uint8_t val_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                                    cJSON* val = NULL;
                                    
                                    switch (val_type) {
                                        case 0:
                                            val = cJSON_CreateNull();
                                            break;
                                        case 1:
                                            val = cJSON_CreateBool(fdp.ConsumeBool());
                                            break;
                                        case 2:
                                            val = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                                            break;
                                        case 3:
                                            if (fdp.remaining_bytes() > 0) {
                                                size_t val_str_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                                                if (fdp.remaining_bytes() >= val_str_len) {
                                                    std::string val_str = fdp.ConsumeBytesAsString(val_str_len);
                                                    val = cJSON_CreateString(val_str.c_str());
                                                }
                                            }
                                            break;
                                    }
                                    
                                    if (val != NULL) {
                                        cJSON_AddItemToObject(item1, key.c_str(), val);
                                    }
                                }
                            }
                        }
                        break;
                }
                
                // Create second item of same type
                if (item1 != NULL && fdp.remaining_bytes() > 0) {
                    switch (item_type) {
                        case 0:
                            item2 = cJSON_CreateNull();
                            break;
                        case 1:
                            item2 = cJSON_CreateBool(fdp.ConsumeBool());
                            break;
                        case 2:
                            item2 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                            break;
                        case 3:
                            if (fdp.remaining_bytes() > 0) {
                                size_t str_len2 = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                                if (fdp.remaining_bytes() >= str_len2) {
                                    std::string str_val2 = fdp.ConsumeBytesAsString(str_len2);
                                    item2 = cJSON_CreateString(str_val2.c_str());
                                }
                            }
                            break;
                        case 4:
                            if (fdp.remaining_bytes() > 0) {
                                size_t raw_len2 = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                                if (fdp.remaining_bytes() >= raw_len2) {
                                    std::string raw_val2 = fdp.ConsumeBytesAsString(raw_len2);
                                    item2 = cJSON_CreateRaw(raw_val2.c_str());
                                }
                            }
                            break;
                        case 5:
                            item2 = cJSON_CreateArray();
                            if (item2 != NULL && fdp.remaining_bytes() > 0) {
                                int array_size2 = fdp.ConsumeIntegralInRange<int>(0, 3);
                                for (int i = 0; i < array_size2 && fdp.remaining_bytes() > 0; i++) {
                                    uint8_t elem_type2 = fdp.ConsumeIntegral<uint8_t>() % 4;
                                    cJSON* elem2 = NULL;
                                    
                                    switch (elem_type2) {
                                        case 0:
                                            elem2 = cJSON_CreateNull();
                                            break;
                                        case 1:
                                            elem2 = cJSON_CreateBool(fdp.ConsumeBool());
                                            break;
                                        case 2:
                                            elem2 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                                            break;
                                        case 3:
                                            if (fdp.remaining_bytes() > 0) {
                                                size_t elem_str_len2 = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                                                if (fdp.remaining_bytes() >= elem_str_len2) {
                                                    std::string elem_str2 = fdp.ConsumeBytesAsString(elem_str_len2);
                                                    elem2 = cJSON_CreateString(elem_str2.c_str());
                                                }
                                            }
                                            break;
                                    }
                                    
                                    if (elem2 != NULL) {
                                        cJSON_AddItemToArray(item2, elem2);
                                    }
                                }
                            }
                            break;
                        case 6:
                            item2 = cJSON_CreateObject();
                            if (item2 != NULL && fdp.remaining_bytes() > 0) {
                                int obj_size2 = fdp.ConsumeIntegralInRange<int>(0, 3);
                                for (int i = 0; i < obj_size2 && fdp.remaining_bytes() > 0; i++) {
                                    size_t key_len2 = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                                    if (fdp.remaining_bytes() >= key_len2) {
                                        std::string key2 = fdp.ConsumeBytesAsString(key_len2);
                                        
                                        uint8_t val_type2 = fdp.ConsumeIntegral<uint8_t>() % 4;
                                        cJSON* val2 = NULL;
                                        
                                        switch (val_type2) {
                                            case 0:
                                                val2 = cJSON_CreateNull();
                                                break;
                                            case 1:
                                                val2 = cJSON_CreateBool(fdp.ConsumeBool());
                                                break;
                                            case 2:
                                                val2 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                                                break;
                                            case 3:
                                                if (fdp.remaining_bytes() > 0) {
                                                    size_t val_str_len2 = fdp.ConsumeIntegralInRange<size_t>(1, 32);
                                                    if (fdp.remaining_bytes() >= val_str_len2) {
                                                        std::string val_str2 = fdp.ConsumeBytesAsString(val_str_len2);
                                                        val2 = cJSON_CreateString(val_str2.c_str());
                                                    }
                                                }
                                                break;
                                        }
                                        
                                        if (val2 != NULL) {
                                            cJSON_AddItemToObject(item2, key2.c_str(), val2);
                                        }
                                    }
                                }
                            }
                            break;
                    }
                }
            }
            break;
            
        case 1:
            // Test 2: Compare different type items
            if (fdp.remaining_bytes() > 2) {
                uint8_t type1 = fdp.ConsumeIntegral<uint8_t>() % 7;
                uint8_t type2 = fdp.ConsumeIntegral<uint8_t>() % 7;
                // Ensure they're different types for this scenario
                while (type2 == type1) {
                    type2 = (type2 + 1) % 7;
                }
                
                // Create first item
                switch (type1) {
                    case 0:
                        item1 = cJSON_CreateNull();
                        break;
                    case 1:
                        item1 = cJSON_CreateBool(fdp.ConsumeBool());
                        break;
                    case 2:
                        item1 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                        break;
                    case 3:
                        if (fdp.remaining_bytes() > 0) {
                            size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                            if (fdp.remaining_bytes() >= str_len) {
                                std::string str_val = fdp.ConsumeBytesAsString(str_len);
                                item1 = cJSON_CreateString(str_val.c_str());
                            }
                        }
                        break;
                    case 4:
                        if (fdp.remaining_bytes() > 0) {
                            size_t raw_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                            if (fdp.remaining_bytes() >= raw_len) {
                                std::string raw_val = fdp.ConsumeBytesAsString(raw_len);
                                item1 = cJSON_CreateRaw(raw_val.c_str());
                            }
                        }
                        break;
                    case 5:
                        item1 = cJSON_CreateArray();
                        break;
                    case 6:
                        item1 = cJSON_CreateObject();
                        break;
                }
                
                // Create second item of different type
                if (fdp.remaining_bytes() > 0) {
                    switch (type2) {
                        case 0:
                            item2 = cJSON_CreateNull();
                            break;
                        case 1:
                            item2 = cJSON_CreateBool(fdp.ConsumeBool());
                            break;
                        case 2:
                            item2 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                            break;
                        case 3:
                            if (fdp.remaining_bytes() > 0) {
                                size_t str_len2 = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                                if (fdp.remaining_bytes() >= str_len2) {
                                    std::string str_val2 = fdp.ConsumeBytesAsString(str_len2);
                                    item2 = cJSON_CreateString(str_val2.c_str());
                                }
                            }
                            break;
                        case 4:
                            if (fdp.remaining_bytes() > 0) {
                                size_t raw_len2 = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                                if (fdp.remaining_bytes() >= raw_len2) {
                                    std::string raw_val2 = fdp.ConsumeBytesAsString(raw_len2);
                                    item2 = cJSON_CreateRaw(raw_val2.c_str());
                                }
                            }
                            break;
                        case 5:
                            item2 = cJSON_CreateArray();
                            break;
                        case 6:
                            item2 = cJSON_CreateObject();
                            break;
                    }
                }
            }
            break;
            
        case 2:
            // Test 3: Compare null pointers (edge cases)
            // item1 and item2 remain NULL for this test
            break;
            
        case 3:
            // Test 4: Compare invalid JSON objects
            if (fdp.remaining_bytes() > 0) {
                // Create invalid cJSON structures
                cJSON invalid1[1];
                cJSON invalid2[1];
                memset(invalid1, 0, sizeof(invalid1));
                memset(invalid2, 0, sizeof(invalid2));
                // Set invalid type
                invalid1->type = 0xFF; // Invalid type
                invalid2->type = 0xFF;
                
                // Test comparison of invalid objects
                cJSON_bool result1 = cJSON_Compare(invalid1, invalid2, case_sensitive);
                cJSON_bool result2 = cJSON_Compare(invalid1, invalid2, !case_sensitive);
                
                // Clean up - no need to free stack-allocated objects
                return 0;
            }
            break;
            
        case 4:
            // Test 5: Compare numbers with special values
            if (fdp.remaining_bytes() > 0) {
                // Create numbers with potentially special values
                double num1, num2;
                
                uint8_t num_case = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (num_case) {
                    case 0:
                        // Same number
                        num1 = fdp.ConsumeFloatingPoint<double>();
                        num2 = num1;
                        break;
                    case 1:
                        // Different numbers
                        num1 = fdp.ConsumeFloatingPoint<double>();
                        num2 = fdp.ConsumeFloatingPoint<double>();
                        break;
                    case 2:
                        // Zero and near-zero
                        num1 = 0.0;
                        num2 = fdp.ConsumeFloatingPointInRange<double>(-1e-10, 1e-10);
                        break;
                    case 3:
                        // Large numbers
                        num1 = fdp.ConsumeFloatingPointInRange<double>(1e100, 1e200);
                        num2 = fdp.ConsumeFloatingPointInRange<double>(1e100, 1e200);
                        break;
                }
                item1 = cJSON_CreateNumber(num1);
                item2 = cJSON_CreateNumber(num2);
            }
            break;
            
        case 5:
            // Test 6: Compare strings with case sensitivity variations
            if (fdp.remaining_bytes() > 4) {
                size_t str_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                if (fdp.remaining_bytes() >= str_len * 2) {
                    std::string str1 = fdp.ConsumeBytesAsString(str_len);
                    std::string str2;
                    
                    uint8_t str_case = fdp.ConsumeIntegral<uint8_t>() % 3;
                    switch (str_case) {
                        case 0:
                            // Same string
                            str2 = str1;
                            break;
                        case 1:
                            // Different string
                            str2 = fdp.ConsumeBytesAsString(str_len);
                            break;
                        case 2:
                            // Case-different string (e.g., "Test" vs "test")
                            str2 = str1;
                            // Modify case of first character if possible
                            if (!str2.empty()) {
                                if (str2[0] >= 'a' && str2[0] <= 'z') {
                                    str2[0] = str2[0] - 'a' + 'A';
                                } else if (str2[0] >= 'A' && str2[0] <= 'Z') {
                                    str2[0] = str2[0] - 'A' + 'a';
                                }
                            }
                            break;
                    }
                    item1 = cJSON_CreateString(str1.c_str());
                    item2 = cJSON_CreateString(str2.c_str());
                }
            }
            break;
            
        case 6:
            // Test 7: Compare arrays
            if (fdp.remaining_bytes() > 2) {
                item1 = cJSON_CreateArray();
                if (item1 != NULL && fdp.remaining_bytes() > 0) {
                    int array_size = fdp.ConsumeIntegralInRange<int>(0, 3);
                    for (int i = 0; i < array_size && fdp.remaining_bytes() > 0; i++) {
                        uint8_t elem_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                        cJSON* elem = NULL;
                        
                        switch (elem_type) {
                            case 0:
                                elem = cJSON_CreateNull();
                                break;
                            case 1:
                                elem = cJSON_CreateBool(fdp.ConsumeBool());
                                break;
                            case 2:
                                elem = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                                break;
                            case 3:
                                if (fdp.remaining_bytes() > 0) {
                                    size_t elem_str_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                                    if (fdp.remaining_bytes() >= elem_str_len) {
                                        std::string elem_str = fdp.ConsumeBytesAsString(elem_str_len);
                                        elem = cJSON_CreateString(elem_str.c_str());
                                    }
                                }
                                break;
                        }
                        
                        if (elem != NULL) {
                            cJSON_AddItemToArray(item1, elem);
                        }
                    }
                }
                
                // Create second array (could be same or different)
                if (fdp.ConsumeBool() && item1 != NULL) {
                    // Same structure (deep copy)
                    item2 = cJSON_Duplicate(item1, 1);
                } else {
                    // Different array
                    item2 = cJSON_CreateArray();
                    if (item2 != NULL && fdp.remaining_bytes() > 0) {
                        int array_size2 = fdp.ConsumeIntegralInRange<int>(0, 3);
                        for (int i = 0; i < array_size2 && fdp.remaining_bytes() > 0; i++) {
                            uint8_t elem_type2 = fdp.ConsumeIntegral<uint8_t>() % 4;
                            cJSON* elem2 = NULL;
                            
                            switch (elem_type2) {
                                case 0:
                                    elem2 = cJSON_CreateNull();
                                    break;
                                case 1:
                                    elem2 = cJSON_CreateBool(fdp.ConsumeBool());
                                    break;
                                case 2:
                                    elem2 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                                    break;
                                case 3:
                                    if (fdp.remaining_bytes() > 0) {
                                        size_t elem_str_len2 = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                                        if (fdp.remaining_bytes() >= elem_str_len2) {
                                            std::string elem_str2 = fdp.ConsumeBytesAsString(elem_str_len2);
                                            elem2 = cJSON_CreateString(elem_str2.c_str());
                                        }
                                    }
                                    break;
                            }
                            
                            if (elem2 != NULL) {
                                cJSON_AddItemToArray(item2, elem2);
                            }
                        }
                    }
                }
            }
            break;
            
        case 7:
            // Test 8: Compare objects
            if (fdp.remaining_bytes() > 2) {
                item1 = cJSON_CreateObject();
                if (item1 != NULL && fdp.remaining_bytes() > 0) {
                    int obj_size = fdp.ConsumeIntegralInRange<int>(0, 3);
                    for (int i = 0; i < obj_size && fdp.remaining_bytes() > 0; i++) {
                        size_t key_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                        if (fdp.remaining_bytes() >= key_len) {
                            std::string key = fdp.ConsumeBytesAsString(key_len);
                            
                            uint8_t val_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                            cJSON* val = NULL;
                            
                            switch (val_type) {
                                case 0:
                                    val = cJSON_CreateNull();
                                    break;
                                case 1:
                                    val = cJSON_CreateBool(fdp.ConsumeBool());
                                    break;
                                case 2:
                                    val = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                                    break;
                                case 3:
                                    if (fdp.remaining_bytes() > 0) {
                                        size_t val_str_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                                        if (fdp.remaining_bytes() >= val_str_len) {
                                            std::string val_str = fdp.ConsumeBytesAsString(val_str_len);
                                            val = cJSON_CreateString(val_str.c_str());
                                        }
                                    }
                                    break;
                            }
                            
                            if (val != NULL) {
                                cJSON_AddItemToObject(item1, key.c_str(), val);
                            }
                        }
                    }
                }
                
                // Create second object (could be same or different)
                if (fdp.ConsumeBool() && item1 != NULL) {
                    // Same structure (deep copy)
                    item2 = cJSON_Duplicate(item1, 1);
                } else {
                    // Different object
                    item2 = cJSON_CreateObject();
                    if (item2 != NULL && fdp.remaining_bytes() > 0) {
                        int obj_size2 = fdp.ConsumeIntegralInRange<int>(0, 3);
                        for (int i = 0; i < obj_size2 && fdp.remaining_bytes() > 0; i++) {
                            size_t key_len2 = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                            if (fdp.remaining_bytes() >= key_len2) {
                                std::string key2 = fdp.ConsumeBytesAsString(key_len2);
                                
                                uint8_t val_type2 = fdp.ConsumeIntegral<uint8_t>() % 4;
                                cJSON* val2 = NULL;
                                
                                switch (val_type2) {
                                    case 0:
                                        val2 = cJSON_CreateNull();
                                        break;
                                    case 1:
                                        val2 = cJSON_CreateBool(fdp.ConsumeBool());
                                        break;
                                    case 2:
                                        val2 = cJSON_CreateNumber(fdp.ConsumeFloatingPoint<double>());
                                        break;
                                    case 3:
                                        if (fdp.remaining_bytes() > 0) {
                                            size_t val_str_len2 = fdp.ConsumeIntegralInRange<size_t>(1, 16);
                                            if (fdp.remaining_bytes() >= val_str_len2) {
                                                std::string val_str2 = fdp.ConsumeBytesAsString(val_str_len2);
                                                val2 = cJSON_CreateString(val_str2.c_str());
                                            }
                                        }
                                        break;
                                }
                                
                                if (val2 != NULL) {
                                    cJSON_AddItemToObject(item2, key2.c_str(), val2);
                                }
                            }
                        }
                    }
                }
            }
            break;
    }
    
    // Perform the comparison if we have valid items
    if (item1 != NULL || item2 != NULL) {
        cJSON_bool result = cJSON_Compare(item1, item2, case_sensitive);
        
        // Also test with opposite case sensitivity for string/raw types
        if ((item1 != NULL && item2 != NULL)) {
            cJSON_bool result2 = cJSON_Compare(item1, item2, !case_sensitive);
        }
    }
    
    // Clean up
    if (item1 != NULL) {
        cJSON_Delete(item1);
    }
    if (item2 != NULL) {
        cJSON_Delete(item2);
    }
    
    return 0;
}

} // namespace harness_006
