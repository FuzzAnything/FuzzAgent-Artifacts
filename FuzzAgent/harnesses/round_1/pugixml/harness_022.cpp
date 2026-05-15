#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

namespace harness_022 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for XML structure, traversal operations, and iterator testing
    if (size < 512) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== PHASE 1: CREATE COMPLEX XML STRUCTURE FOR TRAVERSAL TESTING ====================
    // Build a rich XML document with multiple hierarchy levels, diverse attributes, and varied content
    // to test comprehensive node search, traversal, and iterator operations
    pugi::xml_document doc;
    
    // Create root element with attributes
    pugi::xml_node root = doc.append_child("traversal_test_root");
    root.append_attribute("version") = "2.0";
    root.append_attribute("iterator_test") = "true";
    root.append_attribute("depth") = "0";
    
    // Track created nodes for iterator testing
    std::vector<pugi::xml_node> all_nodes;
    std::vector<pugi::xml_attribute> all_attributes;
    all_nodes.push_back(root);
    
    // Create multiple levels of hierarchy for comprehensive traversal testing
    int level1_count = fdp.ConsumeIntegralInRange<int>(5, 15);
    for (int i = 0; i < level1_count && fdp.remaining_bytes() > 100; ++i) {
        // Generate node name and attributes from fuzzer input
        std::string level1_name = "level1_node_" + std::to_string(i);
        std::string level1_attr_name = fdp.ConsumeRandomLengthString(20);
        std::string level1_attr_value = fdp.ConsumeRandomLengthString(40);
        
        pugi::xml_node level1_node = root.append_child(level1_name.c_str());
        all_nodes.push_back(level1_node);
        
        // Add diverse attributes for attribute-based searching and iterator testing
        pugi::xml_attribute id_attr = level1_node.append_attribute("id");
        id_attr.set_value(("id_" + std::to_string(i * 1000)).c_str());
        all_attributes.push_back(id_attr);
        
        pugi::xml_attribute type_attr = level1_node.append_attribute("type");
        std::string type_value = fdp.ConsumeBool() ? "even" : "odd";
        type_attr.set_value(type_value.c_str());
        all_attributes.push_back(type_attr);
        
        pugi::xml_attribute custom_attr = level1_node.append_attribute(level1_attr_name.c_str());
        custom_attr.set_value(level1_attr_value.c_str());
        all_attributes.push_back(custom_attr);
        
        // Add text content for child_value testing
        std::string level1_text = fdp.ConsumeRandomLengthString(200);
        level1_node.text() = level1_text.c_str();
        
        // Create second level children for sibling navigation testing
        int level2_count = fdp.ConsumeIntegralInRange<int>(3, 10);
        for (int j = 0; j < level2_count && fdp.remaining_bytes() > 50; ++j) {
            std::string level2_name = "level2_node_" + std::to_string(i) + "_" + std::to_string(j);
            pugi::xml_node level2_node = level1_node.append_child(level2_name.c_str());
            all_nodes.push_back(level2_node);
            
            // Add attributes with patterns for find_child_by_attribute testing
            pugi::xml_attribute level2_id = level2_node.append_attribute("node_id");
            level2_id.set_value(("node_" + std::to_string(i * 100 + j)).c_str());
            all_attributes.push_back(level2_id);
            
            // Add special attributes for targeted search
            if (j % 3 == 0) {
                pugi::xml_attribute special_attr = level2_node.append_attribute("special");
                special_attr.set_value("true");
                all_attributes.push_back(special_attr);
            }
            
            if (j % 4 == 0) {
                pugi::xml_attribute flag_attr = level2_node.append_attribute("flags");
                flag_attr.set_value("enabled");
                all_attributes.push_back(flag_attr);
            }
            
            // Add text content with specific patterns
            std::string level2_text = fdp.ConsumeRandomLengthString(150);
            level2_node.text() = level2_text.c_str();
            
            // Create third level for deep traversal testing
            if (j % 2 == 0 && fdp.remaining_bytes() > 30) {
                std::string level3_name = "level3_node_" + std::to_string(i) + "_" + std::to_string(j);
                pugi::xml_node level3_node = level2_node.append_child(level3_name.c_str());
                all_nodes.push_back(level3_node);
                
                pugi::xml_attribute depth_attr = level3_node.append_attribute("depth");
                depth_attr.set_value("3");
                all_attributes.push_back(depth_attr);
                
                pugi::xml_attribute final_attr = level3_node.append_attribute("final");
                final_attr.set_value("yes");
                all_attributes.push_back(final_attr);
                
                std::string level3_text = fdp.ConsumeRandomLengthString(100);
                level3_node.text() = level3_text.c_str();
            }
        }
    }
    
    // ==================== PHASE 2: COMPREHENSIVE ITERATOR OPERATIONS TESTING ====================
    // TARGET: xml_node_iterator, xml_attribute_iterator, xml_named_node_iterator operations
    
    // Test 1: Standard xml_node_iterator for child traversal
    if (!all_nodes.empty()) {
        for (size_t i = 0; i < std::min(all_nodes.size(), size_t(5)); ++i) {
            pugi::xml_node current_node = all_nodes[i];
            
            // Test xml_node_iterator (standard child iterator)
            pugi::xml_node_iterator child_begin = current_node.begin();
            pugi::xml_node_iterator child_end = current_node.end();
            
            // Forward traversal with iterator
            for (pugi::xml_node_iterator it = child_begin; it != child_end; ++it) {
                pugi::xml_node child = *it;
                (void)child.name();
                (void)child.value();
                (void)child.type();
                
                // Test child_value() functions
                const char* child_val = child.child_value();
                (void)child_val;
                
                // Test child_value with name
                std::string search_name = fdp.ConsumeRandomLengthString(20);
                const char* named_child_val = child.child_value(search_name.c_str());
                (void)named_child_val;
            }
            
            // Reverse traversal if we have at least 2 children
            if (child_begin != child_end) {
                pugi::xml_node_iterator last = child_end;
                --last; // Move to last element
                
                // Can also test -- operator
                pugi::xml_node_iterator it = child_end;
                if (it != child_begin) {
                    --it; // Test prefix decrement
                    pugi::xml_node_iterator it2 = it--; // Test postfix decrement
                    (void)it2;
                }
            }
        }
    }
    
    // Test 2: xml_attribute_iterator for attribute traversal
    for (size_t i = 0; i < std::min(all_nodes.size(), size_t(5)); ++i) {
        pugi::xml_node current_node = all_nodes[i];
        
        // Test attribute iterators
        pugi::xml_attribute_iterator attr_begin = current_node.attributes_begin();
        pugi::xml_attribute_iterator attr_end = current_node.attributes_end();
        
        // Forward traversal of attributes
        for (pugi::xml_attribute_iterator it = attr_begin; it != attr_end; ++it) {
            pugi::xml_attribute attr = *it;
            (void)attr.name();
            (void)attr.value();
            (void)attr.empty();
            
            // Test comparison operators
            if (it != attr_begin) {
                pugi::xml_attribute_iterator prev = it;
                --prev;
                bool not_equal = (it != prev);
                bool equal = (it == it);
                (void)not_equal;
                (void)equal;
            }
        }
        
        // Test reverse attribute traversal
        if (attr_begin != attr_end) {
            pugi::xml_attribute_iterator attr_it = attr_end;
            --attr_it; // Move to last attribute
            (void)attr_it;
        }
    }
    
    // Test 3: xml_named_node_iterator for named child traversal
    // Create test nodes with specific names for named iterator testing
    std::vector<std::string> target_names = {"target_a", "target_b", "target_c", "special_node", "iterator_test"};
    
    for (const auto& name : target_names) {
        // Add some nodes with these names
        for (int i = 0; i < 3 && !all_nodes.empty(); ++i) {
            size_t parent_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
            pugi::xml_node parent = all_nodes[parent_idx];
            pugi::xml_node named_node = parent.append_child(name.c_str());
            all_nodes.push_back(named_node);
            
            named_node.append_attribute("named_iter_test") = "true";
            std::string node_text = fdp.ConsumeRandomLengthString(50);
            named_node.text() = node_text.c_str();
        }
    }
    
    // Now test xml_named_node_iterator
    for (const auto& name : target_names) {
        pugi::xml_object_range<pugi::xml_named_node_iterator> named_children = root.children(name.c_str());
        
        pugi::xml_named_node_iterator named_begin = named_children.begin();
        pugi::xml_named_node_iterator named_end = named_children.end();
        
        // Forward traversal with named iterator
        for (pugi::xml_named_node_iterator it = named_begin; it != named_end; ++it) {
            pugi::xml_node named_node = *it;
            (void)named_node.name();
            (void)named_node.child_value();
            
            // Test iterator operations
            if (it != named_begin) {
                pugi::xml_named_node_iterator prev = it;
                --prev;
                (void)prev;
            }
        }
        
        // Test reverse traversal if we have multiple named nodes
        if (named_begin != named_end) {
            pugi::xml_named_node_iterator last = named_end;
            --last;
            
            // Test iterator movement
            pugi::xml_named_node_iterator it = named_begin;
            while (it != named_end) {
                ++it;
            }
        }
    }
    
    // ==================== PHASE 3: NODE SEARCH AND TRAVERSAL OPERATIONS ====================
    // TARGET: find_child_by_attribute, child_value, sibling navigation functions
    
    // Test 1: find_child_by_attribute operations
    while (fdp.remaining_bytes() > 100) {
        uint8_t search_op = fdp.ConsumeIntegral<uint8_t>() % 8;
        
        switch (search_op) {
            case 0: {
                // Test find_child_by_attribute with name and attribute
                std::string node_name = fdp.ConsumeRandomLengthString(30);
                std::string attr_name = fdp.ConsumeRandomLengthString(25);
                std::string attr_value = fdp.ConsumeRandomLengthString(35);
                
                pugi::xml_node found = root.find_child_by_attribute(node_name.c_str(), attr_name.c_str(), attr_value.c_str());
                (void)found;
                break;
            }
            
            case 1: {
                // Test find_child_by_attribute with just attribute name/value (any node)
                std::string attr_name = fdp.ConsumeRandomLengthString(25);
                std::string attr_value = fdp.ConsumeRandomLengthString(35);
                
                pugi::xml_node found = root.find_child_by_attribute(attr_name.c_str(), attr_value.c_str());
                (void)found;
                break;
            }
            
            case 2: {
                // Test child_value retrieval
                if (!all_nodes.empty()) {
                    size_t node_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
                    pugi::xml_node test_node = all_nodes[node_idx];
                    
                    const char* child_val = test_node.child_value();
                    (void)child_val;
                    
                    // Test child_value with specific name
                    std::string child_name = fdp.ConsumeRandomLengthString(20);
                    const char* named_child_val = test_node.child_value(child_name.c_str());
                    (void)named_child_val;
                }
                break;
            }
            
            case 3: {
                // Test next_sibling() navigation
                if (!all_nodes.empty()) {
                    size_t node_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
                    pugi::xml_node start_node = all_nodes[node_idx];
                    
                    pugi::xml_node sibling = start_node;
                    int max_siblings = fdp.ConsumeIntegralInRange<int>(1, 10);
                    
                    for (int i = 0; i < max_siblings && sibling; ++i) {
                        sibling = sibling.next_sibling();
                        if (sibling) {
                            (void)sibling.name();
                            (void)sibling.child_value();
                        }
                    }
                }
                break;
            }
            
            case 4: {
                // Test previous_sibling() navigation
                if (!all_nodes.empty()) {
                    size_t node_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
                    pugi::xml_node start_node = all_nodes[node_idx];
                    
                    pugi::xml_node sibling = start_node;
                    int max_siblings = fdp.ConsumeIntegralInRange<int>(1, 10);
                    
                    for (int i = 0; i < max_siblings && sibling; ++i) {
                        sibling = sibling.previous_sibling();
                        if (sibling) {
                            (void)sibling.name();
                            (void)sibling.child_value();
                        }
                    }
                }
                break;
            }
            
            case 5: {
                // Test next_sibling(name) with specific name
                if (!all_nodes.empty()) {
                    size_t node_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
                    pugi::xml_node start_node = all_nodes[node_idx];
                    
                    std::string target_name = fdp.ConsumeRandomLengthString(25);
                    pugi::xml_node named_sibling = start_node.next_sibling(target_name.c_str());
                    
                    // Continue finding next siblings with same name
                    while (named_sibling) {
                        (void)named_sibling.name();
                        (void)named_sibling.child_value();
                        named_sibling = named_sibling.next_sibling(target_name.c_str());
                    }
                }
                break;
            }
            
            case 6: {
                // Test previous_sibling(name) with specific name
                if (!all_nodes.empty()) {
                    size_t node_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
                    pugi::xml_node start_node = all_nodes[node_idx];
                    
                    std::string target_name = fdp.ConsumeRandomLengthString(25);
                    pugi::xml_node named_sibling = start_node.previous_sibling(target_name.c_str());
                    
                    // Continue finding previous siblings with same name
                    while (named_sibling) {
                        (void)named_sibling.name();
                        (void)named_sibling.child_value();
                        named_sibling = named_sibling.previous_sibling(target_name.c_str());
                    }
                }
                break;
            }
            
            case 7: {
                // Test iterator edge cases: empty ranges, single element ranges
                // Create a node with no children and test its iterators
                pugi::xml_node empty_node = root.append_child("empty_test_node");
                pugi::xml_node_iterator empty_begin = empty_node.begin();
                pugi::xml_node_iterator empty_end = empty_node.end();
                
                // These should be equal for empty node
                bool empty_equal = (empty_begin == empty_end);
                (void)empty_equal;
                
                // Test attribute iterator on node with no attributes
                pugi::xml_attribute_iterator empty_attr_begin = empty_node.attributes_begin();
                pugi::xml_attribute_iterator empty_attr_end = empty_node.attributes_end();
                bool attr_empty_equal = (empty_attr_begin == empty_attr_end);
                (void)attr_empty_equal;
                
                break;
            }
        }
        
        // Break early if we're running low on input
        if (fdp.remaining_bytes() < 50) {
            break;
        }
    }
    
    // ==================== PHASE 4: ADVANCED ITERATOR PATTERNS AND EDGE CASES ====================
    
    // Test STL algorithm compatibility with pugixml iterators
    if (!all_nodes.empty() && fdp.remaining_bytes() > 50) {
        // Test std::distance with node iterators
        size_t node_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
        pugi::xml_node test_node = all_nodes[node_idx];
        
        pugi::xml_node_iterator node_begin = test_node.begin();
        pugi::xml_node_iterator node_end = test_node.end();
        
        // Calculate distance (number of children)
        std::iterator_traits<pugi::xml_node_iterator>::difference_type child_count = 
            std::distance(node_begin, node_end);
        (void)child_count;
        
        // Test std::advance
        if (node_begin != node_end) {
            pugi::xml_node_iterator advanced = node_begin;
            size_t advance_count = fdp.ConsumeIntegralInRange<size_t>(0, 5);
            std::advance(advanced, std::min(advance_count, static_cast<size_t>(child_count)));
            (void)advanced;
        }
    }
    
    // Test range-based for loops (C++11 style)
    // Note: This tests the iterator interface compatibility
    if (!all_nodes.empty()) {
        size_t node_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
        pugi::xml_node test_node = all_nodes[node_idx];
        
        // This simulates range-based for: for (auto& child : node.children())
        for (pugi::xml_node_iterator it = test_node.begin(); it != test_node.end(); ++it) {
            pugi::xml_node child = *it;
            (void)child.name();
        }
        
        // Test attribute range-based iteration
        for (pugi::xml_attribute_iterator it = test_node.attributes_begin(); 
             it != test_node.attributes_end(); ++it) {
            pugi::xml_attribute attr = *it;
            (void)attr.name();
        }
    }
    
    // Test iterator copying and assignment
    if (!all_nodes.empty()) {
        pugi::xml_node test_node = all_nodes[0];
        pugi::xml_node_iterator it1 = test_node.begin();
        pugi::xml_node_iterator it2 = it1; // Copy constructor
        pugi::xml_node_iterator it3;
        it3 = it2; // Assignment operator
        
        // Test comparison of copied iterators
        bool copied_equal = (it1 == it2);
        bool assigned_equal = (it2 == it3);
        (void)copied_equal;
        (void)assigned_equal;
    }
    
    // ==================== PHASE 5: CLEANUP AND VALIDATION ====================
    
    // Final validation: ensure all iterators and nodes are in valid state
    // by performing some final traversals and checks
    
    // Quick final traversal of document structure
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        (void)node.name();
        
        // Final attribute traversal
        for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
            (void)attr.name();
            (void)attr.value();
        }
        
        // Final child traversal
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            (void)child.name();
            (void)child.child_value();
        }
    }
    
    return 0;
}

} // namespace harness_022
