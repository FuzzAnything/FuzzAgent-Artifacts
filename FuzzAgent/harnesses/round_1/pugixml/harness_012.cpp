#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>
#include <cstring>

namespace harness_012 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough for XML content, node operations, and buffer data
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ==================== PHASE 1: DOCUMENT SETUP ====================
    // Create XML document with structure for node manipulation testing
    pugi::xml_document doc;
    
    // Create root element and initial structure
    pugi::xml_node root = doc.append_child("root");
    
    // Create initial nodes for move/copy operations
    pugi::xml_node source_node = root.append_child("source_node");
    source_node.append_attribute("id") = "source";
    source_node.append_child("source_child").append_attribute("value") = "1";
    
    pugi::xml_node target_node = root.append_child("target_node");
    target_node.append_attribute("id") = "target";
    target_node.append_child("target_child").append_attribute("value") = "2";
    
    pugi::xml_node sibling1 = root.append_child("sibling1");
    sibling1.append_attribute("pos") = "before";
    sibling1.append_child("child1");
    
    pugi::xml_node sibling2 = root.append_child("sibling2");
    sibling2.append_attribute("pos") = "after";
    sibling2.append_child("child2");
    
    // Create nodes for buffer appending
    pugi::xml_node buffer_node = root.append_child("buffer_node");
    
    // Create additional nodes for complex move/copy scenarios
    pugi::xml_node container1 = root.append_child("container1");
    pugi::xml_node container2 = root.append_child("container2");
    
    for (int i = 0; i < 3; i++) {
        pugi::xml_node item = container1.append_child("item");
        item.append_attribute("index") = std::to_string(i).c_str();
        item.text() = ("Item " + std::to_string(i)).c_str();
    }
    
    // ==================== PHASE 2: NODE MOVE OPERATIONS ====================
    // TARGET: xml_node::insert_move_after, xml_node::insert_move_before
    
    while (fdp.remaining_bytes() > 32) {
        uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
        
        switch (operation) {
            case 0: { // insert_move_after - move node after another node
                // Try moving source_node after target_node
                pugi::xml_node result = root.insert_move_after(source_node, target_node);
                (void)result; // Use result to avoid compiler warnings
                
                // Try moving container1 child after container2
                pugi::xml_node container1_first = container1.first_child();
                pugi::xml_node container2_first = container2.first_child();
                if (container1_first && container2_first) {
                    pugi::xml_node move_result = container2.insert_move_after(container1_first, container2_first);
                    (void)move_result;
                }
                break;
            }
            
            case 1: { // insert_move_before - move node before another node
                // Try moving target_node before source_node
                pugi::xml_node result = root.insert_move_before(target_node, source_node);
                (void)result;
                
                // Try moving between siblings
                pugi::xml_node result2 = root.insert_move_before(sibling1, sibling2);
                (void)result2;
                break;
            }
            
            case 2: { // insert_copy_after - copy node after another node
                // TARGET: xml_node::insert_copy_after
                pugi::xml_node result = root.insert_copy_after(source_node, target_node);
                (void)result;
                
                // Copy with different node types
                pugi::xml_node copy_result = container1.insert_copy_after(
                    container1.first_child(), 
                    container1.last_child()
                );
                (void)copy_result;
                break;
            }
            
            case 3: { // insert_copy_before - copy node before another node
                // TARGET: xml_node::insert_copy_before
                pugi::xml_node result = root.insert_copy_before(target_node, source_node);
                (void)result;
                
                // Copy sibling before another sibling
                pugi::xml_node copy_result = root.insert_copy_before(sibling2, sibling1);
                (void)copy_result;
                break;
            }
            
            case 4: { // append_buffer - append XML buffer to node
                // TARGET: xml_node::append_buffer
                std::string buffer_content = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 256));
                
                // Try appending buffer to buffer_node
                pugi::xml_parse_result append_result = buffer_node.append_buffer(
                    buffer_content.c_str(), 
                    buffer_content.size(),
                    pugi::parse_default,
                    pugi::encoding_auto
                );
                (void)append_result;
                
                // Try with different parsing options
                if (fdp.remaining_bytes() > 16) {
                    std::string buffer_content2 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 128));
                    pugi::xml_parse_result append_result2 = root.append_buffer(
                        buffer_content2.c_str(),
                        buffer_content2.size(),
                        pugi::parse_minimal,
                        pugi::encoding_auto
                    );
                    (void)append_result2;
                }
                break;
            }
            
            case 5: { // set_value with size parameter - node
                // TARGET: xml_node::set_value(const char_t* rhs, size_t size)
                std::string node_value = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 128));
                
                // Set value on a node with size parameter
                bool set_success = source_node.set_value(node_value.c_str(), node_value.size());
                (void)set_success;
                
                // Try on different nodes
                if (fdp.remaining_bytes() > 8) {
                    std::string node_value2 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 64));
                    bool set_success2 = target_node.set_value(node_value2.c_str(), node_value2.size());
                    (void)set_success2;
                }
                break;
            }
            
            case 6: { // set_value with size parameter - attribute
                // TARGET: xml_attribute::set_value(const char_t* rhs, size_t size)
                std::string attr_value = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 128));
                
                // Create or get an attribute
                pugi::xml_attribute attr = source_node.attribute("id");
                if (!attr) {
                    attr = source_node.append_attribute("id");
                }
                
                // Set attribute value with size parameter
                bool attr_success = attr.set_value(attr_value.c_str(), attr_value.size());
                (void)attr_success;
                
                // Try on another attribute
                if (fdp.remaining_bytes() > 8) {
                    std::string attr_value2 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 64));
                    pugi::xml_attribute attr2 = target_node.attribute("id");
                    if (attr2) {
                        bool attr_success2 = attr2.set_value(attr_value2.c_str(), attr_value2.size());
                        (void)attr_success2;
                    }
                }
                break;
            }
            
            case 7: { // Complex move/copy chain operations
                // Create a chain of move/copy operations to test edge cases
                if (container1.first_child() && container2.first_child()) {
                    // Move then copy
                    pugi::xml_node moved = container1.insert_move_before(
                        container1.first_child(),
                        container1.last_child()
                    );
                    (void)moved;
                    
                    if (moved && container2.first_child()) {
                        pugi::xml_node copied = container2.insert_copy_after(
                            moved,
                            container2.first_child()
                        );
                        (void)copied;
                    }
                }
                
                // Test moving between different parent nodes
                if (source_node.first_child() && target_node) {
                    pugi::xml_node child_moved = target_node.insert_move_after(
                        source_node.first_child(),
                        target_node.first_child()
                    );
                    (void)child_moved;
                }
                break;
            }
        }
        
        // Break if we're running low on input
        if (fdp.remaining_bytes() < 32) {
            break;
        }
    }
    
    // ==================== PHASE 3: VALIDATION AND CLEANUP ====================
    // Verify document structure integrity after operations
    bool has_root = !root.empty();
    (void)has_root;
    
    // Count children to ensure operations didn't break the document
    int child_count = 0;
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        child_count++;
    }
    (void)child_count;
    
    // Test navigation after move/copy operations
    pugi::xml_node first_child = root.first_child();
    pugi::xml_node last_child = root.last_child();
    (void)first_child;
    (void)last_child;
    
    // Test attribute access after operations
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        for (pugi::xml_attribute attr = child.first_attribute(); attr; attr = attr.next_attribute()) {
            const char* attr_name = attr.name();
            const char* attr_value = attr.value();
            (void)attr_name;
            (void)attr_value;
        }
    }
    
    // ==================== PHASE 4: ADDITIONAL BUFFER APPENDING TESTS ====================
    // Test append_buffer with various input sizes and encodings
    if (fdp.remaining_bytes() > 16) {
        // Generate test buffers from fuzzer input instead of hardcoded values
        std::vector<std::string> test_buffers;
        
        // Generate 3 random buffers from remaining input
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 16; i++) {
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)256));
            std::string buffer = fdp.ConsumeRandomLengthString(buffer_size);
            test_buffers.push_back(buffer);
        }
        
        // Buffer from remaining fuzzer input
        if (fdp.remaining_bytes() > 0) {
            std::string fuzzer_buffer = fdp.ConsumeRemainingBytesAsString();
            if (!fuzzer_buffer.empty()) {
                pugi::xml_node test_node = root.append_child("fuzzer_buffer_test");
                pugi::xml_parse_result buffer_result = test_node.append_buffer(
                    fuzzer_buffer.c_str(),
                    fuzzer_buffer.size(),
                    pugi::parse_default,
                    pugi::encoding_auto
                );
                (void)buffer_result;
            }
        }
        
        // Try appending generated buffers to different node types
        for (const auto& buffer : test_buffers) {
            if (buffer.empty()) continue;
            
            pugi::xml_node test_target = root.append_child("buffer_target");
            pugi::xml_parse_result result = test_target.append_buffer(
                buffer.c_str(),
                buffer.size(),
                pugi::parse_default,
                pugi::encoding_auto
            );
            (void)result;
        }
    }
    
    return 0;
}

} // namespace harness_012
