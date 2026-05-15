#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace harness_004 {


// Custom tree walker implementation for traversal testing
class FuzzTreeWalker : public pugi::xml_tree_walker {
public:
    int node_count = 0;
    int max_depth = 0;
    
    virtual bool begin(pugi::xml_node& node) {
        node_count = 0;
        max_depth = 0;
        return true;
    }
    
    virtual bool for_each(pugi::xml_node& node) {
        node_count++;
        if (depth() > max_depth) {
            max_depth = depth();
        }
        
        // Exercise node access methods during traversal
        const char* name = node.name();
        const char* value = node.value();
        pugi::xml_node_type type = node.type();
        
        // Access attributes
        for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
            const char* attr_name = attr.name();
            const char* attr_value = attr.value();
        }
        
        // Continue traversal
        return true;
    }
    
    virtual bool end(pugi::xml_node& node) {
        return true;
    }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create XML document
    pugi::xml_document doc;
    
    // Consume some bytes for XML content
    size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(1, size / 2);
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
    
    // Try to parse XML
    pugi::xml_parse_result result = doc.load_buffer(xml_buffer.data(), xml_buffer.size());
    
    // If parsing failed, create a simple document to work with
    if (!result) {
        doc.reset();
        pugi::xml_node root = doc.append_child("root");
        root.append_child("child1");
        root.append_child("child2");
        root.append_child("child3");
    }
    
    // Get document element
    pugi::xml_node root = doc.document_element();
    if (!root) {
        root = doc.append_child("root");
    }
    
    // Consume data for node manipulation operations
    while (fdp.remaining_bytes() > 16) {
        // Decide which operation to perform based on consumed byte
        uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
        
        // Consume node names for operations
        std::string node_name1 = fdp.ConsumeRandomLengthString(32);
        std::string node_name2 = fdp.ConsumeRandomLengthString(32);
        
        switch (operation) {
            case 0: { // append_child
                pugi::xml_node new_node = root.append_child(node_name1.c_str());
                if (new_node) {
                    // Add some attributes
                    std::string attr_name = fdp.ConsumeRandomLengthString(16);
                    std::string attr_value = fdp.ConsumeRandomLengthString(32);
                    new_node.append_attribute(attr_name.c_str()) = attr_value.c_str();
                    
                    // Add text content
                    std::string text_content = fdp.ConsumeRandomLengthString(64);
                    new_node.text() = text_content.c_str();
                }
                break;
            }
            
            case 1: { // prepend_child
                pugi::xml_node new_node = root.prepend_child(node_name1.c_str());
                if (new_node) {
                    // Add attributes
                    std::string attr_name = fdp.ConsumeRandomLengthString(16);
                    std::string attr_value = fdp.ConsumeRandomLengthString(32);
                    new_node.append_attribute(attr_name.c_str()) = attr_value.c_str();
                }
                break;
            }
            
            case 2: { // insert_child_before
                // Get a reference node to insert before
                pugi::xml_node ref_node;
                for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                    if (fdp.ConsumeBool()) {
                        ref_node = child;
                        break;
                    }
                }
                
                if (ref_node) {
                    pugi::xml_node new_node = root.insert_child_before(node_name1.c_str(), ref_node);
                    if (new_node) {
                        std::string attr_name = fdp.ConsumeRandomLengthString(16);
                        std::string attr_value = fdp.ConsumeRandomLengthString(32);
                        new_node.append_attribute(attr_name.c_str()) = attr_value.c_str();
                    }
                }
                break;
            }
            
            case 3: { // insert_child_after
                // Get a reference node to insert after
                pugi::xml_node ref_node;
                for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                    if (fdp.ConsumeBool()) {
                        ref_node = child;
                        break;
                    }
                }
                
                if (ref_node) {
                    pugi::xml_node new_node = root.insert_child_after(node_name1.c_str(), ref_node);
                    if (new_node) {
                        std::string attr_name = fdp.ConsumeRandomLengthString(16);
                        std::string attr_value = fdp.ConsumeRandomLengthString(32);
                        new_node.append_attribute(attr_name.c_str()) = attr_value.c_str();
                    }
                }
                break;
            }
            
            case 4: { // remove_child
                // Try to remove a child by name
                bool removed = root.remove_child(node_name1.c_str());
                
                // Also try to remove by node reference
                pugi::xml_node child_to_remove;
                for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                    if (fdp.ConsumeBool()) {
                        child_to_remove = child;
                        break;
                    }
                }
                
                if (child_to_remove) {
                    root.remove_child(child_to_remove);
                }
                break;
            }
            
            case 5: { // traverse with xml_tree_walker
                FuzzTreeWalker walker;
                root.traverse(walker);
                break;
            }
        }
        
        // Break early if we don't have much input left
        if (fdp.remaining_bytes() < 32) {
            break;
        }
    }
    
    // Final tree traversal
    FuzzTreeWalker final_walker;
    root.traverse(final_walker);
    
    // Also traverse the entire document
    FuzzTreeWalker doc_walker;
    doc.traverse(doc_walker);
    
    // Clean up
    doc.reset();
    
    return 0;
}

} // namespace harness_004
