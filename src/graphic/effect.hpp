#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace Graphic {

	class Effect
	{
	public:
		Effect(const std::string_view& _vertexShader, const std::string_view& _fragmentShader);
		~Effect();
	private:
		VkShaderModule LoadShader(const std::string_view& _name);

		VkShaderModule m_vertexShader;
		VkShaderModule m_geometryShader;
		VkShaderModule m_fragmentShader;

		VkRenderPass m_renderPass;
		VkPipelineLayout m_pipelineLayout;

		VkPipeline m_graphicsPipeline;
		std::vector<VkFramebuffer> m_swapChainFramebuffers; // frame buffers are per pipeline

		friend class Device;
	};
}