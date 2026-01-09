#include "Node.hpp"

#include <memory>
#include <vector>

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
        if (this->name == name)
        {
            return shared_from_this();
        }        
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

std::shared_ptr<Node> Node::createNode(const std::shared_ptr<Node> parent,
    const std::string& name, std::uint32_t unique_identifier,  SymbolType type,
    std::string file, std::uint32_t line)
{
    return std::dynamic_pointer_cast<Node>(std::make_shared<NodeImplementation>(parent, name,
         unique_identifier, type, file, line));
}