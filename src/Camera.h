#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//! Class that contains the behavior of the camera.
class Camera{
    private:
        //! Position of the camera
        glm::vec3 position;

        //! Front of the camera
        glm::vec3 front;

        //! Up of the camera
        glm::vec3 up;

        //! Field of view of the camera, expressed in degrees.
        float fov;

        //! Speed of the camera
        float speed;
    public:
        //! Parameterless camera constructor
        Camera();

        //! Caculates the right vector. That is the result of the cross product between FRONT and and UP vectors. 
        /*!
        \return Right direction of the camera (that is a normalize vector)
        */
        glm::vec3 right() const;

        //! Yaw (rotate upon the UP vector) the camera at a certain angle from its current orientation
        /*!
        \param degree Angle, expressed in degrees, at which the camera will be yawed.
        */
        void yaw(float degree);

        //! Pitch (rotate upon the RIGHT vector) the camera at a certain angle from its current orientation.
        /*!
        \param degree Angle, expressed in degrees, at which the camera will be pitched.
        */
        void pitch(float degree);

        //! Moves the camera forward. That is camera.position = camera.front*camera.speed
        void moveForward();

        //! Moves the camera backward. That is camera.position = -camera.front*camera.speed
        void moveBackward();

        //! Moves the camera right. That is camera.position = camera.right()*camera.speed
        void moveRight();

        //! Moves the camera left. That is camera.position = -camera.right()*camera.speed
        void moveLeft();

        //! Get the current position of the camera relative to the world origin.
        /*!
        \return Vector containing the position of the camera relative to the origin.
        */
        glm::vec3 getPosition() const;

        //! Get the front direcction of the camera.
        /*!
        \return Normalize vector which indicates the front direction of the camera.
        */
        glm::vec3 getFront() const;

        //! Get the up direction of the camera.
        /*!
        \return Normalize vector which indicates the up direction of the camera
        */
        glm::vec3 getUp() const;

        //! Get the current field of view (FOV) of the camera
        /*!
        \return FOV
        */
        float getFov() const;

        void reset();
        ~Camera();
};