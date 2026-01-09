#include "Node.hpp"

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

static std::uint32_t global_identifier_counter = 0;
static std::unordered_map<std::uint32_t, std::weak_ptr<Node>> node_registry;
static std::mutex node_registry_mutex;

class NodeImplementation : public Node, public std::enable_shared_from_this<NodeImplementation>
{
private:
    std::weak_ptr<Node> parent;
    std::string name;
    std::uint32_t unique_identifier;
    SymbolType type;
    std::string file;
    std::uint32_t line;
    std::vector<std::shared_ptr<Node>> children;

public:
    NodeImplementation(const std::shared_ptr<Node> parent,
        const std::string& name, std::uint32_t unique_identifier,
        SymbolType type, std::string file, std::uint32_t line)
        : parent(parent), name(name), unique_identifier(unique_identifier), type(type), file(file), line(line)
        {        
        }

    virtual ~NodeImplementation() = default;

    virtual std::shared_ptr<Node> lookup(const std::string& name) override
    {
        // four paths for this. the correct one depends on use case:
        // we are the object
        if (this->name == name)
        {
            return shared_from_this();
        }           
        // we are the scope but not the object, traverse up
        if (auto node_parent = parent.lock())
        {
            return node_parent->lookup(name);
        }
        // we are the parent, traverse down        
        for (auto& child : children)
        {
            if (child->getName() == name)
            {
                return child;
            }
            auto result = child->lookup(name);
            if (result)
            {
                return result;
            }
        }
        // the object doesn't exist within the scope of the symbol table
        return nullptr;
    }

    virtual std::string getName() const override
    {
        return name;
    }

    virtual std::uint32_t getIdentifier() const override
    {
        return unique_identifier;
    }

    virtual SymbolType getType() const override
    {
        return type;
    }

    virtual std::string getFile() const override
    {
        return file;
    }

    virtual std::uint32_t getLine() const override
    {
        return line;
    }

    virtual std::weak_ptr<Node> getParent() const override
    {
        return parent;
    }

    virtual void addChild(std::shared_ptr<Node> child) override
    {
        children.emplace_back(child);
    }
};

std::shared_ptr<Node> Node::lookup(const std::uint32_t identifier)
{
    std::lock_guard<std::mutex> guard(node_registry_mutex);
    auto node = node_registry.find(identifier);
    if (node != node_registry.end())
    {
        if (auto shared_node = node->second.lock())
        {
            return shared_node;
        }
    }
    return nullptr;
}

std::shared_ptr<Node> Node::createNode(const std::shared_ptr<Node> parent,
    const std::string& name, const SymbolType type,
    const std::string& file, const std::uint32_t line)
{
    std::lock_guard<std::mutex> guard(node_registry_mutex);
    ++global_identifier_counter;
    auto node = std::make_shared<NodeImplementation>(parent, name, global_identifier_counter, type, file, line);
    node_registry[global_identifier_counter] = node;
    return node;
}