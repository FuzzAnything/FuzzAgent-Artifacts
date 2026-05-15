#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <string_view>

namespace harness_008 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for string_view operations
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Test different string_view API scenarios
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Test xml_attribute string_view APIs
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Create attributes using string_view constructors/operators
            size_t attr_count = fdp.ConsumeIntegralInRange<size_t>(1, 10);
            
            for (size_t i = 0; i < attr_count && fdp.remaining_bytes() > 20; ++i) {
                // Consume attribute name and value as strings
                std::string attr_name_str = fdp.ConsumeRandomLengthString(32);
                std::string attr_value_str = fdp.ConsumeRandomLengthString(128);
                
                // Convert to string_view for API calls
                std::string_view attr_name_sv(attr_name_str);
                std::string_view attr_value_sv(attr_value_str);
                
                // Test append_attribute with string_view
                pugi::xml_attribute attr = root.append_attribute(attr_name_sv);
                
                // Test set_value with string_view
                attr.set_value(attr_value_sv);
                
                // Test assignment operator with string_view
                if (fdp.ConsumeBool()) {
                    std::string new_value_str = fdp.ConsumeRandomLengthString(64);
                    std::string_view new_value_sv(new_value_str);
                    attr = new_value_sv;
                }
                
                // Test set_name with string_view
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
                    std::string new_name_str = fdp.ConsumeRandomLengthString(24);
                    std::string_view new_name_sv(new_name_str);
                    attr.set_name(new_name_sv);
                }
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Test xml_node string_view APIs for node creation
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Test various node creation methods with string_view
            size_t node_count = fdp.ConsumeIntegralInRange<size_t>(1, 8);
            
            for (size_t i = 0; i < node_count && fdp.remaining_bytes() > 30; ++i) {
                std::string node_name_str = fdp.ConsumeRandomLengthString(32);
                std::string_view node_name_sv(node_name_str);
                
                // Test different insertion methods with string_view
                uint8_t insert_method = fdp.ConsumeIntegral<uint8_t>() % 4;
                
                switch (insert_method) {
                    case 0:
                        root.append_child(node_name_sv);
                        break;
                    case 1:
                        root.prepend_child(node_name_sv);
                        break;
                    case 2: {
                        if (root.first_child()) {
                            root.insert_child_after(node_name_sv, root.first_child());
                        }
                        break;
                    }
                    case 3: {
                        if (root.first_child()) {
                            root.insert_child_before(node_name_sv, root.first_child());
                        }
                        break;
                    }
                }
            }
            
            // Test node search methods with string_view
            if (!root.first_child()) {
                // Add at least one child for searching
                std::string search_name_str = fdp.ConsumeRandomLengthString(16);
                std::string_view search_name_sv(search_name_str);
                root.append_child(search_name_sv);
            }
            
            // Test child() with string_view
            std::string child_name_str = fdp.ConsumeRandomLengthString(16);
            std::string_view child_name_sv(child_name_str);
            pugi::xml_node found_child = root.child(child_name_sv);
            
            // Test set_name and set_value with string_view on nodes
            if (root.first_child()) {
                pugi::xml_node first_child = root.first_child();
                
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 20) {
                    std::string new_node_name_str = fdp.ConsumeRandomLengthString(24);
                    std::string_view new_node_name_sv(new_node_name_str);
                    first_child.set_name(new_node_name_sv);
                }
                
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 30) {
                    std::string node_value_str = fdp.ConsumeRandomLengthString(100);
                    std::string_view node_value_sv(node_value_str);
                    first_child.set_value(node_value_sv);
                }
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Test xml_node string_view APIs for attribute manipulation
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Add some attributes first using traditional methods
            size_t initial_attrs = fdp.ConsumeIntegralInRange<size_t>(1, 5);
            for (size_t i = 0; i < initial_attrs && fdp.remaining_bytes() > 20; ++i) {
                std::string attr_name = "attr" + std::to_string(i);
                root.append_attribute(attr_name.c_str()) = "value";
            }
            
            // Test attribute search methods with string_view
            if (fdp.remaining_bytes() > 10) {
                std::string search_attr_str = fdp.ConsumeRandomLengthString(16);
                std::string_view search_attr_sv(search_attr_str);
                
                // Test attribute() with string_view
                pugi::xml_attribute found_attr = root.attribute(search_attr_sv);
                
                // Test attribute() with hint and string_view
                if (root.first_attribute()) {
                    pugi::xml_attribute hint = root.first_attribute();
                    pugi::xml_attribute found_with_hint = root.attribute(search_attr_sv, hint);
                }
            }
            
            // Test attribute insertion methods with string_view
            if (fdp.remaining_bytes() > 20) {
                std::string new_attr_name_str = fdp.ConsumeRandomLengthString(20);
                std::string_view new_attr_name_sv(new_attr_name_str);
                
                uint8_t attr_insert_method = fdp.ConsumeIntegral<uint8_t>() % 4;
                
                switch (attr_insert_method) {
                    case 0:
                        root.append_attribute(new_attr_name_sv);
                        break;
                    case 1:
                        root.prepend_attribute(new_attr_name_sv);
                        break;
                    case 2: {
                        if (root.first_attribute()) {
                            root.insert_attribute_after(new_attr_name_sv, root.first_attribute());
                        }
                        break;
                    }
                    case 3: {
                        if (root.first_attribute()) {
                            root.insert_attribute_before(new_attr_name_sv, root.first_attribute());
                        }
                        break;
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Test sibling navigation with string_view
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Create multiple sibling nodes with different names
            size_t sibling_count = fdp.ConsumeIntegralInRange<size_t>(2, 6);
            std::vector<std::string> sibling_names;
            
            for (size_t i = 0; i < sibling_count && fdp.remaining_bytes() > 20; ++i) {
                std::string sibling_name = "sibling" + std::to_string(i);
                sibling_names.push_back(sibling_name);
                root.append_child(sibling_name.c_str());
            }
            
            // Test next_sibling and previous_sibling with string_view
            if (!sibling_names.empty() && fdp.remaining_bytes() > 10) {
                // Pick a random sibling name to search for
                size_t search_idx = fdp.ConsumeIntegralInRange<size_t>(0, sibling_names.size() - 1);
                std::string_view search_name_sv(sibling_names[search_idx]);
                
                // Start from first child
                pugi::xml_node current = root.first_child();
                
                // Test next_sibling with string_view
                pugi::xml_node next_with_name = current.next_sibling(search_name_sv);
                
                // Test previous_sibling with string_view (need to move to a later node first)
                if (current.next_sibling()) {
                    pugi::xml_node second_node = current.next_sibling();
                    pugi::xml_node prev_with_name = second_node.previous_sibling(search_name_sv);
                }
            }
            break;
        }
        
        case 4: {
            // Scenario 4: Test xml_text string_view APIs
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Create xml_text object
            pugi::xml_text text_obj = root.append_child(pugi::node_pcdata).text();
            
            // Test xml_text string_view operations
            if (fdp.remaining_bytes() > 20) {
                std::string text_str = fdp.ConsumeRandomLengthString(256);
                std::string_view text_sv(text_str);
                
                // Test set() with string_view
                text_obj.set(text_sv);
                
                // Test assignment operator with string_view
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 30) {
                    std::string new_text_str = fdp.ConsumeRandomLengthString(128);
                    std::string_view new_text_sv(new_text_str);
                    text_obj = new_text_sv;
                }
            }
            break;
        }
        
        case 5: {
            // Scenario 5: Test removal operations with string_view
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Create nodes and attributes for removal testing
            size_t nodes_to_create = fdp.ConsumeIntegralInRange<size_t>(3, 8);
            std::vector<std::string> node_names;
            
            for (size_t i = 0; i < nodes_to_create && fdp.remaining_bytes() > 20; ++i) {
                std::string node_name = "node" + std::to_string(i);
                node_names.push_back(node_name);
                pugi::xml_node node = root.append_child(node_name.c_str());
                
                // Add attributes
                size_t attrs_per_node = fdp.ConsumeIntegralInRange<size_t>(1, 3);
                for (size_t j = 0; j < attrs_per_node && fdp.remaining_bytes() > 10; ++j) {
                    std::string attr_name = "attr" + std::to_string(j);
                    node.append_attribute(attr_name.c_str()) = "value";
                }
            }
            
            // Test remove_child with string_view
            if (!node_names.empty() && fdp.remaining_bytes() > 10) {
                size_t remove_idx = fdp.ConsumeIntegralInRange<size_t>(0, node_names.size() - 1);
                std::string_view remove_name_sv(node_names[remove_idx]);
                bool removed = root.remove_child(remove_name_sv);
            }
            
            // Test remove_attribute with string_view on remaining nodes
            if (root.first_child() && fdp.remaining_bytes() > 10) {
                pugi::xml_node first_child = root.first_child();
                if (first_child.first_attribute()) {
                    std::string attr_to_remove = "attr0";
                    std::string_view attr_to_remove_sv(attr_to_remove);
                    bool attr_removed = first_child.remove_attribute(attr_to_remove_sv);
                }
            }
            break;
        }
        
        case 6: {
            // Scenario 6: Test edge cases and boundary conditions for string_view
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Test empty string_view
            std::string_view empty_sv("");
            
            // Test with empty string_view - should handle gracefully
            pugi::xml_attribute attr = root.append_attribute(empty_sv);
            if (attr) {
                attr.set_value(empty_sv);
            }
            
            // Test with very short string_view
            if (fdp.remaining_bytes() > 5) {
                std::string short_str = fdp.ConsumeRandomLengthString(1);
                std::string_view short_sv(short_str);
                root.append_child(short_sv);
            }
            
            // Test with string_view containing special characters
            if (fdp.remaining_bytes() > 20) {
                std::string special_chars = fdp.ConsumeRandomLengthString(50);
                // Ensure we have some special characters
                for (char& c : special_chars) {
                    if (fdp.ConsumeBool()) {
                        c = static_cast<char>(fdp.ConsumeIntegralInRange<int>(1, 31));
                    }
                }
                std::string_view special_sv(special_chars);
                
                pugi::xml_node special_node = root.append_child(special_sv);
                if (special_node && fdp.remaining_bytes() > 10) {
                    std::string special_value = fdp.ConsumeRandomLengthString(30);
                    std::string_view special_value_sv(special_value);
                    special_node.set_value(special_value_sv);
                }
            }
            
            // Test interoperability between string_view and traditional string APIs
            if (fdp.remaining_bytes() > 30) {
                std::string interop_str = fdp.ConsumeRandomLengthString(40);
                std::string_view interop_sv(interop_str);
                
                // Create with string_view
                pugi::xml_node node1 = root.append_child(interop_sv);
                
                // Modify with traditional string API
                if (node1) {
                    std::string traditional_str = fdp.ConsumeRandomLengthString(20);
                    node1.set_value(traditional_str.c_str());
                    
                    // Then modify back with string_view
                    std::string new_sv_str = fdp.ConsumeRandomLengthString(25);
                    std::string_view new_sv(new_sv_str);
                    node1.set_value(new_sv);
                }
            }
            break;
        }
        
        case 7: {
            // Scenario 7: Test mixed usage of string_view and char* APIs
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Mix string_view and char* APIs in the same operations
            size_t mixed_ops = fdp.ConsumeIntegralInRange<size_t>(3, 7);
            
            for (size_t i = 0; i < mixed_ops && fdp.remaining_bytes() > 40; ++i) {
                std::string name_str = fdp.ConsumeRandomLengthString(24);
                std::string value_str = fdp.ConsumeRandomLengthString(80);
                
                // Sometimes use string_view, sometimes use char*
                if (fdp.ConsumeBool()) {
                    // Use string_view for name, char* for value
                    std::string_view name_sv(name_str);
                    pugi::xml_node node = root.append_child(name_sv);
                    if (node) {
                        node.set_value(value_str.c_str());
                    }
                } else {
                    // Use char* for name, string_view for value
                    pugi::xml_node node = root.append_child(name_str.c_str());
                    if (node) {
                        std::string_view value_sv(value_str);
                        node.set_value(value_sv);
                    }
                }
                
                // Add attributes with mixed API usage
                if (root.first_child() && fdp.remaining_bytes() > 20) {
                    pugi::xml_node child = root.first_child();
                    std::string attr_name_str = fdp.ConsumeRandomLengthString(16);
                    std::string attr_value_str = fdp.ConsumeRandomLengthString(40);
                    
                    if (fdp.ConsumeBool()) {
                        std::string_view attr_name_sv(attr_name_str);
                        pugi::xml_attribute attr = child.append_attribute(attr_name_sv);
                        if (attr) {
                            attr.set_value(attr_value_str.c_str());
                        }
                    } else {
                        pugi::xml_attribute attr = child.append_attribute(attr_name_str.c_str());
                        if (attr) {
                            std::string_view attr_value_sv(attr_value_str);
                            attr = attr_value_sv;
                        }
                    }
                }
            }
            
            // Test comparison between nodes/attributes created with different APIs
            if (root.first_child() && root.first_child().next_sibling()) {
                pugi::xml_node node1 = root.first_child();
                pugi::xml_node node2 = root.first_child().next_sibling();
                
                // Get names using both APIs for comparison
                const char* name1_char = node1.name();
                const char* name2_char = node2.name();
                
                // Could compare if we had the actual strings, but just accessing is fine
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_008
