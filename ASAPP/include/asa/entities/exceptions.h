#pragma once
#include "baseentity.h"
#include "asa/core/exceptions.h"

namespace asa
{
    /**
     * @brief Parent class for any exception that may be thrown by an entity.
     */
    class entity_error : public asapp_error
    {
    public:
        explicit entity_error(const base_entity* t_entity);

        entity_error(base_entity* t_entity, std::string t_info);

        /**
         * @brief Gets the pointer to the entity this error occured on.
         */
        [[nodiscard]] base_entity* get_entity() const { return entity_; };

        /**
         * @brief Gets the error information that was provided.
         */
        [[nodiscard]] const char* what() const noexcept override;

    private:
        std::string info_;
        base_entity* entity_;
    };

    /**
     * @brief Thrown when an entity could not be accessed.
     */
    class entity_access_failed final : public entity_error
    {
    public:
        explicit entity_access_failed(const base_entity* t_entity);
    };

    /**
     * @brief Thrown when a dino entity could not be mounted.
     */
    class entity_mount_failed final : public entity_error
    {
    public:
        explicit entity_mount_failed(const base_entity* t_entity);
    };

    /**
     * @brief Thrown when our player was unable to suicide.
     */
    class suicide_failed final : public entity_error
    {
    public:
        suicide_failed();
    };

    class travel_failed : public entity_error
    {
    public:
        explicit travel_failed(const std::string& t_where);

        travel_failed(const std::string& t_where, const std::string& t_why);
    };

    /**
     * @brief Thrown when our player was unable to fast travel.
     */
    class fast_travel_failed final : public travel_failed
    {
    public:
        using travel_failed::travel_failed;
    };

    /**
     * @brief Thrown when our player was unable to teleport.
     */
    class teleport_failed final : public travel_failed
    {
    public:
        using travel_failed::travel_failed;
    };
}
