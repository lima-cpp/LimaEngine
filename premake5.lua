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
        "%{prj.name}/vendor/spdlog/include"
    }

    buildoptions
    {
        "/utf-8"
    }

    disablewarnings
    {
        "4251"
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
        "Lima/src"
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
        "Lima"
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
