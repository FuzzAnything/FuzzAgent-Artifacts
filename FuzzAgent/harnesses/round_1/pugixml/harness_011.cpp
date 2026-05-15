#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <cstring>

namespace harness_011 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for text and attribute operations
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== PHASE 1: DOCUMENT SETUP ====================
    // Create XML document with structure for text and attribute testing
    pugi::xml_document doc;
    
    // Create root element
    pugi::xml_node root = doc.append_child("root");
    
    // Create test nodes for text manipulation
    pugi::xml_node text_test_node = root.append_child("text_test");
    pugi::xml_node attr_test_node = root.append_child("attr_test");
    pugi::xml_node mixed_test_node = root.append_child("mixed_test");
    
    // Create initial attributes for testing
    attr_test_node.append_attribute("int_attr") = "42";
    attr_test_node.append_attribute("float_attr") = "3.14";
    attr_test_node.append_attribute("bool_attr") = "true";
    attr_test_node.append_attribute("string_attr") = "initial_value";
    
    mixed_test_node.append_attribute("test_attr1") = "100";
    mixed_test_node.append_attribute("test_attr2") = "2.718";
    
    // ==================== PHASE 2: TEXT VALUE MANIPULATION TESTING ====================
    // TARGET: xml_text type-safe value manipulation functions (15+ APIs, 200+ branches)
    
    // Get text object for manipulation
    pugi::xml_text text_obj = text_test_node.text();
    pugi::xml_text mixed_text_obj = mixed_test_node.text();
    
    // Consume data for text operations
    while (fdp.remaining_bytes() > 16) {
        uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 20;
        
        switch (operation) {
            case 0: { // Test as_int() with default value
                int default_val = fdp.ConsumeIntegral<int>();
                int result = text_obj.as_int(default_val);
                (void)result; // Use result to avoid compiler warnings
                break;
            }
            case 1: { // Test as_uint() with default value
                unsigned int default_val = fdp.ConsumeIntegral<unsigned int>();
                unsigned int result = text_obj.as_uint(default_val);
                (void)result;
                break;
            }
            case 2: { // Test as_double() with default value
                double default_val = fdp.ConsumeFloatingPoint<double>();
                double result = text_obj.as_double(default_val);
                (void)result;
                break;
            }
            case 3: { // Test as_float() with default value
                float default_val = fdp.ConsumeFloatingPoint<float>();
                float result = text_obj.as_float(default_val);
                (void)result;
                break;
            }
            case 4: { // Test as_bool() with default value
                bool default_val = fdp.ConsumeBool();
                bool result = text_obj.as_bool(default_val);
                (void)result;
                break;
            }
            case 5: { // Test as_string() with default value
                std::string default_val = fdp.ConsumeRandomLengthString(50);
                const char* result = text_obj.as_string(default_val.c_str());
                (void)result;
                break;
            }
            case 6: { // Test get() method
                const char* result = text_obj.get();
                (void)result;
                break;
            }
            case 7: { // Test empty() method
                bool is_empty = text_obj.empty();
                (void)is_empty;
                break;
            }
            case 8: { // Test set() with string
                std::string str_val = fdp.ConsumeRandomLengthString(100);
                bool success = text_obj.set(str_val.c_str());
                (void)success;
                break;
            }
            case 9: { // Test set() with string and size
                std::string str_val = fdp.ConsumeRandomLengthString(100);
                bool success = text_obj.set(str_val.c_str(), str_val.size());
                (void)success;
                break;
            }
            case 10: { // Test set() with int
                int int_val = fdp.ConsumeIntegral<int>();
                bool success = text_obj.set(int_val);
                (void)success;
                break;
            }
            case 11: { // Test set() with unsigned int
                unsigned int uint_val = fdp.ConsumeIntegral<unsigned int>();
                bool success = text_obj.set(uint_val);
                (void)success;
                break;
            }
            case 12: { // Test set() with long
                long long_val = fdp.ConsumeIntegral<long>();
                bool success = text_obj.set(long_val);
                (void)success;
                break;
            }
            case 13: { // Test set() with unsigned long
                unsigned long ulong_val = fdp.ConsumeIntegral<unsigned long>();
                bool success = text_obj.set(ulong_val);
                (void)success;
                break;
            }
            case 14: { // Test set() with double
                double double_val = fdp.ConsumeFloatingPoint<double>();
                bool success = text_obj.set(double_val);
                (void)success;
                break;
            }
            case 15: { // Test set() with double and precision
                double double_val = fdp.ConsumeFloatingPoint<double>();
                int precision = fdp.ConsumeIntegralInRange<int>(0, 10);
                bool success = text_obj.set(double_val, precision);
                (void)success;
                break;
            }
            case 16: { // Test set() with float
                float float_val = fdp.ConsumeFloatingPoint<float>();
                bool success = text_obj.set(float_val);
                (void)success;
                break;
            }
            case 17: { // Test set() with float and precision
                float float_val = fdp.ConsumeFloatingPoint<float>();
                int precision = fdp.ConsumeIntegralInRange<int>(0, 10);
                bool success = text_obj.set(float_val, precision);
                (void)success;
                break;
            }
            case 18: { // Test set() with bool
                bool bool_val = fdp.ConsumeBool();
                bool success = text_obj.set(bool_val);
                (void)success;
                break;
            }
            case 19: { // Test operator= overloads
                uint8_t assign_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                switch (assign_type) {
                    case 0: {
                        std::string str_val = fdp.ConsumeRandomLengthString(50);
                        mixed_text_obj = str_val.c_str();
                        break;
                    }
                    case 1: {
                        int int_val = fdp.ConsumeIntegral<int>();
                        mixed_text_obj = int_val;
                        break;
                    }
                    case 2: {
                        unsigned int uint_val = fdp.ConsumeIntegral<unsigned int>();
                        mixed_text_obj = uint_val;
                        break;
                    }
                    case 3: {
                        long long_val = fdp.ConsumeIntegral<long>();
                        mixed_text_obj = long_val;
                        break;
                    }
                    case 4: {
                        unsigned long ulong_val = fdp.ConsumeIntegral<unsigned long>();
                        mixed_text_obj = ulong_val;
                        break;
                    }
                    case 5: {
                        double double_val = fdp.ConsumeFloatingPoint<double>();
                        mixed_text_obj = double_val;
                        break;
                    }
                    case 6: {
                        float float_val = fdp.ConsumeFloatingPoint<float>();
                        mixed_text_obj = float_val;
                        break;
                    }
                    case 7: {
                        bool bool_val = fdp.ConsumeBool();
                        mixed_text_obj = bool_val;
                        break;
                    }
                }
                break;
            }
        }
        
        // After setting text, test reading it back with different type conversions
        if (!text_obj.empty()) {
            // Test all type conversions on the same text
            text_obj.as_int(0);
            text_obj.as_uint(0);
            text_obj.as_double(0.0);
            text_obj.as_float(0.0f);
            text_obj.as_bool(false);
            text_obj.as_string("");
        }
    }
    
    // ==================== PHASE 3: ATTRIBUTE VALUE MANIPULATION TESTING ====================
    // Test attribute value getting/setting with type conversions
    
    // Get attributes for testing
    pugi::xml_attribute int_attr = attr_test_node.attribute("int_attr");
    pugi::xml_attribute float_attr = attr_test_node.attribute("float_attr");
    pugi::xml_attribute bool_attr = attr_test_node.attribute("bool_attr");
    pugi::xml_attribute string_attr = attr_test_node.attribute("string_attr");
    
    // Test attribute as_* methods
    if (int_attr) {
        int int_val = int_attr.as_int(0);
        unsigned int uint_val = int_attr.as_uint(0);
        double double_val = int_attr.as_double(0.0);
        float float_val = int_attr.as_float(0.0f);
        bool bool_val = int_attr.as_bool(false);
        const char* str_val = int_attr.as_string("");
        (void)int_val; (void)uint_val; (void)double_val; (void)float_val; (void)bool_val; (void)str_val;
        
        // Test setting attribute with different types
        int_attr.set_value(fdp.ConsumeIntegral<int>());
        int_attr = fdp.ConsumeIntegral<unsigned int>(); // operator= overload
    }
    
    if (float_attr) {
        float_attr.as_double(0.0);
        float_attr.as_float(0.0f);
        
        // Test setting with floating point values
        float_attr.set_value(fdp.ConsumeFloatingPoint<double>());
        float_attr = fdp.ConsumeFloatingPoint<float>(); // operator= overload
    }
    
    if (bool_attr) {
        bool_attr.as_bool(false);
        
        // Test setting with boolean
        bool_attr.set_value(fdp.ConsumeBool());
        bool_attr = fdp.ConsumeBool(); // operator= overload
    }
    
    if (string_attr) {
        string_attr.as_string("");
        
        // Test setting with string
        std::string new_str = fdp.ConsumeRandomLengthString(100);
        string_attr.set_value(new_str.c_str());
        string_attr = new_str.c_str(); // operator= overload
    }
    
    // ==================== PHASE 4: MEMORY MANAGEMENT AND EDGE CASES ====================
    // Test memory management for text data and edge cases
    
    // Create additional nodes with various text content for edge case testing
    pugi::xml_node edge_cases = root.append_child("edge_cases");
    
    // Test with empty text
    pugi::xml_text empty_text = edge_cases.text();
    empty_text.as_int(999);
    empty_text.as_uint(999);
    empty_text.as_double(999.0);
    empty_text.as_float(999.0f);
    empty_text.as_bool(true);
    empty_text.as_string("default");
    
    // Test with numeric text
    pugi::xml_node numeric_node = edge_cases.append_child("numeric");
    numeric_node.text() = "12345";
    pugi::xml_text numeric_text = numeric_node.text();
    numeric_text.as_int(0);
    numeric_text.as_uint(0);
    numeric_text.as_double(0.0);
    numeric_text.as_float(0.0f);
    numeric_text.as_bool(false);
    
    // Test with floating point text
    pugi::xml_node float_node = edge_cases.append_child("float");
    float_node.text() = "3.1415926535";
    pugi::xml_text float_text = float_node.text();
    float_text.as_int(0);
    float_text.as_double(0.0);
    float_text.as_float(0.0f);
    
    // Test with boolean-like text
    pugi::xml_node bool_node = edge_cases.append_child("bool");
    bool_node.text() = "true";
    pugi::xml_text bool_text = bool_node.text();
    bool_text.as_bool(false);
    bool_text.as_int(0);
    
    // Test with invalid number text
    pugi::xml_node invalid_node = edge_cases.append_child("invalid");
    invalid_node.text() = "not_a_number";
    pugi::xml_text invalid_text = invalid_node.text();
    invalid_text.as_int(999);
    invalid_text.as_double(999.0);
    invalid_text.as_float(999.0f);
    
    // Test string_view support if available
