#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <stdint.h>
#include <string>
#include <vector>
#include <string_view>
#include <memory>
#include <functional>

namespace harness_013 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for comprehensive string_view testing
    if (size < 128) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Parse XML document or create structured document for testing
    // Split input: first part for XML content, rest for string_view operations
    size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(32, size / 3);
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_full);
    
    // If parsing failed, create a document with complex structure for string_view testing
    if (!parse_result) {
        doc.reset();
        
        // Create root with fuzzed name
        std::string root_name = fdp.remaining_bytes() > 5 ? fdp.ConsumeRandomLengthString(15) : "root";
        pugi::xml_node root = doc.append_child(pugi::node_element);
        root.set_name(root_name.c_str());
        
        // Create hierarchical structure with various element names
        int depth_levels = fdp.ConsumeIntegralInRange<int>(1, 4);
        std::vector<pugi::xml_node> current_level;
        current_level.push_back(root);
        
        for (int level = 0; level < depth_levels && fdp.remaining_bytes() > 50; level++) {
            std::vector<pugi::xml_node> next_level;
            
            for (auto& parent_node : current_level) {
                int child_count = fdp.ConsumeIntegralInRange<int>(1, 6);
                
                for (int i = 0; i < child_count && fdp.remaining_bytes() > 30; i++) {
                    std::string child_name = fdp.ConsumeRandomLengthString(12);
                    pugi::xml_node child = parent_node.append_child(child_name.c_str());
                    
                    // Add attributes with different name patterns
                    int attr_count = fdp.ConsumeIntegralInRange<int>(0, 5);
                    for (int j = 0; j < attr_count && fdp.remaining_bytes() > 20; j++) {
                        std::string attr_name = fdp.ConsumeRandomLengthString(10);
                        std::string attr_value = fdp.ConsumeRandomLengthString(25);
                        child.append_attribute(attr_name.c_str()).set_value(attr_value.c_str());
                    }
                    
                    // Add text content
                    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 15) {
                        std::string text = fdp.ConsumeRandomLengthString(40);
                        child.append_child(pugi::node_pcdata).set_value(text.c_str());
                    }
                    
                    next_level.push_back(child);
                }
            }
            
            current_level = next_level;
        }
    }
    
    // Get root node
    pugi::xml_node root = doc.document_element();
    if (root.empty()) {
        root = doc.first_child();
        if (root.empty()) {
            return 0; // No document structure
        }
    }
    
    // Step 2: Generate test strings for string_view operations
    // Check we have enough data for string operations
    if (fdp.remaining_bytes() < 2000) {
        return 0; // Not enough data for comprehensive string_view testing
    }
    
    // Generate multiple test strings with different characteristics
    std::vector<std::string> test_strings;
    std::vector<std::string_view> test_string_views;
    // Reserve capacity to prevent reallocation that would invalidate string_views
    test_strings.reserve(20);
    for (int i = 0; i < 20 && fdp.remaining_bytes() > 50; i++) {
        size_t str_len = fdp.ConsumeIntegralInRange<size_t>(0, 100);
        std::string str = fdp.ConsumeRandomLengthString(str_len);
        test_strings.push_back(str);
        test_string_views.push_back(test_strings.back());  // Use the persistent copy in vector
    }
    
    // Store some std::string objects for lifetime management
    std::string persistent_strings[10];
    std::string_view persistent_views[10];
    for (int i = 0; i < 10 && fdp.remaining_bytes() > 30; i++) {
        persistent_strings[i] = fdp.ConsumeRandomLengthString(50);
        persistent_views[i] = persistent_strings[i];
    }
    
    // Step 3: Comprehensive testing of string_view APIs
    
    // 3.1 Test xml_node string_view APIs
    
    // Create a test node for operations
    pugi::xml_node test_node = root;
    if (root.first_child()) {
        test_node = root.first_child();
    }
    
    // Test string_view constructors and setters
    
    // 3.1.1 Test set_name with string_view
#ifdef PUGIXML_HAS_STRING_VIEW
    if (!test_string_views.empty() && !test_node.empty()) {
        size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, test_string_views.size() - 1);
        test_node.set_name(test_string_views[idx]);
    }
#endif
    
    // 3.1.2 Test set_value with string_view for nodes
#ifdef PUGIXML_HAS_STRING_VIEW
    if (test_string_views.size() > 1 && !test_node.empty()) {
        size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, test_string_views.size() - 1);
        test_node.set_value(test_string_views[idx]);
    }
#endif
    
    // 3.1.3 Test append_child with string_view
    pugi::xml_node new_child;
#ifdef PUGIXML_HAS_STRING_VIEW
    if (!test_string_views.empty() && !root.empty()) {
        size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, test_string_views.size() - 1);
        new_child = root.append_child(test_string_views[idx]);
    }
