#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <cstring>
#include <climits>
#include <limits>
#include "pugixml.hpp"

namespace harness_006 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for XML content and various data types
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume control values first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool test_assignment_operators = fdp.ConsumeBool();
    bool test_set_methods = fdp.ConsumeBool();
    bool test_as_methods = fdp.ConsumeBool();
    bool test_string_view = fdp.ConsumeBool();
    bool test_long_long = fdp.ConsumeBool();
    // Consume various data values for xml_text operations
    double double_val = fdp.ConsumeFloatingPoint<double>();
    float float_val = fdp.ConsumeFloatingPoint<float>();
    bool bool_val = fdp.ConsumeBool();
    int int_val = fdp.ConsumeIntegral<int>();
    unsigned int uint_val = fdp.ConsumeIntegral<unsigned int>();
    long long_val = fdp.ConsumeIntegral<long>();
    unsigned long ulong_val = fdp.ConsumeIntegral<unsigned long>();
    // Consume string values
    std::string char_ptr_str = fdp.ConsumeRandomLengthString(256);
    std::string string_view_str = fdp.ConsumeRandomLengthString(256);
    
    // Consume long long values if enabled
    long long long_long_val = 0;
    unsigned long long ulong_long_val = 0;
    if (test_long_long) {
        long_long_val = fdp.ConsumeIntegral<long long>();
        ulong_long_val = fdp.ConsumeIntegral<unsigned long long>();
    }
    
    // Use remaining bytes as XML content
    std::vector<uint8_t> xml_content = fdp.ConsumeRemainingBytes<uint8_t>();
    if (xml_content.empty()) return 0;
    
    // Create XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(xml_content.data(), xml_content.size(), pugi::parse_default);
    
    // If parsing succeeded or we need to create new nodes
    pugi::xml_node text_node;
    
    if (result) {
        // Try to find existing text nodes
        pugi::xml_node root = doc.document_element();
        if (root) {
            // Find first text node
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                if (child.type() == pugi::node_pcdata || child.type() == pugi::node_cdata) {
                    text_node = child;
                    break;
                }
            }
        }
    }
    
    // If no text node found, create one
    if (!text_node) {
        pugi::xml_node root = doc.append_child("root");
        text_node = root.append_child(pugi::node_pcdata);
    }
    
    // Get xml_text object
    pugi::xml_text text_obj = text_node.text();
    
    // Test assignment operators if enabled
    if (test_assignment_operators && text_obj) {
        // Test operator=(double)
        text_obj = double_val;
        
        // Test operator=(float)
        text_obj = float_val;
        
        // Test operator=(bool)
        text_obj = bool_val;
        
        // Test operator=(int)
        text_obj = int_val;
        
        // Test operator=(unsigned int)
        text_obj = uint_val;
        
        // Test operator=(long)
        text_obj = long_val;
        
        // Test operator=(unsigned long)
        text_obj = ulong_val;
        // Test operator=(const char*)
        text_obj = char_ptr_str.c_str();
        
        // Test string_view operator if available and enabled
#ifdef PUGIXML_HAS_STRING_VIEW
        if (test_string_view) {
            std::string_view sv(string_view_str);
            text_obj = sv;
        }
#endif
        
        // Test long long operators if enabled and available
#ifdef PUGIXML_HAS_LONG_LONG
        if (test_long_long) {
            text_obj = long_long_val;
            text_obj = ulong_long_val;
        }
#endif
    }
    
    // Test set() methods if enabled
    if (test_set_methods && text_obj) {
        // Test set(const char*)
        text_obj.set(char_ptr_str.c_str());
        
        // Test set(const char*, size_t)
        size_t str_len = char_ptr_str.length();
        if (str_len > 0) {
            text_obj.set(char_ptr_str.c_str(), str_len / 2); // Test with partial length
            text_obj.set(char_ptr_str.c_str(), str_len); // Test with full length
        }
        
        // Test set(string_view) if available
#ifdef PUGIXML_HAS_STRING_VIEW
        if (test_string_view) {
            std::string_view sv(string_view_str);
            text_obj.set(sv);
        }
#endif
        
        // Test set with type conversion methods
        text_obj.set(int_val);
        text_obj.set(uint_val);
        text_obj.set(long_val);
        text_obj.set(ulong_val);
        text_obj.set(double_val);
        text_obj.set(double_val, 6); // Test with precision
        text_obj.set(float_val);
        text_obj.set(float_val, 4); // Test with precision
        text_obj.set(bool_val);
        
        // Test long long set methods if available
#ifdef PUGIXML_HAS_LONG_LONG
        if (test_long_long) {
            text_obj.set(long_long_val);
            text_obj.set(ulong_long_val);
        }
#endif
    }
    
    // Test as_*() methods if enabled
    if (test_as_methods && text_obj) {
        // Get default values for as_* methods
        int int_def = fdp.ConsumeIntegralInRange<int>(-1000, 1000);
        unsigned int uint_def = fdp.ConsumeIntegral<unsigned int>();
        double double_def = fdp.ConsumeFloatingPoint<double>();
        float float_def = fdp.ConsumeFloatingPoint<float>();
        bool bool_def = fdp.ConsumeBool();
        
        // Test as_string()
        const char* str_result = text_obj.as_string();
        const char* str_result_with_def = text_obj.as_string("default");
        
        // Test as_int()
        int int_result = text_obj.as_int();
        int int_result_with_def = text_obj.as_int(int_def);
        
        // Test as_uint()
        unsigned int uint_result = text_obj.as_uint();
        unsigned int uint_result_with_def = text_obj.as_uint(uint_def);
        
        // Test as_double()
        double double_result = text_obj.as_double();
        double double_result_with_def = text_obj.as_double(double_def);
        
        // Test as_float()
        float float_result = text_obj.as_float();
        float float_result_with_def = text_obj.as_float(float_def);
        
        // Test as_bool()
        bool bool_result = text_obj.as_bool();
        bool bool_result_with_def = text_obj.as_bool(bool_def);
        
        // Test long long as methods if available
#ifdef PUGIXML_HAS_LONG_LONG
        if (test_long_long) {
            long long llong_def = fdp.ConsumeIntegral<long long>();
            unsigned long long ullong_def = fdp.ConsumeIntegral<unsigned long long>();
            
            long long llong_result = text_obj.as_llong();
            long long llong_result_with_def = text_obj.as_llong(llong_def);
            
            unsigned long long ullong_result = text_obj.as_ullong();
            unsigned long long ullong_result_with_def = text_obj.as_ullong(ullong_def);
        }
#endif
        
        // Test get() method
        const char* get_result = text_obj.get();
        
        // Test empty() method
        bool is_empty = text_obj.empty();
    }
    
    // Test with different scenarios
    switch (test_scenario) {
        case 0:
            // Test with multiple text nodes
            if (text_node.parent()) {
                pugi::xml_node parent = text_node.parent();
                for (int i = 0; i < 3; i++) {
                    pugi::xml_node new_text = parent.append_child(pugi::node_pcdata);
                    pugi::xml_text new_text_obj = new_text.text();
                    if (new_text_obj) {
                        new_text_obj = double_val + i;
                        double verified = new_text_obj.as_double();
                    }
                }
            }
            break;
            
        case 1:
            // Test chained assignments
            if (text_obj) {
                text_obj = int_val;
                // Safe increment - check for overflow before adding 1
                int current_value = text_obj.as_int();
                if (current_value < INT_MAX) {
                    text_obj = current_value + 1;
                } else {
                    // If at INT_MAX, subtract 1 instead to avoid overflow
                    text_obj = current_value - 1;
                }
                text_obj = text_obj.as_string();
            }
            break;
            
        case 2:
            // Test with different text content types
            if (text_obj) {
                // Test with consumed strings for numeric conversion
                text_obj = char_ptr_str.c_str();
                int num_result = text_obj.as_int();
                double num_double_result = text_obj.as_double();
                
                // Test boolean conversion with consumed strings
                text_obj = string_view_str.c_str();
                bool bool_result = text_obj.as_bool();
                
                // Test with empty string (edge case)
                text_obj = "";
                bool_result = text_obj.as_bool();
                
                // Test with various string representations
                // Use first character of consumed string to create simple test strings
                if (!char_ptr_str.empty()) {
                    char first_char = char_ptr_str[0];
                    std::string single_char_str(1, first_char);
                    text_obj = single_char_str.c_str();
                    bool_result = text_obj.as_bool();
                }
            }
            break;
            
        case 3:
            // Test edge cases and error conditions
            if (text_obj) {
                // Test with empty string
                text_obj = "";
                bool empty_check = text_obj.empty();
                const char* empty_str = text_obj.get();
                
                // Test with long string using consumed data
                // Create a longer version of consumed string for edge case testing
                std::string long_str;
                if (char_ptr_str.length() > 0) {
                    // Repeat the consumed string to create a longer test string
                    size_t repeat_count = fdp.ConsumeIntegralInRange<size_t>(1, 10);
                    for (size_t i = 0; i < repeat_count; i++) {
                        long_str += char_ptr_str;
                    }
                    if (long_str.length() > 0) {
                        text_obj.set(long_str.c_str(), long_str.length());
                    }
                }
                
                // Test with null pointer (should be handled)
                text_obj.set(nullptr, 0);
                
                // Test get() on potentially empty text
                const char* text_content = text_obj.get();
                
                // Test with string that contains only whitespace
                text_obj = "   ";
                const char* whitespace_result = text_obj.get();
                int whitespace_int = text_obj.as_int();
                bool whitespace_bool = text_obj.as_bool();
            }
            break;
    }
    
    // Clean up document
    doc.reset();
    
    return 0;
}

} // namespace harness_006
