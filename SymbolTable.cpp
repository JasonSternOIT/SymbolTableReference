#include <memory>
#include <string>
#include <vector>
#include "Node.hpp"
#include "SymbolTable.hpp"

class SymbolTableImplementation : public SymbolTable, std::enable_shared_from_this<SymbolTableImplementation>
{
private:
    std::vector<Node> children;
public:
    std::shared_ptr<Node> lookup(const std::string& name)
    {
        std::shared_ptr<Node> result = nullptr;
        for (auto& child : children)
        {
            if (child.getName() == name)
            {
                return std::make_shared<Node>(child);
            }
            result = child.lookup(name);
            if (result)
            {
                break;
            }
        }
        return result;       
    }

    virtual void insert(const std::string& name, std::shared_ptr<Node> node)
    {
        children.push_back(*node);
    }

    virtual ~SymbolTableImplementation() = default;
};

std::shared_ptr<SymbolTable> SymbolTable::createInstance()
{
    return std::dynamic_pointer_cast<SymbolTable>(std::make_shared<SymbolTableImplementation>());
}
