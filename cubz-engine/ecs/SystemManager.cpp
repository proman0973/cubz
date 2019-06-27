#include "SystemManager.h"

namespace cubz::ecs {
    System::System(EntityComponentSystem *ecs) : m_ecs(ecs) {}

    void System::addEntity(Entity entity) {
        m_entities.insert(entity);
    }

    void System::removeEntity(Entity entity) {
        m_entities.erase(entity);
    }

    void SystemManager::entityDestroyed(Entity entity) {
        for (const auto& [typeName, system] : m_systems) {
            system->removeEntity(entity);
        }
    }

    void SystemManager::entitySignatureChanged(Entity entity, Signature entitySignature) {
        for (const auto& [typeName, system] : m_systems) {
            const auto& systemSignature = m_signatures[typeName];

            // Signatures match
            if ((entitySignature & systemSignature) == systemSignature) {
                system->addEntity(entity);
            } else {
                system->removeEntity(entity);
            }
        }
    }
}