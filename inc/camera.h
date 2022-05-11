#pragma once
#include <GL/glew.h>
#include <dependency/glm/glm/glm.hpp>
#include <dependency/glm/glm/gtc/matrix_transform.hpp>
#include <dependency/glm/glm/gtc/quaternion.hpp>
#include <dependency/glm/glm/gtx/quaternion.hpp>
#include <cmath>
#include <dependency/glm/glm/gtx/string_cast.hpp>
class orbitCamera
{
public:
    // camera Attributes
    orbitCamera(float initX, float initY, float initZ, float width, float height) : m_initCamPosition(initX, initY, initZ), m_width(width), m_height(height){};
    glm::mat4 getView()
    {
        // std::cout << "x=" << m_eulerRotAngle.x << ", y=" << m_eulerRotAngle.y << ", z=" << m_eulerRotAngle.z << std::endl;
        m_rotQuat = glm::quat(glm::radians(m_eulerRotAngle));
        glm::vec3 camPos = m_rotQuat * m_initCamPosition * glm::inverse(m_rotQuat);
        std::cout << glm::to_string(camPos) << std::endl;
        glm::mat4 view = glm::lookAt(camPos, m_lookAtOrigin, upVector);
        // std::cout << glm::to_string(view) << std::endl;
        return view;
    };
    glm::mat4 getProjection()
    {
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(m_zoom), m_width / m_height, 0.1f, 200.0f);
        return projection;
    };
    void setZoom(float zoom)
    {
        m_zoom = m_zoom + zoom;
        if (m_zoom < 1.0f)
            m_zoom = 1.0f;
        if (m_zoom > 89.0f)
            m_zoom = 89.0f;
    }
    void setYaw(float yaw)
    {
        m_eulerRotAngle.y = yaw;
    }
    void setPitch(float pitch)
    {
        m_eulerRotAngle.x = pitch;
        // std::cout << "pitch=" << m_eulerRotAngle.x << std::endl;
    };
    float getPitch()
    {
        return m_eulerRotAngle.x;
    };
    float getYaw()
    {
        return m_eulerRotAngle.y;
    };
    ~orbitCamera(){};
    // constructor with vectors

private:
    const glm::vec3 m_lookAtOrigin = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_eulerRotAngle = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_initCamPosition;
    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::quat m_rotQuat;
    float m_width;
    float m_height;
    float m_zoom = 75.0f;
};
