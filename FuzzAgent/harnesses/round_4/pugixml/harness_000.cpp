#include "pugixml.hpp"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <cstring>

namespace harness_000 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0; // Need minimum input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t parse_options = fdp.ConsumeIntegral<uint8_t>();
    uint8_t node_operations = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume variable-size data
    std::string xml_content = fdp.ConsumeRandomLengthString(4096);
    std::string node_name = fdp.ConsumeRandomLengthString(128);
    std::string attr_name = fdp.ConsumeRandomLengthString(128);
    std::string attr_value = fdp.ConsumeRandomLengthString(256);
    std::string node_value = fdp.ConsumeRandomLengthString(512);
    
    // Create XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result;
    
    // Test different parsing methods based on test_mode
    switch (test_mode) {
        case 0:
            // Test load_buffer
            result = doc.load_buffer(xml_content.c_str(), xml_content.size(), parse_options);
            break;
        case 1:
            // Test load_string
            result = doc.load_string(xml_content.c_str(), parse_options);
            break;
        case 2:
            // Test load_buffer_inplace_own - need mutable buffer allocated with pugixml allocation function
            if (xml_content.size() > 0) {
                // Get pugixml's memory allocation function
                pugi::allocation_function alloc = pugi::get_memory_allocation_function();
                char* buffer = static_cast<char*>(alloc(xml_content.size()));
                if (buffer) {
                    memcpy(buffer, xml_content.c_str(), xml_content.size());
                    result = doc.load_buffer_inplace_own(buffer, xml_content.size(), parse_options);
                    // Buffer ownership is transferred to pugixml, don't free it
                }
            }
            break;
        case 3:
            // Test load_buffer with remaining bytes
            std::vector<uint8_t> buffer_data = fdp.ConsumeRemainingBytes<uint8_t>();
            if (!buffer_data.empty()) {
                result = doc.load_buffer(buffer_data.data(), buffer_data.size(), parse_options);
            }
            break;
    }
    
    // If parsing succeeded, test node and attribute operations
    if (result) {
        // Get document element
        pugi::xml_node root = doc.document_element();
        
        if (root) {
            // Test node operations based on node_operations
            switch (node_operations) {
                case 0:
                    // Create a new child node
                    {
                        pugi::xml_node child = root.append_child(node_name.c_str());
                        if (child) {
                            child.append_attribute(attr_name.c_str()) = attr_value.c_str();
                            child.text().set(node_value.c_str());
                        }
                    }
                    break;
                case 1:
                    // Modify existing attributes
                    for (pugi::xml_attribute attr = root.first_attribute(); attr; attr = attr.next_attribute()) {
                        attr.set_value(attr_value.c_str());
                    }
                    break;
                case 2:
                    // Traverse child nodes
                    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                        // Access node properties
                        const char* name = child.name();
                        const char* value = child.value();
                        pugi::xml_text text = child.text();
                        (void)name; (void)value; (void)text; // Avoid unused warnings
                    }
                    break;
                case 3:
                    // Create and remove nodes
                    {
                        pugi::xml_node new_node = root.append_child("fuzz_node");
                        root.remove_child(new_node);
                    }
                    break;
                case 4:
                    // Test attribute finding and manipulation
                    {
                        pugi::xml_attribute attr = root.attribute(attr_name.c_str());
                        if (!attr) {
                            attr = root.append_attribute(attr_name.c_str());
                        }
                        if (attr) {
                            std::string current_value = attr.value();
                            attr.set_value((current_value + "_modified").c_str());
                        }
                    }
                    break;
                case 5:
                    // Test node finding by name
                    {
                        pugi::xml_node found = root.child(node_name.c_str());
                        if (found) {
                            found.set_name((std::string(found.name()) + "_renamed").c_str());
                        }
                    }
                    break;
                case 6:
                    // Test text manipulation
                    {
                        pugi::xml_text text = root.text();
                        if (text) {
                            std::string current_text = text.get();
                            text.set((current_text + "_appended").c_str());
                        } else {
                            root.append_child(pugi::node_pcdata).set_value(node_value.c_str());
                        }
                    }
                    break;
                case 7:
                    // Test node copying
                    if (root.first_child()) {
                        pugi::xml_node copy = root.append_copy(root.first_child());
                        root.remove_child(copy);
                    }
                    break;
            }
        } else {
            // If no root element, create one
            pugi::xml_node new_root = doc.append_child("root");
            new_root.append_attribute("created") = "by_fuzzer";
            new_root.text().set("Fuzzer-generated content");
        }
        
        // Test saving functionality (in-memory only)
        std::ostringstream oss;
        doc.save(oss, "  ", pugi::format_default);
        std::string output = oss.str();
    } else {
        // Even if parsing failed, test document creation from scratch
        doc.reset();
        pugi::xml_node root = doc.append_child("fuzz_root");
        root.append_attribute("test") = "failed_parse";
        root.append_child("recovery").text().set("Parsing failed but document created");
    }
    
    return 0;
}

} // namespace harness_000
