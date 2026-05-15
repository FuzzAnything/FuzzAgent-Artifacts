#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <functional>

namespace harness_010 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for complex navigation operations
    if (size < 128) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create a complex XML document with hierarchical structure for navigation testing
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Build a complex tree structure with multiple levels and branches
    size_t max_depth = fdp.ConsumeIntegralInRange<size_t>(2, 8);
    size_t max_width = fdp.ConsumeIntegralInRange<size_t>(2, 10);
    
    std::vector<pugi::xml_node> all_nodes;
    all_nodes.push_back(root);
    
    // Create nodes with different names and attributes for searching
    std::vector<std::string> node_names;
    std::vector<std::string> attr_names;
    std::vector<std::string> attr_values;
    
    // Generate some unique names and values for searching
    for (size_t i = 0; i < 10 && fdp.remaining_bytes() > 20; ++i) {
        node_names.push_back(fdp.ConsumeRandomLengthString(16));
        attr_names.push_back(fdp.ConsumeRandomLengthString(12));
        attr_values.push_back(fdp.ConsumeRandomLengthString(32));
    }
    
    // Build tree structure
    std::function<void(pugi::xml_node, size_t)> build_tree = 
        [&](pugi::xml_node parent, size_t depth) {
            if (depth >= max_depth) return;
            
            size_t child_count = fdp.ConsumeIntegralInRange<size_t>(1, max_width);
            for (size_t i = 0; i < child_count && fdp.remaining_bytes() > 30; ++i) {
                // Choose node name
                std::string node_name;
                if (!node_names.empty() && fdp.ConsumeBool()) {
                    size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, node_names.size() - 1);
                    node_name = node_names[idx];
                } else {
                    node_name = "node_" + std::to_string(depth) + "_" + std::to_string(i);
                }
                
                pugi::xml_node child = parent.append_child(node_name.c_str());
                all_nodes.push_back(child);
                
                // Add attributes
                size_t attr_count = fdp.ConsumeIntegralInRange<size_t>(0, 5);
                for (size_t a = 0; a < attr_count && fdp.remaining_bytes() > 15; ++a) {
                    std::string attr_name;
                    std::string attr_value;
                    
                    if (!attr_names.empty() && fdp.ConsumeBool()) {
                        size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, attr_names.size() - 1);
                        attr_name = attr_names[idx];
                    } else {
                        attr_name = "attr_" + std::to_string(a);
                    }
                    
                    if (!attr_values.empty() && fdp.ConsumeBool()) {
                        size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, attr_values.size() - 1);
                        attr_value = attr_values[idx];
                    } else {
                        attr_value = "value_" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
                    }
                    
                    child.append_attribute(attr_name.c_str()) = attr_value.c_str();
                }
                
                // Add text content
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
                    std::string text = fdp.ConsumeRandomLengthString(64);
                    child.append_child(pugi::node_pcdata).set_value(text.c_str());
                }
                
                // Recursively build deeper levels
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 50) {
                    build_tree(child, depth + 1);
                }
            }
        };
    
    build_tree(root, 0);
    
    // Test 1: Basic child navigation
    if (!all_nodes.empty()) {
        size_t test_node_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
        pugi::xml_node test_node = all_nodes[test_node_idx];
        
        // Test first_child() and last_child()
        pugi::xml_node first_child = test_node.first_child();
        pugi::xml_node last_child = test_node.last_child();
        
        // Test child navigation with specific names
        if (!node_names.empty()) {
            size_t name_idx = fdp.ConsumeIntegralInRange<size_t>(0, node_names.size() - 1);
            pugi::xml_node named_child = test_node.child(node_names[name_idx].c_str());
            
            #ifdef PUGIXML_HAS_STRING_VIEW
            std::string_view name_sv(node_names[name_idx]);
            pugi::xml_node named_child_sv = test_node.child(name_sv);
            #endif
        }
    }
    
    // Test 2: Sibling navigation
    for (size_t i = 0; i < std::min<size_t>(all_nodes.size(), 10) && fdp.remaining_bytes() > 20; ++i) {
        pugi::xml_node node = all_nodes[i];
        
        // Test next_sibling() and previous_sibling()
        pugi::xml_node next_sib = node.next_sibling();
        pugi::xml_node prev_sib = node.previous_sibling();
        
        // Test named sibling navigation
        if (!node_names.empty() && next_sib) {
            size_t name_idx = fdp.ConsumeIntegralInRange<size_t>(0, node_names.size() - 1);
            pugi::xml_node named_next = node.next_sibling(node_names[name_idx].c_str());
            
            #ifdef PUGIXML_HAS_STRING_VIEW
            std::string_view name_sv(node_names[name_idx]);
            pugi::xml_node named_next_sv = node.next_sibling(name_sv);
            #endif
        }
        
        if (!node_names.empty() && prev_sib) {
            size_t name_idx = fdp.ConsumeIntegralInRange<size_t>(0, node_names.size() - 1);
            pugi::xml_node named_prev = node.previous_sibling(node_names[name_idx].c_str());
            
            #ifdef PUGIXML_HAS_STRING_VIEW
            std::string_view name_sv(node_names[name_idx]);
            pugi::xml_node named_prev_sv = node.previous_sibling(name_sv);
            #endif
        }
    }
    
    // Test 3: Parent and root navigation
    for (size_t i = 0; i < std::min<size_t>(all_nodes.size(), 10) && fdp.remaining_bytes() > 10; ++i) {
        pugi::xml_node node = all_nodes[i];
        
        pugi::xml_node parent = node.parent();
        pugi::xml_node doc_root = node.root();
        
        // Test navigation chains
        if (parent) {
            pugi::xml_node grandparent = parent.parent();
            pugi::xml_node parent_first_child = parent.first_child();
            pugi::xml_node parent_last_child = parent.last_child();
        }
    }
    
    // Test 4: find_child() with predicates
    if (!all_nodes.empty() && fdp.remaining_bytes() > 30) {
        size_t search_node_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
        pugi::xml_node search_node = all_nodes[search_node_idx];
        
        // Predicate 1: Find child with specific name pattern
        std::string search_pattern = fdp.ConsumeRandomLengthString(8);
        auto name_contains_pred = [&search_pattern](pugi::xml_node node) {
            const char* name = node.name();
            return name && strstr(name, search_pattern.c_str()) != nullptr;
        };
        pugi::xml_node found_by_name = search_node.find_child(name_contains_pred);
        
        // Predicate 2: Find child with attributes
        auto has_attributes_pred = [](pugi::xml_node node) {
            return node.first_attribute();
        };
        pugi::xml_node found_with_attrs = search_node.find_child(has_attributes_pred);
        
        // Predicate 3: Find child with text content
        auto has_text_pred = [](pugi::xml_node node) {
            return node.child_value() && node.child_value()[0] != '\0';
        };
        pugi::xml_node found_with_text = search_node.find_child(has_text_pred);
    }
    
    // Test 5: find_child_by_attribute() - important search function
    if (!all_nodes.empty() && !attr_names.empty() && !attr_values.empty() && fdp.remaining_bytes() > 40) {
        size_t search_node_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
        pugi::xml_node search_node = all_nodes[search_node_idx];
        
        size_t attr_name_idx = fdp.ConsumeIntegralInRange<size_t>(0, attr_names.size() - 1);
        size_t attr_value_idx = fdp.ConsumeIntegralInRange<size_t>(0, attr_values.size() - 1);
        
        // Test two-parameter version (attribute name and value)
        pugi::xml_node found_by_attr = search_node.find_child_by_attribute(
            attr_names[attr_name_idx].c_str(), 
            attr_values[attr_value_idx].c_str()
        );
        
        // Test three-parameter version (node name, attribute name, attribute value)
        if (!node_names.empty()) {
            size_t node_name_idx = fdp.ConsumeIntegralInRange<size_t>(0, node_names.size() - 1);
            pugi::xml_node found_by_name_and_attr = search_node.find_child_by_attribute(
                node_names[node_name_idx].c_str(),
                attr_names[attr_name_idx].c_str(),
                attr_values[attr_value_idx].c_str()
            );
        }
        
        // Test with empty/null strings
        search_node.find_child_by_attribute("", "");
        search_node.find_child_by_attribute("nonexistent", "nonexistent");
        
        if (!node_names.empty()) {
            search_node.find_child_by_attribute(node_names[0].c_str(), "", "");
        }
    }
    
    // Test 6: find_node() for depth-first search in subtree
    if (!all_nodes.empty() && fdp.remaining_bytes() > 30) {
        size_t start_node_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
        pugi::xml_node start_node = all_nodes[start_node_idx];
        
        // Predicate for depth-first search
        std::string search_text = fdp.ConsumeRandomLengthString(6);
        auto text_contains_pred = [&search_text](pugi::xml_node node) {
            const char* value = node.child_value();
            return value && strstr(value, search_text.c_str()) != nullptr;
        };
        
        pugi::xml_node found_deep = start_node.find_node(text_contains_pred);
        
        // Another predicate: node with specific attribute
        if (!attr_names.empty()) {
            size_t attr_idx = fdp.ConsumeIntegralInRange<size_t>(0, attr_names.size() - 1);
            auto has_specific_attr_pred = [&attr_names, attr_idx](pugi::xml_node node) {
                return node.attribute(attr_names[attr_idx].c_str());
            };
            
            pugi::xml_node found_with_specific_attr = start_node.find_node(has_specific_attr_pred);
        }
    }
    
    // Test 7: Complex navigation patterns
    if (!all_nodes.empty() && fdp.remaining_bytes() > 50) {
        size_t start_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
        pugi::xml_node current = all_nodes[start_idx];
        
        // Perform a random walk through the tree
        size_t steps = fdp.ConsumeIntegralInRange<size_t>(1, 20);
        for (size_t step = 0; step < steps && current && fdp.remaining_bytes() > 5; ++step) {
            uint8_t direction = fdp.ConsumeIntegral<uint8_t>() % 8;
            
            switch (direction) {
                case 0: // Move to parent
                    current = current.parent();
                    break;
                case 1: // Move to first child
                    current = current.first_child();
                    break;
                case 2: // Move to last child
                    current = current.last_child();
                    break;
                case 3: // Move to next sibling
                    current = current.next_sibling();
                    break;
                case 4: // Move to previous sibling
                    current = current.previous_sibling();
                    break;
                case 5: // Move to root
                    current = current.root();
                    break;
                case 6: // Move to named child
                    if (!node_names.empty()) {
                        size_t name_idx = fdp.ConsumeIntegralInRange<size_t>(0, node_names.size() - 1);
                        current = current.child(node_names[name_idx].c_str());
                    }
                    break;
                case 7: // Move to named next sibling
                    if (!node_names.empty() && current) {
                        size_t name_idx = fdp.ConsumeIntegralInRange<size_t>(0, node_names.size() - 1);
                        current = current.next_sibling(node_names[name_idx].c_str());
                    }
                    break;
            }
            
            // If we moved to an empty node, try to recover
            if (!current) {
                // Try to go back to a valid node
                if (step > 0) {
                    // Simplified: just break out
                    break;
                }
            }
        }
    }
    
    // Test 8: Child value accessors
    for (size_t i = 0; i < std::min<size_t>(all_nodes.size(), 5) && fdp.remaining_bytes() > 10; ++i) {
        pugi::xml_node node = all_nodes[i];
        
        // Test child_value() methods
        const char* child_val = node.child_value();
        
        if (!node_names.empty()) {
            size_t name_idx = fdp.ConsumeIntegralInRange<size_t>(0, node_names.size() - 1);
            const char* named_child_val = node.child_value(node_names[name_idx].c_str());
        }
    }
    
    // Test 9: Edge cases with empty nodes
    pugi::xml_node empty_node;
    empty_node.child("test");
    empty_node.first_child();
    empty_node.last_child();
    empty_node.next_sibling();
    empty_node.previous_sibling();
    empty_node.parent();
    empty_node.root();
    
    // Test empty node with find methods
    auto always_true_pred = [](pugi::xml_node) { return true; };
    empty_node.find_child(always_true_pred);
    empty_node.find_node(always_true_pred);
    empty_node.find_child_by_attribute("test", "value");
    empty_node.find_child_by_attribute("node", "attr", "value");
    
    // Test 10: Path-based navigation (if not in NO_STL mode)
    #ifndef PUGIXML_NO_STL
    if (!all_nodes.empty() && fdp.remaining_bytes() > 20) {
        size_t node_idx = fdp.ConsumeIntegralInRange<size_t>(0, all_nodes.size() - 1);
        pugi::xml_node path_node = all_nodes[node_idx];
        
        // Get node path
        char delimiter = fdp.ConsumeBool() ? '/' : '.';
        std::string node_path = path_node.path(delimiter);
        
        // Use path to find element
        if (!node_path.empty()) {
            pugi::xml_node found_by_path = root.first_element_by_path(node_path.c_str(), delimiter);
            
            // Test with random path strings
            std::string random_path = fdp.ConsumeRandomLengthString(64);
            root.first_element_by_path(random_path.c_str(), delimiter);
            
            // Test with relative paths
            std::string relative_path = "./../";
            if (!node_names.empty()) {
                relative_path += node_names[0];
            }
            path_node.first_element_by_path(relative_path.c_str(), delimiter);
        }
    }
    #endif
    
    return 0;
}

} // namespace harness_010
