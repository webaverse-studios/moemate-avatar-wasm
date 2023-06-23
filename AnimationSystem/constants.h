const float narutoRunTimeFactor = 2;
const float crouchMaxTime = 200;
const float aimMaxTime = 1000;

class BoneIndexes {
public:
  // positions ---
  int HipsPosition;
  // quaternions ---
  int Hips;
  int Spine;
  int Chest;
  int UpperChest;
  int Neck;
  int Head;
  int Left_shoulder;
  int Left_arm;
  int Left_elbow;
  int Left_wrist;
  int Left_middleFinger1;
  int Left_middleFinger2;
  int Left_middleFinger3;
  int Left_thumb0;
  int Left_thumb1;
  int Left_thumb2;
  int Left_indexFinger1;
  int Left_indexFinger2;
  int Left_indexFinger3;
  int Left_ringFinger1;
  int Left_ringFinger2;
  int Left_ringFinger3;
  int Left_littleFinger1;
  int Left_littleFinger2;
  int Left_littleFinger3;
  int Right_shoulder;
  int Right_arm;
  int Right_elbow;
  int Right_wrist;
  int Right_middleFinger1;
  int Right_middleFinger2;
  int Right_middleFinger3;
  int Right_thumb0;
  int Right_thumb1;
  int Right_thumb2;
  int Right_indexFinger1;
  int Right_indexFinger2;
  int Right_indexFinger3;
  int Right_ringFinger1;
  int Right_ringFinger2;
  int Right_ringFinger3;
  int Right_littleFinger1;
  int Right_littleFinger2;
  int Right_littleFinger3;
  int Right_leg;
  int Right_knee;
  int Right_ankle;
  int Right_toe;
  int Left_leg;
  int Left_knee;
  int Left_ankle;
  int Left_toe;
};
BoneIndexes boneIndexes;

// ---------------------------------------------------------

class AnimationDeclaration {
public:
  std::string keyName;
  int index;
  std::string fileName;
};

class AnimationGroupDeclaration {
public:
  std::string groupName;
  int index;
  std::vector<AnimationDeclaration> animationDeclarations;
};

typedef std::vector<AnimationGroupDeclaration> AnimationGroupDeclarations;

// ------

int animationGroupIota = 0;
class AnimationGroupIndexes {
public:
  int Single;
 
  int Emote;
  int RandomIdle;
  int Speak;
  int Think;
  int Search;
};
AnimationGroupIndexes animationGroupIndexes;

// ---   note: Start from 0, -1 means null/undefined.

int singleAnimationIota = 0;
class SingleAnimationIndexes {
public:
  int Idle;
};
SingleAnimationIndexes singleAnimationIndexes;

int emoteAnimationIota = 0;
class EmoteAnimationIndexes {
public:
  int Alert;
  int AlertSoft;
  int Angry;
  int AngrySoft;
  int Embarrassed;
  int EmbarrassedSoft;
  int HeadNod;
  int HeadNodSoft;
  int HeadShake;
  int HeadShakeSoft;
  int Sad;
  int SadSoft;
  int Surprise;
  int SurpriseSoft;
  int Victory;
  int VictorySoft;

  int Agree;
  int AngryFists;
  int AngryFistsSoft;
  int Apologetic;
  int Confused;
  int Curious;
  int Cry;
  int Disagree;
  int Embarrassed2;
  int Excited;
  int Explain;
  int HeadShake2;
  int KeyboardSoft;
  int Listen;
  int Shocked;
  int Upset;

  int Applaud;
  int Dab;
  int Blessing;
  int Love;
  int Kiss;
  int Anxiety;
  int Stop;
  int Celebrate;

};
EmoteAnimationIndexes emoteAnimationIndexes;

int randomIdleAnimationIota = 0;
class RandomIdleAnimationIndexes {
public:
  int ArmsUp;
  int Confused;
  int BreathSoft;
  int BreathStrong;
  int HeadShift;
  int LookAround;
  int LookAroundUp;
  int NeckShift;
  int Wiggle;
};
RandomIdleAnimationIndexes randomIdleAnimationIndexes;

int speakAnimationIota = 0;
class SpeakAnimationIndexes {
public:
  int Speak1;
  int Speak2;
  int Speak3;
};
SpeakAnimationIndexes speakAnimationIndexes;

int thinkAnimationIota = 0;
class ThinkAnimationIndexes {
public:
  int Think1;
  int Think2;
  int Think3;
};
ThinkAnimationIndexes thinkAnimationIndexes;

int searchAnimationIota = 0;
class SearchAnimationIndexes {
public:
  int Search1;
  int Search2;
  int Search3;
};
SearchAnimationIndexes searchAnimationIndexes;

