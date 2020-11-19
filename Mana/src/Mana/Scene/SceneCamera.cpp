#include "mapch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Mana {

    SceneCamera::SceneCamera()
    {
        recalculateProjection();
    }

    void SceneCamera::setOrthographic(float size, float nearClip, float farClip)
    {
        m_projectionType = ProjectionType::Orthographic;
        m_orthographicSize = size;
        m_orthographicNear = nearClip;
        m_orthographicFar = farClip;

        recalculateProjection();
    }

    void SceneCamera::setPerspective(float fov, float nearClip, float farClip)
    {
        m_projectionType = ProjectionType::Perspective;
        m_perspectiveFOV = fov;
        m_perspectiveNear = nearClip;
        m_perspectiveFar = farClip;

        recalculateProjection();
    }

    void SceneCamera::setViewportSize(uint32_t width, uint32_t height)
    {
        m_aspectRatio = (float)width / (float)height;
        recalculateProjection();
    }

    void SceneCamera::recalculateProjection()
    {
        if (m_projectionType == ProjectionType::Orthographic)
        {
            float orthoLeft = -m_orthographicSize * m_aspectRatio * 0.5f;
            float orthoRight = m_orthographicSize * m_aspectRatio * 0.5f;
            float orthoTop = m_orthographicSize * 0.5f;
            float orthoBottom = -m_orthographicSize * 0.5f;

            m_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_orthographicNear, m_orthographicFar);
        }
        else
        {
            m_projection = glm::perspective(m_perspectiveFOV, m_aspectRatio, m_perspectiveNear, m_perspectiveFar);
        }

        
    }

}