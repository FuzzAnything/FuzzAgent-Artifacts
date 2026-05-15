#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <algorithm>
#include <string_view>

namespace harness_012 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for complex node insertion operations with string_view
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Test different node insertion scenarios with string_view variants
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Comprehensive node insertion operations with string_view
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Create initial structure with some nodes
            size_t initial_nodes = fdp.ConsumeIntegralInRange<size_t>(3, 8);
            std::vector<pugi::xml_node> initial_children;
            
            for (size_t i = 0; i < initial_nodes && fdp.remaining_bytes() > 20; ++i) {
                std::string node_name = "initial_" + std::to_string(i);
                pugi::xml_node child = root.append_child(node_name.c_str());
                
                // Add some text content
                std::string text = fdp.ConsumeRandomLengthString(32);
                child.append_child(pugi::node_pcdata).set_value(text.c_str());
                
                initial_children.push_back(child);
            }
            
            if (!initial_children.empty()) {
                // Test all string_view node insertion variants
                #ifdef PUGIXML_HAS_STRING_VIEW
                // Test append_child with string_view
                std::string append_name_str = fdp.ConsumeRandomLengthString(16);
                std::string_view append_name_sv(append_name_str);
                pugi::xml_node appended = root.append_child(append_name_sv);
                
                // Test prepend_child with string_view
                std::string prepend_name_str = fdp.ConsumeRandomLengthString(16);
                std::string_view prepend_name_sv(prepend_name_str);
                pugi::xml_node prepended = root.prepend_child(prepend_name_sv);
                
                // Test insert_child_after with string_view (insert after middle node)
                if (initial_children.size() > 1) {
                    pugi::xml_node& reference_node = initial_children[initial_children.size() / 2];
                    std::string insert_after_name_str = fdp.ConsumeRandomLengthString(16);
                    std::string_view insert_after_name_sv(insert_after_name_str);
                    pugi::xml_node inserted_after = root.insert_child_after(insert_after_name_sv, reference_node);
                }
                
                // Test insert_child_before with string_view (insert before first node)
                if (!initial_children.empty()) {
                    pugi::xml_node& first_child = initial_children[0];
                    std::string insert_before_name_str = fdp.ConsumeRandomLengthString(16);
                    std::string_view insert_before_name_sv(insert_before_name_str);
                    pugi::xml_node inserted_before = root.insert_child_before(insert_before_name_sv, first_child);
                }
                #endif
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Edge cases for string_view node insertion
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            #ifdef PUGIXML_HAS_STRING_VIEW
            // Test with empty string_view
            std::string_view empty_sv("");
            pugi::xml_node empty_named = root.append_child(empty_sv);
            
            // Test with very long string_view (edge case for buffer handling)
            std::string long_name(1024, 'X');
            std::string_view long_sv(long_name);
            pugi::xml_node long_named = root.append_child(long_sv);
            
            // Test with string_view from substring (partial string)
            std::string full_name = "prefix_middle_suffix";
            std::string_view partial_sv(full_name.data() + 7, 6); // "middle"
            pugi::xml_node partial_named = root.append_child(partial_sv);
            
            // Test insertion with null/invalid reference nodes
            pugi::xml_node invalid_node;
            std::string valid_name_str = fdp.ConsumeRandomLengthString(16);
            std::string_view valid_name_sv(valid_name_str);
            
            // Should handle gracefully
            pugi::xml_node result1 = root.insert_child_after(valid_name_sv, invalid_node);
            pugi::xml_node result2 = root.insert_child_before(valid_name_sv, invalid_node);
            
            // Test insertion with self as reference (should fail gracefully)
            if (root.first_child()) {
                pugi::xml_node self_ref = root.first_child();
                std::string self_name_str = fdp.ConsumeRandomLengthString(16);
                std::string_view self_name_sv(self_name_str);
                pugi::xml_node self_result = root.insert_child_after(self_name_sv, self_ref);
            }
            #endif
            
            break;
        }
        
        case 2: {
            // Scenario 2: Complex hierarchical insertion with string_view
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Build a multi-level tree
            size_t depth = fdp.ConsumeIntegralInRange<size_t>(2, 6);
            pugi::xml_node current = root;
            
            for (size_t level = 0; level < depth && fdp.remaining_bytes() > 30; ++level) {
                #ifdef PUGIXML_HAS_STRING_VIEW
                std::string level_name_str = "level_" + std::to_string(level);
                std::string_view level_name_sv(level_name_str);
                pugi::xml_node new_child = current.append_child(level_name_sv);
                
                // Add siblings at each level using string_view variants
                size_t sibling_count = fdp.ConsumeIntegralInRange<size_t>(1, 4);
                for (size_t s = 0; s < sibling_count && fdp.remaining_bytes() > 20; ++s) {
                    std::string sibling_name_str = "sibling_" + std::to_string(s);
                    std::string_view sibling_name_sv(sibling_name_str);
                    
                    uint8_t insert_method = fdp.ConsumeIntegral<uint8_t>() % 3;
                    switch (insert_method) {
                        case 0:
                            current.append_child(sibling_name_sv);
                            break;
                        case 1:
                            current.prepend_child(sibling_name_sv);
                            break;
                        case 2:
                            if (new_child) {
                                current.insert_child_after(sibling_name_sv, new_child);
                            }
                            break;
                    }
                }
                
                current = new_child;
                #endif
            }
            
            // Now insert nodes at various positions in the hierarchy
            #ifdef PUGIXML_HAS_STRING_VIEW
            pugi::xml_node traversal = root;
            size_t insert_count = fdp.ConsumeIntegralInRange<size_t>(1, 5);
            
            for (size_t i = 0; i < insert_count && traversal && fdp.remaining_bytes() > 20; ++i) {
                std::string insert_name_str = "inserted_" + std::to_string(i);
                std::string_view insert_name_sv(insert_name_str);
                
                // Insert at current level using different methods
                uint8_t method = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (method) {
                    case 0:
                        traversal.append_child(insert_name_sv);
                        break;
                    case 1:
                        traversal.prepend_child(insert_name_sv);
                        break;
                    case 2:
                        if (traversal.first_child()) {
                            traversal.insert_child_after(insert_name_sv, traversal.first_child());
                        }
                        break;
                    case 3:
                        if (traversal.first_child()) {
                            traversal.insert_child_before(insert_name_sv, traversal.first_child());
                        }
                        break;
                }
                
                // Move deeper in the tree if possible
                if (traversal.first_child()) {
                    traversal = traversal.first_child();
                }
            }
            #endif
            
            break;
        }
        
        case 3: {
            // Scenario 3: Mixed string_view and traditional API usage
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Create mixed structure using both traditional and string_view APIs
            size_t node_count = fdp.ConsumeIntegralInRange<size_t>(5, 15);
            
            for (size_t i = 0; i < node_count && fdp.remaining_bytes() > 25; ++i) {
                std::string node_name = "node_" + std::to_string(i);
                
                #ifdef PUGIXML_HAS_STRING_VIEW
                if (fdp.ConsumeBool()) {
                    // Use string_view API
                    std::string_view node_name_sv(node_name);
                    pugi::xml_node child = root.append_child(node_name_sv);
                    
                    // Add attributes using mixed APIs
                    size_t attr_count = fdp.ConsumeIntegralInRange<size_t>(0, 3);
                    for (size_t a = 0; a < attr_count && fdp.remaining_bytes() > 15; ++a) {
                        std::string attr_name = "attr_" + std::to_string(a);
                        std::string attr_value = fdp.ConsumeRandomLengthString(16);
                        
                        if (fdp.ConsumeBool()) {
                            // Use string_view for attribute
                            std::string_view attr_name_sv(attr_name);
                            std::string_view attr_value_sv(attr_value);
                            pugi::xml_attribute attr = child.append_attribute(attr_name_sv);
                            attr.set_value(attr_value_sv);
                        } else {
                            // Use traditional API
                            child.append_attribute(attr_name.c_str()) = attr_value.c_str();
                        }
                    }
                } else {
                    // Use traditional API
                    pugi::xml_node child = root.append_child(node_name.c_str());
                }
                #else
                pugi::xml_node child = root.append_child(node_name.c_str());
                #endif
            }
            
            // Now perform insertion operations on the mixed structure
            #ifdef PUGIXML_HAS_STRING_VIEW
            if (root.first_child() && root.first_child().next_sibling()) {
                pugi::xml_node first = root.first_child();
                pugi::xml_node second = first.next_sibling();
                
                // Insert using string_view between first and second
                std::string between_name = fdp.ConsumeRandomLengthString(16);
                std::string_view between_sv(between_name);
                root.insert_child_after(between_sv, first);
                
                // Insert using traditional API
                std::string before_name = fdp.ConsumeRandomLengthString(16);
                root.insert_child_before(before_name.c_str(), second);
                
                // Test remove_child with string_view
                std::string remove_target = between_name;
                std::string_view remove_sv(remove_target);
                root.remove_child(remove_sv);
            }
            #endif
            
            break;
        }
        
        case 4: {
            // Scenario 4: Error condition testing for string_view insertion
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            #ifdef PUGIXML_HAS_STRING_VIEW
            // Test with string_view that becomes invalid (dangling reference)
            std::string* temp_name = new std::string("temporary_name");
            std::string_view dangling_sv(*temp_name);
            
            // Create node with dangling_sv
            pugi::xml_node dangling_node = root.append_child(dangling_sv);
            
            // Delete the original string - string_view should still work if implementation copied it
            delete temp_name;
            
            // Try to use the node (implementation should have copied the string)
            if (dangling_node) {
                // Add a child to verify node is still valid
                std::string child_name = fdp.ConsumeRandomLengthString(16);
                std::string_view child_sv(child_name);
                dangling_node.append_child(child_sv);
            }
            
            // Test insertion with very short-lived string_view
            {
                std::string short_lived = "short_lived";
                std::string_view short_sv(short_lived);
                pugi::xml_node short_node = root.append_child(short_sv);
                // short_lived goes out of scope here
            }
            
            // Test insertion with null data pointer but zero length (edge case)
            std::string_view null_sv(nullptr, 0);
            pugi::xml_node null_node = root.append_child(null_sv);
            
            // Test insertion with string_view from fuzzer data directly
            if (fdp.remaining_bytes() > 10) {
                size_t sv_length = fdp.ConsumeIntegralInRange<size_t>(1, 20);
                std::vector<uint8_t> sv_data = fdp.ConsumeBytes<uint8_t>(sv_length);
                std::string_view direct_sv(reinterpret_cast<const char*>(sv_data.data()), sv_length);
                pugi::xml_node direct_node = root.append_child(direct_sv);
            }
            #endif
            
            break;
        }
        
        case 5: {
            // Scenario 5: Performance/Stress testing with many string_view insertions
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            #ifdef PUGIXML_HAS_STRING_VIEW
            size_t batch_size = fdp.ConsumeIntegralInRange<size_t>(10, 50);
            std::vector<pugi::xml_node> created_nodes;
            
            // Create many nodes quickly with string_view
            for (size_t i = 0; i < batch_size && fdp.remaining_bytes() > 10; ++i) {
                std::string node_name = "batch_" + std::to_string(i);
                std::string_view node_sv(node_name);
                pugi::xml_node node = root.append_child(node_sv);
                created_nodes.push_back(node);
            }
            
            // Now insert nodes between existing ones
            for (size_t i = 0; i + 1 < created_nodes.size() && fdp.remaining_bytes() > 10; i += 2) {
                std::string insert_name = "interleaved_" + std::to_string(i);
                std::string_view insert_sv(insert_name);
                
                pugi::xml_node inserted = root.insert_child_after(insert_sv, created_nodes[i]);
                
                if (inserted && fdp.remaining_bytes() > 10) {
                    // Add child to the inserted node
                    std::string child_name = "child_of_interleaved";
                    std::string_view child_sv(child_name);
                    inserted.append_child(child_sv);
                }
            }
            
            // Remove some nodes using string_view remove_child
            size_t remove_count = fdp.ConsumeIntegralInRange<size_t>(1, std::min(batch_size, size_t(5)));
            for (size_t i = 0; i < remove_count && !created_nodes.empty(); ++i) {
                std::string remove_name = "batch_" + std::to_string(i);
                std::string_view remove_sv(remove_name);
                root.remove_child(remove_sv);
            }
            #endif
            
            break;
        }
        
        case 6: {
            // Scenario 6: Document fragment insertion with string_view
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            // Create a separate document to copy from
            pugi::xml_document source_doc;
            pugi::xml_node source_root = source_doc.append_child("source");
            
            #ifdef PUGIXML_HAS_STRING_VIEW
            // Create nodes in source document with string_view
            size_t source_nodes = fdp.ConsumeIntegralInRange<size_t>(3, 8);
            for (size_t i = 0; i < source_nodes && fdp.remaining_bytes() > 15; ++i) {
                std::string src_name = "src_" + std::to_string(i);
                std::string_view src_sv(src_name);
                pugi::xml_node src_node = source_root.append_child(src_sv);
                
                // Add some content
                std::string src_text = fdp.ConsumeRandomLengthString(32);
                src_node.append_child(pugi::node_pcdata).set_value(src_text.c_str());
            }
            
            // Copy nodes from source to destination using string_view APIs
            if (source_root.first_child()) {
                pugi::xml_node current_src = source_root.first_child();
                while (current_src) {
                    // Use string_view for the copy operation if available
                    std::string copy_name = current_src.name();
                    std::string_view copy_sv(copy_name);
                    
                    // Append copy to destination
                    pugi::xml_node copied = root.append_child(copy_sv);
                    
                    // Copy attributes if any
                    for (pugi::xml_attribute attr = current_src.first_attribute(); attr; attr = attr.next_attribute()) {
                        std::string attr_name = attr.name();
                        std::string attr_value = attr.value();
                        std::string_view attr_name_sv(attr_name);
                        std::string_view attr_value_sv(attr_value);
                        
                        pugi::xml_attribute new_attr = copied.append_attribute(attr_name_sv);
                        new_attr.set_value(attr_value_sv);
                    }
                    
                    current_src = current_src.next_sibling();
                }
            }
            #endif
            
            break;
        }
        
        case 7: {
            // Scenario 7: Complex insertion patterns and validation
            pugi::xml_document doc;
            pugi::xml_node root = doc.append_child("root");
            
            #ifdef PUGIXML_HAS_STRING_VIEW
            // Create a pattern of insertions that tests ordering
            std::vector<std::string> names_to_insert;
            size_t pattern_size = fdp.ConsumeIntegralInRange<size_t>(5, 20);
            
            for (size_t i = 0; i < pattern_size && fdp.remaining_bytes() > 10; ++i) {
                names_to_insert.push_back("pattern_" + std::to_string(i));
            }
            
            // Insert in specific patterns
            for (size_t i = 0; i < names_to_insert.size(); ++i) {
                std::string_view name_sv(names_to_insert[i]);
                
                if (i == 0) {
                    // First element
                    root.append_child(name_sv);
                } else if (i == 1) {
                    // Second element at beginning
                    root.prepend_child(name_sv);
                } else {
                    // Insert relative to existing nodes
                    pugi::xml_node first = root.first_child();
                    pugi::xml_node last = root.last_child();
                    
                    if (first && last && first != last) {
                        if (i % 3 == 0) {
                            root.insert_child_after(name_sv, first);
                        } else if (i % 3 == 1) {
                            root.insert_child_before(name_sv, last);
                        } else {
                            // Find a middle node
                            pugi::xml_node middle = first;
                            for (size_t j = 0; j < i / 2 && middle.next_sibling(); ++j) {
                                middle = middle.next_sibling();
                            }
                            if (middle) {
                                root.insert_child_after(name_sv, middle);
                            }
                        }
                    }
                }
            }
            
            // Validate the structure by traversing
            std::vector<std::string> collected_names;
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                collected_names.push_back(child.name());
            }
            
            // Try some more insertions based on collected structure
            if (!collected_names.empty() && fdp.remaining_bytes() > 20) {
                std::string extra_name = "validation_insert";
                std::string_view extra_sv(extra_name);
                
                // Find a node by name and insert after it
                std::string target_name = collected_names[collected_names.size() / 2];
                std::string_view target_sv(target_name);
                
                pugi::xml_node target = root.child(target_sv);
                if (target) {
                    root.insert_child_after(extra_sv, target);
                }
            }
            #endif
            
            break;
        }
    }
    
    return 0;
}

} // namespace harness_012
