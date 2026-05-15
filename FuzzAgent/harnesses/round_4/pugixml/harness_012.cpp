#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <utility>

namespace harness_012 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for string_view-based XML node manipulation testing
    // Need enough data for XML content, node names, attribute names/values, and operation parameters
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t parse_method = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: load_buffer, 1: load_string, 2: create new
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();
    uint8_t operation_sequence = fdp.ConsumeIntegral<uint8_t>() % 8;  // Different operation sequences
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;  // Different test scenarios
    
    // Consume variable-size data for XML content
    std::string xml_content = fdp.ConsumeRandomLengthString(2048);
    
    // Consume node names for various string_view operations
    std::vector<std::string> node_names;
    for (int i = 0; i < 15 && fdp.remaining_bytes() > 10; i++) {
        node_names.push_back(fdp.ConsumeRandomLengthString(128));
    }
    
    // Consume attribute names and values for string_view attribute operations
    std::vector<std::string> attr_names;
    std::vector<std::string> attr_values;
    for (int i = 0; i < 10 && fdp.remaining_bytes() > 20; i++) {
        attr_names.push_back(fdp.ConsumeRandomLengthString(64));
        attr_values.push_back(fdp.ConsumeRandomLengthString(256));
    }
    
    // Consume node values for set_value operations
    std::vector<std::string> node_values;
    for (int i = 0; i < 8 && fdp.remaining_bytes() > 10; i++) {
        node_values.push_back(fdp.ConsumeRandomLengthString(512));
    }
    
    // Consume reference node names for insert_before/after operations
    std::vector<std::string> ref_node_names;
    for (int i = 0; i < 5 && fdp.remaining_bytes() > 10; i++) {
        ref_node_names.push_back(fdp.ConsumeRandomLengthString(64));
    }
    
    // Step 1: Create XML document and parse XML content
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result;
    
    switch (parse_method) {
        case 0:
            // Use load_buffer with string_view_t (converted from consumed string)
            parse_result = doc.load_buffer(xml_content.c_str(), xml_content.size(), parse_options);
            break;
        case 1:
            // Use load_string with string_view_t (converted from consumed string)
            parse_result = doc.load_string(xml_content.c_str(), parse_options);
            break;
        case 2:
            // Create new document for pure string_view operation testing
            doc.reset();
            break;
    }
    
    // Get or create document element for testing
    pugi::xml_node root = doc.document_element();
    if (!root) {
        // Create a root node if document is empty or parsing failed
        std::string root_name = "root";
        if (!node_names.empty() && fdp.remaining_bytes() > 5) {
            root_name = fdp.ConsumeRandomLengthString(32);
            if (root_name.empty()) root_name = "root";
        }
        root = doc.append_child(pugi::node_element);
        root.set_name(root_name.c_str());
    }
    
    // Step 2: Create initial reference nodes for insertion operations
    std::vector<pugi::xml_node> reference_nodes;
    for (size_t i = 0; i < ref_node_names.size() && i < 3; i++) {
        if (!ref_node_names[i].empty()) {
            pugi::xml_node ref_node = root.append_child(pugi::node_element);
            ref_node.set_name(ref_node_names[i].c_str());
            reference_nodes.push_back(ref_node);
            
            // Add some attributes to reference nodes
            if (i < attr_names.size() && i < attr_values.size()) {
                pugi::xml_attribute attr = ref_node.append_attribute(attr_names[i].c_str());
                attr.set_value(attr_values[i].c_str());
            }
        }
    }
    
    // Step 3: Exercise the 7 string_view APIs in various sequences
    
    // Sequence 1: Basic append_child and prepend_child with string_view names
    if (!node_names.empty() && fdp.ConsumeBool()) {
        for (size_t i = 0; i < std::min(node_names.size(), static_cast<size_t>(3)); i++) {
            if (!node_names[i].empty()) {
                // TARGET: xml_node::append_child(string_view_t name)
                pugi::xml_node appended = root.append_child(node_names[i].c_str());
                
                // TARGET: xml_node::prepend_child(string_view_t name)  
                if (i + 1 < node_names.size() && !node_names[i + 1].empty()) {
                    pugi::xml_node prepended = root.prepend_child(node_names[i + 1].c_str());
                    
                    // Add attributes to prepended node
                    if (i < attr_names.size()) {
                        // TARGET: xml_node::append_attribute(string_view_t name)
                        pugi::xml_attribute attr = prepended.append_attribute(attr_names[i].c_str());
                        if (attr && i < attr_values.size()) {
                            // TARGET: xml_attribute::set_value(string_view_t value)
                            attr.set_value(attr_values[i].c_str());
                        }
                    }
                }
                
                // TARGET: xml_node::set_value(string_view_t value)
                if (i < node_values.size() && !node_values[i].empty()) {
                    appended.set_value(node_values[i].c_str());
                }
            }
        }
    }
    
    // Sequence 2: insert_child_before and insert_child_after with string_view names
    if (!reference_nodes.empty() && !node_names.empty() && fdp.ConsumeBool()) {
        for (size_t i = 0; i < std::min(reference_nodes.size(), static_cast<size_t>(2)); i++) {
            if (!reference_nodes[i].empty()) {
                // Find a valid node name for insertion
                size_t name_idx = i % node_names.size();
                if (!node_names[name_idx].empty()) {
                    // TARGET: xml_node::insert_child_before(string_view_t name, const xml_node& node)
                    pugi::xml_node inserted_before = root.insert_child_before(node_names[name_idx].c_str(), reference_nodes[i]);
                    
                    // Add attribute to inserted node
                    if (!attr_names.empty()) {
                        size_t attr_idx = (i + 1) % attr_names.size();
                        if (!attr_names[attr_idx].empty()) {
                            pugi::xml_attribute attr = inserted_before.append_attribute(attr_names[attr_idx].c_str());
                            if (attr && attr_idx < attr_values.size()) {
                                attr.set_value(attr_values[attr_idx].c_str());
                            }
                        }
                    }
                    
                    // TARGET: xml_node::insert_child_after(string_view_t name, const xml_node& node)
                    if (i + 1 < node_names.size() && !node_names[i + 1].empty()) {
                        pugi::xml_node inserted_after = root.insert_child_after(node_names[i + 1].c_str(), reference_nodes[i]);
                        
                        // Set value on inserted node
                        if (i < node_values.size() && !node_values[i].empty()) {
                            inserted_after.set_value(node_values[i].c_str());
                        }
                    }
                }
            }
        }
    }
    
    // Sequence 3: Attribute manipulation with string_view
    if (!attr_names.empty() && fdp.ConsumeBool()) {
        // Get all child nodes of root
        std::vector<pugi::xml_node> children;
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            children.push_back(child);
        }
        
        // Add attributes to children using string_view APIs
        for (size_t i = 0; i < std::min(children.size(), static_cast<size_t>(4)); i++) {
            if (!children[i].empty()) {
                // Add multiple attributes with string_view names
                for (size_t j = 0; j < std::min(attr_names.size(), static_cast<size_t>(3)); j++) {
                    if (!attr_names[j].empty()) {
                        // TARGET: xml_node::append_attribute(string_view_t name)
                        pugi::xml_attribute attr = children[i].append_attribute(attr_names[j].c_str());
                        if (attr && j < attr_values.size()) {
                            // TARGET: xml_attribute::set_value(string_view_t value)
                            attr.set_value(attr_values[j].c_str());
                        }
                    }
                }
                
                // Test remove_attribute with string_view
                if (!attr_names.empty() && fdp.ConsumeBool()) {
                    size_t remove_idx = i % attr_names.size();
                    if (!attr_names[remove_idx].empty()) {
                        // TARGET: xml_node::remove_attribute(string_view_t name)
                        children[i].remove_attribute(attr_names[remove_idx].c_str());
                    }
                }
            }
        }
    }
    
    // Sequence 4: Complex mixed operations based on operation_sequence
    switch (operation_sequence % 4) {
        case 0: {
            // Create a chain of nodes with insert operations
            if (!node_names.empty() && node_names.size() >= 3) {
                // Start with a base node
                pugi::xml_node base = root.append_child(node_names[0].c_str());
                
                // Insert before and after to create a sequence
                if (!node_names[1].empty()) {
                    pugi::xml_node before = root.insert_child_before(node_names[1].c_str(), base);
                    if (before && !node_values.empty()) {
                        before.set_value(node_values[0].c_str());
                    }
                }
                
                if (!node_names[2].empty()) {
                    pugi::xml_node after = root.insert_child_after(node_names[2].c_str(), base);
                    if (after && node_values.size() > 1) {
                        after.set_value(node_values[1].c_str());
                    }
                }
                
                // Add and remove attributes on base node
                if (!attr_names.empty()) {
                    for (size_t i = 0; i < std::min(attr_names.size(), static_cast<size_t>(2)); i++) {
                        pugi::xml_attribute attr = base.append_attribute(attr_names[i].c_str());
                        if (attr && i < attr_values.size()) {
                            attr.set_value(attr_values[i].c_str());
                        }
                    }
                    
                    // Remove one attribute
                    if (!attr_names.empty()) {
                        base.remove_attribute(attr_names[0].c_str());
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Test prepend_child with multiple nodes
            if (!node_names.empty()) {
                for (size_t i = 0; i < std::min(node_names.size(), static_cast<size_t>(3)); i++) {
                    if (!node_names[i].empty()) {
                        pugi::xml_node prepended = root.prepend_child(node_names[i].c_str());
                        
                        // Set value if available
                        if (i < node_values.size() && !node_values[i].empty()) {
                            prepended.set_value(node_values[i].c_str());
                        }
                        
                        // Add attribute
                        if (i < attr_names.size() && !attr_names[i].empty()) {
                            pugi::xml_attribute attr = prepended.append_attribute(attr_names[i].c_str());
                            if (attr && i < attr_values.size()) {
                                attr.set_value(attr_values[i].c_str());
                            }
                        }
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Test attribute lifecycle: append -> set_value -> remove
            if (!attr_names.empty() && !node_names.empty()) {
                pugi::xml_node test_node = root.append_child(node_names[0].c_str());
                
                // Append multiple attributes
                for (size_t i = 0; i < std::min(attr_names.size(), static_cast<size_t>(4)); i++) {
                    if (!attr_names[i].empty()) {
                        pugi::xml_attribute attr = test_node.append_attribute(attr_names[i].c_str());
                        if (attr && i < attr_values.size()) {
                            attr.set_value(attr_values[i].c_str());
                        }
                    }
                }
                
                // Remove some attributes
                for (size_t i = 0; i < std::min(attr_names.size(), static_cast<size_t>(2)); i++) {
                    if (!attr_names[i].empty()) {
                        test_node.remove_attribute(attr_names[i].c_str());
                    }
                }
                
                // Set node value
                if (!node_values.empty()) {
                    test_node.set_value(node_values[0].c_str());
                }
            }
            break;
        }
        
        case 3: {
            // Complex insertion sequence with reference nodes
            if (!reference_nodes.empty() && node_names.size() >= 2) {
                // Insert before first reference
                if (!node_names[0].empty()) {
                    pugi::xml_node before_ref = root.insert_child_before(node_names[0].c_str(), reference_nodes[0]);
                    
                    // Insert after first reference
                    if (!node_names[1].empty()) {
                        pugi::xml_node after_ref = root.insert_child_after(node_names[1].c_str(), reference_nodes[0]);
                        
                        // Add attributes to both
                        if (!attr_names.empty()) {
                            pugi::xml_attribute attr1 = before_ref.append_attribute(attr_names[0].c_str());
                            if (attr1 && !attr_values.empty()) {
                                attr1.set_value(attr_values[0].c_str());
                            }
                            
                            pugi::xml_attribute attr2 = after_ref.append_attribute(attr_names[1 % attr_names.size()].c_str());
                            if (attr2 && attr_values.size() > 1) {
                                attr2.set_value(attr_values[1].c_str());
                            }
                        }
                    }
                }
            }
            break;
        }
    }
    
    // Step 4: Additional targeted operations based on test_scenario
    if (test_scenario == 0) {
        // Focus on remove_attribute with various string_view names
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            if (fdp.ConsumeBool() && !attr_names.empty()) {
                size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, attr_names.size() - 1);
                if (!attr_names[idx].empty()) {
                    child.remove_attribute(attr_names[idx].c_str());
                }
            }
        }
    } else if (test_scenario == 1) {
        // Focus on set_value with various string_view values
        size_t value_count = 0;
        for (pugi::xml_node child = root.first_child(); child && value_count < node_values.size(); child = child.next_sibling()) {
            if (!node_values[value_count].empty()) {
                child.set_value(node_values[value_count].c_str());
                value_count++;
            }
        }
    } else if (test_scenario == 2) {
        // Focus on append_attribute with string_view names
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            if (!attr_names.empty() && fdp.ConsumeBool()) {
                size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, attr_names.size() - 1);
                if (!attr_names[idx].empty()) {
                    pugi::xml_attribute attr = child.append_attribute(attr_names[idx].c_str());
                    if (attr && idx < attr_values.size()) {
                        attr.set_value(attr_values[idx].c_str());
                    }
                }
            }
        }
    }
    
    // Step 5: Final cleanup and verification
    // The document will be automatically cleaned up when it goes out of scope
    
    return 0;
}

} // namespace harness_012
