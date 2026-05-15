#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <stdint.h>
#include <string>
#include <vector>
#include <cstring>

namespace harness_010 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing of attribute navigation and XPath queries
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create or parse XML document for testing
    // Split input: first part for XML content, rest for navigation parameters
    size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(16, size / 2);
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_full);
    
    // If parsing failed, create a structured document with many attributes for testing
    if (!parse_result) {
        doc.reset();
        pugi::xml_node root = doc.append_child(pugi::node_element);
        
        // Use fuzzed data for root name
        std::string root_name = fdp.remaining_bytes() > 5 ? fdp.ConsumeRandomLengthString(10) : "root";
        root.set_name(root_name.c_str());
        
        // Create multiple elements with many attributes for attribute navigation testing
        int element_count = fdp.ConsumeIntegralInRange<int>(1, 10);
        for (int i = 0; i < element_count && fdp.remaining_bytes() > 20; ++i) {
            pugi::xml_node element = root.append_child(pugi::node_element);
            
            // Element name from fuzzed data
            std::string elem_name = "elem" + std::to_string(i);
            if (fdp.remaining_bytes() > 5) {
                elem_name = fdp.ConsumeRandomLengthString(15);
            }
            element.set_name(elem_name.c_str());
            
            // Add text content
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
                std::string text = fdp.ConsumeRandomLengthString(50);
                element.append_child(pugi::node_pcdata).set_value(text.c_str());
            }
            
            // Add multiple attributes for navigation chain testing
            int attr_count = fdp.ConsumeIntegralInRange<int>(1, 15);
            for (int j = 0; j < attr_count && fdp.remaining_bytes() > 10; ++j) {
                std::string attr_name = "attr" + std::to_string(j);
                std::string attr_value = "value" + std::to_string(j);
                
                if (fdp.remaining_bytes() > 10) {
                    attr_name = fdp.ConsumeRandomLengthString(10);
                    attr_value = fdp.ConsumeRandomLengthString(30);
                }
                
                element.append_attribute(attr_name.c_str()).set_value(attr_value.c_str());
            }
        }
        
        // Create nested elements for hierarchical navigation
        if (fdp.remaining_bytes() > 30) {
            pugi::xml_node nested = root.append_child("nested");
            
            // Add attributes with specific patterns for XPath testing
            nested.append_attribute("id") = "nested1";
            nested.append_attribute("class") = "container";
            nested.append_attribute("data-index") = "42";
            nested.append_attribute("enabled") = "true";
            nested.append_attribute("count") = "100";
            
            // Add child elements
            for (int k = 0; k < 5 && fdp.remaining_bytes() > 10; ++k) {
                pugi::xml_node child = nested.append_child("child");
                child.append_attribute("pos") = std::to_string(k).c_str();
                child.append_attribute("active") = (k % 2 == 0) ? "true" : "false";
            }
        }
    }
    
    // Step 2: Get root node for navigation
    pugi::xml_node root = doc.document_element();
    if (root.empty()) {
        root = doc.first_child();
        if (root.empty()) {
            return 0; // No document structure to navigate
        }
    }
    
    // Step 3: Comprehensive attribute navigation testing
    
    // 3.1 Test first_attribute() and last_attribute() on all elements
    std::vector<pugi::xml_node> nodes_with_attrs;
    
    // Collect all element nodes that might have attributes
    for (pugi::xml_node node = root; node; node = node.next_sibling()) {
        if (node.type() == pugi::node_element) {
            nodes_with_attrs.push_back(node);
            
            // Also collect children recursively
            for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
                if (child.type() == pugi::node_element) {
                    nodes_with_attrs.push_back(child);
                }
            }
        }
    }
    
    // Test attribute navigation chains on each node
    for (const auto& node : nodes_with_attrs) {
        // Get first and last attributes
        pugi::xml_attribute first_attr = node.first_attribute();
        pugi::xml_attribute last_attr = node.last_attribute();
        
        // Test forward navigation chain: first -> next -> next -> ...
        if (!first_attr.empty()) {
            pugi::xml_attribute current = first_attr;
            int forward_count = 0;
            
            while (!current.empty() && forward_count < 50) { // Safety limit
                // Access attribute properties
                const char* name = current.name();
                const char* value = current.value();
                std::string as_string = current.as_string();
                int as_int = current.as_int();
                double as_double = current.as_double();
                bool as_bool = current.as_bool();
                
                // Move to next attribute
                current = current.next_attribute();
                forward_count++;
            }
        }
        
        // Test backward navigation chain: last -> previous -> previous -> ...
        if (!last_attr.empty()) {
            pugi::xml_attribute current = last_attr;
            int backward_count = 0;
            
            while (!current.empty() && backward_count < 50) { // Safety limit
                // Test attribute comparison operations
                pugi::xml_attribute next = current.next_attribute();
                pugi::xml_attribute prev = current.previous_attribute();
                
                // Test empty() method
                bool is_empty = current.empty();
                
                // Test assignment and comparison
                pugi::xml_attribute copy = current;
                bool equal = (current == copy);
                bool not_equal = (current != next);
                
                // Move to previous attribute
                current = current.previous_attribute();
                backward_count++;
            }
        }
        
        // Test mixed navigation: start from middle if possible
        if (!first_attr.empty() && !first_attr.next_attribute().empty()) {
            pugi::xml_attribute middle = first_attr.next_attribute();
            
            // Navigate forward from middle
            pugi::xml_attribute forward = middle;
            while (!forward.empty()) {
                forward = forward.next_attribute();
            }
            
            // Navigate backward from middle
            pugi::xml_attribute backward = middle;
            while (!backward.empty()) {
                backward = backward.previous_attribute();
            }
        }
    }
    
    // Step 4: Test attribute() method with name lookup
    // Create various attribute name patterns from fuzzed data
    std::vector<std::string> attr_names_to_search;
    int search_name_count = fdp.ConsumeIntegralInRange<int>(0, 10);
    
    for (int i = 0; i < search_name_count && fdp.remaining_bytes() > 10; ++i) {
        std::string attr_name = fdp.ConsumeRandomLengthString(15);
        attr_names_to_search.push_back(attr_name);
    }
    
    // Add some common attribute names
    attr_names_to_search.push_back("id");
    attr_names_to_search.push_back("class");
    attr_names_to_search.push_back("name");
    attr_names_to_search.push_back("value");
    attr_names_to_search.push_back("type");
    
    // Search for attributes by name on each node
    for (const auto& node : nodes_with_attrs) {
        for (const auto& attr_name : attr_names_to_search) {
            pugi::xml_attribute found_attr = node.attribute(attr_name.c_str());
            
            if (!found_attr.empty()) {
                // Test found attribute
                std::string found_value = found_attr.as_string();
                
                // Test attribute with hint (optimized lookup)
                pugi::xml_attribute hint = node.first_attribute();
                pugi::xml_attribute found_with_hint = node.attribute(attr_name.c_str(), hint);
            }
        }
    }
    
    // Step 5: XPath-based search functions (select_node, select_nodes)
    
    // Create XPath variable set for parameterized queries
    pugi::xpath_variable_set vars;
    
    // Add some variables from fuzzed data
    int var_count = fdp.ConsumeIntegralInRange<int>(0, 5);
    for (int i = 0; i < var_count && fdp.remaining_bytes() > 20; ++i) {
        std::string var_name = "var" + std::to_string(i);
        int var_type = fdp.ConsumeIntegralInRange<int>(0, 3);
        
        switch (var_type) {
            case 0: { // boolean
                bool val = fdp.ConsumeBool();
                vars.add(var_name.c_str(), pugi::xpath_type_boolean);
                vars.set(var_name.c_str(), val);
                break;
            }
            case 1: { // number
                double val = fdp.ConsumeFloatingPointInRange<double>(-1000.0, 1000.0);
                vars.add(var_name.c_str(), pugi::xpath_type_number);
                vars.set(var_name.c_str(), val);
                break;
            }
            case 2: { // string
                std::string val = fdp.ConsumeRandomLengthString(50);
                vars.add(var_name.c_str(), pugi::xpath_type_string);
                vars.set(var_name.c_str(), val.c_str());
                break;
            }
            case 3: { // node-set
                // Create empty node set
                pugi::xpath_node_set empty_set;
                vars.add(var_name.c_str(), pugi::xpath_type_node_set);
                vars.set(var_name.c_str(), empty_set);
                break;
            }
        }
    }
    
    // Generate XPath queries from fuzzed data
    std::vector<std::string> xpath_queries;
    int query_count = fdp.ConsumeIntegralInRange<int>(1, 15);
    
    for (int i = 0; i < query_count && fdp.remaining_bytes() > 10; ++i) {
        std::string query = fdp.ConsumeRandomLengthString(100);
        xpath_queries.push_back(query);
    }
    
    // Add some meaningful XPath queries for testing common patterns
    xpath_queries.push_back("//*");
    xpath_queries.push_back("//@*");
    xpath_queries.push_back("//*[@*]");
    xpath_queries.push_back("//*[attribute()]");
    xpath_queries.push_back("//*[last()]");
    xpath_queries.push_back("//*[position() < 3]");
    xpath_queries.push_back("//*[@id]");
    xpath_queries.push_back("//*[@class='container']");
    xpath_queries.push_back("//*[text()]");
    xpath_queries.push_back("/root/*");
    xpath_queries.push_back("//child");
    xpath_queries.push_back("//@id");
    xpath_queries.push_back("//@class");
    xpath_queries.push_back("//*[@enabled='true']");
    xpath_queries.push_back("//*[count(@*) > 2]");
    
    // Test select_node() - single node selection
    for (const auto& query : xpath_queries) {
        try {
            // Test with and without variables
            bool use_vars = fdp.ConsumeBool() && var_count > 0;
            pugi::xpath_node selected_node;
            
            if (use_vars) {
                selected_node = root.select_node(query.c_str(), &vars);
            } else {
                selected_node = root.select_node(query.c_str());
            }
            
            if (selected_node) {
                // Test the selected node
                pugi::xml_node node = selected_node.node();
                pugi::xml_attribute attr = selected_node.attribute();
                
                if (node) {
                    // Navigate attributes on selected node
                    pugi::xml_attribute first = node.first_attribute();
                    pugi::xml_attribute last = node.last_attribute();
                    
                    // Test attribute navigation on selected node
                    for (pugi::xml_attribute a = first; a; a = a.next_attribute()) {
                        // Just accessing is enough for coverage
                    }
                }
                
                if (attr) {
                    // Test attribute properties
                    std::string attr_string = attr.as_string();
                }
            }
        } catch (...) {
            // XPath parsing errors are expected for malformed queries
        }
    }
    
    // Test select_nodes() - multiple node selection
    for (const auto& query : xpath_queries) {
        try {
            // Test with and without variables
            bool use_vars = fdp.ConsumeBool() && var_count > 0;
            pugi::xpath_node_set selected_nodes;
            
            if (use_vars) {
                selected_nodes = root.select_nodes(query.c_str(), &vars);
            } else {
                selected_nodes = root.select_nodes(query.c_str());
            }
            
            // Test node set properties and iteration
            size_t node_count = selected_nodes.size();
            bool empty = selected_nodes.empty();
            
            // Iterate through selected nodes
            for (size_t i = 0; i < node_count && i < 100; ++i) { // Safety limit
                pugi::xpath_node xnode = selected_nodes[i];
                pugi::xml_node node = xnode.node();
                pugi::xml_attribute attr = xnode.attribute();
                
                if (node) {
                    // Test attribute navigation on each selected node
                    pugi::xml_attribute first_attr = node.first_attribute();
                    pugi::xml_attribute last_attr = node.last_attribute();
                    
                    // Forward navigation
                    for (pugi::xml_attribute a = first_attr; a; a = a.next_attribute()) {
                        // Access attribute
                    }
                    
                    // Backward navigation (if we have last)
                    if (!last_attr.empty()) {
                        for (pugi::xml_attribute a = last_attr; a; a = a.previous_attribute()) {
                            // Access attribute
                        }
                    }
                }
                
                if (attr) {
                    // Test attribute value conversions
                    attr.as_string();
                    attr.as_int();
                    attr.as_double();
                    attr.as_bool();
                }
            }
            
            // Test node set iteration using iterator
            for (pugi::xpath_node_set::const_iterator it = selected_nodes.begin();
                 it != selected_nodes.end() && std::distance(selected_nodes.begin(), it) < 100; ++it) {
                pugi::xpath_node xnode = *it;
                // Access node or attribute
            }
        } catch (...) {
            // XPath parsing errors are expected
        }
    }
    
    // Step 6: Combined attribute and node navigation scenarios
    
    // Scenario 1: Find nodes via XPath, then navigate their attributes
    try {
        pugi::xpath_node_set elements_with_attrs = root.select_nodes("//*[@*]");
        
        for (size_t i = 0; i < elements_with_attrs.size() && i < 50; ++i) {
            pugi::xml_node elem = elements_with_attrs[i].node();
            if (elem) {
                // Comprehensive attribute navigation
                pugi::xml_attribute attr = elem.first_attribute();
                while (attr) {
                    // Test all navigation methods
                    pugi::xml_attribute next = attr.next_attribute();
                    pugi::xml_attribute prev = attr.previous_attribute();
                    
                    // Move to next
                    attr = next;
                }
            }
        }
    } catch (...) {
        // Ignore XPath errors
    }
    
    // Scenario 2: Navigate document tree and for each element, navigate attributes
    for (pugi::xml_node node = root; node; node = node.next_sibling()) {
        if (node.type() == pugi::node_element) {
            // Test attribute existence check
            bool has_attributes = !node.first_attribute().empty();
            
            if (has_attributes) {
                // Forward chain
                pugi::xml_attribute attr = node.first_attribute();
                while (attr) {
                    attr = attr.next_attribute();
                }
                
                // Backward chain (if we can get to last)
                pugi::xml_attribute last = node.last_attribute();
                if (!last.empty()) {
                    pugi::xml_attribute attr2 = last;
                    while (attr2) {
                        attr2 = attr2.previous_attribute();
                    }
                }
            }
        }
    }
    
    // Scenario 3: Create xpath_query objects and evaluate on nodes with attributes
    if (fdp.remaining_bytes() > 50) {
        std::string complex_query = fdp.ConsumeRandomLengthString(80);
        
        try {
            pugi::xpath_query query(complex_query.c_str());
            
            if (query) {
                // Evaluate on root
                bool bool_result = query.evaluate_boolean(root);
                double num_result = query.evaluate_number(root);
                std::string string_result = query.evaluate_string(root).c_str();
                pugi::xpath_node_set node_set_result = query.evaluate_node_set(root);
                
                // For node set results, test attribute navigation
                for (size_t i = 0; i < node_set_result.size() && i < 20; ++i) {
                    pugi::xml_node result_node = node_set_result[i].node();
                    if (result_node) {
                        // Navigate attributes
                        pugi::xml_attribute attr = result_node.first_attribute();
                        while (attr) {
                            attr = attr.next_attribute();
                        }
                    }
                }
            }
        } catch (...) {
            // Query compilation errors are expected
        }
    }
    
    return 0;
}

} // namespace harness_010
