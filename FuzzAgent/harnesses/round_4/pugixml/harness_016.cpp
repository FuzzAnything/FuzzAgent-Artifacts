#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

namespace harness_016 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for size-parameter XML node operations testing
    // Need enough data for XML content, node names, values, and size parameters
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;  // Different testing modes
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();  // XML parse options
    uint8_t num_nodes = fdp.ConsumeIntegralInRange<uint8_t>(1, 10);  // Number of nodes to test
    uint8_t num_attrs = fdp.ConsumeIntegralInRange<uint8_t>(1, 10);  // Number of attributes to test
    bool use_size_params = fdp.ConsumeBool();  // Whether to use size parameter variants
    bool test_string_view = fdp.ConsumeBool();  // Whether to test string_view variants
    bool test_prepend = fdp.ConsumeBool();  // Whether to test prepend operations
    
    // Consume variable-size data for XML content
    std::string xml_content = fdp.ConsumeRandomLengthString(2048);
    
    // Consume node names and values for size-parameter testing
    std::vector<std::string> node_names;
    std::vector<std::string> node_values;
    std::vector<size_t> name_sizes;  // Explicit size parameters for names
    std::vector<size_t> value_sizes; // Explicit size parameters for values
    
    for (int i = 0; i < num_nodes && fdp.remaining_bytes() > 20; i++) {
        std::string name = fdp.ConsumeRandomLengthString(128);
        std::string value = fdp.ConsumeRandomLengthString(256);
        size_t name_size = fdp.ConsumeIntegralInRange<size_t>(0, name.size());
        size_t value_size = fdp.ConsumeIntegralInRange<size_t>(0, value.size());
        
        node_names.push_back(name);
        node_values.push_back(value);
        name_sizes.push_back(name_size);
        value_sizes.push_back(value_size);
    }
    
    // Consume attribute names and values for attribute testing
    std::vector<std::string> attr_names;
    std::vector<std::string> attr_values;
    
    for (int i = 0; i < num_attrs && fdp.remaining_bytes() > 20; i++) {
        attr_names.push_back(fdp.ConsumeRandomLengthString(128));
        attr_values.push_back(fdp.ConsumeRandomLengthString(256));
    }
    
    // Step 1: Create XML document
    pugi::xml_document doc;
    
    // Try parsing XML content first
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
        
        // Test set_name with size parameter on root node
        if (use_size_params && !root_name.empty()) {
            size_t size_to_use = name_sizes.empty() ? root_name.size() : name_sizes[0];
            root.set_name(root_name.c_str(), size_to_use);
        } else {
            root.set_name(root_name.c_str());
        }
    }
    
    // Step 2: Create child nodes and test size-parameter operations
    for (size_t i = 0; i < node_names.size() && i < 10; i++) {
        if (node_names[i].empty()) continue;
        
        // Test different node creation methods based on test_mode
        pugi::xml_node child;
        switch (test_mode % 3) {
            case 0:
                // Append child
                child = root.append_child(pugi::node_element);
                break;
            case 1:
                if (test_prepend) {
                    // Prepend child (string_view variant if enabled)
                    if (test_string_view) {
                        std::string_view name_view(node_names[i].c_str(), 
                                                  std::min(node_names[i].size(), (size_t)128));
                        child = root.prepend_child(name_view);
                    } else {
                        child = root.prepend_child(node_names[i].c_str());
                    }
                } else {
                    child = root.append_child(pugi::node_element);
                }
                break;
            case 2:
                // Create node with type first, then set name
                child = root.append_child(pugi::node_element);
                break;
        }
        
        if (!child) continue;
        
        // Test set_name with explicit size parameter
        if (use_size_params) {
            // Use explicit size parameter variant: set_name(const char_t* rhs, size_t size)
            size_t size_to_use = name_sizes[i] % (node_names[i].size() + 1);
            child.set_name(node_names[i].c_str(), size_to_use);
        } else {
            // Use regular variant
            child.set_name(node_names[i].c_str());
        }
        
        // Test set_value with explicit size parameter
        if (!node_values[i].empty()) {
            if (use_size_params) {
                // Use explicit size parameter variant: set_value(const char_t* rhs, size_t size)
                size_t size_to_use = value_sizes[i] % (node_values[i].size() + 1);
                child.set_value(node_values[i].c_str(), size_to_use);
            } else {
                // Use regular variant
                child.set_value(node_values[i].c_str());
            }
        }
        
        // Add attributes to nodes for attribute testing
        if (i < attr_names.size() && i < attr_values.size() && 
            !attr_names[i].empty() && !attr_values[i].empty()) {
            
            pugi::xml_attribute attr;
            if (test_prepend) {
                // Test prepend_attribute
                if (test_string_view) {
                    std::string_view attr_name_view(attr_names[i].c_str(),
                                                   std::min(attr_names[i].size(), (size_t)128));
                    // Note: prepend_attribute with string_view might not be available
                    // Fall back to regular prepend_attribute
                    attr = child.prepend_attribute(attr_names[i].c_str());
                } else {
                    attr = child.prepend_attribute(attr_names[i].c_str());
                }
            } else {
                // Use append_attribute
                attr = child.append_attribute(attr_names[i].c_str());
            }
            
            if (attr) {
                // Set attribute value (attributes also have set_value with size parameter)
                if (use_size_params) {
                    size_t attr_value_size = fdp.ConsumeIntegralInRange<size_t>(
                        0, attr_values[i].size());
                    attr.set_value(attr_values[i].c_str(), attr_value_size);
                } else {
                    attr.set_value(attr_values[i].c_str());
                }
            }
        }
        
        // Test additional text value variants on child nodes
        if (fdp.remaining_bytes() > 10) {
            // Test numeric text value variants using xml_text::set()
            uint8_t value_type = fdp.ConsumeIntegral<uint8_t>() % 8;
            pugi::xml_text text = child.text();
            
            if (text) {
                switch (value_type) {
                    case 0:
                        text.set(fdp.ConsumeIntegral<int>());
                        break;
                    case 1:
                        text.set(fdp.ConsumeIntegral<unsigned int>());
                        break;
                    case 2:
                        text.set(fdp.ConsumeIntegral<long>());
                        break;
                    case 3:
                        text.set(fdp.ConsumeIntegral<unsigned long>());
                        break;
                    case 4:
                        text.set(fdp.ConsumeFloatingPoint<double>());
                        break;
                    case 5:
                        // Test set with precision for double
                        text.set(fdp.ConsumeFloatingPoint<double>(), 
                                fdp.ConsumeIntegralInRange<int>(0, 10));
                        break;
                    case 6:
                        text.set(fdp.ConsumeFloatingPoint<float>());
                        break;
                    case 7:
                        text.set(fdp.ConsumeBool());
                        break;
                }
            }
        }
    }
    
    // Step 3: Test additional prepend operations on root if enabled
    if (test_prepend && fdp.remaining_bytes() > 10) {
        // Test prepend_child with string_view
        if (test_string_view && !node_names.empty()) {
            std::string_view prepend_name_view(node_names[0].c_str(),
                                              std::min(node_names[0].size(), (size_t)64));
            pugi::xml_node prepended = root.prepend_child(prepend_name_view);
            
            if (prepended && !node_values.empty()) {
                // Set value on prepended node
                if (use_size_params) {
                    size_t value_size = value_sizes[0] % (node_values[0].size() + 1);
                    prepended.set_value(node_values[0].c_str(), value_size);
                } else {
                    prepended.set_value(node_values[0].c_str());
                }
            }
        }
        
        // Test prepend_attribute on root
        if (!attr_names.empty() && !attr_values.empty()) {
            pugi::xml_attribute prepended_attr;
            if (test_string_view) {
                // Note: prepend_attribute with string_view might not be available
                // Fall back to regular version
                prepended_attr = root.prepend_attribute(attr_names[0].c_str());
            } else {
                prepended_attr = root.prepend_attribute(attr_names[0].c_str());
            }
            
            if (prepended_attr) {
                if (use_size_params) {
                    size_t attr_value_size = fdp.ConsumeIntegralInRange<size_t>(
                        0, attr_values[0].size());
                    prepended_attr.set_value(attr_values[0].c_str(), attr_value_size);
                } else {
                    prepended_attr.set_value(attr_values[0].c_str());
                }
            }
        }
    }
    
    // Step 4: Test edge cases for size parameters
    if (use_size_params && fdp.remaining_bytes() > 5) {
        // Test with zero size
        if (!node_names.empty()) {
            pugi::xml_node edge_node = root.append_child(pugi::node_element);
            edge_node.set_name(node_names[0].c_str(), 0);  // Zero size
            
            if (!node_values.empty()) {
                edge_node.set_value(node_values[0].c_str(), 0);  // Zero size
            }
        }
        
        // Test with various valid size parameters (not exceeding actual string length)
        if (!node_names.empty() && node_names[0].size() > 0) {
            pugi::xml_node valid_node = root.append_child(pugi::node_element);
            
            // Test different valid sizes: full size, half size, single character
            size_t valid_size_options[] = {
                node_names[0].size(),                    // Full string
                node_names[0].size() / 2,                // Half string
                1,                                       // Single character
                std::min(node_names[0].size(), (size_t)4) // Up to 4 chars
            };
            
            // Pick a valid size from available options
            size_t valid_size_idx = fdp.ConsumeIntegralInRange<size_t>(0, 3) % 4;
            size_t valid_size = valid_size_options[valid_size_idx];
            
            valid_node.set_name(node_names[0].c_str(), valid_size);
            
            // Also test value with valid size if we have values
            if (!node_values.empty() && node_values[0].size() > 0) {
                size_t value_size_options[] = {
                    node_values[0].size(),
                    node_values[0].size() / 2,
                    1,
                    std::min(node_values[0].size(), (size_t)4)
                };
                size_t value_size_idx = fdp.ConsumeIntegralInRange<size_t>(0, 3) % 4;
                size_t value_size = value_size_options[value_size_idx];
                valid_node.set_value(node_values[0].c_str(), value_size);
            }
        }
    }
    
    // Step 5: Test document operations and cleanup
    // The document will be automatically cleaned up when it goes out of scope
    
    // Optional: Reset document to test cleanup
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        doc.reset();
    }
    
    return 0;
}

} // namespace harness_016
