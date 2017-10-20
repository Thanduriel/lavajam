#include "device.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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
	GLFWwindow* Device::m_window;
	VkInstance Device::m_instance;
	VkDebugReportCallbackEXT Device::m_callback;
	VkPhysicalDevice Device::m_physicalDevice;
	VkDevice Device::m_logicalDevice;
	VkSurfaceKHR Device::m_surface;

	VkQueue Device::m_graphicsQueue;
	VkQueue Device::m_presentQueue;

	void Device::Init()
	{
		// window creation
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		m_window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

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
	}

	void Device::Exit()
	{
		DestroyDebugReportCallbackEXT(m_instance, m_callback, nullptr);
		vkDestroyDevice(m_logicalDevice, nullptr);
		vkDestroyInstance(m_instance, nullptr);

		glfwDestroyWindow(m_window);

		glfwTerminate();
	}

	GLFWwindow* Device::GetWindow()
	{
		return m_window;
	}

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

		std::cout << "available extensions:" << std::endl;
		for (auto& ext : extensions)
			std::cout << ext.extensionName << "[v." << ext.specVersion << "]" << std::endl;

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

			return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
				&& deviceFeatures.geometryShader
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