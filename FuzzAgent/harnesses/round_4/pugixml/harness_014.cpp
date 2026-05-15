#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>

namespace harness_014 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for XML tree manipulation testing
    // Need enough data for XML content, node/attribute names/values, and operation parameters
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t parse_method = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: load_string, 1: load_buffer, 2: create new
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();
    uint8_t operation_sequence = fdp.ConsumeIntegral<uint8_t>() % 4;  // Different test sequences
    uint8_t use_string_view = fdp.ConsumeBool();
    uint8_t test_attribute_ops = fdp.ConsumeBool();
    uint8_t test_node_copy_ops = fdp.ConsumeBool();
    uint8_t test_child_insert_ops = fdp.ConsumeBool();
    
    // Consume variable-size data for XML content
    std::string xml_content = fdp.ConsumeRandomLengthString(2048);
    
    // Consume node names for various operations
    std::vector<std::string> node_names;
    for (int i = 0; i < 10 && fdp.remaining_bytes() > 10; i++) {
        node_names.push_back(fdp.ConsumeRandomLengthString(64));
    }
    
    // Consume attribute names and values for attribute operations
    std::vector<std::string> attr_names;
    std::vector<std::string> attr_values;
    for (int i = 0; i < 8 && fdp.remaining_bytes() > 20; i++) {
        attr_names.push_back(fdp.ConsumeRandomLengthString(64));
        attr_values.push_back(fdp.ConsumeRandomLengthString(128));
    }
    
    // Consume reference node names for insert_before/after operations
    std::vector<std::string> ref_node_names;
    for (int i = 0; i < 5 && fdp.remaining_bytes() > 10; i++) {
        ref_node_names.push_back(fdp.ConsumeRandomLengthString(64));
    }
    
    // Consume reference attribute names for insert_before/after operations
    std::vector<std::string> ref_attr_names;
    for (int i = 0; i < 5 && fdp.remaining_bytes() > 10; i++) {
        ref_attr_names.push_back(fdp.ConsumeRandomLengthString(64));
    }
    
    // Step 1: Create XML document and parse XML content
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result;
    
    switch (parse_method) {
        case 0:
            parse_result = doc.load_string(xml_content.c_str(), parse_options);
            break;
        case 1:
            parse_result = doc.load_buffer(xml_content.c_str(), xml_content.size(), parse_options);
            break;
        case 2:
            // Create new document for pure tree manipulation testing
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
    
    // Step 2: Create initial structure with nodes and attributes for manipulation
    std::vector<pugi::xml_node> base_nodes;
    std::vector<pugi::xml_attribute> base_attributes;
    
    // Create base nodes for node manipulation
    for (size_t i = 0; i < 3 && i < node_names.size(); i++) {
        if (!node_names[i].empty()) {
            pugi::xml_node node = root.append_child(node_names[i].c_str());
            base_nodes.push_back(node);
            
            // Add some text content
            if (i < attr_values.size() && !attr_values[i].empty()) {
                node.text().set(attr_values[i].c_str());
            }
        }
    }
    
    // Create base attributes for attribute manipulation
    if (!base_nodes.empty()) {
        pugi::xml_node& first_node = base_nodes[0];
        for (size_t i = 0; i < 3 && i < attr_names.size(); i++) {
            if (!attr_names[i].empty()) {
                pugi::xml_attribute attr = first_node.append_attribute(attr_names[i].c_str());
                if (i < attr_values.size() && !attr_values[i].empty()) {
                    attr.set_value(attr_values[i].c_str());
                }
                base_attributes.push_back(attr);
            }
        }
    }
    
    // Step 3: Create reference nodes for insert_before/after operations
    std::vector<pugi::xml_node> reference_nodes;
    for (size_t i = 0; i < 2 && i < ref_node_names.size(); i++) {
        if (!ref_node_names[i].empty()) {
            pugi::xml_node ref_node = root.append_child(ref_node_names[i].c_str());
            reference_nodes.push_back(ref_node);
            
            // Add identifier attribute
            ref_node.append_attribute("ref_id") = std::to_string(i).c_str();
        }
    }
    
    // Step 4: Create reference attributes for insert_before/after operations
    std::vector<pugi::xml_attribute> reference_attributes;
    if (!base_nodes.empty() && base_nodes.size() > 1) {
        pugi::xml_node& second_node = base_nodes[1];
        for (size_t i = 0; i < 2 && i < ref_attr_names.size(); i++) {
            if (!ref_attr_names[i].empty()) {
                pugi::xml_attribute ref_attr = second_node.append_attribute(ref_attr_names[i].c_str());
                ref_attr.set_value(("ref_value_" + std::to_string(i)).c_str());
                reference_attributes.push_back(ref_attr);
            }
        }
    }
    
    // Step 5: Test the 8 target tree manipulation functions
    // Based on operation_sequence, test different combinations
    
    if (test_attribute_ops && !base_attributes.empty() && !reference_attributes.empty()) {
        // Test attribute copy operations
        if (base_attributes.size() > 0 && reference_attributes.size() > 0) {
            pugi::xml_attribute proto = base_attributes[0];
            pugi::xml_attribute ref_attr = reference_attributes[0];
            
            // 1. insert_copy_before for attributes
            if (root) {
                pugi::xml_attribute inserted1 = root.insert_copy_before(proto, ref_attr);
                (void)inserted1; // Use to avoid unused variable warning
            }
            
            // 2. insert_copy_after for attributes
            if (root) {
                pugi::xml_attribute inserted2 = root.insert_copy_after(proto, ref_attr);
                (void)inserted2;
            }
            
            // 3. append_copy for attributes
            if (root) {
                pugi::xml_attribute appended = root.append_copy(proto);
                (void)appended;
            }
            
            // 4. prepend_copy for attributes
            if (root) {
                pugi::xml_attribute prepended = root.prepend_copy(proto);
                (void)prepended;
            }
        }
        
        // Test attribute insertion operations with names
        if (!attr_names.empty() && !reference_attributes.empty()) {
            pugi::xml_attribute ref_attr = reference_attributes[0];
            std::string new_attr_name = "new_attr";
            if (!attr_names.empty()) {
                new_attr_name = attr_names[0];
            }
            
            // 5. insert_attribute_before
            if (root) {
                if (use_string_view) {
                    std::string_view name_sv(new_attr_name);
                    pugi::xml_attribute inserted_before = root.insert_attribute_before(name_sv, ref_attr);
                    (void)inserted_before;
                } else {
                    pugi::xml_attribute inserted_before = root.insert_attribute_before(new_attr_name.c_str(), ref_attr);
                    (void)inserted_before;
                }
            }
            
            // 6. insert_attribute_after
            if (root) {
                if (use_string_view) {
                    std::string_view name_sv(new_attr_name);
                    pugi::xml_attribute inserted_after = root.insert_attribute_after(name_sv, ref_attr);
                    (void)inserted_after;
                } else {
                    pugi::xml_attribute inserted_after = root.insert_attribute_after(new_attr_name.c_str(), ref_attr);
                    (void)inserted_after;
                }
            }
        }
    }
    
    if (test_node_copy_ops && !base_nodes.empty() && !reference_nodes.empty()) {
        // Test node copy operations
        if (base_nodes.size() > 0 && reference_nodes.size() > 0) {
            pugi::xml_node proto = base_nodes[0];
            pugi::xml_node ref_node = reference_nodes[0];
            
            // 7. insert_copy_before for nodes
            if (root) {
                pugi::xml_node inserted_node1 = root.insert_copy_before(proto, ref_node);
                (void)inserted_node1;
            }
            
            // 8. insert_copy_after for nodes
            if (root) {
                pugi::xml_node inserted_node2 = root.insert_copy_after(proto, ref_node);
                (void)inserted_node2;
            }
        }
    }
    
    if (test_child_insert_ops && !node_names.empty() && !reference_nodes.empty()) {
        // Test child insertion operations with names
        pugi::xml_node ref_node = reference_nodes[0];
        std::string new_child_name = "new_child";
        if (!node_names.empty()) {
            new_child_name = node_names[0];
        }
        
        // 9. insert_child_before
        if (root) {
            if (use_string_view) {
                std::string_view name_sv(new_child_name);
                pugi::xml_node inserted_child_before = root.insert_child_before(name_sv, ref_node);
                (void)inserted_child_before;
            } else {
                pugi::xml_node inserted_child_before = root.insert_child_before(new_child_name.c_str(), ref_node);
                (void)inserted_child_before;
            }
        }
        
        // 10. insert_child_after
        if (root) {
            if (use_string_view) {
                std::string_view name_sv(new_child_name);
                pugi::xml_node inserted_child_after = root.insert_child_after(name_sv, ref_node);
                (void)inserted_child_after;
            } else {
                pugi::xml_node inserted_child_after = root.insert_child_after(new_child_name.c_str(), ref_node);
                (void)inserted_child_after;
            }
        }
    }
    
    // Additional comprehensive testing based on operation_sequence
    switch (operation_sequence) {
        case 0: {
            // Test all operations in sequence on first base node
            if (!base_nodes.empty()) {
                pugi::xml_node& target_node = base_nodes[0];
                
                // Create more attributes for comprehensive testing
                for (size_t i = 0; i < 2 && i < attr_names.size(); i++) {
                    if (!attr_names[i].empty()) {
                        std::string attr_name = "seq_attr_" + std::to_string(i);
                        if (use_string_view) {
                            std::string_view name_sv(attr_name);
                            target_node.append_attribute(name_sv);
                        } else {
                            target_node.append_attribute(attr_name.c_str());
                        }
                    }
                }
                
                // Test attribute chain operations
                pugi::xml_attribute first_attr = target_node.first_attribute();
                if (first_attr) {
                    // Insert new attribute before first
                    std::string new_name = "before_first";
                    if (use_string_view) {
                        std::string_view name_sv(new_name);
                        target_node.insert_attribute_before(name_sv, first_attr);
                    } else {
                        target_node.insert_attribute_before(new_name.c_str(), first_attr);
                    }
                    
                    // Insert new attribute after first
                    std::string new_name2 = "after_first";
                    if (use_string_view) {
                        std::string_view name_sv2(new_name2);
                        target_node.insert_attribute_after(name_sv2, first_attr);
                    } else {
                        target_node.insert_attribute_after(new_name2.c_str(), first_attr);
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Test node copy operations between different parents
            if (base_nodes.size() > 1 && reference_nodes.size() > 0) {
                pugi::xml_node& source_node = base_nodes[0];
                pugi::xml_node& target_parent = base_nodes[1];
                pugi::xml_node& ref_node = reference_nodes[0];
                
                // Copy source node before reference node in target parent
                pugi::xml_node copied = target_parent.insert_copy_before(source_node, ref_node);
                (void)copied;
                
                // Copy source node after reference node in target parent
                pugi::xml_node copied2 = target_parent.insert_copy_after(source_node, ref_node);
                (void)copied2;
            }
            break;
        }
        
        case 2: {
            // Test mixed operations with cleanup
            if (!base_nodes.empty()) {
                pugi::xml_node& target_node = base_nodes[0];
                
                // Create attributes with different names
                for (size_t i = 0; i < 3 && i < attr_names.size(); i++) {
                    if (!attr_names[i].empty()) {
                        std::string attr_name = attr_names[i] + "_mixed";
                        pugi::xml_attribute attr = target_node.append_attribute(attr_name.c_str());
                        if (i < attr_values.size()) {
                            attr.set_value(attr_values[i].c_str());
                        }
                        
                        // Test append_copy and prepend_copy
                        if (i == 0) {
                            target_node.append_copy(attr);
                            target_node.prepend_copy(attr);
                        }
                    }
                }
                
                // Cleanup: remove some attributes
                pugi::xml_attribute attr_to_remove = target_node.first_attribute();
                if (attr_to_remove) {
                    target_node.remove_attribute(attr_to_remove);
                }
            }
            break;
        }
        
        case 3: {
            // Test edge cases and error conditions
            if (!base_nodes.empty()) {
                pugi::xml_node& target_node = base_nodes[0];
                
                // Test with empty/invalid references
                pugi::xml_node empty_node;
                pugi::xml_attribute empty_attr;
                
                // These should handle empty references gracefully
                target_node.insert_copy_before(empty_node, empty_node);
                target_node.insert_copy_after(empty_node, empty_node);
                target_node.append_copy(empty_attr);
                target_node.prepend_copy(empty_attr);
                
                // Test with self-reference (should be handled by library)
                pugi::xml_attribute self_attr = target_node.first_attribute();
                if (self_attr) {
                    target_node.insert_attribute_before("self_test", self_attr);
                }
            }
            break;
        }
    }
    
    // Step 6: Cleanup
    // Remove test attributes
    for (auto& attr : base_attributes) {
        if (attr) {
            // Try to remove by name if available
            const char* name = attr.name();
            if (name && root) {
                root.remove_attribute(name);
            }
        }
    }
    
    // Remove reference attributes
    for (auto& attr : reference_attributes) {
        if (attr) {
            const char* name = attr.name();
            if (name && root) {
                root.remove_attribute(name);
            }
        }
    }
    
    // Reset document
    doc.reset();
    
    return 0;
}

} // namespace harness_014
