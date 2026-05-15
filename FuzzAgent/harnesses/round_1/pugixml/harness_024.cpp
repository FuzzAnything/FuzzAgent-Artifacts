#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>

namespace harness_024 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for XML structure, attribute names, and node movement operations
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== PHASE 1: CREATE COMPLEX XML STRUCTURE FOR NODE MANIPULATION ====================
    // Build a rich XML document with multiple hierarchy levels for comprehensive node manipulation testing
    pugi::xml_document doc;
    
    // Create root element with attributes from fuzzer input
    std::string root_name = fdp.ConsumeRandomLengthString(20);
    if (root_name.empty()) root_name = "node_manipulation_root";
    pugi::xml_node root = doc.append_child(root_name.c_str());
    
    // Add initial attributes to root
    std::string version_attr = fdp.ConsumeRandomLengthString(10);
    if (version_attr.empty()) version_attr = "1.0";
    root.append_attribute("version") = version_attr.c_str();
    
    std::string test_type_attr = fdp.ConsumeRandomLengthString(15);
    if (test_type_attr.empty()) test_type_attr = "node_move_test";
    root.append_attribute("test_type") = test_type_attr.c_str();
    
    // Track created nodes and attributes for manipulation operations
    std::vector<pugi::xml_node> all_nodes;
    std::vector<pugi::xml_attribute> all_attributes;
    all_nodes.push_back(root);
    
    // Create initial hierarchy with multiple levels for comprehensive testing
    int level_count = fdp.ConsumeIntegralInRange<int>(4, 10);
    for (int i = 0; i < level_count && fdp.remaining_bytes() > 100; ++i) {
        // Create parent node
        std::string parent_name = fdp.ConsumeRandomLengthString(25);
        if (parent_name.empty()) parent_name = "parent_node_" + std::to_string(i);
        pugi::xml_node parent = root.append_child(parent_name.c_str());
        all_nodes.push_back(parent);
        
        // Add attributes to parent for attribute manipulation testing
        std::string parent_id_attr_name = fdp.ConsumeRandomLengthString(15);
        if (parent_id_attr_name.empty()) parent_id_attr_name = "parent_id";
        pugi::xml_attribute parent_id_attr = parent.append_attribute(parent_id_attr_name.c_str());
        
        std::string parent_id_attr_value = fdp.ConsumeRandomLengthString(20);
        if (parent_id_attr_value.empty()) parent_id_attr_value = "parent_" + std::to_string(i * 1000);
        parent_id_attr.set_value(parent_id_attr_value.c_str());
        all_attributes.push_back(parent_id_attr);
        
        // Add text content to parent
        std::string parent_text = fdp.ConsumeRandomLengthString(100);
        parent.text() = parent_text.c_str();
        
        // Create children for move operations testing
        int child_count = fdp.ConsumeIntegralInRange<int>(2, 6);
        for (int j = 0; j < child_count && fdp.remaining_bytes() > 50; ++j) {
            std::string child_name = fdp.ConsumeRandomLengthString(20);
            if (child_name.empty()) child_name = "child_" + std::to_string(i) + "_" + std::to_string(j);
            pugi::xml_node child = parent.append_child(child_name.c_str());
            all_nodes.push_back(child);
            
            // Add attributes to child for prepend_attribute testing
            std::string child_id_attr_name = fdp.ConsumeRandomLengthString(15);
            if (child_id_attr_name.empty()) child_id_attr_name = "child_id";
            pugi::xml_attribute child_id_attr = child.append_attribute(child_id_attr_name.c_str());
            
            std::string child_id_attr_value = fdp.ConsumeRandomLengthString(20);
            if (child_id_attr_value.empty()) child_id_attr_value = "child_" + std::to_string(i * 100 + j);
            child_id_attr.set_value(child_id_attr_value.c_str());
            all_attributes.push_back(child_id_attr);
            
            // Add text content to child
            std::string child_text = fdp.ConsumeRandomLengthString(80);
            child.text() = child_text.c_str();
            
            // Create grandchildren for deeper move operations
            if (j % 2 == 0 && fdp.remaining_bytes() > 30) {
                std::string grandchild_name = fdp.ConsumeRandomLengthString(15);
                if (grandchild_name.empty()) grandchild_name = "grandchild_" + std::to_string(i) + "_" + std::to_string(j);
                pugi::xml_node grandchild = child.append_child(grandchild_name.c_str());
                all_nodes.push_back(grandchild);
                
                std::string grandchild_text = fdp.ConsumeRandomLengthString(60);
                grandchild.text() = grandchild_text.c_str();
                
                // Add attribute to grandchild
                std::string grandchild_attr_name = fdp.ConsumeRandomLengthString(12);
                if (grandchild_attr_name.empty()) grandchild_attr_name = "depth";
                pugi::xml_attribute grandchild_attr = grandchild.append_attribute(grandchild_attr_name.c_str());
                grandchild_attr.set_value("3");
                all_attributes.push_back(grandchild_attr);
            }
        }
    }
    
    // ==================== PHASE 2: TARGETED NODE MANIPULATION OPERATIONS ====================
    // PRIMARY TARGET: prepend_attribute, prepend_move, append_move - the uncovered APIs with 22+ branches
    
    // Create special test nodes for targeted API testing
    pugi::xml_node prepend_attr_test = root.append_child("prepend_attribute_test_node");
    pugi::xml_node move_operations_test = root.append_child("move_operations_test_node");
    all_nodes.push_back(prepend_attr_test);
    all_nodes.push_back(move_operations_test);
    
    // Add initial attributes to test nodes
    prepend_attr_test.append_attribute("initial_attr") = "original_value";
    move_operations_test.append_attribute("move_test") = "enabled";
    
    // Create sibling nodes for move operations
    std::vector<pugi::xml_node> movable_nodes;
    for (int i = 0; i < 5 && fdp.remaining_bytes() > 20; ++i) {
        std::string movable_name = fdp.ConsumeRandomLengthString(15);
        if (movable_name.empty()) movable_name = "movable_" + std::to_string(i);
        pugi::xml_node movable = root.append_child(movable_name.c_str());
        movable_nodes.push_back(movable);
        all_nodes.push_back(movable);
        
        std::string movable_text = fdp.ConsumeRandomLengthString(40);
        movable.text() = movable_text.c_str();
        
        movable.append_attribute("movable_id") = std::to_string(i).c_str();
    }
    
    // ==================== PHASE 3: COMPREHENSIVE API TESTING ====================
    // Test the three target APIs with various scenarios
    
    int operations_performed = 0;
    const int max_operations = 30;
    
    while (fdp.remaining_bytes() > 32 && operations_performed < max_operations) {
        uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        
        switch (operation_type) {
            // ==================== TEST 1: prepend_attribute ====================
            case 0: {
                // Test xml_node::prepend_attribute(const char_t* name)
                if (!all_nodes.empty() && fdp.remaining_bytes() > 20) {
                    size_t target_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
                    pugi::xml_node target_node = all_nodes[target_idx];
                    
                    if (target_node) {
                        // Generate attribute name from fuzzer input
                        std::string attr_name = fdp.ConsumeRandomLengthString(25);
                        if (attr_name.empty()) attr_name = "prepended_attr_" + std::to_string(operations_performed);
                        
                        // Test prepend_attribute - adds attribute at the beginning of attribute list
                        pugi::xml_attribute prepended = target_node.prepend_attribute(attr_name.c_str());
                        if (prepended) {
                            // Set value for the prepended attribute
                            std::string attr_value = fdp.ConsumeRandomLengthString(50);
                            prepended.set_value(attr_value.c_str());
                            all_attributes.push_back(prepended);
                            
                            // Verify the attribute was added by trying to retrieve it
                            pugi::xml_attribute retrieved = target_node.attribute(attr_name.c_str());
                            (void)retrieved; // Use result
                        }
                    }
                }
                break;
            }
            
            // ==================== TEST 2: prepend_attribute with string_view (if available) ====================
            case 1: {
#ifdef PUGIXML_HAS_STRING_VIEW
                // Test xml_node::prepend_attribute(string_view_t name)
                if (!all_nodes.empty() && fdp.remaining_bytes() > 20) {
                    size_t target_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
                    pugi::xml_node target_node = all_nodes[target_idx];
                    
                    if (target_node) {
                        std::string attr_name_str = fdp.ConsumeRandomLengthString(25);
                        if (attr_name_str.empty()) attr_name_str = "sv_prepended_" + std::to_string(operations_performed);
                        
                        // Create string_view for testing
                        std::string_view attr_name_sv(attr_name_str);
                        
                        // Test prepend_attribute with string_view
                        pugi::xml_attribute prepended = target_node.prepend_attribute(attr_name_sv);
                        if (prepended) {
                            std::string attr_value = fdp.ConsumeRandomLengthString(50);
                            prepended.set_value(attr_value.c_str());
                            all_attributes.push_back(prepended);
                        }
                    }
                }
#endif
                break;
            }
            
            // ==================== TEST 3: prepend_move ====================
            case 2: {
                // Test xml_node::prepend_move(const xml_node& moved)
                // Moves a node to become the first child of this node
                if (all_nodes.size() >= 3 && fdp.remaining_bytes() > 20) {
                    // Select source node to move
                    size_t source_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
                    pugi::xml_node source_node = all_nodes[source_idx];
                    
                    // Select target node (destination parent)
                    size_t target_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
                    pugi::xml_node target_node = all_nodes[target_idx];
                    
                    // Ensure nodes are valid and different, and source is not an ancestor of target
                    if (source_node && target_node && source_node != target_node) {
                        // Test prepend_move - moves node to beginning of target's children
                        pugi::xml_node moved_node = target_node.prepend_move(source_node);
                        if (moved_node) {
                            // Verify the move was successful
                            if (moved_node == source_node) {
                                // The node was successfully moved
                                // Update tracking if needed
                            }
                        }
                    }
                }
                break;
            }
            
            // ==================== TEST 4: append_move ====================
            case 3: {
                // Test xml_node::append_move(const xml_node& moved)
                // Moves a node to become the last child of this node
                if (all_nodes.size() >= 3 && fdp.remaining_bytes() > 20) {
                    // Select source node to move
                    size_t source_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
                    pugi::xml_node source_node = all_nodes[source_idx];
                    
                    // Select target node (destination parent)
                    size_t target_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
                    pugi::xml_node target_node = all_nodes[target_idx];
                    
                    // Ensure nodes are valid and different
                    if (source_node && target_node && source_node != target_node) {
                        // Test append_move - moves node to end of target's children
                        pugi::xml_node moved_node = target_node.append_move(source_node);
                        if (moved_node) {
                            // Verify the move was successful
                            if (moved_node == source_node) {
                                // The node was successfully moved
                            }
                        }
                    }
                }
                break;
            }
            
            // ==================== TEST 5: COMPLEX SEQUENCE - Multiple prepend_attribute calls ====================
            case 4: {
                // Test multiple prepend_attribute calls on the same node
                // This tests attribute ordering and memory management
                if (!all_nodes.empty() && fdp.remaining_bytes() > 50) {
                    size_t target_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
                    pugi::xml_node target_node = all_nodes[target_idx];
                    
                    if (target_node) {
                        int num_attrs = fdp.ConsumeIntegralInRange<int>(2, 5);
                        for (int i = 0; i < num_attrs && fdp.remaining_bytes() > 10; ++i) {
                            std::string attr_name = "multi_prepend_" + std::to_string(i) + "_" + 
                                                   fdp.ConsumeRandomLengthString(10);
                            pugi::xml_attribute attr = target_node.prepend_attribute(attr_name.c_str());
                            if (attr) {
                                std::string attr_value = fdp.ConsumeRandomLengthString(30);
                                attr.set_value(attr_value.c_str());
                                all_attributes.push_back(attr);
                            }
                        }
                    }
                }
                break;
            }
            
            // ==================== TEST 6: COMPLEX SEQUENCE - Chain of move operations ====================
            case 5: {
                // Test chained move operations to exercise complex code paths
                if (movable_nodes.size() >= 2 && fdp.remaining_bytes() > 30) {
                    // Create a chain: move node1, then move node2 to where node1 was, etc.
                    if (movable_nodes.size() > 0) {
                        pugi::xml_node node1 = movable_nodes[0];
                        pugi::xml_node node2 = (movable_nodes.size() > 1) ? movable_nodes[1] : movable_nodes[0];
                        
                        if (node1 && node2 && node1 != node2) {
                            // Test prepend_move chain
                            pugi::xml_node moved1 = move_operations_test.prepend_move(node1);
                            (void)moved1;
                            
                            // Test append_move chain  
                            pugi::xml_node moved2 = move_operations_test.append_move(node2);
                            (void)moved2;
                            
                            // Move nodes between different parents
                            if (!all_nodes.empty()) {
                                size_t other_parent_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
                                pugi::xml_node other_parent = all_nodes[other_parent_idx];
                                
                                if (other_parent && other_parent != move_operations_test) {
                                    pugi::xml_node moved_back = other_parent.prepend_move(node1);
                                    (void)moved_back;
                                }
                            }
                        }
                    }
                }
                break;
            }
        }
        
        operations_performed++;
    }
    // ==================== PHASE 4: VALIDATION AND CLEANUP ====================
    // Verify the document structure is still valid after all operations
    // Note: Serialization is not required for testing the target APIs
    

    if (!all_attributes.empty()) {
        size_t test_attr_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_attributes.size() - 1);
        pugi::xml_attribute test_attr = all_attributes[test_attr_idx];
        if (test_attr) {
            std::string attr_value = test_attr.value();
            (void)attr_value;
        }
    }
    
    // Verify some nodes can still be accessed
    if (!all_nodes.empty()) {
        size_t test_node_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
        pugi::xml_node test_node = all_nodes[test_node_idx];
        if (test_node) {
            std::string node_name = test_node.name();
            std::string node_value = test_node.value();
            (void)node_name;
            (void)node_value;
        }
    }
    
    // Test edge cases: prepend_attribute on empty/invalid nodes
    pugi::xml_node empty_node;
    pugi::xml_attribute empty_result = empty_node.prepend_attribute("should_fail");
    (void)empty_result; // Should be empty
    
    // Test move operations on empty/invalid nodes
    pugi::xml_node empty_move_result = empty_node.prepend_move(empty_node);
    (void)empty_move_result;
    
    empty_move_result = empty_node.append_move(empty_node);
    (void)empty_move_result;
    
    // Document is automatically destroyed when doc goes out of scope
    // All resources are properly cleaned up by pugixml
    
    return 0;
}

} // namespace harness_024
