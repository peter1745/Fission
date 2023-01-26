project "FissionTest"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "Off"

    files {
        "./**.cpp",
        "./**.hpp",
        "./**.h"
    }

    includedirs { "../Source" }

    links "Fission"

    vectorextensions "AVX2"

    filter "system:windows"
        systemversion "latest"

        buildoptions { "/EHsc", "/Zc:preprocessor" }

    filter "system:linux"
        debugformat "Dwarf-4"

    filter "configurations:Debug"
        defines "FISSION_DEBUG"
        runtime "Debug"
        symbols "On"
        conformancemode "On"

    filter "configurations:Release"
        defines "FISSION_RELEASE"
        runtime "Release"
        optimize "On"
        conformancemode "On"

    filter "configurations:Distribution"
        defines "FISSION_DIST"
        runtime "Release"
        optimize "Full"
        conformancemode "On"
