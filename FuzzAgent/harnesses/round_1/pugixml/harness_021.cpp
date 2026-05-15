#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace harness_021 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for string operations
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== PHASE 1: DOCUMENT SETUP ====================
    // Create XML document specifically for string_view API testing
    pugi::xml_document doc;
    
    // Create root element
    pugi::xml_node root = doc.append_child("string_view_test_root");
    
    // Create specialized test nodes
    pugi::xml_node attr_test_node = root.append_child("attribute_string_view_tests");
    pugi::xml_node name_value_test_node = root.append_child("name_value_string_view_tests");
    pugi::xml_node edge_case_node = root.append_child("edge_case_string_view_tests");
    pugi::xml_node operator_test_node = root.append_child("operator_string_view_tests");
    
    // Check if string_view is available in this build
#ifdef PUGIXML_HAS_STRING_VIEW
    // ==================== PHASE 2: STRING_VIEW ATTRIBUTE OPERATIONS ====================
    // TARGET 1: xml_node::append_attribute(string_view_t)
    // TARGET 2: xml_node::prepend_attribute(string_view_t)
    
    while (fdp.remaining_bytes() > 64) {
        uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 12;
        
        switch (operation) {
            case 0: { // Test append_attribute with string_view
                // Consume attribute name from fuzzer input
                size_t attr_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                std::string attr_name_str = fdp.ConsumeBytesAsString(attr_name_len);
                pugi::string_view_t sv_attr_name(attr_name_str.data(), attr_name_str.size());
                
                // Append attribute using string_view
                pugi::xml_attribute appended_attr = attr_test_node.append_attribute(sv_attr_name);
                
                if (appended_attr) {
                    // Set value using fuzzer input
                    size_t attr_value_len = fdp.ConsumeIntegralInRange<size_t>(0, 100);
                    std::string attr_value_str = fdp.ConsumeBytesAsString(attr_value_len);
                    pugi::string_view_t sv_attr_value(attr_value_str.data(), attr_value_str.size());
                    appended_attr.set_value(sv_attr_value);
                }
                break;
            }
            
            case 1: { // Test prepend_attribute with string_view
                // Consume attribute name from fuzzer input
                size_t attr_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                std::string attr_name_str = fdp.ConsumeBytesAsString(attr_name_len);
                pugi::string_view_t sv_attr_name(attr_name_str.data(), attr_name_str.size());
                
                // Prepend attribute using string_view
                pugi::xml_attribute prepended_attr = attr_test_node.prepend_attribute(sv_attr_name);
                
                if (prepended_attr) {
                    // Set value using fuzzer input
                    size_t attr_value_len = fdp.ConsumeIntegralInRange<size_t>(0, 100);
                    std::string attr_value_str = fdp.ConsumeBytesAsString(attr_value_len);
                    pugi::string_view_t sv_attr_value(attr_value_str.data(), attr_value_str.size());
                    prepended_attr.set_value(sv_attr_value);
                }
                break;
            }
            
            case 2: { // Test insert_attribute_before with string_view
                // First create a base attribute
                pugi::xml_attribute base_attr = attr_test_node.append_attribute("base_attr");
                
                if (base_attr) {
                    // Consume attribute name from fuzzer input
                    size_t attr_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                    std::string attr_name_str = fdp.ConsumeBytesAsString(attr_name_len);
                    pugi::string_view_t sv_attr_name(attr_name_str.data(), attr_name_str.size());
                    
                    // Insert attribute before using string_view
                    pugi::xml_attribute inserted_before = attr_test_node.insert_attribute_before(sv_attr_name, base_attr);
                    
                    if (inserted_before) {
                        // Set value using fuzzer input
                        size_t attr_value_len = fdp.ConsumeIntegralInRange<size_t>(0, 100);
                        std::string attr_value_str = fdp.ConsumeBytesAsString(attr_value_len);
                        pugi::string_view_t sv_attr_value(attr_value_str.data(), attr_value_str.size());
                        inserted_before.set_value(sv_attr_value);
                    }
                }
                break;
            }
            
            case 3: { // Test insert_attribute_after with string_view
                // First create a base attribute
                pugi::xml_attribute base_attr = attr_test_node.append_attribute("base_attr2");
                
                if (base_attr) {
                    // Consume attribute name from fuzzer input
                    size_t attr_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                    std::string attr_name_str = fdp.ConsumeBytesAsString(attr_name_len);
                    pugi::string_view_t sv_attr_name(attr_name_str.data(), attr_name_str.size());
                    
                    // Insert attribute after using string_view
                    pugi::xml_attribute inserted_after = attr_test_node.insert_attribute_after(sv_attr_name, base_attr);
                    
                    if (inserted_after) {
                        // Set value using fuzzer input
                        size_t attr_value_len = fdp.ConsumeIntegralInRange<size_t>(0, 100);
                        std::string attr_value_str = fdp.ConsumeBytesAsString(attr_value_len);
                        pugi::string_view_t sv_attr_value(attr_value_str.data(), attr_value_str.size());
                        inserted_after.set_value(sv_attr_value);
                    }
                }
                break;
            }
            
            case 4: { // TARGET 3: xml_node::set_name(string_view_t)
                // Create a test node
                pugi::xml_node rename_node = name_value_test_node.append_child("original_name");
                
                if (rename_node) {
                    // Consume new name from fuzzer input
                    size_t new_name_len = fdp.ConsumeIntegralInRange<size_t>(0, 100);
                    std::string new_name_str = fdp.ConsumeBytesAsString(new_name_len);
                    
                    if (!new_name_str.empty()) {
                        pugi::string_view_t sv_new_name(new_name_str.data(), new_name_str.size());
                        bool rename_result = rename_node.set_name(sv_new_name);
                        (void)rename_result; // Use to avoid compiler warnings
                    }
                }
                break;
            }
            
            case 5: { // TARGET 4: xml_node::set_value(string_view_t)
                // Create a test node
                pugi::xml_node value_node = name_value_test_node.append_child("value_test_node");
                
                if (value_node) {
                    // Consume new value from fuzzer input
                    size_t new_value_len = fdp.ConsumeIntegralInRange<size_t>(0, 200);
                    std::string new_value_str = fdp.ConsumeBytesAsString(new_value_len);
                    
                    pugi::string_view_t sv_new_value(new_value_str.data(), new_value_str.size());
                    bool set_value_result = value_node.set_value(sv_new_value);
                    (void)set_value_result; // Use to avoid compiler warnings
                }
                break;
            }
            
            case 6: { // TARGET 5: xml_attribute::operator=(string_view_t)
                // Create a test attribute
                pugi::xml_attribute operator_test_attr = operator_test_node.append_attribute("test_attr");
                
                if (operator_test_attr) {
                    // Consume value from fuzzer input
                    size_t attr_value_len = fdp.ConsumeIntegralInRange<size_t>(0, 150);
                    std::string attr_value_str = fdp.ConsumeBytesAsString(attr_value_len);
                    
                    pugi::string_view_t sv_attr_value(attr_value_str.data(), attr_value_str.size());
                    operator_test_attr = sv_attr_value; // Use operator= overload
                }
                break;
            }
            
            case 7: { // Test xml_attribute::set_name(string_view_t)
                // Create a test attribute
                pugi::xml_attribute rename_attr_test = operator_test_node.append_attribute("original_attr_name");
                
                if (rename_attr_test) {
                    // Consume new name from fuzzer input
                    size_t new_attr_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 50);
                    std::string new_attr_name_str = fdp.ConsumeBytesAsString(new_attr_name_len);
                    
                    pugi::string_view_t sv_new_attr_name(new_attr_name_str.data(), new_attr_name_str.size());
                    bool rename_attr_result = rename_attr_test.set_name(sv_new_attr_name);
                    (void)rename_attr_result; // Use to avoid compiler warnings
                }
                break;
            }
            
            case 8: { // Test xml_attribute::set_value(string_view_t)
                // Create a test attribute
                pugi::xml_attribute set_value_attr_test = operator_test_node.append_attribute("attr_for_set_value");
                
                if (set_value_attr_test) {
                    // Consume value from fuzzer input
                    size_t attr_value_len = fdp.ConsumeIntegralInRange<size_t>(0, 150);
                    std::string attr_value_str = fdp.ConsumeBytesAsString(attr_value_len);
                    
                    pugi::string_view_t sv_attr_value(attr_value_str.data(), attr_value_str.size());
                    bool set_value_result = set_value_attr_test.set_value(sv_attr_value);
                    (void)set_value_result; // Use to avoid compiler warnings
                }
                break;
            }
            
            case 9: { // Test edge cases: empty string_view
                // Test with empty string_view for name
                pugi::string_view_t sv_empty_name("", 0);
                pugi::xml_attribute empty_name_attr = edge_case_node.append_attribute(sv_empty_name);
                
                if (empty_name_attr) {
                    // Try to set value
                    pugi::string_view_t sv_empty_value("", 0);
                    empty_name_attr.set_value(sv_empty_value);
                }
                
                // Test set_name with empty string_view
                pugi::xml_node empty_name_node = edge_case_node.append_child("test_node");
                if (empty_name_node) {
                    empty_name_node.set_name(sv_empty_name);
                }
                break;
            }
            
            case 10: { // Test edge cases: string_view with null characters
                // Create string with null characters
                size_t data_len = fdp.ConsumeIntegralInRange<size_t>(5, 50);
                std::string data_with_nulls = fdp.ConsumeBytesAsString(data_len);
                
                // Insert null characters at various positions
                if (data_with_nulls.size() > 2) {
                    data_with_nulls[1] = '\0';
                    data_with_nulls[data_with_nulls.size()/2] = '\0';
                }
                
                pugi::string_view_t sv_with_nulls(data_with_nulls.data(), data_with_nulls.size());
                
                // Try to use it as attribute name
                pugi::xml_attribute null_char_attr = edge_case_node.append_attribute(sv_with_nulls);
                
                if (null_char_attr) {
                    // Try to set value with null chars
                    null_char_attr.set_value(sv_with_nulls);
                }
                break;
            }
            
            case 11: { // Test string_view interoperability with other APIs
                // Create string_view from fuzzer input
                size_t sv_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                std::string sv_str = fdp.ConsumeBytesAsString(sv_len);
                pugi::string_view_t sv_test(sv_str.data(), sv_str.size());
                
                // Test multiple APIs with the same string_view
                pugi::xml_node interop_node = edge_case_node.append_child(sv_test);
                
                if (interop_node) {
                    // Use same string_view for various operations
                    interop_node.set_name(sv_test);
                    interop_node.set_value(sv_test);
                    
                    pugi::xml_attribute interop_attr = interop_node.append_attribute(sv_test);
                    if (interop_attr) {
                        interop_attr.set_name(sv_test);
                        interop_attr.set_value(sv_test);
                        interop_attr = sv_test; // operator=
                    }
                }
                break;
            }
        }
    }
    
    // ==================== PHASE 3: VALIDATION AND CLEANUP ====================
    // Verify document structure is still valid after all operations
    // Simple traversal to ensure no crashes
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        const char* name = child.name();
        const char* value = child.value();
        (void)name; (void)value; // Use to avoid compiler warnings
        
        for (pugi::xml_attribute attr = child.first_attribute(); attr; attr = attr.next_attribute()) {
            const char* attr_name = attr.name();
            const char* attr_value = attr.value();
            (void)attr_name; (void)attr_value; // Use to avoid compiler warnings
        }
        
        for (pugi::xml_node grandchild = child.first_child(); grandchild; grandchild = grandchild.next_sibling()) {
            const char* gc_name = grandchild.name();
            const char* gc_value = grandchild.value();
            (void)gc_name; (void)gc_value; // Use to avoid compiler warnings
        }
    }
    
#endif // PUGIXML_HAS_STRING_VIEW
    
    // Cleanup happens automatically via RAII
    return 0;
}

} // namespace harness_021
