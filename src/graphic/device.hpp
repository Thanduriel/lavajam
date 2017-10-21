#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

namespace Graphic {

	class Effect;
	class BasicVertexBuffer;

	class Device
	{
	public:
		static void Init(int _resolutionX, int _resolutionY);
		static void Exit();

		static GLFWwindow* GetWindow();

		static VkDevice GetVkDevice() { return m_logicalDevice; }
		static int GetResolutionX() { return m_resolutionX; }
		static int GetResolutionY() { return m_resolutionY; }

		// set the pipeline
		static void SetEffect(const Effect& _effect);

		// draw current pipline
		static void Draw(const BasicVertexBuffer& _vertexBuffer);

		// looks for a suitable memory type supported by the choosen physical device
		static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	private:
		static std::vector<const char*> GetRequiredExtensions();
		static void CheckExtensions(unsigned _count = 0, const char** const  _required = nullptr);
		static void CheckValidationLayers(unsigned _count, const char*const*  _layers);
		static void PickPhysicalDevice();
		static void CreateLogicalDevice();
		static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		static void CreateSwapChain();
		static void CreateImageViews();
		static void CreateCommandPool();
		static void CreateSemaphores();

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

		static int m_resolutionX;
		static int m_resolutionY;

		static const Effect* m_currentEffect;

		// vulkan handles
		static GLFWwindow* m_window;
		static VkInstance m_instance;
		static VkPhysicalDevice m_physicalDevice;
		static VkDevice m_logicalDevice;
		static VkDebugReportCallbackEXT m_callback;
		static VkSurfaceKHR m_surface;

		// queues
		static VkQueue m_graphicsQueue;
		static VkQueue m_presentQueue;

		//frame buffers
		static VkSwapchainKHR m_swapChain;
		static std::vector<VkImage> m_swapChainImages;
		static VkFormat m_swapChainImageFormat;
		static VkExtent2D m_swapChainExtent;
		static std::vector<VkImageView> m_swapChainImageViews;

		static VkCommandPool m_commandPool;

		static VkSemaphore m_imageAvailableSemaphore;
		static VkSemaphore m_renderFinishedSemaphore;

		friend class Effect;
	};
}