#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace Graphic {

	class BasicVertexBuffer;

	class Effect
	{
	public:
		// @_buffer Required for vertex description
		Effect(const std::string_view& _vertexShader, const std::string_view& _fragmentShader,
			const BasicVertexBuffer& _buffer, const std::string_view& _geometryShader = "");
		~Effect();
	private:
		VkShaderModule LoadShader(const std::string_view& _name);
		void CreateCommandBuffers(const BasicVertexBuffer& _vertexBuffer);
		void BuildCommandBuffers(const BasicVertexBuffer& _vertexBuffer) const;

		VkShaderModule m_vertexShader;
		VkShaderModule m_geometryShader;
		VkShaderModule m_fragmentShader;

		VkRenderPass m_renderPass;
		VkPipelineLayout m_pipelineLayout;

		VkPipeline m_graphicsPipeline;
		std::vector<VkFramebuffer> m_swapChainFramebuffers; // frame buffers are per pipeline

		std::vector<VkCommandBuffer> m_commandBuffers;

		friend class Device;
	};
}