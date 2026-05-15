#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>

namespace harness_005 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for basic operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();
    bool use_load_string = fdp.ConsumeBool();
    
    // Consume variable-size data for XML content
    std::string xml_content = fdp.ConsumeRandomLengthString(4096);
    
    // Consume multiple node and attribute names from fuzzer input
    // We need names for various operations
    std::string child_name1 = fdp.ConsumeRandomLengthString(64);
    std::string child_name2 = fdp.ConsumeRandomLengthString(64);
    std::string child_name3 = fdp.ConsumeRandomLengthString(64);
    std::string child_name4 = fdp.ConsumeRandomLengthString(64);
    std::string attr_name1 = fdp.ConsumeRandomLengthString(64);
    std::string attr_name2 = fdp.ConsumeRandomLengthString(64);
    std::string remove_target_name = fdp.ConsumeRandomLengthString(64);
    
    // Create XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result;
    
    // Load XML document from fuzzer input
    if (use_load_string) {
        result = doc.load_string(xml_content.c_str(), parse_options);
    } else {
        result = doc.load_buffer(xml_content.c_str(), xml_content.size(), parse_options);
    }
    
    // If parsing failed, create a minimal document for testing
    if (!result) {
        // Create a simple document with root element using fuzzed name
        std::string root_name = fdp.ConsumeRandomLengthString(32);
        std::string xml_minimal = "<" + root_name + "></" + root_name + ">";
        result = doc.load_string(xml_minimal.c_str(), parse_options);
        if (!result) {
            return 0;
        }
    }
    
    // Get document root element
    pugi::xml_node root = doc.document_element();
    if (!root) {
        // If no root, try to get first child
        root = doc.first_child();
        if (!root) {
            return 0;
        }
    }
    
    // Perform node manipulation operations based on operation mode
    // Each mode tests different combinations of the target APIs
    switch (operation_mode) {
        case 0:
            // Test basic child operations: append, prepend, remove
            {
                // 1. xml_node::append_child
                pugi::xml_node appended_child = root.append_child(child_name1.c_str());
                
                // 2. xml_node::prepend_child  
                pugi::xml_node prepended_child = root.prepend_child(child_name2.c_str());
                
                if (appended_child && prepended_child) {
                    // 6. xml_node::append_attribute
                    appended_child.append_attribute(attr_name1.c_str());
                    prepended_child.append_attribute(attr_name2.c_str());
                    
                    // Create another child for insertion operations
                    pugi::xml_node middle_child = root.append_child(child_name3.c_str());
                    
                    if (middle_child) {
                        // 3. xml_node::insert_child_after
                        pugi::xml_node after_child = root.insert_child_after(child_name4.c_str(), middle_child);
                        
                        // 4. xml_node::insert_child_before
                        pugi::xml_node before_child = root.insert_child_before(remove_target_name.c_str(), middle_child);
                        
                        // 5. xml_node::remove_child (by name)
                        if (!remove_target_name.empty()) {
                            root.remove_child(remove_target_name.c_str());
                        }
                        
                        // Also test remove_child by node reference
                        root.remove_child(middle_child);
                    }
                }
            }
            break;
            
        case 1:
            // Test insertion operations with sibling relationships
            {
                // Create initial structure
                pugi::xml_node base1 = root.append_child(child_name1.c_str());
                pugi::xml_node base2 = root.append_child(child_name2.c_str());
                
                if (base1 && base2) {
                    // Test insert_child_before and insert_child_after
                    pugi::xml_node before_node = root.insert_child_before(child_name3.c_str(), base1);
                    pugi::xml_node after_node = root.insert_child_after(child_name4.c_str(), base2);
                    
                    // Add attributes to all nodes
                    if (before_node) before_node.append_attribute(attr_name1.c_str());
                    if (base1) base1.append_attribute(attr_name2.c_str());
                    if (base2) base2.append_attribute(attr_name1.c_str());
                    if (after_node) after_node.append_attribute(attr_name2.c_str());
                    
                    // Test remove_child operations
                    if (base1) {
                        root.remove_child(base1);
                    }
                    
                    // Try to remove by name (may or may not exist)
                    root.remove_child(child_name2.c_str());
                    
                    // Test prepend_child on remaining structure
                    if (before_node) {
                        before_node.prepend_child(remove_target_name.c_str());
                    }
                }
            }
            break;
            
        case 2:
            // Test complex nested operations
            {
                // Create a hierarchy
                pugi::xml_node level1 = root.append_child(child_name1.c_str());
                if (level1) {
                    level1.append_attribute(attr_name1.c_str());
                    
                    pugi::xml_node level2 = level1.append_child(child_name2.c_str());
                    if (level2) {
                        level2.append_attribute(attr_name2.c_str());
                        
                        // Test all insertion operations at different levels
                        pugi::xml_node sibling1 = level1.insert_child_before(child_name3.c_str(), level2);
                        pugi::xml_node sibling2 = level1.insert_child_after(child_name4.c_str(), level2);
                        
                        if (sibling1) {
                            sibling1.append_attribute(attr_name1.c_str());
                            // Nested prepend
                            sibling1.prepend_child(remove_target_name.c_str());
                        }
                        
                        if (sibling2) {
                            sibling2.append_attribute(attr_name2.c_str());
                            // Nested append
                            sibling2.append_child(child_name1.c_str());
                        }
                        
                        // Remove operations at different levels
                        level1.remove_child(level2);
                        if (sibling1) {
                            level1.remove_child(sibling1);
                        }
                        
                        // Try to remove non-existent child
                        root.remove_child("non_existent_fuzzed_name");
                    }
                }
            }
            break;
            
        case 3:
            // Test edge cases and error conditions
            {
                // Test with empty strings
                pugi::xml_node empty_name_child = root.append_child("");
                if (empty_name_child) {
                    empty_name_child.append_attribute("");
                    root.remove_child(empty_name_child);
                }
                
                // Test append_child and immediate remove
                pugi::xml_node temp_child = root.append_child(child_name1.c_str());
                if (temp_child) {
                    root.remove_child(temp_child);
                }
                
                // Test prepend_child and immediate remove by name
                pugi::xml_node temp_prepend = root.prepend_child(child_name2.c_str());
                if (temp_prepend) {
                    root.remove_child(child_name2.c_str());
                }
                
                // Test insert operations with empty reference node
                pugi::xml_node empty_ref;
                root.insert_child_before(child_name3.c_str(), empty_ref);
                root.insert_child_after(child_name4.c_str(), empty_ref);
                
                // Test append_attribute with various names
                pugi::xml_node attr_test = root.append_child("attr_test_node");
                if (attr_test) {
                    attr_test.append_attribute(attr_name1.c_str());
                    attr_test.append_attribute("");
                    attr_test.append_attribute(attr_name2.c_str());
                    
                    // Create child for insertion testing
                    pugi::xml_node ref_child = attr_test.append_child("ref_child");
                    if (ref_child) {
                        attr_test.insert_child_before("before_ref", ref_child);
                        attr_test.insert_child_after("after_ref", ref_child);
                        
                        // Remove the reference child
                        attr_test.remove_child(ref_child);
                    }
                }
            }
            break;
    }
    
    // Cleanup - reset document
    doc.reset();
    
    return 0;
}

} // namespace harness_005
