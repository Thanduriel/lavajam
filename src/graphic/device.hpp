#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace Graphic {

	class Device
	{
	public:
		static void Init();
		static void Exit();

		static GLFWwindow* GetWindow();
	private:
		static std::vector<const char*> GetRequiredExtensions();
		static void CheckExtensions(unsigned _count = 0, const char** const  _required = nullptr);
		static void CheckValidationLayers(unsigned _count, const char*const*  _layers);
		static void PickPhysicalDevice();
		static void CreateLogicalDevice();

		struct QueueFamilyIndices
		{
			int graphicsFamily = -1;
			int presentFamily = -1;

			bool IsComplete() {
				return graphicsFamily >= 0 && presentFamily >= 0;
			}
		};
		static QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice _device);

		struct SwapChainSupportDetails 
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};
		static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice _device);

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugReportFlagsEXT flags,
			VkDebugReportObjectTypeEXT objType,
			uint64_t obj,
			size_t location,
			int32_t code,
			const char* layerPrefix,
			const char* msg,
			void* userData);

		static GLFWwindow* m_window;
		static VkInstance m_instance;
		static VkPhysicalDevice m_physicalDevice;
		static VkDevice m_logicalDevice;
		static VkDebugReportCallbackEXT m_callback;
		static VkSurfaceKHR m_surface;

		// queues
		static VkQueue m_graphicsQueue;
		static VkQueue m_presentQueue;
	};
}