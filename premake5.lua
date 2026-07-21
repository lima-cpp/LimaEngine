workspace "Lima"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Lima/vendor/GLFW"

project "Lima"
    location "Lima"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/vendor/GLFW/include",
        "%{prj.name}/vendor/glm/glm",
        "%{prj.name}/vendor/VulkanSDK/1.4.350.0/Include"
    }

    buildoptions
    {
        "/utf-8"
    }

    disablewarnings
    {
        "4251"
    }

    libdirs
    {
        "%{prj.name}/vendor/VulkanSDK/1.4.350.0/Lib"
    }

    links
    {
        "GLFW",
        "vulkan-1"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "L_PLATFORM_WINDOWS",
            "LIMA_BUILD_DLL"
        }

        postbuildcommands
        {
            'xcopy /Y /D /I "$(TargetPath)" "$(SolutionDir)bin\\' .. outputdir .. '\\Sandbox\\"'
        }

    filter "configurations:Debug"
        defines "L_DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "L_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "L_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Lima/vendor/spdlog/include",
        "Lima/vendor/glm/glm",
        "Lima/vendor/GLFW/include",
        "Lima/vendor/VulkanSDK/1.4.350.0/Include",
        "Lima/src"
    }

    libdirs
    {
        "Lima/vendor/VulkanSDK/1.4.350.0/Lib"
    }

    buildoptions
    {
        "/utf-8"
    }

    disablewarnings
    {
        "4251"
    }

    links
    {
        "Lima",
        "GLFW",
        "vulkan-1"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "L_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "L_DEBUG"
        symbols "On"
    
    filter "configurations:Release"
        defines "L_RELEASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "L_DIST"
        optimize "On"
