// Extension lib defines
#define EXTENSION_NAME androidnative
#define LIB_NAME "androidnative"
#define MODULE_NAME LIB_NAME

// Defold SDK
#define DLIB_LOG_DOMAIN LIB_NAME
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_ANDROID)

#include "testlib.h" // Multiply

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

namespace {
struct AttachScope
{
    JNIEnv* m_Env;
    AttachScope() : m_Env(Attach())
    {
    }
    ~AttachScope()
    {
        Detach(m_Env);
    }
};
}

static jclass GetClass(JNIEnv* env, const char* classname)
{
    jclass activity_class = env->FindClass("android/app/NativeActivity");
    jmethodID get_class_loader = env->GetMethodID(activity_class,"getClassLoader", "()Ljava/lang/ClassLoader;");
    jobject cls = env->CallObjectMethod(dmGraphics::GetNativeAndroidActivity(), get_class_loader);
    jclass class_loader = env->FindClass("java/lang/ClassLoader");
    jmethodID find_class = env->GetMethodID(class_loader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

    jstring str_class_name = env->NewStringUTF(classname);
    jclass outcls = (jclass)env->CallObjectMethod(cls, find_class, str_class_name);
    env->DeleteLocalRef(str_class_name);
    return outcls;
}

static int DoStuffJava(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    jclass cls = GetClass(env, "com.defold.androidnativeext.NativeExample");
    jmethodID method = env->GetStaticMethodID(cls, "DoStuff", "()Ljava/lang/String;");
    
    jstring return_value = (jstring)env->CallStaticObjectMethod(cls, method);
    lua_pushstring(L, env->GetStringUTFChars(return_value, 0));
    env->DeleteLocalRef(return_value);
    return 1;
}

static int DoStuffJar(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    jclass cls = GetClass(env, "com.svenandersson.dummy.Dummy");
    jmethodID method = env->GetStaticMethodID(cls, "DoStuff", "()Ljava/lang/String;");
    
    jstring return_value = (jstring)env->CallStaticObjectMethod(cls, method);
    lua_pushstring(L, env->GetStringUTFChars(return_value, 0));
    env->DeleteLocalRef(return_value);
    return 1;
}

static int Vibrate(lua_State* L)
{
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;
    
    int duration = luaL_checkint(L, 1);

    jclass cls = GetClass(env, "com.defold.androidnativeext.NativeExample");

    jmethodID dummy_method = env->GetStaticMethodID(cls, "vibratePhone", "(Landroid/content/Context;I)V");
    env->CallStaticObjectMethod(cls, dummy_method, dmGraphics::GetNativeAndroidActivity(), duration);
    return 0;
}

static int GetRaw(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    jclass cls = GetClass(env, "com.defold.androidnativeext.NativeExample");

    jmethodID method = env->GetStaticMethodID(cls, "GetRaw", "(Landroid/content/Context;)Ljava/lang/String;");
    jstring return_value = (jstring)env->CallStaticObjectMethod(cls, method, dmGraphics::GetNativeAndroidActivity());
    lua_pushstring(L, env->GetStringUTFChars(return_value, 0));
    env->DeleteLocalRef(return_value);

    return 1;
}

static int Multiply(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);

    lua_Number a = luaL_checknumber(L, 1);
    lua_Number b = luaL_checknumber(L, 2);

    lua_pushnumber(L, a * b);
    return 1;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    {"dostuff_java", DoStuffJava},
    {"dostuff_jar", DoStuffJar},
    {"vibrate", Vibrate},
    {"getraw", GetRaw},
    {"multiply", Multiply},
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

static dmExtension::Result AppInitializeExtension(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result InitializeExtension(dmExtension::Params* params)
{
    // Init Lua
    LuaInit(params->m_L);
    printf("Registered %s Extension\n", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

static dmExtension::Result AppFinalizeExtension(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeExtension(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

#else

static dmExtension::Result AppInitializeExtension(dmExtension::AppParams* params)
{
    dmLogWarning("Registered %s (null) Extension\n", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

static dmExtension::Result InitializeExtension(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result AppFinalizeExtension(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeExtension(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

#endif

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, AppInitializeExtension, AppFinalizeExtension, InitializeExtension, 0, 0, FinalizeExtension)
