#include "mapch.h"
#include "OrthographicCameraController.h"

#include "Mana/Core/Input.h"
#include "Mana/Core/KeyCodes.h"

namespace Mana {

    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
        : m_aspectRatio(aspectRatio), m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel), m_rotation(rotation)
    {

    }

    void OrthographicCameraController::onUpdate(TimeStep dt)
    {
        float moveSpeed = m_zoomLevel;

        if (Input::isKeyPressed(MA_KEY_A))
        {
            m_cameraPosition.x -= cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * dt;
            m_cameraPosition.y -= sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * dt;
        }
        else if (Input::isKeyPressed(MA_KEY_D))
        {
            m_cameraPosition.x += cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * dt;
            m_cameraPosition.y += sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * dt;
        }

        if (Input::isKeyPressed(MA_KEY_W))
        {
            m_cameraPosition.x += -sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * dt;
            m_cameraPosition.y += cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * dt;
        }
        else if (Input::isKeyPressed(MA_KEY_S))
        {
            m_cameraPosition.x -= -sin(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * dt;
            m_cameraPosition.y -= cos(glm::radians(m_cameraRotation)) * m_cameraTranslationSpeed * dt;
        }

        if(m_rotation)
        {
            if (Input::isKeyPressed(MA_KEY_Q))
                m_cameraRotation += m_cameraRotationSpeed * dt;
            if (Input::isKeyPressed(MA_KEY_E))
                m_cameraRotation -= m_cameraRotationSpeed * dt;

            if (m_cameraRotation > 180.0f)
                m_cameraRotation -= 360.0f;
            else if (m_cameraRotation <= -180.0f)
                m_cameraRotation += 360.0f;

            m_camera.setRotation(m_cameraRotation);
        }

        m_camera.setPosition(m_cameraPosition);

        //m_cameraTranslationSpeed = m_zoomLevel;
    }

    void OrthographicCameraController::onEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<MouseScrolledEvent>(MA_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(MA_BIND_EVENT_FN(OrthographicCameraController::onWindowResized));
    }

    bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& event)
    {
        m_zoomLevel -= event.getYOffset() * 0.25f;
        
        m_zoomLevel = std::max(m_zoomLevel, 0.25f);

        /*MA_CORE_INFO("Zoom Level: {0}", m_zoomLevel);
        MA_CORE_INFO("Translation Speed: {0}", m_cameraTranslationSpeed);
        MA_CORE_INFO("Move Speed: {0}", m_cameraTranslationSpeed * m_zoomLevel);*/

        m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
        return false;
    }

    bool OrthographicCameraController::onWindowResized(WindowResizeEvent& event)
    {
        m_aspectRatio = (float)event.getWidth() / (float)event.getHeight();
        m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
        return false;
    }

}