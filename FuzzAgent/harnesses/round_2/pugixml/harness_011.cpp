#include <fuzzer/FuzzedDataProvider.h>
#include "pugixml.hpp"
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace harness_011 {


// Custom tree walker for traverse() API testing
class AdvancedNavigationWalker : public pugi::xml_tree_walker {
private:
    FuzzedDataProvider* fdp;
    int max_depth;
    bool collect_paths;
    std::vector<std::string> node_paths;
    
public:
    AdvancedNavigationWalker(FuzzedDataProvider* provider, bool collect_paths_flag)
        : fdp(provider), max_depth(0), collect_paths(collect_paths_flag) {
    }
    
    virtual bool begin(pugi::xml_node& node) override {
        if (collect_paths) {
            node_paths.clear();
            max_depth = 0;
        }
        return true; // Always continue traversal
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        if (collect_paths) {
            // Track current depth
            int current_depth = depth();
            if (current_depth > max_depth) {
                max_depth = current_depth;
            }
            
            // Collect path if element node
            if (node.type() == pugi::node_element) {
                #ifndef PUGIXML_NO_STL
                std::string node_path = node.path();
                node_paths.push_back(node_path);
                #endif
            }
        }
        
        // Occasionally test parent() during traversal
        if (fdp->ConsumeBool()) {
            pugi::xml_node parent = node.parent();
            (void)parent; // Use to avoid unused variable warning
        }
        
        // Continue traversal based on fuzzed input
        return fdp->ConsumeBool();
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Use collected paths if any
        if (collect_paths && !node_paths.empty()) {
            // Paths are available in node_paths vector
            (void)node_paths.size();
        }
        return true;
    }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful navigation testing
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create or parse XML document for navigation testing
    // Split input: first part for XML content, rest for navigation parameters
    size_t xml_size = fdp.ConsumeIntegralInRange<size_t>(32, size / 2);
    std::vector<uint8_t> xml_buffer = fdp.ConsumeBytes<uint8_t>(xml_size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_buffer(xml_buffer.data(), xml_buffer.size(), pugi::parse_full);
    
    // If parsing failed, create a structured document with many elements/attributes for navigation testing
    if (!parse_result) {
        doc.reset();
        
        // Create root with fuzzed name
        std::string root_name = fdp.remaining_bytes() > 5 ? fdp.ConsumeRandomLengthString(10) : "root";
        pugi::xml_node root = doc.append_child(pugi::node_element);
        root.set_name(root_name.c_str());
        
        // Create hierarchical structure with various element names and attributes
        int depth_levels = fdp.ConsumeIntegralInRange<int>(1, 5);
        std::vector<pugi::xml_node> current_level;
        current_level.push_back(root);
        
        for (int level = 0; level < depth_levels && fdp.remaining_bytes() > 20; level++) {
            std::vector<pugi::xml_node> next_level;
            
            for (auto& parent_node : current_level) {
                int child_count = fdp.ConsumeIntegralInRange<int>(1, 8);
                
                for (int i = 0; i < child_count && fdp.remaining_bytes() > 15; i++) {
                    // Create child with fuzzed name
                    std::string child_name;
                    if (fdp.ConsumeBool()) {
                        child_name = fdp.ConsumeRandomLengthString(8);
                    } else {
                        // Use common names for testing find functions
                        child_name = "item";
                    }
                    
                    pugi::xml_node child = parent_node.append_child(child_name.c_str());
                    
                    // Add attributes with various patterns
                    int attr_count = fdp.ConsumeIntegralInRange<int>(0, 6);
                    for (int j = 0; j < attr_count && fdp.remaining_bytes() > 10; j++) {
                        std::string attr_name;
                        if (fdp.ConsumeBool()) {
                            attr_name = fdp.ConsumeRandomLengthString(6);
                        } else {
                            // Use common attribute names for testing
                            attr_name = (j % 2 == 0) ? "id" : "class";
                        }
                        
                        std::string attr_value;
                        if (fdp.ConsumeBool()) {
                            attr_value = fdp.ConsumeRandomLengthString(20);
                        } else {
                            attr_value = std::to_string(i * 10 + j);
                        }
                        
                        child.append_attribute(attr_name.c_str()).set_value(attr_value.c_str());
                    }
                    
                    // Add text content occasionally
                    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
                        std::string text = fdp.ConsumeRandomLengthString(30);
                        child.append_child(pugi::node_pcdata).set_value(text.c_str());
                    }
                    
                    next_level.push_back(child);
                }
            }
            
            current_level = std::move(next_level);
        }
        
        // Add some elements with specific attribute patterns for find_child_by_attribute testing
        if (fdp.remaining_bytes() > 30) {
            pugi::xml_node special_container = root.append_child("container");
            special_container.append_attribute("type") = "special";
            special_container.append_attribute("id") = "container-1";
            
            for (int i = 0; i < 5; i++) {
                pugi::xml_node special_item = special_container.append_child("item");
                special_item.append_attribute("id") = ("item-" + std::to_string(i)).c_str();
                special_item.append_attribute("category") = (i % 2 == 0) ? "even" : "odd";
                special_item.append_attribute("value") = std::to_string(i * 100).c_str();
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
    
    // Step 3: Comprehensive navigation API testing
    
    // 3.1 Test find_child_by_attribute (two overloads) - TARGET UNCOVERED API
    std::vector<std::string> attr_names_to_search;
    std::vector<std::string> attr_values_to_search;
    std::vector<std::string> element_names_to_search;
    
    // Generate search parameters from fuzzed data
    int search_param_count = fdp.ConsumeIntegralInRange<int>(0, 10);
    for (int i = 0; i < search_param_count && fdp.remaining_bytes() > 10; i++) {
        attr_names_to_search.push_back(fdp.ConsumeRandomLengthString(8));
        attr_values_to_search.push_back(fdp.ConsumeRandomLengthString(15));
        element_names_to_search.push_back(fdp.ConsumeRandomLengthString(8));
    }
    
    // Add common search patterns
    attr_names_to_search.push_back("id");
    attr_names_to_search.push_back("class");
    attr_names_to_search.push_back("type");
    attr_names_to_search.push_back("name");
    attr_names_to_search.push_back("value");
    
    attr_values_to_search.push_back("item-1");
    attr_values_to_search.push_back("special");
    attr_values_to_search.push_back("container-1");
    attr_values_to_search.push_back("even");
    attr_values_to_search.push_back("odd");
    
    element_names_to_search.push_back("item");
    element_names_to_search.push_back("container");
    element_names_to_search.push_back("root");
    element_names_to_search.push_back("element");
    
    // Test find_child_by_attribute with name+attr_name+attr_value
    for (const auto& elem_name : element_names_to_search) {
        for (const auto& attr_name : attr_names_to_search) {
            for (const auto& attr_value : attr_values_to_search) {
                pugi::xml_node found = root.find_child_by_attribute(
                    elem_name.c_str(), 
                    attr_name.c_str(), 
                    attr_value.c_str()
                );
                (void)found; // Use result
            }
        }
    }
    
    // Test find_child_by_attribute with just attr_name+attr_value (any element)
    for (const auto& attr_name : attr_names_to_search) {
        for (const auto& attr_value : attr_values_to_search) {
            pugi::xml_node found = root.find_child_by_attribute(
                attr_name.c_str(), 
                attr_value.c_str()
            );
            (void)found; // Use result
        }
    }
    
    // 3.2 Test first_element_by_path - TARGET UNCOVERED API
    std::vector<std::string> path_queries;
    int path_query_count = fdp.ConsumeIntegralInRange<int>(0, 15);
    
    for (int i = 0; i < path_query_count && fdp.remaining_bytes() > 5; i++) {
        path_queries.push_back(fdp.ConsumeRandomLengthString(50));
    }
    
    // Add meaningful path queries
    path_queries.push_back("/");
    path_queries.push_back("/*");
    path_queries.push_back("//*");
    path_queries.push_back("//item");
    path_queries.push_back("//container/item");
    path_queries.push_back("/*/*");
    path_queries.push_back("./");
    path_queries.push_back("../");
    path_queries.push_back(".");
    path_queries.push_back("..");
    
    for (const auto& path_query : path_queries) {
        pugi::xml_node found_by_path = root.first_element_by_path(path_query.c_str(), '/');
        (void)found_by_path; // Use result
        
        // Also test with different delimiter
        if (fdp.ConsumeBool()) {
            pugi::xml_node found_with_dot = root.first_element_by_path(path_query.c_str(), '.');
            (void)found_with_dot;
        }
    }
    
    // 3.3 Test named sibling/child navigation functions - TARGET UNCOVERED API
    // Collect all element nodes for navigation testing
    std::vector<pugi::xml_node> all_elements;
    for (pugi::xml_node node = root; node; node = node.next_sibling()) {
        if (node.type() == pugi::node_element) {
            all_elements.push_back(node);
            
            // Collect children recursively
            for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
                if (child.type() == pugi::node_element) {
                    all_elements.push_back(child);
                }
            }
        }
    }
    
    // Test next_sibling(const char*) and similar functions
    for (const auto& node : all_elements) {
        for (const auto& name : element_names_to_search) {
            // Test next_sibling with name filter
            pugi::xml_node next_named = node.next_sibling(name.c_str());
            (void)next_named;
            
            // Test child with name filter
            pugi::xml_node child_named = node.child(name.c_str());
            (void)child_named;
            
            // Test previous_sibling with name filter (if available in traversal)
            if (!node.previous_sibling().empty()) {
                // Note: previous_sibling doesn't have a name-filtered version in pugixml
                // But we can test the pattern
            }
        }
    }
    
    // 3.4 Test parent() function - TARGET UNCOVERED API
    for (const auto& node : all_elements) {
        pugi::xml_node parent = node.parent();
        (void)parent;
        
        // Test parent chain traversal
        pugi::xml_node current = node;
        int max_parent_traversal = 10; // Safety limit
        while (!current.parent().empty() && max_parent_traversal-- > 0) {
            current = current.parent();
        }
    }
    
    // 3.5 Test range-based children() and children(const char*) - TARGET UNCOVERED API
    for (const auto& node : all_elements) {
        // Test children() range (all children)
        #ifndef PUGIXML_NO_STL
        auto all_children_range = node.children();
        for (auto child : all_children_range) {
            (void)child; // Use each child
        }
        #endif
        
        // Test children(const char*) range (named children)
        for (const auto& name : element_names_to_search) {
            #ifndef PUGIXML_NO_STL
            auto named_children_range = node.children(name.c_str());
            for (auto named_child : named_children_range) {
                (void)named_child;
            }
            #endif
        }
    }
    
    // 3.6 Test traverse() with custom walker - TARGET UNCOVERED API
    bool collect_paths = fdp.ConsumeBool();
    AdvancedNavigationWalker walker(&fdp, collect_paths);
    
    for (const auto& element : all_elements) {
        if (fdp.ConsumeBool()) {
            // Create a non-const copy for traverse() which requires non-const node
            pugi::xml_node start_node = element;
            bool traversal_result = start_node.traverse(walker);
            (void)traversal_result;
        }
    }
    
    // 3.7 Test path() method (STL only) - TARGET UNCOVERED API
    #ifndef PUGIXML_NO_STL
    for (const auto& node : all_elements) {
        if (node.type() == pugi::node_element) {
            std::string node_path = node.path();
            (void)node_path;
            
            // Test with different delimiter
            if (fdp.ConsumeBool()) {
                std::string path_with_dot = node.path('.');
                (void)path_with_dot;
            }
        }
    }
    #endif
    
    // 3.8 Test advanced navigation patterns combining multiple APIs
    
    // Pattern 1: Find element by attribute, then navigate its siblings
    for (const auto& attr_name : attr_names_to_search) {
        for (const auto& attr_value : attr_values_to_search) {
            pugi::xml_node found = root.find_child_by_attribute(attr_name.c_str(), attr_value.c_str());
            if (!found.empty()) {
                // Navigate siblings
                pugi::xml_node sibling = found;
                int sibling_count = 0;
                while (!(sibling = sibling.next_sibling()).empty() && sibling_count++ < 20) {
                    (void)sibling;
                }
                
                // Navigate parent and its children
                pugi::xml_node parent = found.parent();
                if (!parent.empty()) {
                    for (pugi::xml_node child = parent.first_child(); child; child = child.next_sibling()) {
                        (void)child;
                    }
                }
            }
        }
    }
    
    // Pattern 2: Use path-based navigation combined with attribute search
    for (const auto& path_query : path_queries) {
        pugi::xml_node path_node = root.first_element_by_path(path_query.c_str());
        if (!path_node.empty()) {
            // Search for attributes on path-found node
            for (const auto& attr_name : attr_names_to_search) {
                pugi::xml_attribute attr = path_node.attribute(attr_name.c_str());
                if (!attr.empty()) {
                    // Use attribute value to find related nodes
                    std::string attr_value = attr.value();
                    pugi::xml_node related = root.find_child_by_attribute(attr_name.c_str(), attr_value.c_str());
                    (void)related;
                }
            }
        }
    }
    
    // Pattern 3: Comprehensive navigation chain
    if (!all_elements.empty()) {
        pugi::xml_node start_node = all_elements[fdp.ConsumeIntegralInRange<size_t>(0, all_elements.size() - 1)];
        
        // Chain: parent -> children -> next_sibling -> previous_sibling
        pugi::xml_node current = start_node;
        
        // Go up to root
        while (!current.parent().empty()) {
            current = current.parent();
        }
        
        // Traverse down through children
        std::function<void(pugi::xml_node, int)> traverse_tree = [&](pugi::xml_node node, int depth) {
            if (depth > 5) return; // Limit depth
            
            // Process current node
            (void)node;
            
            // Process all children
            for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
                traverse_tree(child, depth + 1);
            }
        };
        
        traverse_tree(current, 0);
    }
    
    // Step 4: Edge case testing for navigation APIs
    
    // Test with empty nodes
    pugi::xml_node empty_node;
    (void)empty_node.find_child_by_attribute("test", "value");
    (void)empty_node.first_element_by_path("test");
    (void)empty_node.next_sibling("name");
    (void)empty_node.child("name");
    (void)empty_node.parent();
    #ifndef PUGIXML_NO_STL
    (void)empty_node.path();
    #endif
    
    // Test with very long strings
    if (fdp.remaining_bytes() > 100) {
        std::string long_name = fdp.ConsumeRandomLengthString(500);
        std::string long_value = fdp.ConsumeRandomLengthString(1000);
        std::string long_path = fdp.ConsumeRandomLengthString(300);
        
        (void)root.find_child_by_attribute(long_name.c_str(), long_value.c_str());
        (void)root.first_element_by_path(long_path.c_str());
        (void)root.next_sibling(long_name.c_str());
    }
    
    // Test with special characters in names/paths
    std::vector<std::string> special_patterns = {
        "..", ".", "/", "//", "*", "@", "[", "]", "(", ")", "{", "}",
        "test[1]", "item/@id", "//*[@id]", "element[position()=1]"
    };
    
    for (const auto& pattern : special_patterns) {
        (void)root.find_child_by_attribute(pattern.c_str(), "value");
        (void)root.first_element_by_path(pattern.c_str());
        (void)root.next_sibling(pattern.c_str());
    }
    
    return 0;
}

} // namespace harness_011
