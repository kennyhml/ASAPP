#pragma once
#include <exception>
#include <filesystem>

namespace asa::items
{
    class Item;

    class ItemError : public std::exception
    {
    public:
        explicit ItemError(const Item* t_item);
        explicit ItemError(const Item* t_item, const std::string& t_message);

        const char* what() const override { return info_.c_str(); }

    protected:
        const Item* item_;
        std::string info_;
    };

    class ItemIconNotFound final : public ItemError
    {
    public:
        explicit ItemIconNotFound(const std::filesystem::path&);
    };


    class ItemDataNotFound final : public ItemError
    {
    public:
        explicit ItemDataNotFound(const std::string& t_item_name);
    };
}