#ifdef PUGIXML_HAS_STRING_VIEW
    if (fdp.remaining_bytes() > 10) {
        std::string sv_content = fdp.ConsumeRandomLengthString(50);
        pugi::string_view_t sv(sv_content.data(), sv_content.size());
        
        pugi::xml_node sv_node = root.append_child("string_view_test");
        sv_node.text().set(sv);  // Test set() with string_view
        sv_node.text() = sv;     // Test operator= with string_view
    }
#endif
    
    // Test long long support if available
#ifdef PUGIXML_HAS_LONG_LONG
    if (fdp.remaining_bytes() > 10) {
        pugi::xml_node ll_node = root.append_child("long_long_test");
        pugi::xml_text ll_text = ll_node.text();
        
        long long ll_val = fdp.ConsumeIntegral<long long>();
        unsigned long long ull_val = fdp.ConsumeIntegral<unsigned long long>();
        
        // Test set() methods
        ll_text.set(ll_val);
        ll_text.set(ull_val);
        
        // Test operator= overloads
        ll_text = ll_val;
        ll_text = ull_val;
        
        // Test as_* methods
        ll_text.as_llong(0LL);
        ll_text.as_ullong(0ULL);
    }
#endif
    
    // ==================== PHASE 5: CLEANUP AND MEMORY VALIDATION ====================
    // The document will be automatically cleaned up when it goes out of scope
    // This tests that memory is properly managed for text and attribute values
    
    return 0;
}

} // namespace harness_011
