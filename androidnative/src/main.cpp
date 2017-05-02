// Extension lib defines
#define LIB_NAME "androidnative"
#define MODULE_NAME LIB_NAME

// include the Defold SDK
#include <dmsdk/sdk.h>

static JNIEnv* Attach()
{
    
    JNIEnv* env;
    JavaVM* vm = dmGraphics::GetNativeAndroidJavaVM();
    vm->AttachCurrentThread(&env, NULL);
    
    return env;
}

static bool Detach(JNIEnv* env)
{
    bool exception = (bool) env->ExceptionCheck();
    env->ExceptionClear();
    JavaVM* vm = dmGraphics::GetNativeAndroidJavaVM();
    vm->DetachCurrentThread();

    return !exception;
}

static int DoStuff(lua_State* L)
{
    JNIEnv* env = Attach();

    jclass activity_class = env->FindClass("android/app/NativeActivity");
    jmethodID get_class_loader = env->GetMethodID(activity_class,"getClassLoader", "()Ljava/lang/ClassLoader;");
    jobject cls = env->CallObjectMethod(dmGraphics::GetNativeAndroidActivity(), get_class_loader);
    jclass class_loader = env->FindClass("java/lang/ClassLoader");
    jmethodID find_class = env->GetMethodID(class_loader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

    //jstring str_class_name = env->NewStringUTF("com.svenandersson.dummy.Dummy");
    jstring str_class_name = env->NewStringUTF("com.defold.androidnativeext.Sven");
    jclass dummy_class = (jclass)env->CallObjectMethod(cls, find_class, str_class_name);
    env->DeleteLocalRef(str_class_name);

    jmethodID dummy_method = env->GetStaticMethodID(dummy_class, "DoStuff", "()Ljava/lang/String;");
    jstring dummy_return_value = (jstring)env->CallStaticObjectMethod(dummy_class, dummy_method);
    lua_pushstring(L, env->GetStringUTFChars(dummy_return_value, 0));
    env->DeleteLocalRef(dummy_return_value);

    Detach(env);

    return 1;
}

static int Vibrate(lua_State* L)
{
    JNIEnv* env = Attach();

    jclass activity_class = env->FindClass("android/app/NativeActivity");
    jmethodID get_class_loader = env->GetMethodID(activity_class,"getClassLoader", "()Ljava/lang/ClassLoader;");
    jobject cls = env->CallObjectMethod(dmGraphics::GetNativeAndroidActivity(), get_class_loader);
    jclass class_loader = env->FindClass("java/lang/ClassLoader");
    jmethodID find_class = env->GetMethodID(class_loader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

    jstring str_class_name = env->NewStringUTF("com.defold.androidnativeext.Sven");
    jclass dummy_class = (jclass)env->CallObjectMethod(cls, find_class, str_class_name);
    env->DeleteLocalRef(str_class_name);

    jmethodID dummy_method = env->GetStaticMethodID(dummy_class, "vibratePhone", "(Landroid/content/Context;I)V");
    env->CallStaticObjectMethod(dummy_class, dummy_method, dmGraphics::GetNativeAndroidActivity(), 1000);

    Detach(env);

    return 0;
}

static int GetRaw(lua_State* L)
{
    JNIEnv* env = Attach();

    jclass activity_class = env->FindClass("android/app/NativeActivity");
    jmethodID get_class_loader = env->GetMethodID(activity_class,"getClassLoader", "()Ljava/lang/ClassLoader;");
    jobject cls = env->CallObjectMethod(dmGraphics::GetNativeAndroidActivity(), get_class_loader);
    jclass class_loader = env->FindClass("java/lang/ClassLoader");
    jmethodID find_class = env->GetMethodID(class_loader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

    jstring str_class_name = env->NewStringUTF("com.defold.androidnativeext.Sven");
    jclass dummy_class = (jclass)env->CallObjectMethod(cls, find_class, str_class_name);
    env->DeleteLocalRef(str_class_name);

    jmethodID dummy_method = env->GetStaticMethodID(dummy_class, "GetRaw", "(Landroid/content/Context;)Ljava/lang/String;");
    jstring dummy_return_value = (jstring)env->CallStaticObjectMethod(dummy_class, dummy_method, dmGraphics::GetNativeAndroidActivity());
    lua_pushstring(L, env->GetStringUTFChars(dummy_return_value, 0));
    env->DeleteLocalRef(dummy_return_value);

    Detach(env);

    return 1;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    {"dostuff", DoStuff},
    {"vibrate", Vibrate},
    {"getraw", GetRaw},
    {0, 0}
};

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);

    // Register lua names
    luaL_register(L, MODULE_NAME, Module_methods);

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

dmExtension::Result AppInitializeAndroidNative(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeAndroidNative(dmExtension::Params* params)
{
    // Init Lua
    LuaInit(params->m_L);
    printf("Registered %s Extension\n", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeAndroidNative(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeAndroidNative(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(androidnative, LIB_NAME, AppInitializeAndroidNative, AppFinalizeAndroidNative, InitializeAndroidNative, 0, 0, FinalizeAndroidNative)
