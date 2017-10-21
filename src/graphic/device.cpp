#include "graphic/device.hpp"
#include "graphic/effect.hpp"
#include "vertexbuffer.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string.h>
#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <iostream>
#include <set>

namespace Graphic {

	const std::array<const char*, 1> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	// todo check that this extensions exist
	const std::array<const char*,1> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

#ifdef NDEBUG
	const bool ENABLE_VALIDATION_LAYERS = false;
#else
	const bool ENABLE_VALIDATION_LAYERS = true;
#endif


	VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) 
	{
		auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pCallback);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) 
	{
		auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
		if (func != nullptr) {
			func(instance, callback, pAllocator);
		}
	}

	// var definitions
	int Device::m_resolutionX;
	int Device::m_resolutionY;
	const Effect* Device::m_currentEffect;

	GLFWwindow* Device::m_window;
	VkInstance Device::m_instance;
	VkDebugReportCallbackEXT Device::m_callback;
	VkPhysicalDevice Device::m_physicalDevice;
	VkDevice Device::m_logicalDevice;
	VkSurfaceKHR Device::m_surface;

	VkQueue Device::m_graphicsQueue;
	VkQueue Device::m_presentQueue;

	VkSwapchainKHR Device::m_swapChain;
	std::vector<VkImage> Device::m_swapChainImages;
	VkFormat Device::m_swapChainImageFormat;
	VkExtent2D Device::m_swapChainExtent;
	std::vector<VkImageView> Device::m_swapChainImageViews;

	VkCommandPool Device::m_commandPool;

	VkSemaphore Device::m_imageAvailableSemaphore;
	VkSemaphore Device::m_renderFinishedSemaphore;

	void Device::Init(int _resolutionX, int _resolutionY)
	{
		m_resolutionX = _resolutionX;
		m_resolutionY = _resolutionY;
		// window creation
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		m_window = glfwCreateWindow(m_resolutionX, m_resolutionY, "Vulkan window", nullptr, nullptr);

		//vulkan setup
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = GetRequiredExtensions();
		CheckExtensions(static_cast<unsigned>(extensions.size()), extensions.data());

		createInfo.enabledExtensionCount = static_cast<unsigned>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		// optional error checking
		if constexpr(ENABLE_VALIDATION_LAYERS)
		{
			CheckValidationLayers(static_cast<unsigned>(validationLayers.size()), validationLayers.data());

			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else createInfo.enabledLayerCount = 0;
		

		if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) 
			throw std::runtime_error("failed to create instance!");

		// setup error callback
		if constexpr(ENABLE_VALIDATION_LAYERS)
		{
			VkDebugReportCallbackCreateInfoEXT dbgCreateInfo{};
			dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
			dbgCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
			dbgCreateInfo.pfnCallback = DebugCallback;

			if (CreateDebugReportCallbackEXT(m_instance, &dbgCreateInfo, nullptr, &m_callback) != VK_SUCCESS)
				throw std::runtime_error("failed to set up debug callback!");
		}

		// is required in physical device
		if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface) != VK_SUCCESS)
			throw std::runtime_error("failed to create window surface!");

		PickPhysicalDevice();
		CreateLogicalDevice();
		CreateSwapChain();
		CreateImageViews();
	
		CreateCommandPool();
		CreateSemaphores();
	}

	void Device::Exit()
	{
		vkDestroySemaphore(m_logicalDevice, m_renderFinishedSemaphore, nullptr);
		vkDestroySemaphore(m_logicalDevice, m_imageAvailableSemaphore, nullptr);

		vkDestroyCommandPool(m_logicalDevice, m_commandPool, nullptr);

		for (auto& imageView : m_swapChainImageViews) {
			vkDestroyImageView(m_logicalDevice, imageView, nullptr);
		}

		vkDestroySwapchainKHR(m_logicalDevice, m_swapChain, nullptr);
		DestroyDebugReportCallbackEXT(m_instance, m_callback, nullptr);
		vkDestroyDevice(m_logicalDevice, nullptr);
		vkDestroyInstance(m_instance, nullptr);

		glfwDestroyWindow(m_window);

		glfwTerminate();
	}

	// **************************************************************** //
	GLFWwindow* Device::GetWindow()
	{
		return m_window;
	}

	// **************************************************************** //
	void Device::SetEffect(const Effect& _effect)
	{
		m_currentEffect = &_effect;
	//	CreateCommandBuffers(_effect);
	}

	// **************************************************************** //
	void Device::Draw(const BasicVertexBuffer& _vertexBuffer)
	{
		m_currentEffect->BuildCommandBuffers(_vertexBuffer);

		uint32_t imageIndex;
		vkAcquireNextImageKHR(m_logicalDevice, m_swapChain, std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_currentEffect->m_commandBuffers[imageIndex];

		VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
			throw std::runtime_error("failed to submit draw command buffer!");


		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		presentInfo.pResults = nullptr; // Optional
		VkSwapchainKHR swapChains[] = { m_swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		vkQueuePresentKHR(m_presentQueue, &presentInfo);

		vkQueueWaitIdle(m_presentQueue);

	}

	// **************************************************************** //
	uint32_t Device::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
				return i;
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	// **************************************************************** //
	std::vector<const char*> Device::GetRequiredExtensions()
	{
		std::vector<const char*> extensions;

		unsigned int glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		for (unsigned int i = 0; i < glfwExtensionCount; i++) {
			extensions.push_back(glfwExtensions[i]);
		}

		if constexpr(ENABLE_VALIDATION_LAYERS) {
			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		}

		return std::move(extensions);
	}

	// **************************************************************** //
	using namespace std::string_literals;
	void Device::CheckExtensions(unsigned _count, const char** const  _required)
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		#ifndef NDEBUG
			std::cout << "available extensions:" << std::endl;
			for (auto& ext : extensions)
				std::cout << ext.extensionName << "[v." << ext.specVersion << "]" << std::endl;
		#endif

		if (_required)
		{
			for (unsigned i = 0; i < _count; ++i)
				if (std::find_if(extensions.begin(), extensions.end(), [&](const VkExtensionProperties& _ext)
				{
					return !strcmp(_ext.extensionName, _required[i]);
				}) == extensions.end())
					throw std::runtime_error("Missing vulkan extension support for: "s + _required[i]);
		}

	}

	// **************************************************************** //
	void Device::CheckValidationLayers(unsigned _count, const char*const* _layers)
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for(unsigned i = 0; i < _count; ++i)
			if (std::find_if(availableLayers.begin(), availableLayers.end(), [&](const VkLayerProperties& _ext)
			{
				return _ext.layerName == _layers[i];
			}) != availableLayers.end())
				throw std::runtime_error("validation layer not available: "s + _layers[i]);
	}

	Device::QueueFamilyIndices Device::GetQueueFamilies(VkPhysicalDevice _device) 
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(_device, &queueFamilyCount, queueFamilies.data());
		int i = 0;
		for (auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i;
			}
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(_device, i, m_surface, &presentSupport);

			if (queueFamily.queueCount > 0 && presentSupport)
				indices.presentFamily = i;

			if (indices.IsComplete()) break;

			++i;
		}

		return indices;
	}

	// **************************************************************** //
	void Device::PickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

		if (!deviceCount) 
			throw std::runtime_error("failed to find GPUs with Vulkan support!");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

		//prefer discrete GPUs
		std::sort(devices.begin(), devices.end(), [](const VkPhysicalDevice a,const VkPhysicalDevice b){
			VkPhysicalDeviceProperties devicePropertiesA;
			VkPhysicalDeviceProperties devicePropertiesB;			
			vkGetPhysicalDeviceProperties(a, &devicePropertiesA);
			vkGetPhysicalDeviceProperties(b, &devicePropertiesB);			
			return (devicePropertiesA.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && devicePropertiesA.deviceType != devicePropertiesB.deviceType);
		});

		// look for suitable
		auto it = std::find_if(devices.begin(), devices.end(), [](const VkPhysicalDevice _device) 
		{
			VkPhysicalDeviceProperties deviceProperties;
			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceProperties(_device, &deviceProperties);
			vkGetPhysicalDeviceFeatures(_device, &deviceFeatures);

			bool swapChainAdequate;
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(_device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
			#ifndef NDEBUG
				std::cout << "Found GPU: "<< deviceProperties.deviceName << std::endl;
			#endif
			return deviceFeatures.geometryShader
				&& GetQueueFamilies(_device).IsComplete()
				&& swapChainAdequate;
		});
		if (it == devices.end()) throw std::runtime_error("Could not find a suitable GPU");
		else m_physicalDevice = *it;
	}

	// **************************************************************** //
	void Device::CreateLogicalDevice()
	{
		QueueFamilyIndices indices = GetQueueFamilies(m_physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

		static const float queuePriority = 1.0f;
		for (int queueFamily : uniqueQueueFamilies) 
		{
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.geometryShader = VK_TRUE;

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		if constexpr(ENABLE_VALIDATION_LAYERS)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else createInfo.enabledLayerCount = 0;

		if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_logicalDevice) != VK_SUCCESS)
			throw std::runtime_error("failed to create logical device!");

		vkGetDeviceQueue(m_logicalDevice, indices.graphicsFamily, 0, &m_graphicsQueue);
		vkGetDeviceQueue(m_logicalDevice, indices.presentFamily, 0, &m_presentQueue);
	}

	// **************************************************************** //
	Device::SwapChainSupportDetails Device::QuerySwapChainSupport(VkPhysicalDevice _device) 
	{
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_device, m_surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(_device, m_surface, &formatCount, nullptr);

		if (formatCount) 
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(_device, m_surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(_device, m_surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(_device, m_surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	// **************************************************************** //
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) 
	{
		if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
			return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

		for (const auto& availableFormat : availableFormats) 
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return availableFormat;
		}

		return availableFormats[0];

	}

	// **************************************************************** //
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) 
	{
		VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

		for (const auto& availablePresentMode : availablePresentModes) 
		{
			// buffering with with possibility to update not yet displayed pictures
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				return availablePresentMode;
			else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) // no buffering
				bestMode = availablePresentMode;
		}

		return bestMode;
	}

	// **************************************************************** //
	VkExtent2D Device::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) 
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
			return capabilities.currentExtent;
		else 
		{
			VkExtent2D actualExtent = { (uint32_t)m_resolutionX, (uint32_t)m_resolutionY};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	// **************************************************************** //
	void Device::CreateSwapChain()
	{
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_physicalDevice);

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
			imageCount = swapChainSupport.capabilities.maxImageCount;

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_surface;

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = GetQueueFamilies(m_physicalDevice);
		uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily, (uint32_t)indices.presentFamily };

		if (indices.graphicsFamily != indices.presentFamily) 
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}

		// no global blending
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(m_logicalDevice, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
			throw std::runtime_error("failed to create swap chain!");

		// fetch individual image buffers
		vkGetSwapchainImagesKHR(m_logicalDevice, m_swapChain, &imageCount, nullptr);
		m_swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(m_logicalDevice, m_swapChain, &imageCount, m_swapChainImages.data());

		m_swapChainImageFormat = surfaceFormat.format;
		m_swapChainExtent = extent;
	}

	// **************************************************************** //
	void Device::CreateImageViews()
	{
		m_swapChainImageViews.resize(m_swapChainImages.size());

		for (size_t i = 0; i < m_swapChainImages.size(); i++)
		{
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_swapChainImages[i];

			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_swapChainImageFormat;

			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(m_logicalDevice, &createInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS)
				throw std::runtime_error("failed to create image views!");
		}
	}

	// **************************************************************** //
	void Device::CreateCommandPool()
	{
		QueueFamilyIndices queueFamilyIndices = GetQueueFamilies(m_physicalDevice);

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional

		if (vkCreateCommandPool(m_logicalDevice, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
			throw std::runtime_error("failed to create command pool!");
	}

	// **************************************************************** //
	void Device::CreateSemaphores()
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		if (vkCreateSemaphore(m_logicalDevice, &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS ||
			vkCreateSemaphore(m_logicalDevice, &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS)
			throw std::runtime_error("failed to create semaphores!");

	}

	// **************************************************************** //
	VKAPI_ATTR VkBool32 VKAPI_CALL Device::DebugCallback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t obj,
		size_t location,
		int32_t code,
		const char* layerPrefix,
		const char* msg,
		void* userData)
	{
		std::cerr << "validation error: " << msg << std::endl;

		return VK_FALSE;
	}
}
