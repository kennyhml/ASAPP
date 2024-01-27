#pragma once
#include <stdexcept>
#include "baseentity.h"
#include "dinoent.h"

namespace asa::entities
{
    /**
     * @brief Parent class for any exception that may be thrown while
     * interacting with an entity.
     */
    class EntityError : public std::exception
    {
    private:
        std::string info;
        const BaseEntity* entity;

    public:
        explicit EntityError(const BaseEntity* t_entity);
        EntityError(const BaseEntity* t_entity, std::string t_info);

        /**
         * @ Gets the pointer to the entity this error occured on.
         */
        [[nodiscard]] const BaseEntity* get_entity() const { return entity; };
        [[nodiscard]] const char* what() const noexcept override;
    };

    /**
     * @brief Thrown when an entity could not be accessed.
     */
    class EntityNotAccessed : public EntityError
    {
    public:
        explicit EntityNotAccessed(const BaseEntity*);
    };

    /**
     * @brief Thrown when a dino entity could not be mounted.
     */
    class EntityNotMounted : public EntityError
    {
    public:
        explicit EntityNotMounted(const DinoEnt*);
    };

    /**
     * @brief Thrown when our player was unable to suicide.
     */
    class SuicideFailedError : public EntityError
    {
    public:
        SuicideFailedError();
    };

    /**
     * @brief Thrown when our player was unable to fast travel.
     */
    class FastTravelFailedError : public EntityError
    {
    public:
        explicit FastTravelFailedError(const std::string& t_where);
        FastTravelFailedError(const std::string& t_where, const std::string& t_why);

    };

    /**
     * @brief Thrown when our player was unable to teleport.
     */
    class TeleportFailedError : public EntityError
    {
    public:
        explicit TeleportFailedError(const std::string& t_where);
        TeleportFailedError(const std::string& t_where, const std::string& t_why);

    };




}
