#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <algorithm>
#include "pugixml.hpp"

namespace harness_009 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic XML and string operations
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume control values first - determine which APIs to test
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 8;  // 8 different testing modes
    bool test_string_view_apis = fdp.ConsumeBool();
    bool test_buffer_operations = fdp.ConsumeBool();
    bool test_path_operations = fdp.ConsumeBool();
    bool test_error_conditions = fdp.ConsumeBool();
    bool test_tree_manipulation = fdp.ConsumeBool();
    
    // Consume string data for names, values, and attributes
    std::string root_name = fdp.ConsumeRandomLengthString(32);
    std::string child_name = fdp.ConsumeRandomLengthString(32);
    std::string attr_name = fdp.ConsumeRandomLengthString(32);
    std::string attr_value = fdp.ConsumeRandomLengthString(128);
    std::string insert_name = fdp.ConsumeRandomLengthString(32);
    std::string text_content = fdp.ConsumeRandomLengthString(256);
    
    // Consume delimiter for path operations
    char path_delimiter = '/';
    if (fdp.ConsumeBool()) {
        uint8_t delim_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (delim_choice) {
            case 0: path_delimiter = '/'; break;
            case 1: path_delimiter = '.'; break;
            case 2: path_delimiter = '|'; break;
            case 3: path_delimiter = '\\'; break;
        }
    }
    
    // Use remaining bytes as XML content for parsing
    std::string xml_content = fdp.ConsumeRemainingBytesAsString();
    if (xml_content.empty()) {
        // Create minimal XML if no content provided
        xml_content = "<root><child attr='value'>text</child></root>";
    }
    
    // Create XML document
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result;
    
    // Test error conditions to exercise xml_parse_result::description()
    if (test_error_conditions) {
        // Test parsing with invalid data to trigger errors
        uint8_t error_test = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (error_test) {
            case 0:
                // Empty string
                parse_result = doc.load_buffer("", 0);
                break;
            case 1:
                // Malformed XML
                parse_result = doc.load_buffer("<root><unclosed>", 16);
                break;
            case 2:
                // Invalid encoding
                parse_result = doc.load_buffer("\xFF\xFE", 2);
                break;
            case 3:
                // Valid parse - use fuzzer content
                parse_result = doc.load_buffer(xml_content.c_str(), xml_content.size());
                break;
        }
        
        // Exercise xml_parse_result::description() - PRIMARY TARGET API
        if (!parse_result) {
            const char* error_desc = parse_result.description();
            // Use the description to ensure it's not optimized away
            if (error_desc && error_desc[0] != '\0') {
                // Do something with the description
                volatile const char* volatile_desc = error_desc;
                (void)volatile_desc;
            }
        }
    } else {
        // Normal parsing
        parse_result = doc.load_buffer(xml_content.c_str(), xml_content.size());
    }
    
    // If parsing succeeded (or we created document from scratch), test other APIs
    pugi::xml_node root;
    
    if (!parse_result) {
        // Create document from scratch if parsing failed
        root = doc.append_child("root");
        pugi::xml_node child = root.append_child("child");
        child.append_attribute("attr") = "value";
        child.text().set("text");
    } else {
        root = doc.first_child();
        if (!root) {
            root = doc.append_child("root");
        }
    }
    
    // Test string_view APIs if enabled
    if (test_string_view_apis && root) {
        // Create string_view objects from fuzzer-provided strings
        std::string_view root_name_sv(root_name);
        std::string_view child_name_sv(child_name);
        std::string_view attr_name_sv(attr_name);
        std::string_view attr_value_sv(attr_value);
        std::string_view insert_name_sv(insert_name);
        std::string_view text_content_sv(text_content);
        
        // 1. Test xml_node::attribute(string_view, xml_attribute&) - PRIMARY TARGET API
        if (!attr_name_sv.empty()) {
            pugi::xml_attribute found_attr;
            pugi::xml_attribute attr_result = root.attribute(attr_name_sv, found_attr);
            
            // Create attribute if it doesn't exist for testing
            if (!attr_result) {
                pugi::xml_attribute new_attr = root.append_attribute(attr_name_sv);
                if (new_attr) {
                    new_attr.set_value(attr_value_sv);
                    
                    // Test attribute retrieval again
                    attr_result = root.attribute(attr_name_sv, found_attr);
                }
            }
        }
        
        // 2. Test other string_view overloads for completeness
        if (!child_name_sv.empty()) {
            // xml_node::child(string_view)
            pugi::xml_node child = root.child(child_name_sv);
            
            // Create child if it doesn't exist
            if (!child) {
                child = root.append_child(child_name_sv);
                child.text().set(text_content_sv);
                
                // Test child retrieval again
                child = root.child(child_name_sv);
            }
            
            // Test xml_node::next_sibling(string_view) and previous_sibling(string_view)
            if (child) {
                // Create another sibling
                std::string sibling_name = "sibling_" + child_name;
                std::string_view sibling_name_sv(sibling_name);
                pugi::xml_node sibling = root.append_child(sibling_name_sv);
                
                // Test sibling navigation with string_view
                pugi::xml_node next_sib = child.next_sibling(sibling_name_sv);
                pugi::xml_node prev_sib = sibling.previous_sibling(child_name_sv);
                
                // Test set_name and set_value with string_view
                if (!insert_name_sv.empty()) {
                    child.set_name(insert_name_sv);
                    child.set_value(text_content_sv);
                    
                    // Update attribute with string_view
                    pugi::xml_attribute attr = child.first_attribute();
                    if (attr) {
                        attr.set_name(attr_name_sv);
                        attr.set_value(attr_value_sv);
                    }
                }
            }
        }
    }
    
    // Test append_buffer() API - PRIMARY TARGET API
    if (test_buffer_operations && root) {
        // Create buffer content from fuzzer data (reuse text_content or generate new)
        std::string buffer_content;
        if (text_content.size() > 0) {
            buffer_content = "<buffer_node attr='buffer_attr'>" + text_content + "</buffer_node>";
        } else {
            buffer_content = "<buffer_node><child>buffer_content</child></buffer_node>";
        }
        
        // Test xml_node::append_buffer()
        pugi::xml_parse_result buffer_result = root.append_buffer(
            buffer_content.c_str(), 
            buffer_content.size()
        );
        
        // Test with different parsing options
        if (buffer_result) {
            // Try another append with different options
            std::string buffer2 = "<another>more content</another>";
            pugi::xml_parse_result buffer_result2 = root.append_buffer(
                buffer2.c_str(),
                buffer2.size(),
                pugi::parse_fragment | pugi::parse_ws_pcdata
            );
        }
    }
    
    // Test path() API - PRIMARY TARGET API  
    if (test_path_operations && root) {
        // Get path from root node
        std::string root_path = root.path(path_delimiter);
        
        // Create a more complex tree to test path generation
        if (test_tree_manipulation) {
            pugi::xml_node level1 = root.append_child("level1");
            pugi::xml_node level2 = level1.append_child("level2");
            pugi::xml_node level3 = level2.append_child("level3");
            
            // Test path() on deep node
            std::string deep_path = level3.path(path_delimiter);
            
            // Test with different delimiter
            char alt_delimiter = (path_delimiter == '/') ? '.' : '/';
            std::string alt_path = level3.path(alt_delimiter);
            
            // Test first_element_by_path() - inverse operation
            pugi::xml_node found_by_path = root.first_element_by_path(deep_path.c_str(), path_delimiter);
            pugi::xml_node found_by_alt_path = root.first_element_by_path(alt_path.c_str(), alt_delimiter);
        }
    }
    
    // Test insert_child_before/after with string_view - PRIMARY TARGET APIs
    if (test_tree_manipulation && root) {
        // Create reference nodes
        pugi::xml_node ref_child = root.child("ref_child");
        if (!ref_child) {
            ref_child = root.append_child("ref_child");
        }
        
        pugi::xml_node before_child = root.child("before_child");
        if (!before_child) {
            before_child = root.append_child("before_child");
        }
        
        pugi::xml_node after_child = root.child("after_child");
        if (!after_child) {
            after_child = root.append_child("after_child");
        }
        
        // Use string_view for insertion names
        std::string insert_before_name = "inserted_before";
        std::string insert_after_name = "inserted_after";
        std::string_view insert_before_sv(insert_before_name);
        std::string_view insert_after_sv(insert_after_name);
        
        // Test xml_node::insert_child_before(string_view, xml_node)
        if (ref_child && before_child) {
            pugi::xml_node inserted_before = root.insert_child_before(insert_before_sv, before_child);
            if (inserted_before) {
                inserted_before.text().set("inserted before text");
            }
        }
        
        // Test xml_node::insert_child_after(string_view, xml_node)
        if (ref_child && after_child) {
            pugi::xml_node inserted_after = root.insert_child_after(insert_after_sv, after_child);
            if (inserted_after) {
                inserted_after.text().set("inserted after text");
            }
        }
        
        // Test other tree manipulation APIs with string_view
        if (!attr_name.empty()) {
            std::string_view attr_name_sv(attr_name);
            
            // Test append_attribute and prepend_attribute with string_view
            pugi::xml_attribute appended_attr = root.append_attribute(attr_name_sv);
            if (appended_attr) {
                appended_attr.set_value("appended_value");
            }
            
            // Test remove_attribute and remove_child with string_view
            if (appended_attr) {
                root.remove_attribute(attr_name_sv);
            }
            
            // Test remove_child with string_view
            if (before_child) {
                std::string_view before_name_sv("before_child");
                root.remove_child(before_name_sv);
            }
        }
    }
    
    // Test xml_text APIs with string_view
    if (root) {
        pugi::xml_node text_node = root.child("text_node");
        if (!text_node) {
            text_node = root.append_child("text_node");
        }
        
        if (text_node) {
            pugi::xml_text text = text_node.text();
            
            // Test xml_text::set(string_view)
            if (!text_content.empty()) {
                std::string_view text_sv(text_content);
                text.set(text_sv);
            }
            
            // Test xml_text assignment with string_view
            std::string_view assign_sv("assigned via string_view");
            text = assign_sv;
        }
    }
    
    // Final test: traverse document and exercise all created nodes
    if (root) {
        // Simple traversal to ensure nodes are accessible
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            // Access node properties
            const char* name = child.name();
            const char* value = child.value();
            
            // Access attributes
            for (pugi::xml_attribute attr = child.first_attribute(); attr; attr = attr.next_attribute()) {
                const char* attr_name = attr.name();
                const char* attr_value = attr.value();
            }
        }
    }
    
    // Document will be cleaned up automatically when it goes out of scope
    return 0;
}

} // namespace harness_009
