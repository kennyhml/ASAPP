#pragma once
#include "baseentity.h"
#include "asa/core/exceptions.h"
#include "asa/structures/basestructure.h"

namespace asa
{
    /**
     * @brief Parent class for any exception that may be thrown by an entity.
     */
    class entity_error : public asapp_error
    {
    public:
        explicit entity_error(const base_entity* t_entity);

        entity_error(const base_entity* t_entity, std::string t_info);

        /**
         * @brief Gets the pointer to the entity this error occured on.
         */
        [[nodiscard]] const base_entity* get_entity() const { return entity_; };

    private:
        const base_entity* entity_;
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
     * @brief Thrown when an entity could not be accessed.
     */
    class structure_access_failed final : public entity_error
    {
    public:
        explicit structure_access_failed(const base_structure* t_entity);
    };

    /**
     * @brief Thrown when an entity could not be accessed.
     */
    class target_not_in_range final : public entity_error
    {
    public:
        explicit target_not_in_range(const std::string& t_target);
    };

    class deposit_failed final : public entity_error
    {
    public:
        explicit deposit_failed(const std::string& t_what);
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
