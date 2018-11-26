#ifndef RemoteConfig_h
#define RemoteConfig_h

#include "base/ccConfig.h"
#include "jsapi.h"
#include "jsfriendapi.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif

void register_all_remoteconfig_framework(JSContext* cx, JS::HandleObject obj);

#endif /* RemoteConfig_h */