#endif
    
    // 3.1.4 Test prepend_child with string_view
    pugi::xml_node prepended_child;
#ifdef PUGIXML_HAS_STRING_VIEW
    if (!test_string_views.empty() && !root.empty()) {
        size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, test_string_views.size() - 1);
        prepended_child = root.prepend_child(test_string_views[idx]);
    }
#endif
    
    // 3.1.5 Test insert_child_after and insert_child_before with string_view
#ifdef PUGIXML_HAS_STRING_VIEW
    if (!test_string_views.empty() && !new_child.empty() && !prepended_child.empty()) {
        size_t idx1 = fdp.ConsumeIntegralInRange<size_t>(0, test_string_views.size() - 1);
        size_t idx2 = fdp.ConsumeIntegralInRange<size_t>(0, test_string_views.size() - 1);
        
        root.insert_child_after(test_string_views[idx1], new_child);
        root.insert_child_before(test_string_views[idx2], prepended_child);
    }
#endif
    
    // 3.2 Test xml_attribute string_view APIs
    
    // Create a test attribute
    pugi::xml_attribute test_attr;
    if (!test_node.empty()) {
        // Create attribute for testing
#ifdef PUGIXML_HAS_STRING_VIEW
        if (!test_string_views.empty()) {
            size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, test_string_views.size() - 1);
            test_attr = test_node.append_attribute(test_string_views[idx]);
        }
#endif
    }
    
    // 3.2.1 Test xml_attribute::set_value with string_view
#ifdef PUGIXML_HAS_STRING_VIEW
    if (!test_attr.empty() && !test_string_views.empty()) {
        size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, test_string_views.size() - 1);
        test_attr.set_value(test_string_views[idx]);
    }
#endif
    
    // 3.2.2 Test xml_attribute::operator= with string_view
#ifdef PUGIXML_HAS_STRING_VIEW
    if (!test_attr.empty() && !test_string_views.empty()) {
        size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, test_string_views.size() - 1);
        test_attr = test_string_views[idx];
    }
#endif
    
    // 3.3 Test xml_text string_view APIs
    
    // Create text node for testing
    pugi::xml_text test_text;
    if (!new_child.empty()) {
        test_text = new_child.text();
    }
    
    // 3.3.1 Test xml_text::set with string_view
#ifdef PUGIXML_HAS_STRING_VIEW
    if (test_text && !test_string_views.empty()) {
        size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, test_string_views.size() - 1);
        test_text.set(test_string_views[idx]);
    }
#endif
    
    // 3.3.2 Test xml_text::operator= with string_view
#ifdef PUGIXML_HAS_STRING_VIEW
    if (test_text && test_string_views.size() > 1) {
        size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, test_string_views.size() - 1);
        test_text = test_string_views[idx];
    }
#endif
    
    // Step 4: Test mixed std::string and string_view API usage patterns
    
    // 4.1 Mixed pattern: Use std::string then string_view on same data
    if (fdp.remaining_bytes() > 100) {
        std::string mixed_str = fdp.ConsumeRandomLengthString(50);
        
        // Create node with std::string
        pugi::xml_node mixed_node = root.append_child(mixed_str.c_str());
        
        // Modify with string_view of the same string
#ifdef PUGIXML_HAS_STRING_VIEW
        if (!mixed_node.empty()) {
            std::string_view mixed_sv = mixed_str;
            mixed_node.set_value(mixed_sv);
            
            // Create attribute with string_view of same string
            pugi::xml_attribute mixed_attr = mixed_node.append_attribute("mixed_attr");
            if (mixed_attr) {
                mixed_attr.set_value(mixed_sv);
            }
        }
#endif
    }
    
    // 4.2 Pattern: Convert between std::string and string_view
    if (fdp.remaining_bytes() > 100) {
        std::string convert_str = fdp.ConsumeRandomLengthString(40);
        
        pugi::xml_node convert_node = root.append_child("convert_test");
        if (!convert_node.empty()) {
            // Set name with std::string
            convert_node.set_name(convert_str.c_str());
            
            // Get name and convert to string_view
            const char* node_name = convert_node.name();
            if (node_name) {
                std::string_view node_name_sv(node_name);
                
                // Use string_view to find sibling
#ifdef PUGIXML_HAS_STRING_VIEW
                pugi::xml_node found_sibling = root.next_sibling(node_name_sv);
                (void)found_sibling;
#endif
            }
        }
    }
    
    // Step 5: Test string_view find operations
    
    // 5.1 Test child() with string_view
#ifdef PUGIXML_HAS_STRING_VIEW
    if (!test_string_views.empty() && !root.empty()) {
        size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, test_string_views.size() - 1);
        pugi::xml_node found_child = root.child(test_string_views[idx]);
        (void)found_child;
    }
