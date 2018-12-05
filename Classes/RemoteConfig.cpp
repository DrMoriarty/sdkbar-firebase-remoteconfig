#include "RemoteConfig.hpp"
#include "Firebase.hpp"
#include "scripting/js-bindings/manual/cocos2d_specifics.hpp"
#include "scripting/js-bindings/manual/js_manual_conversions.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <sstream>
#include "base/CCDirector.h"
#include "base/CCScheduler.h"
#include "utils/PluginUtils.h"
#include "firebase/remote_config.h"

static void printLog(const char* str) {
    CCLOG("%s", str);
}

///////////////////////////////////////
//
//  Plugin Init
//
///////////////////////////////////////

static bool jsb_admob_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    printLog("jsb_admob_init");
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    JS::CallReceiver rec = JS::CallReceiverFromVp(vp);
    if(argc == 0) {

        printLog("[FirebaseRemoteConfig] Init plugin");
        firebase::App* app = firebase_app();
        if(app == NULL) {
            printLog("Firebase not initialized!");
            rec.rval().set(JSVAL_FALSE);
            return false;
        }
        if(firebase::remote_config::Initialize(*app) == firebase::kInitResultSuccess) {
            if(firebase::remote_config::ActivateFetched()) {
                printLog("Firebase: activate fetched config");
            }
            firebase::remote_config::Fetch();
        }
        rec.rval().set(JSVAL_TRUE);
        return true;
    } else {
        JS_ReportError(cx, "Invalid number of arguments");
        return false;
    }
}

///////////////////////////////////////
//
//  Remote Config
//
///////////////////////////////////////

static bool jsb_admob_get_boolean(JSContext *cx, uint32_t argc, jsval *vp)
{
    printLog("jsb_admob_get_boolean");
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    JS::CallReceiver rec = JS::CallReceiverFromVp(vp);
    if(argc == 1) {
        // key
        bool ok = true;
        std::string key;
        JS::RootedValue arg0Val(cx, args.get(0));
        ok &= jsval_to_std_string(cx, arg0Val, &key);
        if(firebase::remote_config::GetBoolean(key.c_str())) {
            rec.rval().set(JSVAL_TRUE);
        } else {
            rec.rval().set(JSVAL_FALSE);
        }
        return true;
    } else {
        JS_ReportError(cx, "Invalid number of arguments");
        return false;
    }
}


static bool jsb_admob_get_integer(JSContext *cx, uint32_t argc, jsval *vp)
{
    printLog("jsb_admob_get_integer");
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    JS::CallReceiver rec = JS::CallReceiverFromVp(vp);
    if(argc == 1) {
        // key
        bool ok = true;
        std::string key;
        JS::RootedValue arg0Val(cx, args.get(0));
        ok &= jsval_to_std_string(cx, arg0Val, &key);
        int64_t value = firebase::remote_config::GetLong(key.c_str());
        rec.rval().set(JS::Int32Value(value));
        return true;
    } else {
        JS_ReportError(cx, "Invalid number of arguments");
        return false;
    }
}

static bool jsb_admob_get_double(JSContext *cx, uint32_t argc, jsval *vp)
{
    printLog("jsb_admob_get_double");
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    JS::CallReceiver rec = JS::CallReceiverFromVp(vp);
    if(argc == 1) {
        // key
        bool ok = true;
        std::string key;
        JS::RootedValue arg0Val(cx, args.get(0));
        ok &= jsval_to_std_string(cx, arg0Val, &key);
        double value = firebase::remote_config::GetDouble(key.c_str());
        rec.rval().set(JS::DoubleValue(value));
        return true;
    } else {
        JS_ReportError(cx, "Invalid number of arguments");
        return false;
    }
}

static bool jsb_admob_get_string(JSContext *cx, uint32_t argc, jsval *vp)
{
    printLog("jsb_admob_get_string");
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    JS::CallReceiver rec = JS::CallReceiverFromVp(vp);
    if(argc == 1) {
        // key
        bool ok = true;
        std::string key;
        JS::RootedValue arg0Val(cx, args.get(0));
        ok &= jsval_to_std_string(cx, arg0Val, &key);
        std::string value = firebase::remote_config::GetString(key.c_str());
        rec.rval().set(std_string_to_jsval(cx, value));
        return true;
    } else {
        JS_ReportError(cx, "Invalid number of arguments");
        return false;
    }
}

///////////////////////////////////////
//
//  Register JS API
//
///////////////////////////////////////

void register_all_remoteconfig_framework(JSContext* cx, JS::HandleObject obj) {
    printLog("[FirebaseRemoteConfig] register js interface");
    JS::RootedObject ns(cx);
    get_or_create_js_obj(cx, obj, "remoteconfig", &ns);

    JS_DefineFunction(cx, ns, "init", jsb_admob_init, 0, JSPROP_ENUMERATE | JSPROP_PERMANENT);

    JS_DefineFunction(cx, ns, "get_boolean", jsb_admob_get_boolean, 1, JSPROP_ENUMERATE | JSPROP_PERMANENT);
    JS_DefineFunction(cx, ns, "get_integer", jsb_admob_get_integer, 1, JSPROP_ENUMERATE | JSPROP_PERMANENT);
    JS_DefineFunction(cx, ns, "get_double", jsb_admob_get_double, 1, JSPROP_ENUMERATE | JSPROP_PERMANENT);
    JS_DefineFunction(cx, ns, "get_string", jsb_admob_get_string, 1, JSPROP_ENUMERATE | JSPROP_PERMANENT);


}
