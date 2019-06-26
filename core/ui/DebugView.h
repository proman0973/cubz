#pragma once

#include "FreetypeText.h"
#include "Camera.h"
#include "TextGrid.h"

namespace core::ui {
    class DebugView {
    public:
        DebugView(const FreetypeFont& font);

        void render(const Camera& camera);

    private:
        std::string getFpsText(float deltaTime);
        std::string getPositionText(const glm::vec3& position);
        std::string getDirectionText(float yaw, float pitch);

        FreetypeText m_positionText;
        FreetypeText m_directionText;
        FreetypeText m_fpsText;
        TextGrid m_textGrid;
    };
}