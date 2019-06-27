#pragma once

#include <glm/mat4x4.hpp>

namespace cubz::graphics {
    class Camera {
    public:
        Camera(int screenWidth, int screenHeight);

        void lookAt(const glm::vec3& target);
        void setPosition(const glm::vec3& position);
        void setDirection(const glm::vec3& direction);
        void setRotation(float yaw, float pitch);

        glm::mat4 getModelViewProjection(const glm::mat4& model) const;
        glm::mat4 getTextProjection() const;
        glm::vec3 getPosition() const;
        glm::vec3 getDirection() const;

    private:
        glm::mat4 m_textProjection;
        glm::mat4 m_projection;
        glm::vec3 m_position;
        glm::vec3 m_direction;
        glm::vec3 m_right;
    };
}