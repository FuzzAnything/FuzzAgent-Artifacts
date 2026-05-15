#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <stdint.h>
#include <string>
#include <vector>
#include <algorithm>

namespace harness_006 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing of insertion operations
    if (size < 32) return 0;  // Need enough data for node names, attribute names, and positions
    
    FuzzedDataProvider fdp(data, size);
    // Step 1: Parse XML or create base structure
    // Consume a portion of input for XML content
    size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(16, std::max(static_cast<size_t>(16), size / 3));
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_default);
    
    // If parsing failed, create a base document structure for testing
    if (!parse_result) {
        doc.reset();
        pugi::xml_node root = doc.append_child(pugi::node_element);
        
        // Consume root node name from fuzzed data
        std::string root_name = fdp.ConsumeRandomLengthString(20);
        root.set_name(root_name.c_str());
        
        // Create initial children to have targets for insertion operations
        int initial_child_count = fdp.ConsumeIntegralInRange<int>(2, 8);
        for (int i = 0; i < initial_child_count && fdp.remaining_bytes() > 10; ++i) {
            std::string child_name = fdp.ConsumeRandomLengthString(15);
            pugi::xml_node child = root.append_child(child_name.c_str());
            
            // Add some initial attributes to these children
            int initial_attr_count = fdp.ConsumeIntegralInRange<int>(0, 3);
            for (int j = 0; j < initial_attr_count && fdp.remaining_bytes() > 10; ++j) {
                std::string attr_name = fdp.ConsumeRandomLengthString(10);
                std::string attr_value = fdp.ConsumeRandomLengthString(30);
                child.append_attribute(attr_name.c_str()).set_value(attr_value.c_str());
            }
        }
    }
    
    // Step 2: Get root node for manipulation
    pugi::xml_node root = doc.document_element();
    if (root.empty()) {
        root = doc.first_child();
        if (root.empty()) {
            return 0;  // No document structure to manipulate
        }
    }
    
    // Step 3: Test the target insertion operations
    
    // 3.1 Test prepend_child and append_child (basic operations that should work as reference)
    if (fdp.remaining_bytes() > 20) {
        std::string prepend_name = fdp.ConsumeRandomLengthString(15);
        pugi::xml_node prepended = root.prepend_child(prepend_name.c_str());
        
        std::string append_name = fdp.ConsumeRandomLengthString(15);
        pugi::xml_node appended = root.append_child(append_name.c_str());
    }
    
    // 3.2 Create a list of sibling nodes to use as reference points for insert_before/after operations
    std::vector<pugi::xml_node> sibling_nodes;
    pugi::xml_node child = root.first_child();
    while (child) {
        sibling_nodes.push_back(child);
        child = child.next_sibling();
    }
    
    if (sibling_nodes.size() < 2) {
        // Not enough siblings for meaningful insertion testing
        return 0;
    }
    
    // 3.3 Test insert_child_before and insert_child_after operations
    // Choose random reference nodes from existing siblings
    if (fdp.remaining_bytes() > 30 && sibling_nodes.size() >= 2) {
        size_t ref_idx1 = fdp.ConsumeIntegralInRange<size_t>(0, sibling_nodes.size() - 1);
        size_t ref_idx2 = fdp.ConsumeIntegralInRange<size_t>(0, sibling_nodes.size() - 1);
        
        pugi::xml_node ref_node1 = sibling_nodes[ref_idx1];
        pugi::xml_node ref_node2 = sibling_nodes[ref_idx2];
        
        // Test insert_child_before
        std::string insert_before_name = fdp.ConsumeRandomLengthString(15);
        pugi::xml_node inserted_before = root.insert_child_before(insert_before_name.c_str(), ref_node1);
        
        // Test insert_child_after  
        std::string insert_after_name = fdp.ConsumeRandomLengthString(15);
        pugi::xml_node inserted_after = root.insert_child_after(insert_after_name.c_str(), ref_node2);
        
        // Test edge cases: insertion at beginning (before first child)
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
            pugi::xml_node first_child = root.first_child();
            if (!first_child.empty()) {
                std::string edge_name = fdp.ConsumeRandomLengthString(15);
                pugi::xml_node edge_inserted = root.insert_child_before(edge_name.c_str(), first_child);
            }
        }
        
        // Test edge cases: insertion at end (after last child)
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
            pugi::xml_node last_child = root.last_child();
            if (!last_child.empty()) {
                std::string edge_name = fdp.ConsumeRandomLengthString(15);
                pugi::xml_node edge_inserted = root.insert_child_after(edge_name.c_str(), last_child);
            }
        }
    }
    
    // 3.4 Test insert_attribute_before and insert_attribute_after operations
    // First, ensure we have nodes with attributes to use as reference points
    if (fdp.remaining_bytes() > 40) {
        // Pick a random child node to test attribute insertion
        size_t node_idx = fdp.ConsumeIntegralInRange<size_t>(0, sibling_nodes.size() - 1);
        pugi::xml_node target_node = sibling_nodes[node_idx];
        
        if (!target_node.empty()) {
            // Create initial attributes on the target node if it doesn't have any
            if (target_node.first_attribute().empty()) {
                std::string init_attr_name = fdp.ConsumeRandomLengthString(10);
                std::string init_attr_value = fdp.ConsumeRandomLengthString(20);
                target_node.append_attribute(init_attr_name.c_str()).set_value(init_attr_value.c_str());
            }
            
            // Get list of existing attributes for reference points
            std::vector<pugi::xml_attribute> existing_attrs;
            pugi::xml_attribute attr = target_node.first_attribute();
            while (attr) {
                existing_attrs.push_back(attr);
                attr = attr.next_attribute();
            }
            
            if (existing_attrs.size() >= 1) {
                // Test insert_attribute_before
                std::string attr_before_name = fdp.ConsumeRandomLengthString(10);
                pugi::xml_attribute ref_attr1 = existing_attrs[fdp.ConsumeIntegralInRange<size_t>(0, existing_attrs.size() - 1)];
                pugi::xml_attribute inserted_attr_before = target_node.insert_attribute_before(attr_before_name.c_str(), ref_attr1);
                
                if (!inserted_attr_before.empty() && fdp.remaining_bytes() > 10) {
                    std::string attr_before_value = fdp.ConsumeRandomLengthString(30);
                    inserted_attr_before.set_value(attr_before_value.c_str());
                }
                
                // Test insert_attribute_after
                if (fdp.remaining_bytes() > 10) {
                    std::string attr_after_name = fdp.ConsumeRandomLengthString(10);
                    pugi::xml_attribute ref_attr2 = existing_attrs[fdp.ConsumeIntegralInRange<size_t>(0, existing_attrs.size() - 1)];
                    pugi::xml_attribute inserted_attr_after = target_node.insert_attribute_after(attr_after_name.c_str(), ref_attr2);
                    
                    if (!inserted_attr_after.empty() && fdp.remaining_bytes() > 10) {
                        std::string attr_after_value = fdp.ConsumeRandomLengthString(30);
                        inserted_attr_after.set_value(attr_after_value.c_str());
                    }
                }
                
                // Test edge cases: insertion at beginning (before first attribute)
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
                    pugi::xml_attribute first_attr = target_node.first_attribute();
                    if (!first_attr.empty()) {
                        std::string edge_attr_name = fdp.ConsumeRandomLengthString(10);
                        pugi::xml_attribute edge_inserted = target_node.insert_attribute_before(edge_attr_name.c_str(), first_attr);
                    }
                }
                
                // Test edge cases: insertion at end (after last attribute)
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
                    pugi::xml_attribute last_attr = target_node.last_attribute();
                    if (!last_attr.empty()) {
                        std::string edge_attr_name = fdp.ConsumeRandomLengthString(10);
                        pugi::xml_attribute edge_inserted = target_node.insert_attribute_after(edge_attr_name.c_str(), last_attr);
                    }
                }
            }
        }
    }
    
    // Step 4: Verify tree structure integrity (basic sanity checks)
    // Count children to ensure tree structure is valid
    size_t child_count = 0;
    pugi::xml_node check_child = root.first_child();
    while (check_child) {
        child_count++;
        check_child = check_child.next_sibling();
    }
    
    // Count attributes on a random child to ensure attribute structure is valid
    if (sibling_nodes.size() > 0) {
        size_t random_idx = fdp.ConsumeIntegralInRange<size_t>(0, sibling_nodes.size() - 1);
        pugi::xml_node random_node = sibling_nodes[random_idx];
        
        size_t attr_count = 0;
        pugi::xml_attribute check_attr = random_node.first_attribute();
        while (check_attr) {
            attr_count++;
            check_attr = check_attr.next_attribute();
        }
    }
    
    // Step 5: Cleanup (xml_document destructor will handle cleanup automatically)
    
    return 0;
}

} // namespace harness_006
