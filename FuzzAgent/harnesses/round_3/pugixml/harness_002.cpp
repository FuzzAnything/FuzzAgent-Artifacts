#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>

namespace harness_002 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic operations
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create a base XML document
    pugi::xml_document doc;
    
    // Consume operation type to determine what tree manipulation to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 16;
    
    // Create a root element with some initial structure
    pugi::xml_node root = doc.append_child("root");
    
    // Add some initial children and attributes to work with
    size_t initial_children = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    std::vector<pugi::xml_node> child_nodes;
    
    for (size_t i = 0; i < initial_children && fdp.remaining_bytes() > 10; ++i) {
        std::string child_name = "child" + std::to_string(i);
        pugi::xml_node child = root.append_child(child_name.c_str());
        
        // Add some attributes
        size_t attr_count = fdp.ConsumeIntegralInRange<size_t>(0, 5);
        for (size_t j = 0; j < attr_count && fdp.remaining_bytes() > 5; ++j) {
            std::string attr_name = "attr" + std::to_string(j);
            std::string attr_value = fdp.ConsumeRandomLengthString(16);
            child.append_attribute(attr_name.c_str()) = attr_value.c_str();
        }
        
        // Add some text content
        std::string text_content = fdp.ConsumeRandomLengthString(64);
        child.append_child(pugi::node_pcdata).set_value(text_content.c_str());
        
        child_nodes.push_back(child);
    }
    
    // Now perform different tree manipulation operations based on consumed operation
    switch (operation) {
        case 0: {
            // Test node insertion operations (append, prepend, insert_after, insert_before)
            if (!child_nodes.empty()) {
                std::string new_node_name = fdp.ConsumeRandomLengthString(32);
                pugi::xml_node new_node = root.append_child(new_node_name.c_str());
                
                std::string prepend_name = fdp.ConsumeRandomLengthString(32);
                root.prepend_child(prepend_name.c_str());
                
                // Insert before/after existing nodes
                if (child_nodes.size() > 1) {
                    pugi::xml_node& middle_node = child_nodes[child_nodes.size() / 2];
                    std::string insert_after_name = fdp.ConsumeRandomLengthString(32);
                    root.insert_child_after(insert_after_name.c_str(), middle_node);
                    
                    std::string insert_before_name = fdp.ConsumeRandomLengthString(32);
                    root.insert_child_before(insert_before_name.c_str(), middle_node);
                }
            }
            break;
        }
        
        case 1: {
            // Test attribute manipulation operations
            if (!child_nodes.empty()) {
                pugi::xml_node& target_node = child_nodes[0];
                
                // Append attributes
                std::string attr1_name = fdp.ConsumeRandomLengthString(16);
                std::string attr1_value = fdp.ConsumeRandomLengthString(32);
                target_node.append_attribute(attr1_name.c_str()) = attr1_value.c_str();
                
                // Prepend attributes
                std::string attr2_name = fdp.ConsumeRandomLengthString(16);
                std::string attr2_value = fdp.ConsumeRandomLengthString(32);
                target_node.prepend_attribute(attr2_name.c_str()) = attr2_value.c_str();
                
                // Insert attributes after/before existing ones
                if (target_node.first_attribute()) {
                    pugi::xml_attribute first_attr = target_node.first_attribute();
                    std::string attr3_name = fdp.ConsumeRandomLengthString(16);
                    target_node.insert_attribute_after(attr3_name.c_str(), first_attr);
                    
                    std::string attr4_name = fdp.ConsumeRandomLengthString(16);
                    target_node.insert_attribute_before(attr4_name.c_str(), first_attr);
                }
            }
            break;
        }
        
        case 2: {
            // Test node removal operations
            if (!child_nodes.empty()) {
                // Remove specific child by name
                if (child_nodes.size() > 2) {
                    pugi::xml_node& to_remove = child_nodes[1];
                    root.remove_child(to_remove);
                }
                
                // Remove child by name string
                if (!child_nodes.empty()) {
                    std::string child_name_to_remove = fdp.ConsumeRandomLengthString(32);
                    root.remove_child(child_name_to_remove.c_str());
                }
                
                // Remove all children
                if (fdp.ConsumeBool() && child_nodes.size() > 3) {
                    root.remove_children();
                }
            }
            break;
        }
        
        case 3: {
            // Test attribute removal operations
            if (!child_nodes.empty()) {
                pugi::xml_node& target_node = child_nodes[0];
                
                // Remove attributes by name
                std::string attr_to_remove = fdp.ConsumeRandomLengthString(16);
                target_node.remove_attribute(attr_to_remove.c_str());
                
                // Remove all attributes
                if (fdp.ConsumeBool()) {
                    target_node.remove_attributes();
                }
            }
            break;
        }
        
        case 4: {
            // Test node copying operations (append_copy, prepend_copy, insert_copy_after/before)
            if (!child_nodes.empty() && child_nodes.size() > 1) {
                pugi::xml_node& source_node = child_nodes[0];
                pugi::xml_node& reference_node = child_nodes[1];
                
                // Append copy
                root.append_copy(source_node);
                
                // Prepend copy  
                root.prepend_copy(source_node);
                
                // Insert copy after/before
                root.insert_copy_after(source_node, reference_node);
                root.insert_copy_before(source_node, reference_node);
            }
            break;
        }
        
        case 5: {
            // Test attribute copying operations
            if (!child_nodes.empty()) {
                pugi::xml_node& source_node = child_nodes[0];
                pugi::xml_node& target_node = child_nodes.size() > 1 ? child_nodes[1] : child_nodes[0];
                
                if (source_node.first_attribute()) {
                    pugi::xml_attribute source_attr = source_node.first_attribute();
                    pugi::xml_attribute target_first_attr = target_node.first_attribute();
                    
                    if (target_first_attr) {
                        target_node.append_copy(source_attr);
                        target_node.prepend_copy(source_attr);
                        target_node.insert_copy_after(source_attr, target_first_attr);
                        target_node.insert_copy_before(source_attr, target_first_attr);
                    }
                }
            }
            break;
        }
        
        case 6: {
            // Test node moving operations (append_move, prepend_move, insert_move_after/before)
            if (child_nodes.size() > 2) {
                // Create a separate document to move nodes from
                pugi::xml_document doc2;
                pugi::xml_node root2 = doc2.append_child("root2");
                
                // Add some nodes to the second document
                for (size_t i = 0; i < 3 && fdp.remaining_bytes() > 10; ++i) {
                    std::string node_name = "movable" + std::to_string(i);
                    pugi::xml_node movable = root2.append_child(node_name.c_str());
                    movable.append_child(pugi::node_pcdata).set_value("movable content");
                }
                
                // Move nodes between documents
                if (root2.first_child()) {
                    pugi::xml_node first_movable = root2.first_child();
                    root.append_move(first_movable);
                    
                    if (root2.first_child()) {
                        pugi::xml_node second_movable = root2.first_child();
                        root.prepend_move(second_movable);
                    }
                }
            }
            break;
        }
        
        case 7: {
            // Test setting node names and values
            if (!child_nodes.empty()) {
                for (size_t i = 0; i < std::min(child_nodes.size(), size_t(3)); ++i) {
                    pugi::xml_node& node = child_nodes[i];
                    
                    // Set new name
                    std::string new_name = fdp.ConsumeRandomLengthString(32);
                    node.set_name(new_name.c_str());
                    
                    // Set new value
                    std::string new_value = fdp.ConsumeRandomLengthString(128);
                    node.set_value(new_value.c_str());
                    
                    // Try to set child value
                    std::string child_value = fdp.ConsumeRandomLengthString(64);
                    node.child_value(child_value.c_str());
                }
            }
            break;
        }
        
        case 8: {
            // Test complex tree restructuring - deep nesting
            pugi::xml_node current = root;
            size_t depth = fdp.ConsumeIntegralInRange<size_t>(1, 10);
            
            for (size_t i = 0; i < depth && fdp.remaining_bytes() > 5; ++i) {
                std::string level_name = "level" + std::to_string(i);
                current = current.append_child(level_name.c_str());
                
                // Add attributes at each level
                std::string attr_name = "depth";
                std::string attr_value = std::to_string(i);
                current.append_attribute(attr_name.c_str()) = attr_value.c_str();
                
                // Add text content
                std::string text = fdp.ConsumeRandomLengthString(32);
                current.append_child(pugi::node_pcdata).set_value(text.c_str());
            }
            break;
        }
        
        case 9: {
            // Test different node types (not just elements)
            pugi::xml_node comment_node = root.append_child(pugi::node_comment);
            comment_node.set_value(fdp.ConsumeRandomLengthString(64).c_str());
            
            pugi::xml_node pi_node = root.append_child(pugi::node_pi);
            pi_node.set_name("target");
            pi_node.set_value(fdp.ConsumeRandomLengthString(32).c_str());
            
            pugi::xml_node cdata_node = root.append_child(pugi::node_cdata);
            cdata_node.set_value(fdp.ConsumeRandomLengthString(128).c_str());
            
            pugi::xml_node doctype_node = root.append_child(pugi::node_doctype);
            doctype_node.set_value(fdp.ConsumeRandomLengthString(32).c_str());
            break;
        }
        
        case 10: {
            // Test tree traversal and modification during traversal
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                // Modify nodes during traversal
                std::string traversal_attr = "traversed";
                std::string traversal_value = "true";
                child.append_attribute(traversal_attr.c_str()) = traversal_value.c_str();
                
                // Remove every other node during traversal
                static bool remove_flag = false;
                if (remove_flag) {
                    root.remove_child(child);
                }
                remove_flag = !remove_flag;
            }
            break;
        }
        
        case 11: {
            // Test attribute finding and manipulation
            if (!child_nodes.empty()) {
                pugi::xml_node& target = child_nodes[0];
                
                // Add multiple attributes
                for (int i = 0; i < 5 && fdp.remaining_bytes() > 5; ++i) {
                    std::string attr_name = "findable" + std::to_string(i);
                    std::string attr_value = fdp.ConsumeRandomLengthString(16);
                    target.append_attribute(attr_name.c_str()) = attr_value.c_str();
                }
                
                // Find and modify attributes
                for (pugi::xml_attribute attr = target.first_attribute(); attr; attr = attr.next_attribute()) {
                    std::string current_name = attr.name();
                    if (current_name.find("findable") != std::string::npos) {
                        std::string new_value = "modified_" + fdp.ConsumeRandomLengthString(8);
                        attr.set_value(new_value.c_str());
                    }
                }
            }
            break;
        }
        
        case 12: {
            // Test document fragment parsing and appending
            std::string fragment_xml = fdp.ConsumeRandomLengthString(512);
            if (!fragment_xml.empty()) {
                root.append_buffer(fragment_xml.c_str(), fragment_xml.size(), pugi::parse_fragment);
            }
            break;
        }
        
        case 13: {
            // Test sibling and parent navigation with modification
            if (child_nodes.size() > 2) {
                pugi::xml_node& middle = child_nodes[child_nodes.size() / 2];
                
                // Navigate siblings
                pugi::xml_node prev_sibling = middle.previous_sibling();
                pugi::xml_node next_sibling = middle.next_sibling();
                
                // Modify siblings if they exist
                if (prev_sibling) {
                    prev_sibling.append_attribute("modified") = "prev";
                }
                if (next_sibling) {
                    next_sibling.append_attribute("modified") = "next";
                }
                
                // Navigate to parent and modify
                pugi::xml_node parent = middle.parent();
                if (parent) {
                    parent.append_attribute("child_modified") = "true";
                }
            }
            break;
        }
        
        case 14: {
            // Test error conditions and edge cases
            // Try to remove non-existent nodes/attributes
            std::string non_existent = "non_existent_" + fdp.ConsumeRandomLengthString(16);
            root.remove_child(non_existent.c_str());
            
            if (!child_nodes.empty()) {
                child_nodes[0].remove_attribute(non_existent.c_str());
            }
            
            // Try to append empty names
            root.append_child("");
            root.append_attribute("");
            
            // Try to set empty values
            if (!child_nodes.empty()) {
                child_nodes[0].set_name("");
                child_nodes[0].set_value("");
            }
            break;
        }
        
        case 15: {
            // Test complex document restructuring - move subtrees around
            if (child_nodes.size() > 3) {
                // Create a complex structure
                pugi::xml_node& subtree_root = child_nodes[0];
                
                // Add grandchild structure
                for (int i = 0; i < 3 && fdp.remaining_bytes() > 5; ++i) {
                    std::string gc_name = "grandchild" + std::to_string(i);
                    pugi::xml_node grandchild = subtree_root.append_child(gc_name.c_str());
                    grandchild.append_child(pugi::node_pcdata).set_value(fdp.ConsumeRandomLengthString(32).c_str());
                }
                
                // Move the entire subtree to another location
                if (child_nodes.size() > 1) {
                    pugi::xml_node& new_parent = child_nodes[1];
                    new_parent.append_move(subtree_root);
                    
                    // Now move it back
                    root.append_move(subtree_root);
                }
                
                // Test deep copying of complex structure
                if (child_nodes.size() > 2) {
                    pugi::xml_node& copy_target = child_nodes[2];
                    copy_target.append_copy(subtree_root);
                }
            }
            break;
        }
    }
    
    // Final traversal to ensure document integrity
    for (pugi::xml_node child = doc.first_child(); child; child = child.next_sibling()) {
        // Just access to trigger coverage
        child.name();
        child.value();
        
        for (pugi::xml_attribute attr = child.first_attribute(); attr; attr = attr.next_attribute()) {
            attr.name();
            attr.value();
        }
    }
    
    return 0;
}

} // namespace harness_002
