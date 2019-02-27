#include "Light.h"


const float Light::DEFAULT_LIGHT_SCALE[] = {1.0f, 1.0f, 1.0f};
const float Light::GRAYSCALE_LIGHT_SCALE[] = {0.2989f, 0.5870f, 0.1140f};

Light::Light(glm::vec3 color, glm::vec3 position) {
    this->activeColorChannels[0] = true;
    this->activeColorChannels[1] = true;
    this->activeColorChannels[2] = true;
    this->grayscaleActive = false;
    this->scaleColorChannels = Light::DEFAULT_LIGHT_SCALE;
    this->color = color;
    this->position = position;
    this->active = true;
}

glm::vec3 Light::getColor() {
    glm::vec3 tempLightColor = this->color;

	// Adjust the active channels and the color scales
	for(int i = 0; i < 3; i++) {
		if(!this->activeColorChannels[i]){
			tempLightColor[i] = 0.0f;
		}
		
		tempLightColor[i] *= scaleColorChannels[i];
	}
    return tempLightColor;
}

void Light::toggle() {
    this->active = !this->active;
}

bool Light::isActive() {
    return this->active;
}

glm::vec3 Light::getPosition() {
    return this->position;
}

void Light::toggleChannel(Light::CHANNELS channel) {
    this->toggleChannel(channel, !this->activeColorChannels[channel]);
}

void Light::toggleChannel(Light::CHANNELS channel, bool state) {
    this->activeColorChannels[channel] = state;
}

void Light::toggleGrayscale() {
    this->scaleColorChannels = (this->scaleColorChannels == Light::GRAYSCALE_LIGHT_SCALE)? Light::DEFAULT_LIGHT_SCALE : Light::GRAYSCALE_LIGHT_SCALE;
}

LightShader Light::getGLSLStruct() {
    LightShader lightStruct;
    lightStruct.active = this->isActive();
    lightStruct.color = this->getColor();
    lightStruct.position = this->getPosition();

    return lightStruct;
}