#endif
    
    // 5.2 Test attribute() with string_view
#ifdef PUGIXML_HAS_STRING_VIEW
    if (!test_string_views.empty() && !test_node.empty()) {
        size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, test_string_views.size() - 1);
        pugi::xml_attribute found_attr = test_node.attribute(test_string_views[idx]);
        (void)found_attr;
    }
#endif
    
    // 5.3 Test next_sibling() and previous_sibling() with string_view
#ifdef PUGIXML_HAS_STRING_VIEW
    if (!test_string_views.empty() && !new_child.empty()) {
        size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, test_string_views.size() - 1);
        pugi::xml_node next_sib = new_child.next_sibling(test_string_views[idx]);
        pugi::xml_node prev_sib = new_child.previous_sibling(test_string_views[idx]);
        (void)next_sib;
        (void)prev_sib;
    }
#endif
    
    // 5.4 Test attribute() with hint and string_view
#ifdef PUGIXML_HAS_STRING_VIEW
    if (!test_string_views.empty() && !test_node.empty()) {
        size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, test_string_views.size() - 1);
        pugi::xml_attribute hint;
        pugi::xml_attribute found_attr_with_hint = test_node.attribute(test_string_views[idx], hint);
        (void)found_attr_with_hint;
    }
#endif
    
    // Step 6: Test string_view removal operations
    
    // 6.1 Test remove_attribute with string_view
#ifdef PUGIXML_HAS_STRING_VIEW
    if (!test_attr.empty() && !test_string_views.empty()) {
        size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, test_string_views.size() - 1);
        test_node.remove_attribute(test_string_views[idx]);
    }
#endif
    
    // 6.2 Test remove_child with string_view
#ifdef PUGIXML_HAS_STRING_VIEW
    if (!new_child.empty() && !test_string_views.empty()) {
        size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, test_string_views.size() - 1);
        root.remove_child(test_string_views[idx]);
    }
#endif
    
    // Step 7: Test edge cases and advanced scenarios
    
    // 7.1 Test with empty string_view
#ifdef PUGIXML_HAS_STRING_VIEW
    if (!root.empty()) {
        std::string_view empty_sv = "";
        pugi::xml_node empty_child = root.append_child(empty_sv);
        
        if (!empty_child.empty()) {
            empty_child.set_value(empty_sv);
            
            pugi::xml_attribute empty_attr = empty_child.append_attribute(empty_sv);
            if (!empty_attr.empty()) {
                empty_attr.set_value(empty_sv);
            }
        }
    }
#endif
    
    // 7.2 Test with string_view from temporary std::string
    if (fdp.remaining_bytes() > 50) {
        std::string temp_str = fdp.ConsumeRandomLengthString(30);
        
#ifdef PUGIXML_HAS_STRING_VIEW
        // Create string_view from temporary string (valid during this scope)
        std::string_view temp_sv = temp_str;
        
        pugi::xml_node temp_node = root.append_child(temp_sv);
        if (!temp_node.empty()) {
            temp_node.set_value(temp_sv);
        }
#endif
    }
    
    // 7.3 Test with string_view from substring
    if (fdp.remaining_bytes() > 100) {
        std::string long_str = fdp.ConsumeRandomLengthString(100);
        
#ifdef PUGIXML_HAS_STRING_VIEW
        // Create string_view to substring
        if (long_str.length() > 20) {
            std::string_view substring_sv = std::string_view(long_str).substr(10, 20);
            
            pugi::xml_node sub_node = root.append_child(substring_sv);
            if (!sub_node.empty()) {
                sub_node.set_value(substring_sv);
            }
        }
#endif
    }
    
    // Step 8: Validate operations and document structure
    
    // Traverse document to ensure no crashes
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        pugi::xml_node_type type = node.type();
        const char* name = node.name();
        const char* value = node.value();
        
        // Traverse attributes
        for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
            const char* attr_name = attr.name();
            const char* attr_value = attr.value();
        }
        
        // Traverse children
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            const char* child_name = child.name();
            const char* child_value = child.value();
        }
    }
    
    // Step 9: Test with persistent string_view objects
#ifdef PUGIXML_HAS_STRING_VIEW
    for (int i = 0; i < 10; i++) {
        if (!persistent_views[i].empty() && !root.empty()) {
            // Test with persistent string_view
            pugi::xml_node persist_node = root.append_child(persistent_views[i]);
            
            if (!persist_node.empty()) {
                persist_node.set_value(persistent_views[i]);
                
                pugi::xml_attribute persist_attr = persist_node.append_attribute(persistent_views[i]);
                if (!persist_attr.empty()) {
                    persist_attr.set_value(persistent_views[i]);
                }
            }
        }
    }
#endif
    
    return 0;
}

} // namespace harness_013
