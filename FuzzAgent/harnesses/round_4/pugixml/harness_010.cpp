#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>

namespace harness_010 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for node manipulation testing
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t parse_method = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: load_buffer, 1: load_string, 2: load_buffer with remaining bytes
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 8;  // Different operation sequences
    uint8_t data_type_selection = fdp.ConsumeIntegral<uint8_t>() % 16;
    
    // Consume variable-size data for XML content
    std::string xml_content = fdp.ConsumeRandomLengthString(4096);
    
    // Consume node names for various operations
    std::vector<std::string> node_names;
    for (int i = 0; i < 10 && fdp.remaining_bytes() > 10; i++) {
        node_names.push_back(fdp.ConsumeRandomLengthString(64));
    }
    
    // Consume attribute names and values
    std::vector<std::string> attr_names;
    std::vector<std::string> attr_values;
    for (int i = 0; i < 8 && fdp.remaining_bytes() > 20; i++) {
        attr_names.push_back(fdp.ConsumeRandomLengthString(64));
        attr_values.push_back(fdp.ConsumeRandomLengthString(128));
    }
    
    // Consume text content for node text operations
    std::vector<std::string> text_contents;
    for (int i = 0; i < 5 && fdp.remaining_bytes() > 10; i++) {
        text_contents.push_back(fdp.ConsumeRandomLengthString(256));
    }
    
    // Step 1: Create XML document and parse XML content
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result;
    
    switch (parse_method) {
        case 0:
            parse_result = doc.load_buffer(xml_content.c_str(), xml_content.size(), parse_options);
            break;
        case 1:
            parse_result = doc.load_string(xml_content.c_str(), parse_options);
            break;
        case 2: {
            std::vector<uint8_t> buffer = fdp.ConsumeRemainingBytes<uint8_t>();
            if (!buffer.empty()) {
                parse_result = doc.load_buffer(buffer.data(), buffer.size(), parse_options);
            }
            break;
        }
    }
    
    // If parsing failed, create a structured document for node manipulation testing
    if (!parse_result) {
        doc.reset();
        
        // Use fuzzer input for root name
        std::string root_name = "root";  // Default
        if (fdp.remaining_bytes() > 5) {
            root_name = fdp.ConsumeRandomLengthString(32);
            if (root_name.empty()) root_name = "root";
        }
        
        pugi::xml_node root = doc.append_child(root_name.c_str());
        
        // Create initial document structure with multiple nodes and attributes
        for (size_t i = 0; i < node_names.size() && i < 5; i++) {
            if (!node_names[i].empty()) {
                pugi::xml_node child = root.append_child(node_names[i].c_str());
                
                // Add attributes
                if (i < attr_names.size() && i < attr_values.size() && 
                    !attr_names[i].empty() && !attr_values[i].empty()) {
                    child.append_attribute(attr_names[i].c_str()) = attr_values[i].c_str();
                }
                
                // Add text content
                if (i < text_contents.size() && !text_contents[i].empty()) {
                    child.text().set(text_contents[i].c_str());
                }
                
                // Add nested children for more complex manipulation
                if (i % 2 == 0 && !node_names.empty()) {
                    std::string nested_name = "nested_" + node_names[i];
                    pugi::xml_node nested = child.append_child(nested_name.c_str());
                    
                    // Use fuzzer input for level attribute value
                    std::string level_value = std::to_string(fdp.ConsumeIntegralInRange<int>(1, 10));
                    nested.append_attribute("level") = level_value.c_str();
                }
            }
        }
    }
    
    // Get document root for manipulation
    pugi::xml_node root = doc.document_element();
    if (!root) {
        root = doc.first_child();
        if (!root) {
            return 0;  // No nodes to manipulate
        }
    }
    
    // Step 2: Exercise node manipulation operations based on operation_mode
    // Focus on the 8 target APIs: insert_move_before, insert_copy_before, insert_move_after, 
    // insert_copy_after, insert_child_before, insert_child_after, insert_attribute_before, insert_attribute_after
    
    // Collect existing nodes and attributes for manipulation
    std::vector<pugi::xml_node> nodes;
    std::vector<pugi::xml_attribute> attributes;
    
    // Gather existing nodes
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        nodes.push_back(child);
    }
    
    // Gather existing attributes from root
    for (pugi::xml_attribute attr = root.first_attribute(); attr; attr = attr.next_attribute()) {
        attributes.push_back(attr);
    }
    
    // Perform different operation sequences
    switch (operation_mode) {
        case 0: {
            // Test insert_move_before and insert_move_after operations
            if (nodes.size() >= 2) {
                // Move node 1 before node 0
                root.insert_move_before(nodes[1], nodes[0]);
                
                // Move node 0 after node 1
                if (nodes.size() >= 3) {
                    root.insert_move_after(nodes[0], nodes[2]);
                }
            }
            break;
        }
        
        case 1: {
            // Test insert_copy_before and insert_copy_after operations
            if (nodes.size() >= 2) {
                // Copy node 0 before node 1
                root.insert_copy_before(nodes[0], nodes[1]);
                
                // Copy node 1 after node 0
                if (nodes.size() >= 3) {
                    root.insert_copy_after(nodes[1], nodes[2]);
                }
            }
            break;
        }
        
        case 2: {
            // Test insert_child_before and insert_child_after operations
            if (nodes.size() >= 1) {
                // Insert new child before existing node
                if (node_names.size() > 0 && !node_names[0].empty()) {
                    root.insert_child_before(node_names[0].c_str(), nodes[0]);
                }
                
                // Insert new child after existing node
                if (node_names.size() > 1 && !node_names[1].empty() && nodes.size() >= 2) {
                    root.insert_child_after(node_names[1].c_str(), nodes[1]);
                }
            }
            break;
        }
        
        case 3: {
            // Test insert_attribute_before and insert_attribute_after operations
            if (attributes.size() >= 1) {
                // Insert new attribute before existing attribute
                if (attr_names.size() > 0 && !attr_names[0].empty()) {
                    root.insert_attribute_before(attr_names[0].c_str(), attributes[0]);
                }
                
                // Insert new attribute after existing attribute
                if (attr_names.size() > 1 && !attr_names[1].empty() && attributes.size() >= 2) {
                    root.insert_attribute_after(attr_names[1].c_str(), attributes[1]);
                }
            }
            break;
        }
        
        case 4: {
            // Comprehensive test: mix of move and copy operations
            if (nodes.size() >= 3) {
                // Create a circular manipulation pattern
                pugi::xml_node copied = root.insert_copy_before(nodes[0], nodes[1]);
                root.insert_move_after(nodes[1], nodes[2]);
                
                if (copied) {
                    root.insert_move_before(nodes[2], copied);
                }
            }
            break;
        }
        
        case 5: {
            // Test with newly created nodes
            if (!node_names.empty()) {
                // Create a new node to use as reference - use fuzzer input for name
                std::string ref_node_name = "ref_node";
                if (fdp.remaining_bytes() > 5) {
                    ref_node_name = fdp.ConsumeRandomLengthString(32);
                    if (ref_node_name.empty()) ref_node_name = "ref_node";
                }
                pugi::xml_node new_node = root.append_child(ref_node_name.c_str());
                
                // Insert child before and after the new node
                if (node_names.size() > 0 && !node_names[0].empty()) {
                    root.insert_child_before(node_names[0].c_str(), new_node);
                }
                
                if (node_names.size() > 1 && !node_names[1].empty()) {
                    root.insert_child_after(node_names[1].c_str(), new_node);
                }
            }
            break;
        }
        
        case 6: {
            // Test attribute insertion with new attributes
            if (!attr_names.empty()) {
                // Create a new attribute to use as reference - use fuzzer input for name and value
                std::string ref_attr_name = "ref_attr";
                if (fdp.remaining_bytes() > 5) {
                    ref_attr_name = fdp.ConsumeRandomLengthString(32);
                    if (ref_attr_name.empty()) ref_attr_name = "ref_attr";
                }
                
                std::string ref_attr_value = "ref_value";
                if (fdp.remaining_bytes() > 5) {
                    ref_attr_value = fdp.ConsumeRandomLengthString(32);
                    if (ref_attr_value.empty()) ref_attr_value = "ref_value";
                }
                
                pugi::xml_attribute new_attr = root.append_attribute(ref_attr_name.c_str());
                new_attr.set_value(ref_attr_value.c_str());
                
                // Insert attributes before and after the new attribute
                if (attr_names.size() > 0 && !attr_names[0].empty()) {
                    root.insert_attribute_before(attr_names[0].c_str(), new_attr);
                }
                
                if (attr_names.size() > 1 && !attr_names[1].empty()) {
                    root.insert_attribute_after(attr_names[1].c_str(), new_attr);
                }
            }
            break;
        }
        
        case 7: {
            // Complex manipulation with nested nodes
            if (nodes.size() >= 2) {
                // Get first child node
                pugi::xml_node target_node = nodes[0];
                
                // Manipulate children within the target node
                std::vector<pugi::xml_node> child_nodes;
                for (pugi::xml_node child = target_node.first_child(); child; child = child.next_sibling()) {
                    child_nodes.push_back(child);
                }
                
                if (child_nodes.size() >= 2) {
                    // Move child within its parent
                    target_node.insert_move_before(child_nodes[1], child_nodes[0]);
                    
                    // Copy child within its parent
                    if (child_nodes.size() >= 3) {
                        target_node.insert_copy_after(child_nodes[0], child_nodes[2]);
                    }
                }
                
                // Insert new children at different positions
                if (node_names.size() > 0 && !node_names[0].empty() && !child_nodes.empty()) {
                    target_node.insert_child_before(node_names[0].c_str(), child_nodes[0]);
                }
            }
            break;
        }
    }
    
    // Step 3: Exercise xml_text::set with various data types
    // Target API: xml_text::set for various types (20 branches each)
    if (!nodes.empty()) {
        pugi::xml_node text_node = nodes[0];
        pugi::xml_text text = text_node.text();
        
        // Use data_type_selection to choose different set operations
        switch (data_type_selection % 8) {
            case 0:
                // Set with C string
                if (!text_contents.empty() && !text_contents[0].empty()) {
                    text.set(text_contents[0].c_str());
                }
                break;
            case 1:
                // Set with integer
                text.set(fdp.ConsumeIntegral<int>());
                break;
            case 2:
                // Set with unsigned integer
                text.set(fdp.ConsumeIntegral<unsigned int>());
                break;
            case 3:
                // Set with double
                text.set(fdp.ConsumeFloatingPoint<double>());
                break;
            case 4:
                // Set with float
                text.set(fdp.ConsumeFloatingPoint<float>());
                break;
            case 5:
                // Set with boolean
                text.set(fdp.ConsumeBool());
                break;
            case 6:
                // Set with long
                text.set(fdp.ConsumeIntegral<long>());
                break;
            case 7:
                // Set with unsigned long
                text.set(fdp.ConsumeIntegral<unsigned long>());
                break;
        }
        
        // Test set with string and size
        if (!text_contents.empty() && text_contents[0].size() > 0) {
            text.set(text_contents[0].c_str(), text_contents[0].size());
        }
    }
    
    // Step 4: Exercise xml_attribute::set_value with various data types
    // Target API: xml_attribute::set_value for various types (2 branches each)
    if (!attributes.empty()) {
        pugi::xml_attribute target_attr = attributes[0];
        
        // Test different set_value overloads
        switch ((data_type_selection + 1) % 6) {
            case 0:
                // Set with C string
                if (!attr_values.empty() && !attr_values[0].empty()) {
                    target_attr.set_value(attr_values[0].c_str());
                }
                break;
            case 1:
                // Set with integer
                target_attr.set_value(fdp.ConsumeIntegral<int>());
                break;
            case 2:
                // Set with unsigned integer
                target_attr.set_value(fdp.ConsumeIntegral<unsigned int>());
                break;
            case 3:
                // Set with double
                target_attr.set_value(fdp.ConsumeFloatingPoint<double>());
                break;
            case 4:
                // Set with float
                target_attr.set_value(fdp.ConsumeFloatingPoint<float>());
                break;
            case 5:
                // Set with boolean
                target_attr.set_value(fdp.ConsumeBool());
                break;
        }
    }
    
    // Step 6: Create new nodes and attributes to test comprehensive manipulation
    if (node_names.size() >= 2) {
        // Create sibling nodes for manipulation
        pugi::xml_node node_a = root.append_child(node_names[0].c_str());
        pugi::xml_node node_b = root.append_child(node_names[1].c_str());
        
        if (node_a && node_b) {
            // Test all insertion operations between newly created nodes
            root.insert_move_before(node_b, node_a);
            root.insert_copy_after(node_a, node_b);
            
            // Create attributes on these nodes
            if (attr_names.size() >= 2) {
                pugi::xml_attribute attr_a = node_a.append_attribute(attr_names[0].c_str());
                pugi::xml_attribute attr_b = node_b.append_attribute(attr_names[1].c_str());
                
                if (attr_a && attr_b) {
                    // Test attribute value setting with fuzzer input
                    if (fdp.remaining_bytes() > 4) {
                        attr_a.set_value(fdp.ConsumeIntegral<int>());
                    }
                    
                    if (fdp.remaining_bytes() > 8) {
                        attr_b.set_value(fdp.ConsumeFloatingPoint<double>());
                    }
                    
                    // Test text setting on nodes with fuzzer input
                    if (!text_contents.empty() && !text_contents[0].empty()) {
                        node_a.text().set(text_contents[0].c_str());
                    } else if (fdp.remaining_bytes() > 4) {
                        node_a.text().set(fdp.ConsumeRandomLengthString(32).c_str());
                    }
                    
                    if (text_contents.size() > 1 && !text_contents[1].empty()) {
                        node_b.text().set(text_contents[1].c_str());
                    } else if (fdp.remaining_bytes() > 4) {
                        node_b.text().set(fdp.ConsumeFloatingPoint<float>());
                    }
                }
            }
        }
    }
    // Step 7: Cleanup happens automatically when doc goes out of scope
    // The document destructor will clean up all allocated memory
    
    return 0;
}

} // namespace harness_010
