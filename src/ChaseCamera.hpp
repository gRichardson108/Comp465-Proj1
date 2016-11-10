#ifndef CHASECAMERA_HPP
#define CHASECAMERA_HPP

#include <DynamicCamera.hpp>


class ChaseCamera : public DynamicCamera
{
    private:

    public:
        ChaseCamera(char* name, MoveableEntity* parent, float headingOffset = 0.0f,
            const glm::vec3& eyeOffset = glm::vec3(0.0f), const glm::vec3& atOffset = glm::vec3(0.0f));

        /** Default destructor */
        ~ChaseCamera();

        void Update();
};

#endif // CHASECAMERA_HPP
