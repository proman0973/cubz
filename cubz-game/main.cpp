#include <core/Engine.h>
#include <utility/Log.h>
#include <graphics/Mesh.h>
#include <graphics/MeshRenderer.h>
#include <graphics/BasicComponents.h>
#include <graphics/ui/DebugView.h>
#include <glm/gtc/matrix_transform.hpp>
#include <core/LightRenderSystem.h>
#include <core/SkyboxRenderSystem.h>
#include <graphics/SkyboxRenderer.h>

#include "ChunkUpdateSystem.h"
#include "World.h"
#include "ChunkEntity.h"

int main(int argc, char **argv) {
    try {
        auto engine = cubz::core::Engine(800, 600, "Test Window", "./assets");

        auto& context = engine.getContext();
        auto& ecs = engine.getECS();
        auto& camera = engine.getCamera();
        auto& resourceManager = engine.getResourceManager();

        ecs.registerComponent<cubz::graphics::Transform>();
        ecs.registerComponent<cubz::graphics::MeshRenderer>();
        ecs.registerComponent<cubz::graphics::PointLight>();
        ecs.registerComponent<cubz::graphics::Mesh>();
        ecs.registerComponent<cubz::graphics::SkyboxRenderer>();
        ecs.registerComponent<cubz::game::ChunkData>();

        // Register chunk update system
        auto chunkUpdateSystem = ecs.registerSystem<cubz::game::ChunkUpdateSystem>();
        auto signature = cubz::ecs::Signature();
        signature.set(ecs.getComponentType<cubz::game::ChunkData>());
        signature.set(ecs.getComponentType<cubz::graphics::Mesh>());
        signature.set(ecs.getComponentType<cubz::graphics::Transform>());
        ecs.setSystemSignature<cubz::game::ChunkUpdateSystem>(signature);

        // Mesh rendering system
        auto meshRenderSystem = ecs.registerSystem<cubz::core::MeshRenderSystem>();
        signature = cubz::ecs::Signature();
        signature.set(ecs.getComponentType<cubz::graphics::Transform>());
        signature.set(ecs.getComponentType<cubz::graphics::MeshRenderer>());
        signature.set(ecs.getComponentType<cubz::graphics::Mesh>());
        ecs.setSystemSignature<cubz::core::MeshRenderSystem>(signature);

        // Skybox rendering system
        auto skyboxRenderSystem = ecs.registerSystem<cubz::core::SkyboxRenderSystem>();
        signature = cubz::ecs::Signature();
        signature.set(ecs.getComponentType<cubz::graphics::SkyboxRenderer>());
        ecs.setSystemSignature<cubz::core::SkyboxRenderSystem>(signature);

        // Light rendering system
        // Create sun
        auto sun = cubz::graphics::DirectionalLight {
            { 1.0f, -1.0f, 1.0f },
            glm::vec3(0.2f),
            glm::vec3(1.0f),
            glm::vec3(1.0f)
        };

        auto lightRenderSystem = ecs.registerSystem<cubz::core::LightRenderSystem>(&resourceManager, &sun);
        signature = cubz::ecs::Signature();
        signature.set(ecs.getComponentType<cubz::graphics::PointLight>());
        ecs.setSystemSignature<cubz::core::LightRenderSystem>(signature);

        // Create skybox
        auto skybox = ecs.createEntity();
        ecs.addComponent<cubz::graphics::SkyboxRenderer>(skybox, cubz::graphics::SkyboxRenderer {
            resourceManager.getCubeMap("skybox"),
            resourceManager.getShader("skybox")
        });

        cubz::game::World world(&engine);

        context.setClearColor({0, 0, 0});

        camera.setPosition(glm::vec3 { 4, 1, 3 } * 10.0f);
        camera.lookAt({ 0, 0, 0 });

        // Test light
        // TODO: add torches
        auto testLight = ecs.createEntity();
        ecs.addComponent<cubz::graphics::PointLight>(testLight, cubz::graphics::PointLight {
            { 0, 0, 0 },
            1.0f,//1.0f, 2.0f
            0.7f,//0.09f, 0.8f
            1.8f,//0.032f, 2.0f
            glm::vec3(0.1f),
            { 5.0f, 1.0f, 1.0f },
            { 5.0f, 1.0f, 1.0f },
            true
        });
        ecs.updateEntity(testLight);

        int i = 0;
        float deltaSum = 0;
        int fpsFrequency = 250;

        while (!context.windowClosed()) {
            float deltaTime = context.getDeltaTime();

            deltaSum += deltaTime;

            if (i++ == fpsFrequency) {
                float averageDeltaTime = deltaSum / fpsFrequency;
                int fps = 1.0f / averageDeltaTime;

                cubz::utility::Log::debug("Fps: " + std::to_string(fps));

                i = 0;
                deltaSum = 0;
            }

            context.clear();
            lightRenderSystem->update(camera.getPosition(), true);
            chunkUpdateSystem->updateChunks();
            meshRenderSystem->update(deltaTime);
            meshRenderSystem->render(camera);
            skyboxRenderSystem->render(camera);
            context.render();
        }
    } catch (std::exception& e) {
        cubz::utility::Log::error(e);
        return 1;
    }

    return 0;
}