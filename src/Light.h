#include <glm/glm.hpp>

struct LightShader {
    glm::vec3 position;
    glm::vec3 color;
    bool active;
};

class Light {
    private:
        const float *scaleColorChannels; /*< A vector containing the scale (between 0.0f and 1.0f) of each current color change*/
        glm::vec3 color; /*< Vector which diffine RGB color of light */
        glm::vec3 position; /*< Position of the light */
        bool activeColorChannels[3]; /*< Active color channels*/
        bool grayscaleActive; /*< If light is grayscale */
        bool active; /*< If light is active */
    public:
        /*< Enumration which contains the various color channels of the light*/
        const enum CHANNELS {
            R /*!< Value for the Red color channel */, 
            G /*!< Value for the Green color channel */, 
            B /*!< Value for the Blue color channel */
        };
        /*< Default scale of the light */
        const static float DEFAULT_LIGHT_SCALE[3];

        /*< Grayscale scales of the light*/
        const static float GRAYSCALE_LIGHT_SCALE[3];

        //! Constructor which constructs a light with a given color and position.
        /*!
        \param color Vector containing the color of the light
        \param position Vector containing the position of the light relative the world's origin.
        */
        Light(glm::vec3 color, glm::vec3 position);

        //! Obtain the color of the light
        /*!
        \return Vector containing the current RGB value of the light
        */
        glm::vec3 getColor();

        //! Obtain the current position of the light
        /*!
        \return Vector containing the position value of the light
        */
        glm::vec3 getPosition();

        //! Toggle the selected color channel
        /*!
        \param channel Light::CHANNELS to be toggle
        */
        void toggleChannel(Light::CHANNELS channel);

        //! Toggle the selected color channel
        /*!
        \param channel Light::CHANNELS to be toggle
        \param state State at which the Light::CHANNELS should be toggle
        */
        void toggleChannel(Light::CHANNELS channel, bool state);

        //! Toggle the active state of the light
        void toggle();

        //! Returns the active state of the light
        /*!
        \return True if active, false otherwise
        */
        bool isActive();

        //! Toggle the grayscale of the light
        void toggleGrayscale();

        //! Convert the current light object to a struct to be passed to the shader
        /*!
        \return Struct containing the position, color and the active state of the light
        */
        LightShader getGLSLStruct();
};