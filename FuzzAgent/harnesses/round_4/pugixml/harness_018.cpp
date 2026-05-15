#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <string_view>

namespace harness_018 {

// Conditionally include string_view based on pugixml configuration
#ifdef PUGIXML_HAS_STRING_VIEW
#endif

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for comprehensive xml_text manipulation testing
    // Need enough data for XML content, multiple data types for set operations
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first for operation parameters
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 4;  // Which xml_text API to test
    uint8_t num_nodes = fdp.ConsumeIntegralInRange<uint8_t>(1, 8);  // Number of text nodes to test
    bool use_precision = fdp.ConsumeBool();  // Whether to use precision parameter for float/double set
    size_t max_string_view_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);  // Max size for string_view
    
    // Consume numeric values for different data types
    int int_val = fdp.ConsumeIntegral<int>();
    unsigned int uint_val = fdp.ConsumeIntegral<unsigned int>();
    long long_val = fdp.ConsumeIntegral<long>();
    unsigned long ulong_val = fdp.ConsumeIntegral<unsigned long>();
    long long llong_val = fdp.ConsumeIntegral<long long>();
    unsigned long long ullong_val = fdp.ConsumeIntegral<unsigned long long>();
    float float_val = fdp.ConsumeFloatingPoint<float>();
    double double_val = fdp.ConsumeFloatingPoint<double>();
    bool bool_val = fdp.ConsumeBool();
    int precision = fdp.ConsumeIntegralInRange<int>(-10, 20);  // Precision for float/double set
    
    // Consume variable-size data for XML content and strings
    std::string xml_content = fdp.ConsumeRandomLengthString(2048);
    std::string root_name = fdp.ConsumeRandomLengthString(64);
    std::string node_prefix = fdp.ConsumeRandomLengthString(32);
    std::string string_val = fdp.ConsumeRandomLengthString(512);
    std::string cstring_val = fdp.ConsumeRandomLengthString(256);
    
    // Create string_view if enabled and supported
    #ifdef PUGIXML_HAS_STRING_VIEW
    bool test_string_view = fdp.ConsumeBool();  // Whether to test string_view variants
    std::string_view string_view_val;
    if (test_string_view && !string_val.empty()) {
        size_t sv_size = std::min(string_val.size(), max_string_view_size);
        string_view_val = std::string_view(string_val.c_str(), sv_size);
    }
    #endif
    
    // Step 1: Parse XML document using load_string as specified in guidance
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_string(xml_content.c_str(), parse_options);
    
    // Get or create document element for testing
    pugi::xml_node root = doc.document_element();
    if (!root) {
        // Create a root node if document is empty or parsing failed
        root = doc.append_child(pugi::node_element);
        if (root_name.empty()) {
            root_name = "root";  // Default fallback
        }
        root.set_name(root_name.c_str());
    }
    
    // Create text nodes for testing xml_text operations
    std::vector<pugi::xml_text> text_objects;
    for (int i = 0; i < num_nodes && i < 8; i++) {
        std::string node_name;
        if (node_prefix.empty()) {
            node_name = "node_" + std::to_string(i);  // Default fallback
        } else {
            node_name = node_prefix + "_" + std::to_string(i);
        }
        pugi::xml_node child = root.append_child(node_name.c_str());
        if (child) {
            text_objects.push_back(child.text());
        }
    }
    
    // If we have text objects to work with, test xml_text APIs
    if (!text_objects.empty()) {
        pugi::xml_text& text_obj = text_objects[0];
        
        // Test different xml_text APIs based on operation_type
        switch (operation_type) {
            case 0: {
                // Test set() methods with various data types
                
                // xml_text::set(float, int) - with precision
                if (use_precision) {
                    text_obj.set(float_val, precision);
                } else {
                    text_obj.set(float_val);
                }
                
                // xml_text::set(long long)
                if (text_objects.size() > 1) {
                    text_objects[1].set(llong_val);
                }
                
                // xml_text::set(unsigned long long)
                if (text_objects.size() > 2) {
                    text_objects[2].set(ullong_val);
                }
                
                // xml_text::set(string_view) - conditional on PUGIXML_HAS_STRING_VIEW
                #ifdef PUGIXML_HAS_STRING_VIEW
                if (test_string_view && !string_view_val.empty()) {
                    if (text_objects.size() > 3) {
                        text_objects[3].set(string_view_val);
                    }
                }
                #endif
                
                // xml_text::set(double, int) - with precision
                if (text_objects.size() > 4) {
                    if (use_precision) {
                        text_objects[4].set(double_val, precision);
                    } else {
                        text_objects[4].set(double_val);
                    }
                }
                
                // Also test other set() variants
                if (text_objects.size() > 5) {
                    text_objects[5].set(int_val);
                }
                if (text_objects.size() > 6) {
                    text_objects[6].set(uint_val);
                }
                if (text_objects.size() > 7) {
                    text_objects[7].set(bool_val);
                }
                break;
            }
            
            case 1: {
                // Test operator= assignment methods
                
                // xml_text::operator=(string_view) - conditional
                #ifdef PUGIXML_HAS_STRING_VIEW
                if (test_string_view && !string_view_val.empty()) {
                    text_obj = string_view_val;
                }
                #endif
                
                // xml_text::operator=(float)
                if (text_objects.size() > 1) {
                    text_objects[1] = float_val;
                }
                
                // xml_text::operator=(double)
                if (text_objects.size() > 2) {
                    text_objects[2] = double_val;
                }
                
                // xml_text::operator=(long)
                if (text_objects.size() > 3) {
                    text_objects[3] = long_val;
                }
                
                // xml_text::operator=(unsigned int)
                if (text_objects.size() > 4) {
                    text_objects[4] = uint_val;
                }
                
                // xml_text::operator=(unsigned long)
                if (text_objects.size() > 5) {
                    text_objects[5] = ulong_val;
                }
                
                // xml_text::operator=(bool)
                if (text_objects.size() > 6) {
                    text_objects[6] = bool_val;
                }
                
                // xml_text::operator=(int)
                if (text_objects.size() > 7) {
                    text_objects[7] = int_val;
                }
                break;
            }
            
            case 2: {
                // Test remaining operator= methods
                
                // xml_text::operator=(const char*)
                if (!cstring_val.empty()) {
                    text_obj = cstring_val.c_str();
                }
                
                // xml_text::operator=(long long)
                if (text_objects.size() > 1) {
                    text_objects[1] = llong_val;
                }
                
                // xml_text::operator=(unsigned long long)
                if (text_objects.size() > 2) {
                    text_objects[2] = ullong_val;
                }
                
                // Test all set() methods on different text objects
                for (size_t i = 3; i < text_objects.size() && i < 8; i++) {
                    switch (i % 7) {
                        case 0: text_objects[i].set(int_val); break;
                        case 1: text_objects[i].set(uint_val); break;
                        case 2: text_objects[i].set(long_val); break;
                        case 3: text_objects[i].set(ulong_val); break;
                        case 4: text_objects[i].set(double_val); break;
                        case 5: text_objects[i].set(float_val); break;
                        case 6: text_objects[i].set(bool_val); break;
                    }
                }
                break;
            }
            
            case 3: {
                // Comprehensive testing: mix of set() and operator= calls
                // Test all APIs across multiple text objects
                
                size_t obj_idx = 0;
                
                // Test set() methods
                if (obj_idx < text_objects.size()) {
                    text_objects[obj_idx++].set(float_val, use_precision ? precision : 0);
                }
                if (obj_idx < text_objects.size()) {
                    text_objects[obj_idx++].set(llong_val);
                }
                if (obj_idx < text_objects.size()) {
                    text_objects[obj_idx++].set(ullong_val);
                }
                #ifdef PUGIXML_HAS_STRING_VIEW
                if (obj_idx < text_objects.size() && test_string_view && !string_view_val.empty()) {
                    text_objects[obj_idx++].set(string_view_val);
                }
                #endif
                if (obj_idx < text_objects.size()) {
                    text_objects[obj_idx++].set(double_val, use_precision ? precision : 0);
                }
                
                // Test operator= methods
                #ifdef PUGIXML_HAS_STRING_VIEW
                if (obj_idx < text_objects.size() && test_string_view && !string_view_val.empty()) {
                    text_objects[obj_idx++] = string_view_val;
                }
                #endif
                if (obj_idx < text_objects.size()) {
                    text_objects[obj_idx++] = float_val;
                }
                if (obj_idx < text_objects.size()) {
                    text_objects[obj_idx++] = double_val;
                }
                if (obj_idx < text_objects.size()) {
                    text_objects[obj_idx++] = long_val;
                }
                if (obj_idx < text_objects.size()) {
                    text_objects[obj_idx++] = uint_val;
                }
                if (obj_idx < text_objects.size()) {
                    text_objects[obj_idx++] = ulong_val;
                }
                if (obj_idx < text_objects.size()) {
                    text_objects[obj_idx++] = bool_val;
                }
                if (obj_idx < text_objects.size()) {
                    text_objects[obj_idx++] = int_val;
                }
                if (obj_idx < text_objects.size() && !cstring_val.empty()) {
                    text_objects[obj_idx++] = cstring_val.c_str();
                }
                if (obj_idx < text_objects.size()) {
                    text_objects[obj_idx++] = llong_val;
                }
                if (obj_idx < text_objects.size()) {
                    text_objects[obj_idx++] = ullong_val;
                }
                break;
            }
        }
        
        // Step 5: Verify results using xml_text getter functions (as specified in guidance)
        for (size_t i = 0; i < text_objects.size() && i < 4; i++) {
            // Test various getter functions to verify the set values
            (void)text_objects[i].get();  // Get raw text
            (void)text_objects[i].as_string();  // Get as string
            (void)text_objects[i].as_int();  // Get as int
            (void)text_objects[i].as_uint();  // Get as unsigned int
            (void)text_objects[i].as_double();  // Get as double
            (void)text_objects[i].as_float();  // Get as float
            (void)text_objects[i].as_bool();  // Get as bool
            
            // Test long long getters if available
            #ifdef PUGIXML_HAS_LONG_LONG
            (void)text_objects[i].as_llong();
            (void)text_objects[i].as_ullong();
            #endif
        }
    }
    
    // Step 6: Clean up with reset as specified in guidance
    doc.reset();
    
    return 0;
}

} // namespace harness_018
