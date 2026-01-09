#pragma once
#include <memory>
#include <string>

class Node
{
public:
    enum class SymbolType
    {
        VARIABLE,
        FUNCTION,
        CLASS,
        NAMESPACE
    };

    virtual std::shared_ptr<Node> lookup(const std::string& name) = 0;
    virtual std::string getName() const = 0;
    virtual std::uint32_t getIdentifier() const = 0;
    virtual SymbolType getType() const = 0;
    virtual std::string getFile() const = 0;
    virtual std::uint32_t getLine() const = 0;
    virtual std::string getName() const = 0;
    virtual std::weak_ptr<Node> getParent() const = 0;
    virtual void addChild(std::shared_ptr<Node> child) = 0;

    static std::shared_ptr<Node> createNode(const std::shared_ptr<Node> parent,
        const std::string& name, std::uint32_t unique_identifier, SymbolType type,
        std::string file, std::uint32_t line);
};