#include <Geode/DefaultInclude.hpp>

#include <Geode/loader/Mod.hpp>
#include <loader/ModImpl.hpp>
#include <dlfcn.h>

using namespace geode::prelude;

template <typename T>
T findSymbolOrMangled(void* dylib, char const* name, char const* mangled) {
    auto res = reinterpret_cast<T>(dlsym(dylib, name));
    if (!res) {
        res = reinterpret_cast<T>(dlsym(dylib, mangled));
    }
    return res;
}

Result<> Mod::Impl::loadPlatformBinary() {
    auto dylib =
        dlopen((m_tempDirName / m_info.binaryName()).string().c_str(), RTLD_LAZY);
    if (dylib) {
        if (m_platformInfo) {
            delete m_platformInfo;
        }
        m_platformInfo = new PlatformInfo { dylib };

        auto geodeImplicitEntry = findSymbolOrMangled<void(*)()>(dylib, "geodeImplicitEntry", "_geodeImplicitEntry@0");
        if (geodeImplicitEntry) {
            geodeImplicitEntry();
        }

        auto geodeCustomEntry = findSymbolOrMangled<void(*)()>(dylib, "geodeCustomEntry", "_geodeCustomEntry@0");
        if (geodeCustomEntry) {
            geodeCustomEntry();
        }

        return Ok();
    }
    std::string err = (char const*)dlerror();
    return Err("Unable to load the DYLIB: dlerror returned (" + err + ")");
}

Result<> Mod::Impl::unloadPlatformBinary() {
    auto dylib = m_platformInfo->m_dylib;
    delete m_platformInfo;
    m_platformInfo = nullptr;
    if (dlclose(dylib) == 0) {
        return Ok();
    }
    else {
        return Err("Unable to free library");
    }
}
