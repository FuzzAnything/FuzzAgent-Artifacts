#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>

namespace harness_017 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for XML node manipulation operations testing
    // Need enough data for XML content, node names, attribute names, and operation parameters
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t num_nodes = fdp.ConsumeIntegralInRange<uint8_t>(1, 8);
    uint8_t num_attrs = fdp.ConsumeIntegralInRange<uint8_t>(1, 8);
    bool test_copy_ops = fdp.ConsumeBool();
    bool test_move_ops = fdp.ConsumeBool();
    bool test_attribute_ops = fdp.ConsumeBool();
    
    // Consume variable-size data for XML content
    std::string xml_content = fdp.ConsumeRandomLengthString(2048);
    
    // Consume node names for creation and manipulation
    std::vector<std::string> node_names;
    std::vector<std::string> node_values;
    
    for (int i = 0; i < num_nodes && fdp.remaining_bytes() > 10; i++) {
        node_names.push_back(fdp.ConsumeRandomLengthString(128));
        node_values.push_back(fdp.ConsumeRandomLengthString(256));
    }
    
    // Consume attribute names and values for attribute operations
    std::vector<std::string> attr_names;
    std::vector<std::string> attr_values;
    
    for (int i = 0; i < num_attrs && fdp.remaining_bytes() > 10; i++) {
        attr_names.push_back(fdp.ConsumeRandomLengthString(128));
        attr_values.push_back(fdp.ConsumeRandomLengthString(256));
    }
    
    // Step 1: Parse XML document from fuzzer input using load_buffer
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(xml_content.c_str(), xml_content.size(), parse_options);
    
    // Get or create document element for testing
    pugi::xml_node root = doc.document_element();
    if (!root) {
        // Create a root node if document is empty or parsing failed
        std::string root_name = "root";
        if (!node_names.empty()) {
            root_name = node_names[0];
        }
        root = doc.append_child(pugi::node_element);
        root.set_name(root_name.c_str());
    }
    
    // Step 2: Create child nodes using append_child
    std::vector<pugi::xml_node> created_nodes;
    for (size_t i = 0; i < node_names.size() && i < 8; i++) {
        if (node_names[i].empty()) continue;
        
        pugi::xml_node child = root.append_child(node_names[i].c_str());
        if (child) {
            child.text().set(node_values[i].c_str());
            created_nodes.push_back(child);
            
            // Also add some attributes to nodes for more complex testing
            if (i < attr_names.size() && i < attr_values.size()) {
                child.append_attribute(attr_names[i].c_str()) = attr_values[i].c_str();
            }
        }
    }
    
    // Step 3: Create additional attributes using append_attribute
    for (size_t i = 0; i < attr_names.size() && i < 8; i++) {
        if (attr_names[i].empty()) continue;
        
        root.append_attribute(attr_names[i].c_str()) = attr_values[i].c_str();
    }
    
    // Step 4: Test copy operations: prepend_copy and append_copy
    if (test_copy_ops && created_nodes.size() >= 2) {
        // Test prepend_copy - copy a node to the beginning of root's children
        pugi::xml_node source_node = created_nodes[0];
        if (source_node) {
            pugi::xml_node copied_node = root.prepend_copy(source_node);
            // copied_node should be a new node at the beginning
            
            // Test append_copy - copy a node to the end of root's children
            if (created_nodes.size() >= 2) {
                pugi::xml_node source_node2 = created_nodes[1];
                pugi::xml_node copied_node2 = root.append_copy(source_node2);
                // copied_node2 should be a new node at the end
            }
        }
        
        // Also test copying nodes between different parent nodes
        if (created_nodes.size() >= 3) {
            pugi::xml_node parent_node = created_nodes[0];
            pugi::xml_node child_to_copy = created_nodes[1];
            
            if (parent_node && child_to_copy) {
                // Create a child node within parent_node to test copying
                pugi::xml_node child_node = parent_node.append_child("child_to_copy");
                if (child_node) {
                    // Now copy this child to another location
                    pugi::xml_node copied_child = root.append_copy(child_node);
                }
            }
        }
    }
    
    // Step 5: Test move operations: prepend_move and append_move
    if (test_move_ops && created_nodes.size() >= 2) {
        // Create separate document for move operations testing
        pugi::xml_document doc2;
        pugi::xml_node root2 = doc2.append_child("root2");
        
        // Create some nodes in doc2 to serve as move sources
        std::vector<pugi::xml_node> source_nodes;
        for (size_t i = 0; i < std::min(node_names.size(), (size_t)3) && i < 3; i++) {
            if (!node_names[i].empty()) {
                pugi::xml_node src_node = root2.append_child(node_names[i].c_str());
                src_node.text().set(node_values[i].c_str());
                source_nodes.push_back(src_node);
            }
        }
        
        // Test prepend_move - move a node from doc2 to the beginning of root's children
        if (!source_nodes.empty()) {
            pugi::xml_node source_to_move = source_nodes[0];
            if (source_to_move) {
                pugi::xml_node moved_node = root.prepend_move(source_to_move);
                // source_to_move should now be empty/invalid (moved away)
            }
        }
        
        // Test append_move - move a node from doc2 to the end of root's children
        if (source_nodes.size() >= 2) {
            pugi::xml_node source_to_move2 = source_nodes[1];
            if (source_to_move2) {
                pugi::xml_node moved_node2 = root.append_move(source_to_move2);
                // source_to_move2 should now be empty/invalid (moved away)
            }
        }
        
        // Test moving nodes between different locations in the same document
        if (created_nodes.size() >= 3) {
            pugi::xml_node node1 = created_nodes[0];
            pugi::xml_node node2 = created_nodes[1];
            
            if (node1 && node2) {
                // Create a child under node1
                pugi::xml_node child_to_move = node1.append_child("movable_child");
                if (child_to_move) {
                    std::string movable_text = fdp.ConsumeRandomLengthString(64);
                    child_to_move.text().set(movable_text.c_str());
                    
                    // Move the child from node1 to node2
                    pugi::xml_node moved_child = node2.append_move(child_to_move);
                }
            }
        }
    }
    
    // Step 6: Test attribute prepending: prepend_attribute
    if (test_attribute_ops && !attr_names.empty()) {
        // Test prepend_attribute on root node
        for (size_t i = 0; i < std::min(attr_names.size(), (size_t)3) && i < 3; i++) {
            if (!attr_names[i].empty()) {
                // Create attribute using prepend_attribute
                pugi::xml_attribute attr = root.prepend_attribute(attr_names[i].c_str());
                if (attr) {
                    attr.set_value(attr_values[i].c_str());
                }
            }
        }
        
        // Also test prepend_attribute on child nodes
        if (!created_nodes.empty() && !attr_names.empty()) {
            pugi::xml_node target_node = created_nodes[0];
            if (target_node) {
                // Use a different attribute for child node
                size_t attr_idx = std::min(attr_names.size() - 1, (size_t)1);
                pugi::xml_attribute child_attr = target_node.prepend_attribute(attr_names[attr_idx].c_str());
                if (child_attr) {
                    child_attr.set_value(attr_values[attr_idx].c_str());
                }
            }
        }
        
        // Test prepend_attribute with string_view if available
        #ifdef PUGIXML_HAS_STRING_VIEW
        if (!attr_names.empty()) {
            std::string_view attr_name_view(attr_names[0].c_str(), 
                                          std::min(attr_names[0].size(), (size_t)128));
            pugi::xml_attribute sv_attr = root.prepend_attribute(attr_name_view);
            if (sv_attr && !attr_values.empty()) {
                sv_attr.set_value(attr_values[0].c_str());
            }
        }
        #endif
    }
    
    // Additional testing based on test_mode
    switch (test_mode) {
        case 0:
            // Test copy operations more extensively
            if (created_nodes.size() >= 2) {
                // Copy node and then modify the copy
                pugi::xml_node original = created_nodes[0];
                pugi::xml_node copy = root.append_copy(original);
                if (copy && !node_names.empty() && node_names.size() > 1) {
                    copy.set_name(node_names[1].c_str());
                }
            }
            break;
            
        case 1:
            // Test move operations with different node types
            if (!created_nodes.empty()) {
                // Create a text node and move it
                pugi::xml_node text_node = root.append_child(pugi::node_pcdata);
                if (text_node) {
                    std::string text_value = fdp.ConsumeRandomLengthString(128);
                    text_node.set_value(text_value.c_str());
                    
                    // Create another node to move to
                    std::string target_name = fdp.ConsumeRandomLengthString(64);
                    pugi::xml_node target = root.append_child(target_name.c_str());
                    if (target) {
                        target.append_move(text_node);
                    }
                }
            }
            break;
            
        case 2:
            // Test attribute operations on copied/moved nodes
            if (created_nodes.size() >= 2) {
                pugi::xml_node node_to_copy = created_nodes[0];
                pugi::xml_node copy = root.prepend_copy(node_to_copy);
                
                if (copy && !attr_names.empty()) {
                    // Add attribute to the copied node
                    copy.append_attribute(attr_names[0].c_str()) = attr_values[0].c_str();
                    
                    // Also prepend attribute to the copy
                    if (attr_names.size() > 1) {
                        copy.prepend_attribute(attr_names[1].c_str()) = attr_values[1].c_str();
                    }
                }
            }
            break;
            
        case 3:
            // Test complex node manipulation scenarios
            // Create a chain of nodes and test various operations
            if (created_nodes.size() >= 3) {
                pugi::xml_node chain_root = created_nodes[0];
                pugi::xml_node middle_node = created_nodes[1];
                pugi::xml_node end_node = created_nodes[2];
                
                if (chain_root && middle_node && end_node) {
                    // Copy middle_node to chain_root
                    chain_root.append_copy(middle_node);
                    
                    // Move end_node to chain_root
                    chain_root.prepend_move(end_node);
                    
                    // Add attributes to all nodes
                    for (size_t i = 0; i < std::min(attr_names.size(), (size_t)2) && i < 2; i++) {
                        chain_root.prepend_attribute(attr_names[i].c_str()) = attr_values[i].c_str();
                    }
                }
            }
            break;
    }
    
    // Step 7: Clean up with reset
    doc.reset();
    
    return 0;
}

} // namespace harness_017
