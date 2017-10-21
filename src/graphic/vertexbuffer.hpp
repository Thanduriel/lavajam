#pragma once

#include <initializer_list>
#include <vulkan/vulkan.hpp>
#include "glm/glm.hpp"
#include "device.hpp"

namespace Graphic {

	enum struct VertexFormat {
		FLOAT,
		VEC2,
		VEC3,
		VEC4,
	//	MAT2,
	//	MAT3,
	//	MAT4,
		COUNT
	};

	const std::array<uint32_t, static_cast<uint32_t>(VertexFormat::COUNT)> VertexFormatSize = {
		sizeof(float),
		sizeof(glm::vec2),
		sizeof(glm::vec3),
		sizeof(glm::vec4),
//		sizeof(glm::mat3),
//		sizeof(glm::mat4)
	};

	const std::array<VkFormat, static_cast<uint32_t>(VertexFormat::COUNT)> VertexFormatVk = {
		VK_FORMAT_R32_SFLOAT,
		VK_FORMAT_R32G32_SFLOAT,
		VK_FORMAT_R32G32B32_SFLOAT,
		VK_FORMAT_R32G32B32A32_SFLOAT
	};


	class BasicVertexBuffer
	{
	protected:
		VkVertexInputBindingDescription m_bindingDescription;
		std::vector<VkVertexInputAttributeDescription> m_vertexAttributes;
		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;
		uint32_t m_numVertices;

		friend class Effect;
	};

	template<typename T, size_t MaxNumEl = 1024>
	class VertexBuffer : public BasicVertexBuffer
	{
	public:
		VertexBuffer(std::initializer_list<VertexFormat> _vertexFormats)
		{
			m_bindingDescription.binding = 0;
			m_bindingDescription.stride = sizeof(T);
			m_bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			m_vertexAttributes.resize(_vertexFormats.size());
			uint32_t off = 0;
			auto it = _vertexFormats.begin();
			for(size_t i = 0; i < _vertexFormats.size(); ++i)
			{
				auto& attr = m_vertexAttributes[i];
				attr.binding = 0;
				attr.location = (uint32_t)i;
				attr.format = VertexFormatVk[(size_t)*it];
				attr.offset = off;
				off += VertexFormatSize[(size_t)*it];
				++it;
			}

			VkBufferCreateInfo bufferInfo{};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = sizeof(T) * MaxNumEl;
			bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			if (vkCreateBuffer(Device::GetVkDevice(), &bufferInfo, nullptr, &m_vertexBuffer) != VK_SUCCESS)
				throw std::runtime_error("failed to create vertex buffer!");

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(Device::GetVkDevice(), m_vertexBuffer, &memRequirements);

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = Device::FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			if (vkAllocateMemory(Device::GetVkDevice(), &allocInfo, nullptr, &m_vertexBufferMemory) != VK_SUCCESS)
				throw std::runtime_error("failed to allocate vertex buffer memory!");

			vkBindBufferMemory(Device::GetVkDevice(), m_vertexBuffer, m_vertexBufferMemory, 0);
		}

		VertexBuffer() = delete;
		VertexBuffer(const VertexBuffer<T>&) = delete;
		VertexBuffer(VertexBuffer<T>&&) = delete;

		~VertexBuffer()
		{
			vkDestroyBuffer(Device::GetVkDevice(), m_vertexBuffer, nullptr);
			vkFreeMemory(Device::GetVkDevice(), m_vertexBufferMemory, nullptr);
		}

		void Clear() { m_vertices.clear(); }
		void Add(const T& _element) { m_vertices.push_back(_element); }
		void Upload()
		{
			m_numVertices = (uint32_t)m_vertices.size();

			void* data;
			size_t s = (size_t)m_vertices.size() * sizeof(T);
			vkMapMemory(Device::GetVkDevice(), m_vertexBufferMemory, 0, s, 0, &data);
			memcpy(data, m_vertices.data(), s);
			vkUnmapMemory(Device::GetVkDevice(), m_vertexBufferMemory);
		}

	private:
		std::vector<T> m_vertices;
	};
}