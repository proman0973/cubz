#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "utility/Log.h"
#include "utility/ConsoleLogger.h"
#include "graphics/Screen.h"
#include "graphics/Model.h"
#include "graphics/FreetypeContext.h"
#include "graphics/FreetypeText.h"
#include "graphics/ShaderManager.h"

using namespace std::string_literals;

int main() {
    utility::Log::addLogger(std::make_shared<utility::ConsoleLogger>());

    try {
        auto screen = graphics::opengl::Screen(800, 600, "Cubz - Experimental Version");
        auto shaderManager = graphics::ShaderManager();
        auto model = graphics::Model(shaderManager.getShader("triangle"), "stone.bmp");
        auto fontCtx = graphics::FreetypeContext();
        auto font = fontCtx.generateFont("Minecraftia_Regular.ttf", 24);
        auto disclaimerText = graphics::FreetypeText(font, shaderManager.getShader("text"));

        disclaimerText.setText("https://github.com/proman973");
        disclaimerText.setPosition(800.0f - disclaimerText.getWidth() - 10.0f, 10.0f);

        while (!screen.shouldQuit()) {
            screen.clear();
            model.render(screen.getCamera());
            disclaimerText.render(screen.getCamera());
            screen.render();
        }
    } catch (std::exception &e) {
        utility::Log::error(e);
    }

    return 0;
}