// ------

AnimationGroupDeclarations declarations = {
  {
    "single",
    animationGroupIndexes.Single = animationGroupIota++,
    {
      {
        "idle",
        singleAnimationIndexes.Idle = singleAnimationIota++,
        "idle.fbx"
      }
    }
  },
  {
    "emote",
    animationGroupIndexes.Emote = animationGroupIota++,
    {
      {
        "alert",
        emoteAnimationIndexes.Alert = emoteAnimationIota++,
        "alert.fbx"
      },
      {
        "alertSoft",
        emoteAnimationIndexes.AlertSoft = emoteAnimationIota++,
        "alert_soft.fbx"
      },
      {
        "angry",
        emoteAnimationIndexes.Angry = emoteAnimationIota++,
        "angry.fbx"
      },
      {
        "angrySoft",
        emoteAnimationIndexes.AngrySoft = emoteAnimationIota++,
        "angry_soft.fbx"
      },
      {
        "embarrassed",
        emoteAnimationIndexes.Embarrassed = emoteAnimationIota++,
        "embarrassed.fbx"
      },
      {
        "embarrassedSoft",
        emoteAnimationIndexes.EmbarrassedSoft = emoteAnimationIota++,
        "embarrassed_soft.fbx"
      },
      {
        "headNod",
        emoteAnimationIndexes.HeadNod = emoteAnimationIota++,
        "head_nod.fbx"
      },
      {
        "headNodSoft",
        emoteAnimationIndexes.HeadNodSoft = emoteAnimationIota++,
        "head_nod_single.fbx"
      },
      {
        "headShake",
        emoteAnimationIndexes.HeadShake = emoteAnimationIota++,
        "head_shake.fbx"
      },
      {
        "headShakeSoft",
        emoteAnimationIndexes.HeadShakeSoft = emoteAnimationIota++,
        "head_shake_single.fbx"
      },
      {
        "sad",
        emoteAnimationIndexes.Sad = emoteAnimationIota++,
        "sad.fbx"
      },
      {
        "sadSoft",
        emoteAnimationIndexes.SadSoft = emoteAnimationIota++,
        "sad_soft.fbx"
      },
      {
        "surprise",
        emoteAnimationIndexes.Surprise = emoteAnimationIota++,
        "surprise.fbx"
      },
      {
        "surpriseSoft",
        emoteAnimationIndexes.SurpriseSoft = emoteAnimationIota++,
        "surprise_soft.fbx"
      },
      {
        "victory",
        emoteAnimationIndexes.Victory = emoteAnimationIota++,
        "victory.fbx"
      },
      {
        "victorySoft",
        emoteAnimationIndexes.VictorySoft = emoteAnimationIota++,
        "victory_soft.fbx"
      },

      {
        "agree",
        emoteAnimationIndexes.Agree = emoteAnimationIota++,
        "agree.fbx"
      },
      {
        "angryFists",
        emoteAnimationIndexes.AngryFists = emoteAnimationIota++,
        "angry_fists.fbx"
      },
      {
        "angryFistsSoft",
        emoteAnimationIndexes.AngryFistsSoft = emoteAnimationIota++,
        "angry_fists_soft.fbx"
      },
      {
        "apologetic",
        emoteAnimationIndexes.Apologetic = emoteAnimationIota++,
        "apologetic.fbx"
      },
      {
        "confused",
        emoteAnimationIndexes.Confused = emoteAnimationIota++,
        "confused.fbx"
      },
      {
        "curious",
        emoteAnimationIndexes.Curious = emoteAnimationIota++,
        "curious.fbx"
      },
      {
        "cry",
        emoteAnimationIndexes.Cry = emoteAnimationIota++,
        "cry.fbx"
      },
      {
        "disagree",
        emoteAnimationIndexes.Disagree = emoteAnimationIota++,
        "disagree.fbx"
      },
      {
        "embarrassed2",
        emoteAnimationIndexes.Embarrassed2 = emoteAnimationIota++,
        "embarrassed_2.fbx"
      },
      {
        "excited",
        emoteAnimationIndexes.Excited = emoteAnimationIota++,
        "excited.fbx"
      },
      {
        "explain",
        emoteAnimationIndexes.Explain = emoteAnimationIota++,
        "explain.fbx"
      },
      {
        "headShake2",
        emoteAnimationIndexes.HeadShake2 = emoteAnimationIota++,
        "head_shake_2.fbx"
      },
      {
        "keyboardSoft",
        emoteAnimationIndexes.KeyboardSoft = emoteAnimationIota++,
        "keyboard_soft.fbx"
      },
      {
        "listen",
        emoteAnimationIndexes.Listen = emoteAnimationIota++,
        "listen.fbx"
      },
      {
        "shocked",
        emoteAnimationIndexes.Shocked = emoteAnimationIota++,
        "shocked.fbx"
      },
      {
        "upset",
        emoteAnimationIndexes.Upset = emoteAnimationIota++,
        "upset.fbx"
      },

      {
        "applaud",
        emoteAnimationIndexes.Applaud = emoteAnimationIota++,
        "applaud.fbx"
      },
      {
        "dab",
        emoteAnimationIndexes.Dab = emoteAnimationIota++,
        "dab.fbx"
      },
      {
        "blessing",
        emoteAnimationIndexes.Blessing = emoteAnimationIota++,
        "blessing.fbx"
      },
      {
        "love",
        emoteAnimationIndexes.Love = emoteAnimationIota++,
        "love.fbx"
      },
      {
        "kiss",
        emoteAnimationIndexes.Kiss = emoteAnimationIota++,
        "kiss.fbx"
      },
      {
        "anxiety",
        emoteAnimationIndexes.Anxiety = emoteAnimationIota++,
        "anxiety.fbx"
      },
      {
        "stop",
        emoteAnimationIndexes.Stop = emoteAnimationIota++,
        "stop.fbx"
      },
      {
        "celebrate",
        emoteAnimationIndexes.Celebrate = emoteAnimationIota++,
        "celebrate.fbx"
      },
    }
  },
  {
    "randomIdle",
    animationGroupIndexes.RandomIdle = animationGroupIota++,
    {
      {
        "armsUp",
        randomIdleAnimationIndexes.ArmsUp = randomIdleAnimationIota++,
        "arms_up.fbx"
      },
      {
        "breathSoft",
        randomIdleAnimationIndexes.BreathSoft = randomIdleAnimationIota++,
        "breath_soft.fbx"
      },
      {
        "breathStrong",
        randomIdleAnimationIndexes.BreathStrong = randomIdleAnimationIota++,
        "breath_strong.fbx"
      },
      {
        "confused",
        randomIdleAnimationIndexes.Confused = randomIdleAnimationIota++,
        "confused_soft.fbx"
      },
      {
        "headShift",
        randomIdleAnimationIndexes.HeadShift = randomIdleAnimationIota++,
        "head_shift.fbx"
      },
      {
        "lookAround",
        randomIdleAnimationIndexes.LookAround = randomIdleAnimationIota++,
        "look_around.fbx"
      },
      {
        "lookAroundUp",
        randomIdleAnimationIndexes.LookAroundUp = randomIdleAnimationIota++,
        "look_around_up.fbx"
      },
      {
        "neckShift",
        randomIdleAnimationIndexes.NeckShift = randomIdleAnimationIota++,
        "neck_shift.fbx"
      },
      {
        "wiggle",
        randomIdleAnimationIndexes.Wiggle = randomIdleAnimationIota++,
        "wiggle.fbx"
      },
    }
  },
  {
    "speak",
    animationGroupIndexes.Speak = animationGroupIota++,
    {
      {
        "speak1",
        speakAnimationIndexes.Speak1 = speakAnimationIota++,
        "speak.fbx"
      },
      {
        "speak2",
        speakAnimationIndexes.Speak2 = speakAnimationIota++,
        "speak_soft.fbx"
      },
      {
        "speak3",
        speakAnimationIndexes.Speak3 = speakAnimationIota++,
        "explain.fbx"
      },
    }
  },
  {
    "think",
    animationGroupIndexes.Think = animationGroupIota++,
    {
      {
        "think1",
        thinkAnimationIndexes.Think1 = thinkAnimationIota++,
        "think_intro.fbx"
      },
      {
        "think2",
        thinkAnimationIndexes.Think2 = thinkAnimationIota++,
        "think1.fbx"
      },
      {
        "think3",
        thinkAnimationIndexes.Think3 = thinkAnimationIota++,
        "think2.fbx"
      },
    }
  },
  {
    "search",
    animationGroupIndexes.Search = animationGroupIota++,
    {
      {
        "search1",
        searchAnimationIndexes.Search1 = searchAnimationIota++,
        "keyboard_soft.fbx"
      },
      {
        "search2",
        searchAnimationIndexes.Search2 = searchAnimationIota++,
        "keyboard_soft.fbx"
      },
      {
        "search3",
        searchAnimationIndexes.Search3 = searchAnimationIota++,
        "keyboard_soft.fbx"
      },
    }
  }
};