#include "mapch.h"
#include "Scene.h"

#include "Components.h"
#include "Mana/Render/Renderer2D.h"
#include "Entity.h"

#include <glm/glm.hpp>

namespace Mana {

    Scene::Scene()
    {
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

    void Scene::destroyEntity(Entity entity)
    {
        m_registry.destroy(entity);
    }

    void Scene::onUpdate(TimeStep dt)
    {
        //update scripts
        {
            m_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
            {
                if (!nsc.Instance)
                {
                    nsc.Instance = nsc.InstantiateScript();
                    nsc.Instance->m_entity = Entity{ entity, this };
                    nsc.Instance->onCreate();
                }

                    nsc.Instance->onUpdate(dt);
            });
        }

        //render
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
        m_viewportHeight = height;

        //resize non-fixed aspect ratio cameras
        auto view = m_registry.view<CameraComponent>();
        for (auto entity : view)
        {
           auto& cameraComponent = view.get<CameraComponent>(entity);
           if (!cameraComponent.FixedAspectRatio)
           {
               cameraComponent.Camera.setViewportSize(width, height);
           }
        }

    }

    template<typename T>
    void Scene::onComponentAdded(Entity entity, T& component)
    {
        static_assert(false);
    }

    template<>
    void Scene::onComponentAdded<TagComponent>(Entity entity, TagComponent& component)
    {

    }

    template<>
    void Scene::onComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {

    }

    template<>
    void Scene::onComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
        component.Camera.setViewportSize(m_viewportWidth, m_viewportHeight);
    }

    template<>
    void Scene::onComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
    {

    }

    template<>
    void Scene::onComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
    {

    }

}