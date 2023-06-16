#include <emscripten.h>
#include "AnimationSystem/AnimationSystem.h"
#include <deque>
#include <map>

extern "C" {

// memory

EMSCRIPTEN_KEEPALIVE void *doMalloc(size_t size) {
  return malloc(size);
}

EMSCRIPTEN_KEEPALIVE void doFree(void *ptr) {
  free(ptr);
}

//

EMSCRIPTEN_KEEPALIVE void initialize() {
  physicsBase = new PBase();
}

//

EMSCRIPTEN_KEEPALIVE PxD6Joint *addJointPhysics(PScene *scene, unsigned int id1, unsigned int id2, float *position1, float *position2, float *quaternion1, float *quaternion2) {
  return scene->addJoint(id1, id2, position1, position2, quaternion1, quaternion2);
}
EMSCRIPTEN_KEEPALIVE void setJointMotionPhysics(PScene *scene, PxD6Joint *joint, PxD6Axis::Enum axis, PxD6Motion::Enum motion) {
  return scene->setJointMotion(joint, axis, motion);
}
EMSCRIPTEN_KEEPALIVE void setJointTwistLimitPhysics(PScene *scene, PxD6Joint *joint, float lowerLimit, float upperLimit, float contactDist = -1.0f) {
  return scene->setJointTwistLimit(joint, lowerLimit, upperLimit, contactDist);
}
EMSCRIPTEN_KEEPALIVE void setJointSwingLimitPhysics(PScene *scene, PxD6Joint *joint, float yLimitAngle, float zLimitAngle, float contactDist = -1.0f) {
  return scene->setJointSwingLimit(joint, yLimitAngle, zLimitAngle, contactDist);
}
EMSCRIPTEN_KEEPALIVE bool updateMassAndInertiaPhysics(PScene *scene, unsigned int id, float shapeDensities) {
  return scene->updateMassAndInertia(id, shapeDensities);
}
EMSCRIPTEN_KEEPALIVE float getBodyMassPhysics(PScene *scene, unsigned int id) {
  return scene->getBodyMass(id);
}

// AnimationSystem

EMSCRIPTEN_KEEPALIVE AnimationSystem::Avatar *createAnimationAvatar(AnimationSystem::AnimationMixer *mixer) {
  return AnimationSystem::createAnimationAvatar(mixer);
}
EMSCRIPTEN_KEEPALIVE void updateInterpolationAnimationAvatar(AnimationSystem::Avatar *avatar, float timeDiff) {
  return avatar->updateInterpolation(timeDiff);
}
EMSCRIPTEN_KEEPALIVE void updateAnimationAvatar(AnimationSystem::Avatar *avatar, float *scratchStack) {
  return avatar->update(scratchStack);
}
EMSCRIPTEN_KEEPALIVE void addActionAnimationAvatar(AnimationSystem::Avatar *avatar, char *scratchStack, unsigned int stringByteLength) {
  return avatar->addAction(scratchStack, stringByteLength);
}
EMSCRIPTEN_KEEPALIVE void removeActionAnimationAvatar(AnimationSystem::Avatar *avatar, char *scratchStack, unsigned int stringByteLength) {
  return avatar->removeAction(scratchStack, stringByteLength);
}
EMSCRIPTEN_KEEPALIVE float getActionInterpolantAnimationAvatar(AnimationSystem::Avatar *avatar, char *scratchStack, unsigned int stringByteLength, unsigned int type) {
  return avatar->getActionInterpolant(scratchStack, stringByteLength, type);
}
EMSCRIPTEN_KEEPALIVE AnimationSystem::AnimationMixer *createAnimationMixer() {
  return AnimationSystem::createAnimationMixer();
}
EMSCRIPTEN_KEEPALIVE float *updateAnimationMixer(AnimationSystem::AnimationMixer *mixer, float now, float nowS) {
  return mixer->update(now, nowS);
}
EMSCRIPTEN_KEEPALIVE void createAnimationMapping(bool isPosition, unsigned int index, bool isTop, bool isArm, char *scratchStack, unsigned int nameByteLength) {
  return AnimationSystem::createAnimationMapping(isPosition, index, isTop, isArm, scratchStack, nameByteLength);
}
EMSCRIPTEN_KEEPALIVE AnimationSystem::Animation *createAnimation(char *scratchStack, unsigned int nameByteLength, float duration) {
  return AnimationSystem::createAnimation(scratchStack, nameByteLength, duration);
}
EMSCRIPTEN_KEEPALIVE unsigned int initAnimationSystem(char *scratchStack) {
  return AnimationSystem::initAnimationSystem(scratchStack);
}
EMSCRIPTEN_KEEPALIVE void createAnimationInterpolant(AnimationSystem::Animation *animation, unsigned int numParameterPositions, float *parameterPositions, unsigned int numSampleValues, float *sampleValues, unsigned int valueSize) {
  return AnimationSystem::createAnimationInterpolant(animation, numParameterPositions, parameterPositions, numSampleValues, sampleValues, valueSize);
}

// End AnimationSystem

} // extern "C"