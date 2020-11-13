#include "mapch.h"
#include "Scene.h"

#include "Components.h"
#include "Mana/Render/Renderer2D.h"
#include "Entity.h"

#include <glm/glm.hpp>

namespace Mana {

    Scene::Scene()
    {
#if 0
        struct TransformComponent
        {
            

        };

        entt::entity entity = m_regisrty.create();

        m_regisrty.emplace<TransformComponent>(entity, glm::mat4(1.0f));

        if(m_regisrty.has<TransformComponent>(entity))
            TransformComponent& tranform = m_regisrty.get<TransformComponent>(entity);

        auto view = m_regisrty.view<TransformComponent>();
        for (auto entity : view)
        {
            TransformComponent& transform = m_regisrty.get<TransformComponent>(entity);
        }
#endif
    }

    Scene::~Scene()
    {

    }

    Entity Scene::createEntity(const std::string& name)
    {
        Entity entity = { m_registry.create(), this };
        entity.addComponent<TransformComponent>();
        auto& tag = entity.addComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        
        return entity;
    }

    void Scene::onUpdate(TimeStep dt)
    {
        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        {
            auto view = m_registry.view<TransformComponent, CameraComponent>();
            for (auto entity : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

                if (camera.Primary)
                {
                    mainCamera = &camera.Camera;
                    cameraTransform = transform.getTransform();
                    break;
                }

            }
        }

        if (mainCamera)
        {
            Renderer2D::beginScene(*mainCamera, cameraTransform);

            auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity : group)
            {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::drawQuad(transform.getTransform(), sprite.Color);
            }

            Renderer2D::endScene();
        }

        
    }

    void Scene::onViewportResize(uint32_t width, uint32_t height)
    {
        m_viewportWidth = width;
        m_viewportWidth = height;

        //resize non-fixed aspect ratio cameras
        auto view = m_registry.view<CameraComponent>();
        for (auto entity : view)
        {
           auto& cameraComponent = view.get<CameraComponent>(entity);
           if (!cameraComponent.fixedApesctRatio)
           {
               cameraComponent.Camera.setViewportSize(width, height);
           }
        }

    }

}