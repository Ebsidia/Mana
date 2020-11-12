#pragma once

#include "Mana/Render/OrthographicCamera.h"
#include "Mana/Core/TimeStep.h"

#include "Mana/Events/ApplicationEvent.h"
#include "Mana/Events/MouseEvent.h"

namespace Mana {

    struct OrthographicCameraBounds
    {
        float Left, Right;
        float Bottom, Top;

        float GetWidth() { return Right - Left; }
        float GetHeight() { return Top - Bottom; }
    };

    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio, bool rotation = false);
        OrthographicCameraController(float aspectRatio, float zoomLevel, bool rotation = false);

        void onUpdate(TimeStep dt);
        void onEvent(Event& event);

        OrthographicCamera& getCamera() { return m_camera; }
        const OrthographicCamera& getCamera() const { return m_camera; }

        float getZoomLevel() const { return m_zoomLevel; }
        void setZoomLevel(float level) { m_zoomLevel = level; calculateView(); }

        void calculateView();
        const OrthographicCameraBounds& getBounds() const { return m_bounds; }
    private:
        

        bool onMouseScrolled(MouseScrolledEvent& event);
        bool onWindowResized(WindowResizeEvent& event);

    private:
        float m_aspectRatio;
        float m_zoomLevel;
        OrthographicCameraBounds m_bounds;
        OrthographicCamera m_camera;

        bool m_rotation;

        glm::vec3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
        float m_cameraRotation = 0.0f;
        float m_cameraTranslationSpeed = 4.0f, m_cameraRotationSpeed = 180.0f;

    };
}