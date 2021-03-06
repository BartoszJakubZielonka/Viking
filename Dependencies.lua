VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["Assimp"] = "%{wks.location}/Viking/Vendor/assimp/include"
IncludeDir["stb_image"] = "%{wks.location}/Viking/Vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/Viking/Vendor/yaml-cpp/include"
IncludeDir["GLFW"] = "%{wks.location}/Viking/Vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Viking/Vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Viking/Vendor/ImGui"
IncludeDir["ImGuizmo"] = "%{wks.location}/Viking/Vendor/ImGuizmo"
IncludeDir["glm"] = "%{wks.location}/Viking/Vendor/glm"
IncludeDir["entt"] = "%{wks.location}/Viking/Vendor/entt/include"
IncludeDir["shaderc"] = "%{wks.location}/Viking/Vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/Viking/Vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/Viking/Vendor/VulkanSDK/Lib"
LibraryDir["VulkanSDK_DLL"] = "%{wks.location}/Viking/Vendor/VulkanSDK/Bin"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"