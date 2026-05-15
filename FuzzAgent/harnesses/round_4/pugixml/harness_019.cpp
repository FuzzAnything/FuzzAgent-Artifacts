#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <string_view>

namespace harness_019 {

// Conditionally include string_view based on pugixml configuration
#ifdef PUGIXML_HAS_STRING_VIEW
#endif

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for comprehensive debugging and removal testing
    // Need enough data for XML content, node/attribute names, and operation parameters
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first for operation parameters
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 4;  // Which removal operation to prioritize
    uint8_t num_test_nodes = fdp.ConsumeIntegralInRange<uint8_t>(1, 10);  // Number of test nodes to create
    bool test_string_view = fdp.ConsumeBool();  // Whether to test string_view variants
    size_t max_string_view_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);  // Max size for string_view
    
    // Consume variable-size data for XML content and strings
    std::string xml_content = fdp.ConsumeRandomLengthString(2048);
    std::string root_name = fdp.ConsumeRandomLengthString(64);
    std::string node_prefix = fdp.ConsumeRandomLengthString(32);
    std::string attr_prefix = fdp.ConsumeRandomLengthString(32);
    std::string remove_name1 = fdp.ConsumeRandomLengthString(64);
    std::string remove_name2 = fdp.ConsumeRandomLengthString(64);
    
    // Create string_view if enabled and supported
    #ifdef PUGIXML_HAS_STRING_VIEW
    std::string_view string_view_name1;
    std::string_view string_view_name2;
    if (test_string_view) {
        if (!remove_name1.empty()) {
            size_t sv_size1 = std::min(remove_name1.size(), max_string_view_size);
            string_view_name1 = std::string_view(remove_name1.c_str(), sv_size1);
        }
        if (!remove_name2.empty()) {
            size_t sv_size2 = std::min(remove_name2.size(), max_string_view_size);
            string_view_name2 = std::string_view(remove_name2.c_str(), sv_size2);
        }
    }
    #endif
    
    // Step 1: Create xml_document object
    pugi::xml_document doc;
    
    // Step 2: Parse XML data using load_string as specified
    pugi::xml_parse_result result = doc.load_string(xml_content.c_str(), parse_options);
    
    // Step 3: Check parse result using xml_parse_result::description() - TARGET API
    const char* parse_description = result.description();
    (void)parse_description; // Avoid unused variable warning
    
    // Step 4: Get root node via document_element()
    pugi::xml_node root = doc.document_element();
    
    // If document is empty or parsing failed, create a test structure
    if (!root) {
        doc.reset();
        if (root_name.empty()) {
            root_name = "root";
        }
        root = doc.append_child(root_name.c_str());
        
        // Create test nodes and attributes for removal operations
        for (int i = 0; i < num_test_nodes && i < 10; i++) {
            std::string node_name;
            if (node_prefix.empty()) {
                node_name = "node_" + std::to_string(i);
            } else {
                node_name = node_prefix + "_" + std::to_string(i);
            }
            
            pugi::xml_node child = root.append_child(node_name.c_str());
            if (child) {
                // Add attributes to test remove_attributes()
                for (int j = 0; j < 3; j++) {
                    std::string attr_name;
                    if (attr_prefix.empty()) {
                        attr_name = "attr_" + std::to_string(j);
                    } else {
                        attr_name = attr_prefix + "_" + std::to_string(j);
                    }
                    std::string attr_value = "value_" + std::to_string(j);
                    child.append_attribute(attr_name.c_str()) = attr_value.c_str();
                }
                
                // Add child nodes to test remove_children()
                for (int k = 0; k < 2; k++) {
                    std::string grandchild_name = "grandchild_" + std::to_string(k);
                    child.append_child(grandchild_name.c_str());
                }
            }
        }
        
        // Add nodes with specific names for remove_child(string_view) testing
        if (!remove_name1.empty()) {
            pugi::xml_node special1 = root.append_child(remove_name1.c_str());
            special1.append_attribute("special") = "true";
        }
        
        if (!remove_name2.empty()) {
            pugi::xml_node special2 = root.append_child(remove_name2.c_str());
            special2.append_attribute("special") = "true";
        }
    }
    
    // Step 5: Call offset_debug() on nodes to inspect internal state - TARGET API
    // Test offset_debug on various nodes in the document
    std::vector<ptrdiff_t> debug_offsets;
    
    // Test offset_debug on root
    if (root) {
        ptrdiff_t root_offset = root.offset_debug();
        debug_offsets.push_back(root_offset);
        
        // Test offset_debug on child nodes
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            ptrdiff_t child_offset = child.offset_debug();
            debug_offsets.push_back(child_offset);
            
            // Test offset_debug on grandchildren
            for (pugi::xml_node grandchild = child.first_child(); grandchild; grandchild = grandchild.next_sibling()) {
                ptrdiff_t grandchild_offset = grandchild.offset_debug();
                debug_offsets.push_back(grandchild_offset);
            }
            
            // Test offset_debug on attributes
            // Note: attributes don't have offset_debug method
            // We've already tested offset_debug on the child node that owns these attributes
        }
    }
    
    // Step 6: Create/modify nodes and attributes to set up test state
    // (Already done above when creating test structure)
    
    // Step 7 & 8: Call removal operations based on operation_mode
    if (root) {
        // Get first child for testing removal operations
        pugi::xml_node test_node = root.first_child();
        
        if (test_node) {
            switch (operation_mode) {
                case 0:
                    // Test remove_attributes() - TARGET API
                    if (test_node.first_attribute()) {
                        bool remove_attrs_result = test_node.remove_attributes();
                        (void)remove_attrs_result; // Avoid unused variable warning
                        
                        // Verify removal
                        bool has_attrs_after = (test_node.first_attribute() != pugi::xml_attribute());
                        // has_attrs_after should be false if removal succeeded
                    }
                    break;
                    
                case 1:
                    // Test remove_children() - TARGET API
                    if (test_node.first_child()) {
                        bool remove_children_result = test_node.remove_children();
                        (void)remove_children_result; // Avoid unused variable warning
                        
                        // Verify removal
                        bool has_children_after = (test_node.first_child() != pugi::xml_node());
                        // has_children_after should be false if removal succeeded
                    }
                    break;
                    
                case 2:
                    // Test remove_attribute(string_view) - TARGET API
                    if (test_node.first_attribute()) {
                        pugi::xml_attribute first_attr = test_node.first_attribute();
                        const char* attr_name = first_attr.name();
                        
                        if (attr_name && strlen(attr_name) > 0) {
                            #ifdef PUGIXML_HAS_STRING_VIEW
                            if (test_string_view) {
                                std::string_view sv_name(attr_name, strlen(attr_name));
                                bool remove_attr_result = test_node.remove_attribute(sv_name);
                                (void)remove_attr_result;
                            } else {
                            #endif
                                bool remove_attr_result = test_node.remove_attribute(attr_name);
                                (void)remove_attr_result;
                            #ifdef PUGIXML_HAS_STRING_VIEW
                            }
                            #endif
                        }
                    }
                    break;
                    
                case 3:
                    // Test remove_child(string_view) - TARGET API
                    if (test_node.first_child()) {
                        pugi::xml_node first_child = test_node.first_child();
                        const char* child_name = first_child.name();
                        
                        if (child_name && strlen(child_name) > 0) {
                            #ifdef PUGIXML_HAS_STRING_VIEW
                            if (test_string_view) {
                                std::string_view sv_name(child_name, strlen(child_name));
                                bool remove_child_result = test_node.remove_child(sv_name);
                                (void)remove_child_result;
                            } else {
                            #endif
                                bool remove_child_result = test_node.remove_child(child_name);
                                (void)remove_child_result;
                            #ifdef PUGIXML_HAS_STRING_VIEW
                            }
                            #endif
                        }
                    }
                    break;
            }
            
            // Also test remove_attribute and remove_child with the specific names we created
            if (!remove_name1.empty()) {
                #ifdef PUGIXML_HAS_STRING_VIEW
                if (test_string_view && !string_view_name1.empty()) {
                    root.remove_child(string_view_name1);
                    // Try to remove attribute with same name (might not exist)
                    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                        child.remove_attribute(string_view_name1);
                    }
                } else {
                #endif
                    root.remove_child(remove_name1.c_str());
                    // Try to remove attribute with same name (might not exist)
                    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                        child.remove_attribute(remove_name1.c_str());
                    }
                #ifdef PUGIXML_HAS_STRING_VIEW
                }
                #endif
            }
            
            if (!remove_name2.empty()) {
                #ifdef PUGIXML_HAS_STRING_VIEW
                if (test_string_view && !string_view_name2.empty()) {
                    root.remove_child(string_view_name2);
                    // Try to remove attribute with same name (might not exist)
                    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                        child.remove_attribute(string_view_name2);
                    }
                } else {
                #endif
                    root.remove_child(remove_name2.c_str());
                    // Try to remove attribute with same name (might not exist)
                    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                        child.remove_attribute(remove_name2.c_str());
                    }
                #ifdef PUGIXML_HAS_STRING_VIEW
                }
                #endif
            }
        }
        
        // Test remove_attributes and remove_children on multiple nodes
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            // Randomly decide which operations to perform based on remaining fuzzer input
            if (fdp.ConsumeBool()) {
                child.remove_attributes();
            }
            
            if (fdp.ConsumeBool()) {
                child.remove_children();
            }
            
            // Test remove_attribute with a random attribute name
            if (child.first_attribute() && fdp.ConsumeBool()) {
                pugi::xml_attribute random_attr = child.first_attribute();
                for (int i = 0; i < fdp.ConsumeIntegralInRange<int>(0, 3) && random_attr; i++) {
                    random_attr = random_attr.next_attribute();
                }
                
                if (random_attr) {
                    const char* attr_to_remove = random_attr.name();
                    if (attr_to_remove) {
                        child.remove_attribute(attr_to_remove);
                    }
                }
            }
            
            // Test remove_child with a random child name
            if (child.first_child() && fdp.ConsumeBool()) {
                pugi::xml_node random_child = child.first_child();
                for (int i = 0; i < fdp.ConsumeIntegralInRange<int>(0, 3) && random_child; i++) {
                    random_child = random_child.next_sibling();
                }
                
                if (random_child) {
                    const char* child_to_remove = random_child.name();
                    if (child_to_remove) {
                        child.remove_child(child_to_remove);
                    }
                }
            }
        }
    }
    
    // Step 9: Verify removal operations (implicitly verified during execution)
    // Step 10: Let xml_document destructor clean up automatically
    
    return 0;
}

} // namespace harness_019
