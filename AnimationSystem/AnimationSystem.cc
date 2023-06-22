#include "AnimationSystem.h"
#include "CubicBezierEasing.h"
#include "constants.h"

namespace AnimationSystem {
  std::vector<Avatar *> avatars;
  std::vector<AnimationMixer *> _animationMixers;
  std::vector<AnimationMapping> _animationMappings;
  std::map<std::string, Animation *> animationAll;

  std::vector<std::vector<Animation *>> animationGroups;
  std::unordered_map<std::string, std::unordered_map<std::string, AnimationDeclaration>> animationGroupsMap;

  std::vector<Animation*> localAnimationGroups(4); 

  float AnimationMixer::nowS;

  unsigned int defaultEmoteAnimationIndex;
  unsigned int defaultSpeakAnimationIndex;
  unsigned int defaultThinkAnimationIndex;
  unsigned int defaultSearchAnimationIndex;

  float localVectorArr[3];
  float localQuaternionArr[4];
  float localVecQuatArr[4];
  float localWeightArr[4];
  float localStartTimeSArr[4];
  float localSpeedArr[4] = {1, 1, 1, 1};
  
  float *localVecQuatPtr;
  float *localVecQuatPtr2;
  
  float directionsWeights[4];
  float directionsWeightsWithReverse[6];

  float identityQuaternion[4] = {0, 0, 0, 1};

  bool isInitedAnimationSystem = false;

  // functions:

  // Utils ------

