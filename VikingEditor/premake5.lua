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
		"%{IncludeDir.ImGuizmo}"
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

	filter "configurations:Release"
		defines "VI_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "VI_DIST"
		runtime "Release"
		optimize "on"