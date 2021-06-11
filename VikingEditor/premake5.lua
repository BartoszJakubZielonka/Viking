project "VikingEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin/int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Source/**.h",
		"Source/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Viking/Vendor/spdlog/include",
		"%{wks.location}/Viking/Source",
		"%{wks.location}/Viking/Vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Assimp}"
	}

	links
	{
		"Viking"
	}

	filter "system:windows"
		systemversion "latest"

		postbuildcommands
		{
			"{COPY} %{LibraryDir.VulkanSDK_DLL} %{wks.location}/bin/" .. outputdir .. "/%{prj.name}"
		}

	filter "configurations:Debug"
		defines "VI_DEBUG"
		runtime "Debug"
		symbols "on"

		links
		{
			"../Viking/Vendor/assimp/bin/Debug/assimp-vc141-mtd.lib"
		}

		postbuildcommands
		{
			'{COPY} "../Viking/Vendor/assimp/bin/Debug/assimp-vc141-mtd.dll" "%{cfg.targetdir}"'
		}

	filter "configurations:Release"
		defines "VI_RELEASE"
		runtime "Release"
		optimize "on"

		links
		{
			"Viking/Vendor/assimp/bin/Release/assimp-vc141-mt.lib"
		}

		postbuildcommands
		{
			'{COPY} "../Viking/Vendor/assimp/bin/Release/assimp-vc141-mt.dll" "%{cfg.targetdir}"',
		}

	filter "configurations:Dist"
		defines "VI_DIST"
		runtime "Release"
		optimize "on"

		links
		{
			"Viking/Vendor/assimp/bin/Release/assimp-vc141-mt.lib"
		}

		postbuildcommands
		{
			'{COPY} "../Viking/Vendor/assimp/bin/Release/assimp-vc141-mt.dll" "%{cfg.targetdir}"',
		}