  float min(float a, float b) {
    if (a > b) {
      return b;
    } else {
      return a;
    }
  }
  float max(float a, float b) {
    if (a > b) {
      return a;
    } else {
      return b;
    }
  }
  float clamp(float value, float minValue, float maxValue) {
    return max( minValue, min( maxValue, value ) );
  }
  void _clearXZ(float *dst, bool isPosition) {
    if (isPosition) {
      dst[0] = 0;
      dst[2] = 0;
    }
  }
  void copyValue(float *dst, float *src, bool isPosition) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    if (!isPosition) dst[3] = src[3];
  }
  void lerpFlat(float *dst, unsigned int dstOffset, float *src0, unsigned int srcOffset0, float *src1, unsigned int srcOffset1, float t) {
    float x0 = src0[srcOffset0 + 0];
    float y0 = src0[srcOffset0 + 1];
    float z0 = src0[srcOffset0 + 2];

    float x1 = src1[srcOffset1 + 0];
    float y1 = src1[srcOffset1 + 1];
    float z1 = src1[srcOffset1 + 2];

    dst[dstOffset + 0] = x0 + (x1 - x0) * t;
    dst[dstOffset + 1] = y0 + (y1 - y0) * t;
    dst[dstOffset + 2] = z0 + (z1 - z0) * t;
  };
  void slerpFlat(float *dst, unsigned int dstOffset, float *src0, unsigned int srcOffset0, float *src1, unsigned int srcOffset1, float t) {

    // fuzz-free, array-based Quaternion SLERP operation

    float x0 = src0[srcOffset0 + 0],
          y0 = src0[srcOffset0 + 1],
          z0 = src0[srcOffset0 + 2],
          w0 = src0[srcOffset0 + 3];

    float x1 = src1[srcOffset1 + 0],
          y1 = src1[srcOffset1 + 1],
          z1 = src1[srcOffset1 + 2],
          w1 = src1[srcOffset1 + 3];

    if (t == 0) {

      dst[dstOffset + 0] = x0;
      dst[dstOffset + 1] = y0;
      dst[dstOffset + 2] = z0;
      dst[dstOffset + 3] = w0;
      return;
    }

    if (t == 1) {

      dst[dstOffset + 0] = x1;
      dst[dstOffset + 1] = y1;
      dst[dstOffset + 2] = z1;
      dst[dstOffset + 3] = w1;
      return;
    }

    if (w0 != w1 || x0 != x1 || y0 != y1 || z0 != z1) {

      float s = 1 - t;
      float cos = x0 * x1 + y0 * y1 + z0 * z1 + w0 * w1,
            dir = (cos >= 0 ? 1 : -1),
            sqrSin = 1 - cos * cos;

      // Skip the Slerp for tiny steps to avoid numeric problems:
      float EPSILON = 2.220446049250313e-16;
      if (sqrSin > EPSILON) {

        float sinVal = sqrt(sqrSin),
              len = atan2(sinVal, cos * dir);

        s = sin(s * len) / sinVal;
        t = sin(t * len) / sinVal;
      }

      float tDir = t * dir;

      x0 = x0 * s + x1 * tDir;
      y0 = y0 * s + y1 * tDir;
      z0 = z0 * s + z1 * tDir;
      w0 = w0 * s + w1 * tDir;

      // Normalize in case we just did a lerp:
      if (s == 1 - t) {

        float f = 1 / sqrt(x0 * x0 + y0 * y0 + z0 * z0 + w0 * w0);

        x0 *= f;
        y0 *= f;
        z0 *= f;
        w0 *= f;
      }
    }

    dst[dstOffset] = x0;
    dst[dstOffset + 1] = y0;
    dst[dstOffset + 2] = z0;
    dst[dstOffset + 3] = w0;
  }
  void interpolateFlat(float *dst, unsigned int dstOffset, float *src0, unsigned int srcOffset0, float *src1, unsigned int srcOffset1, float t, bool isPosition) {
    if (isPosition) {
      lerpFlat(dst, dstOffset, src0, srcOffset0, src1, srcOffset1, t);
    } else {
      slerpFlat(dst, dstOffset, src0, srcOffset0, src1, srcOffset1, t);
    }
  }
  void multiplyQuaternionsFlat(float *dst, unsigned int dstOffset, float *src0, unsigned int srcOffset0, float *src1, unsigned int srcOffset1) {
    float x0 = src0[ srcOffset0 ];
    float y0 = src0[ srcOffset0 + 1 ];
    float z0 = src0[ srcOffset0 + 2 ];
    float w0 = src0[ srcOffset0 + 3 ];

    float x1 = src1[ srcOffset1 ];
    float y1 = src1[ srcOffset1 + 1 ];
    float z1 = src1[ srcOffset1 + 2 ];
    float w1 = src1[ srcOffset1 + 3 ];

    dst[ dstOffset ] = x0 * w1 + w0 * x1 + y0 * z1 - z0 * y1;
    dst[ dstOffset + 1 ] = y0 * w1 + w0 * y1 + z0 * x1 - x0 * z1;
    dst[ dstOffset + 2 ] = z0 * w1 + w0 * z1 + x0 * y1 - y0 * x1;
    dst[ dstOffset + 3 ] = w0 * w1 - x0 * x1 - y0 * y1 - z0 * z1;
  }
  void invertQuaternionFlat(float *dst, unsigned int dstOffset) {
    dst[ dstOffset ] *= -1;
    dst[ dstOffset + 1 ] *= -1;
    dst[ dstOffset + 2 ] *= -1;
  }
  void addVectorsFlat(float *dst, float *src0, float *src1) {
    dst[0] = src0[0] + src1[0];
    dst[1] = src0[1] + src1[1];
    dst[2] = src0[2] + src1[2];
  }
  void subVectorsFlat(float *dst, float *src0, float *src1) {
    dst[0] = src0[0] - src1[0];
    dst[1] = src0[1] - src1[1];
    dst[2] = src0[2] - src1[2];
  }

  // Main ------

  Avatar *createAnimationAvatar(AnimationMixer *mixer) {
    Avatar *avatar = new Avatar();
    avatars.push_back(avatar);
    avatar->mixer = mixer;
    mixer->avatar = avatar;

    // ActionInterpolants
    avatar->actionInterpolants["emote"] = new InfiniteActionInterpolant(0);
    avatar->actionInterpolants["randomIdleTransition"] = new BiActionInterpolant(0, 200);
    avatar->actionInterpolants["speakTransition"] = new BiActionInterpolant(0, 200);
    avatar->actionInterpolants["thinkTransition"] = new BiActionInterpolant(0, 200);
    avatar->actionInterpolants["searchTransition"] = new BiActionInterpolant(0, 200);
    avatar->actionInterpolants["idleFactorTransition"] = new BiActionInterpolant(0, 200);
    return avatar;
  }
  unsigned int initAnimationSystem(char *scratchStack) { // only need init once globally
    std::string jsonStr;

    if (!isInitedAnimationSystem) {
      // -------------------------------------------------------------------------

      for (unsigned int i = 0; i < _animationMappings.size(); i++) {
        if (_animationMappings[i].isPosition) {
          boneIndexes.HipsPosition = i;
        } else {
          std::string boneName = _animationMappings[i].boneName;
          if (boneName == "Hips") boneIndexes.Hips = i;
          else if (boneName == "Spine") boneIndexes.Spine = i;
          else if (boneName == "Chest") boneIndexes.Chest = i;
          else if (boneName == "UpperChest") boneIndexes.UpperChest = i;
          else if (boneName == "Neck") boneIndexes.Neck = i;
          else if (boneName == "Head") boneIndexes.Head = i;
          else if (boneName == "Left_shoulder") boneIndexes.Left_shoulder = i;
          else if (boneName == "Left_arm") boneIndexes.Left_arm = i;
          else if (boneName == "Left_elbow") boneIndexes.Left_elbow = i;
          else if (boneName == "Left_wrist") boneIndexes.Left_wrist = i;
          else if (boneName == "Left_middleFinger1") boneIndexes.Left_middleFinger1 = i;
          else if (boneName == "Left_middleFinger2") boneIndexes.Left_middleFinger2 = i;
          else if (boneName == "Left_middleFinger3") boneIndexes.Left_middleFinger3 = i;
          else if (boneName == "Left_thumb0") boneIndexes.Left_thumb0 = i;
          else if (boneName == "Left_thumb1") boneIndexes.Left_thumb1 = i;
          else if (boneName == "Left_thumb2") boneIndexes.Left_thumb2 = i;
          else if (boneName == "Left_indexFinger1") boneIndexes.Left_indexFinger1 = i;
          else if (boneName == "Left_indexFinger2") boneIndexes.Left_indexFinger2 = i;
          else if (boneName == "Left_indexFinger3") boneIndexes.Left_indexFinger3 = i;
          else if (boneName == "Left_ringFinger1") boneIndexes.Left_ringFinger1 = i;
          else if (boneName == "Left_ringFinger2") boneIndexes.Left_ringFinger2 = i;
          else if (boneName == "Left_ringFinger3") boneIndexes.Left_ringFinger3 = i;
          else if (boneName == "Left_littleFinger1") boneIndexes.Left_littleFinger1 = i;
          else if (boneName == "Left_littleFinger2") boneIndexes.Left_littleFinger2 = i;
          else if (boneName == "Left_littleFinger3") boneIndexes.Left_littleFinger3 = i;
          else if (boneName == "Right_shoulder") boneIndexes.Right_shoulder = i;
          else if (boneName == "Right_arm") boneIndexes.Right_arm = i;
          else if (boneName == "Right_elbow") boneIndexes.Right_elbow = i;
          else if (boneName == "Right_wrist") boneIndexes.Right_wrist = i;
          else if (boneName == "Right_middleFinger1") boneIndexes.Right_middleFinger1 = i;
          else if (boneName == "Right_middleFinger2") boneIndexes.Right_middleFinger2 = i;
          else if (boneName == "Right_middleFinger3") boneIndexes.Right_middleFinger3 = i;
          else if (boneName == "Right_thumb0") boneIndexes.Right_thumb0 = i;
          else if (boneName == "Right_thumb1") boneIndexes.Right_thumb1 = i;
          else if (boneName == "Right_thumb2") boneIndexes.Right_thumb2 = i;
          else if (boneName == "Right_indexFinger1") boneIndexes.Right_indexFinger1 = i;
          else if (boneName == "Right_indexFinger2") boneIndexes.Right_indexFinger2 = i;
          else if (boneName == "Right_indexFinger3") boneIndexes.Right_indexFinger3 = i;
          else if (boneName == "Right_ringFinger1") boneIndexes.Right_ringFinger1 = i;
          else if (boneName == "Right_ringFinger2") boneIndexes.Right_ringFinger2 = i;
          else if (boneName == "Right_ringFinger3") boneIndexes.Right_ringFinger3 = i;
          else if (boneName == "Right_littleFinger1") boneIndexes.Right_littleFinger1 = i;
          else if (boneName == "Right_littleFinger2") boneIndexes.Right_littleFinger2 = i;
          else if (boneName == "Right_littleFinger3") boneIndexes.Right_littleFinger3 = i;
          else if (boneName == "Right_leg") boneIndexes.Right_leg = i;
          else if (boneName == "Right_knee") boneIndexes.Right_knee = i;
          else if (boneName == "Right_ankle") boneIndexes.Right_ankle = i;
          else if (boneName == "Right_toe") boneIndexes.Right_toe = i;
          else if (boneName == "Left_leg") boneIndexes.Left_leg = i;
          else if (boneName == "Left_knee") boneIndexes.Left_knee = i;
          else if (boneName == "Left_ankle") boneIndexes.Left_ankle = i;
          else if (boneName == "Left_toe") boneIndexes.Left_toe = i;
        }
      }

      // -------------------------------------------------------------------------

      json jAnimationGroups;

      for (unsigned int i = 0; i < declarations.size(); i++) {
        AnimationGroupDeclaration declaration = declarations[i];
        std::vector<Animation *> animationGroup;

        json jAnimationGroup;
        jAnimationGroup["name"] = declaration.groupName;
        jAnimationGroup["index"] = declaration.index;

        json jAnimations;
        for (unsigned int j = 0; j < declaration.animationDeclarations.size(); j++) {
          AnimationDeclaration animationDeclaration = declaration.animationDeclarations[j];
          animationGroup.push_back(animationAll[animationDeclaration.fileName]);
          animationGroupsMap[declaration.groupName][animationDeclaration.keyName] = animationDeclaration;

          json jAnimation;
          jAnimation["keyName"] = animationDeclaration.keyName;
          jAnimation["index"] = animationDeclaration.index;
          jAnimation["fileName"] = animationDeclaration.fileName;

          jAnimations.push_back(jAnimation);
        }
        jAnimationGroup["animations"] = jAnimations;

        jAnimationGroups.push_back(jAnimationGroup);
        animationGroups.push_back(animationGroup);
      }

      jsonStr = jAnimationGroups.dump();
      // -------------------------------------------------------------------------
      defaultEmoteAnimationIndex = emoteAnimationIndexes.Angry;
      defaultSpeakAnimationIndex = speakAnimationIndexes.Speak1;
      defaultThinkAnimationIndex = thinkAnimationIndexes.Think1;
      defaultSearchAnimationIndex = searchAnimationIndexes.Search1;
      // -------------------------------------------------------------------------

      CubicBezierEasing::init(0, 1, 0, 1);

      //
      isInitedAnimationSystem = true;
    }
    
    unsigned int jsonStrByteLength = jsonStr.length();
    for (unsigned int i = 0; i < jsonStrByteLength; i++)
    {
      scratchStack[i] = jsonStr.at(i);
    }
    return jsonStrByteLength;
  }
  void Avatar::updateInterpolation(float timeDiff) {
    this->actionInterpolants["emote"]->update(timeDiff, this->emoteState);
    this->actionInterpolants["randomIdleTransition"]->update(timeDiff, this->randomIdleState);
    this->actionInterpolants["speakTransition"]->update(timeDiff, this->speakState);
    this->actionInterpolants["thinkTransition"]->update(timeDiff, this->thinkState);
    this->actionInterpolants["searchTransition"]->update(timeDiff, this->searchState);
    this->actionInterpolants["idleFactorTransition"]->update(timeDiff, this->randomIdleState || this->speakState || this->thinkState || this->searchState);
  }
  void Avatar::update(float *scratchStack) {
    unsigned int index = 0;

    // // values ---
    this->emoteTime = this->actionInterpolants["emote"]->get();

    this->randomIdleTransitionFactor = this->actionInterpolants["randomIdleTransition"]->getNormalized();

    this->speakTransitionFactor = this->actionInterpolants["speakTransition"]->getNormalized();

    this->thinkTransitionFactor = this->actionInterpolants["thinkTransition"]->getNormalized();

    this->searchTransitionFactor = this->actionInterpolants["searchTransition"]->getNormalized();
    
    this->idleFactorTransitionFactor = this->actionInterpolants["idleFactorTransition"]->getNormalized();

    // --- end: Update & Get value of ActionInterpolants

    if (this->actions["emote"] == nullptr) {
      this->emoteAnimationIndex = -1;
    } else {
      this->emoteAnimationIndex = animationGroupsMap["emote"][this->actions["emote"]["animation"]].index;
    }
  }
  void Avatar::addAction(char *scratchStack, unsigned int stringByteLength) {

    std::string jsonStr = "";
    for (unsigned int i = 0; i < stringByteLength; i++) {
      jsonStr += scratchStack[i];
    }

    json j = json::parse(jsonStr);
    this->actions[j["type"]] = j;

    // note: for performance reason, only check string when add/remove action.
    if (j["type"] == "emote") {
      this->emoteState = true;
      this->emoteStartTimeS = j["startTimeS"];
    } else if (j["type"] == "randomIdle") {
      this->randomIdleState = true;
      this->randomIdleStartTimeS = j["startTimeS"];
      this->randomIdleAnimationIndex = animationGroupsMap["randomIdle"][this->actions["randomIdle"]["animation"]].index;
    } else if (j["type"] == "speak") {
      this->speakState = true;
      this->speakStartTimeS = j["startTimeS"];
      this->speakAnimationIndex = animationGroupsMap["speak"][this->actions["speak"]["animation"]].index;
    } else if (j["type"] == "think") {
      this->thinkState = true;
      this->thinkStartTimeS = j["startTimeS"];
      this->thinkSpeed= j["speed"];
      this->thinkAnimationIndex = animationGroupsMap["think"][this->actions["think"]["animation"]].index;
    } else if (j["type"] == "search") {
      this->searchState = true;
      this->searchStartTimeS = j["startTimeS"];
      this->searchAnimationIndex = animationGroupsMap["search"][this->actions["search"]["animation"]].index;
    } 
  }
  void Avatar::removeAction(char *scratchStack, unsigned int stringByteLength) {
    std::string jsonStr = "";
    for (unsigned int i = 0; i < stringByteLength; i++) {
      jsonStr += scratchStack[i];
    }

    json j = json::parse(jsonStr);

    this->actions.erase(j["type"]);

    if (j["type"] == "emote") {
      this->emoteState = false;
    } else if (j["type"] == "randomIdle") {
      this->randomIdleState = false;
    } else if (j["type"] == "speak") {
      this->speakState = false;
    } else if (j["type"] == "think") {
      this->thinkState = false;
    } else if (j["type"] == "search") {
      this->searchState = false;
    } 
  }
  float Avatar::getActionInterpolant(char *scratchStack, unsigned int stringByteLength, unsigned int type) { // 0: get(), 1: getNormalized(), 2: getInverse()
    std::string actionName = "";
    for (unsigned int i = 0; i < stringByteLength; i++) {
      actionName += scratchStack[i];
    }

    if (type == 0) {
      float interpolantValue = this->actionInterpolants[actionName]->get();
      return interpolantValue;
    } else if (type == 1) {
      float interpolantValue = this->actionInterpolants[actionName]->getNormalized();
      return interpolantValue;
    } else if (type == 2) {
      float interpolantValue = this->actionInterpolants[actionName]->getInverse();
      return interpolantValue;
    }
    return -1;
  }
  AnimationMixer *createAnimationMixer() {
    AnimationMixer *animationMixer = new AnimationMixer();
    _animationMixers.push_back(animationMixer);
    animationMixer->animationValues = new float[_animationMappings.size() * 4];
    return animationMixer;
  }
  void createAnimationMapping(bool isPosition, unsigned int index, bool isTop, bool isArm, char *scratchStack, unsigned int nameByteLength) {
    AnimationMapping animationMapping;

    animationMapping.isPosition = isPosition;
    animationMapping.index = index;
    animationMapping.isTop = isTop;
    animationMapping.isArm = isArm;

    std::string boneName = "";
    for (unsigned int i = 0; i < nameByteLength; i++) {
      boneName += scratchStack[i];
    }
    animationMapping.boneName = boneName;

    _animationMappings.push_back(animationMapping);
  }
  Animation *createAnimation(char *scratchStack, unsigned int nameByteLength, float duration) {
    Animation *animation = new Animation();

    animation->duration = duration;

    std::string name = "";
    for (unsigned int i = 0; i < nameByteLength; i++) {
      name += scratchStack[i];
    }
    animation->name = name;

    animationAll[name] = animation;

    return animation;
  }
  void createAnimationInterpolant(Animation *animation, unsigned int numParameterPositions, float *parameterPositions, unsigned int numSampleValues, float *sampleValues, unsigned int valueSize) {
    Interpolant *interpolant = new Interpolant;
    interpolant->numParameterPositions = numParameterPositions;
    interpolant->parameterPositions = new float[numParameterPositions];
    for (unsigned int i = 0; i < numParameterPositions; i++) {
      interpolant->parameterPositions[i] = parameterPositions[i];
    }
    interpolant->numSampleValues = numSampleValues;
    interpolant->sampleValues = new float[numSampleValues];
    for (unsigned int i = 0; i < numSampleValues; i++) {
      interpolant->sampleValues[i] = sampleValues[i];
    }
    interpolant->valueSize = valueSize; // only support 3 (vector) or 4 (quaternion)

    animation->interpolants.push_back(interpolant);
  }
  float *evaluateInterpolant(Animation *animation, unsigned int interpolantIndex, float t) {
    Interpolant *interpolant = animation->interpolants[interpolantIndex];

    if (interpolant->numParameterPositions == 1) {
      interpolant->resultBuffer[0] = interpolant->sampleValues[0];
      interpolant->resultBuffer[1] = interpolant->sampleValues[1];
      interpolant->resultBuffer[2] = interpolant->sampleValues[2];
      if (interpolant->valueSize == 4) {
        interpolant->resultBuffer[3] = interpolant->sampleValues[3];
      }
    } else {
      int index = 0;
      for (; index < interpolant->numParameterPositions; index++) {
        if (interpolant->parameterPositions[index] > t) {
          break;
        }
      }

      if (index == 0) { // note: Handle situation that, parameterPositions[0] > 0, and t == 0 or t < parameterPositions[0].
        interpolant->resultBuffer[0] = interpolant->sampleValues[0];
        interpolant->resultBuffer[1] = interpolant->sampleValues[1];
        interpolant->resultBuffer[2] = interpolant->sampleValues[2];
        if (interpolant->valueSize == 4) {
          interpolant->resultBuffer[3] = interpolant->sampleValues[3];
        }
      } else if (index > interpolant->numParameterPositions - 1) { // note: Handle situation that, t > max parameterPosition.
        unsigned int maxIndex = interpolant->numParameterPositions - 1;
        interpolant->resultBuffer[0] = interpolant->sampleValues[maxIndex * interpolant->valueSize + 0];
        interpolant->resultBuffer[1] = interpolant->sampleValues[maxIndex * interpolant->valueSize + 1];
        interpolant->resultBuffer[2] = interpolant->sampleValues[maxIndex * interpolant->valueSize + 2];
        if (interpolant->valueSize == 4) {
          interpolant->resultBuffer[3] = interpolant->sampleValues[maxIndex * interpolant->valueSize + 3];
        }
      } else {
        unsigned int index0 = index - 1;
        unsigned int index1 = index;

        float time0 = interpolant->parameterPositions[index0];
        float time1 = interpolant->parameterPositions[index1];
        float f = (t - time0) / (time1 - time0);

        interpolateFlat(
          interpolant->resultBuffer, 0,
          interpolant->sampleValues, index0 * interpolant->valueSize,
          interpolant->sampleValues, index1 * interpolant->valueSize,
          f,
          interpolant->valueSize == 3
        );
      }
    }

    return interpolant->resultBuffer;
  }

  float *doBlendList(AnimationMapping &spec, std::vector<Animation *> &animations, float *weights, float &timeS) { // note: Big performance influnce!!! Use `&` to prevent copy parameter's values!!!
    float *resultVecQuat;
    unsigned int indexWeightBigThanZero = 0;
    float currentWeight = 0;
    for (int i = 0; i < animations.size(); i++) {
      float weight = weights[i];
      if (weight > 0) {
        Animation *animation = animations[i];
        float *vecQuat = evaluateInterpolant(animation, spec.index, fmod(timeS, animation->duration));
        if (indexWeightBigThanZero == 0) {
          resultVecQuat = vecQuat;

          indexWeightBigThanZero++;
          currentWeight = weight;
        } else {
          float t = weight / (currentWeight + weight);
          interpolateFlat(resultVecQuat, 0, resultVecQuat, 0, vecQuat, 0, t, spec.isPosition);

          indexWeightBigThanZero++;
          currentWeight += weight;
        }
      }
    }
    return resultVecQuat;
  }

  float *doBlendList(AnimationMapping &spec, std::vector<Animation *> &animations, float *weights, float *startTimeS, float *speed) { // note: Big performance influnce!!! Use `&` to prevent copy parameter's values!!!
    float *resultVecQuat;
    unsigned int indexWeightBigThanZero = 0;
    float currentWeight = 0;
    for (int i = 0; i < animations.size(); i++) {
      float weight = weights[i];
      if (weight > 0) {
        Animation *animation = animations[i];
        float animationTime = AnimationMixer::nowS - startTimeS[i];
        float t2 = min(animationTime, animations[i]->duration / speed[i]);
        float *vecQuat = evaluateInterpolant(animation, spec.index, t2 * speed[i]);
        if (indexWeightBigThanZero == 0) {
          resultVecQuat = vecQuat;

          indexWeightBigThanZero++;
          currentWeight = weight;
        } else {
          float t = weight / (currentWeight + weight);
          interpolateFlat(resultVecQuat, 0, resultVecQuat, 0, vecQuat, 0, t, spec.isPosition);

          indexWeightBigThanZero++;
          currentWeight += weight;
        }
      }
    }
    return resultVecQuat;
  }

  void _handleDefault(AnimationMapping &spec, Avatar *avatar) {
    // blend idle ---
    localVecQuatPtr = evaluateInterpolant(animationGroups[animationGroupIndexes.Single][singleAnimationIndexes.Idle], spec.index, fmod(AnimationMixer::nowS, animationGroups[animationGroupIndexes.Single][singleAnimationIndexes.Idle]->duration));
    copyValue(spec.dst, localVecQuatPtr, spec.isPosition);
    
    if (spec.isTop) { 
      Animation *randomIdleAnimation = animationGroups[animationGroupIndexes.RandomIdle][avatar->randomIdleAnimationIndex];
      Animation *speakAnimation = animationGroups[animationGroupIndexes.Speak][avatar->speakAnimationIndex < 0 ? defaultSpeakAnimationIndex : avatar->speakAnimationIndex];
      Animation *thinkAnimation = animationGroups[animationGroupIndexes.Think][avatar->thinkAnimationIndex < 0 ? defaultThinkAnimationIndex : avatar->thinkAnimationIndex];
      Animation *searchAnimation = animationGroups[animationGroupIndexes.Search][avatar->searchAnimationIndex < 0 ? defaultSearchAnimationIndex : avatar->searchAnimationIndex];
      
      localAnimationGroups[0] = randomIdleAnimation;
      localWeightArr[0] = avatar->randomIdleTransitionFactor;
      localStartTimeSArr[0] = avatar->randomIdleStartTimeS;
      localAnimationGroups[1] = speakAnimation;
      localWeightArr[1] = avatar->speakTransitionFactor;
      localStartTimeSArr[1] = avatar->speakStartTimeS;
      localAnimationGroups[2] = thinkAnimation;
      localWeightArr[2] = avatar->thinkTransitionFactor;
      localStartTimeSArr[2] = avatar->thinkStartTimeS;
      localSpeedArr[2] = avatar->thinkSpeed;
      localAnimationGroups[3] = searchAnimation;
      localWeightArr[3] = avatar->searchTransitionFactor;
      localStartTimeSArr[3] = avatar->searchStartTimeS;

      
      localVecQuatPtr = doBlendList(spec, localAnimationGroups, localWeightArr, localStartTimeSArr, localSpeedArr);
      interpolateFlat(spec.dst, 0, spec.dst, 0, localVecQuatPtr, 0, avatar->idleFactorTransitionFactor, spec.isPosition);
    }
  }

  void _blendEmote(AnimationMapping &spec, Avatar *avatar) {
    _handleDefault(spec, avatar);

    Animation *emoteAnimation = animationGroups[animationGroupIndexes.Emote][avatar->emoteAnimationIndex < 0 ? defaultEmoteAnimationIndex : avatar->emoteAnimationIndex];
    float t2 = min(AnimationMixer::nowS - avatar->emoteStartTimeS, emoteAnimation->duration);
    float *v2 = evaluateInterpolant(emoteAnimation, spec.index, t2);

    float f0 = t2 / 0.2;
    float f1 = (emoteAnimation->duration - t2) / 0.2;
    float f = min(f0, f1);
    f = min(1, f);

    if (spec.index == boneIndexes.Spine || spec.index == boneIndexes.Chest || spec.index == boneIndexes.UpperChest || spec.index == boneIndexes.Neck || spec.index == boneIndexes.Head) {
      if (!spec.isPosition) {
        multiplyQuaternionsFlat(spec.dst, 0, v2, 0, spec.dst, 0);
      } else {
        interpolateFlat(spec.dst, 0, spec.dst, 0, v2, 0, f, spec.isPosition);
      }
    } else {
      interpolateFlat(spec.dst, 0, spec.dst, 0, v2, 0, f, spec.isPosition);
    }

    // _clearXZ(spec.dst, spec.isPosition);
  }

  float *AnimationMixer::update(float now, float nowS) {
    AnimationMixer::nowS = nowS;

    for (int i = 0; i < _animationMappings.size(); i++) {
      AnimationMapping spec = _animationMappings[i];

      if (avatar->emoteState) {
        _blendEmote(spec, this->avatar);
      } 
      else {
        _handleDefault(spec, this->avatar);
      }
      animationValues[i * 4] = spec.dst[0];
      animationValues[i * 4 + 1] = spec.dst[1];
      animationValues[i * 4 + 2] = spec.dst[2];
      animationValues[i * 4 + 3] = spec.dst[3];
    }

    return animationValues;
  }
}