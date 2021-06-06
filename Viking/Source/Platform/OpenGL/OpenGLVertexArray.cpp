//
// Created by batzi on 16.05.2021.
//
#include "vipch.h"
#include "OpenGLVertexArray.h"
#include <glad/gl.h>

namespace Viking {
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type)
        {
            case ShaderDataType::Float:    return GL_FLOAT;
            case ShaderDataType::Float2:   return GL_FLOAT;
            case ShaderDataType::Float3:   return GL_FLOAT;
            case ShaderDataType::Float4:   return GL_FLOAT;
            case ShaderDataType::Mat3:     return GL_FLOAT;
            case ShaderDataType::Mat4:     return GL_FLOAT;
            case ShaderDataType::Int:      return GL_INT;
            case ShaderDataType::Int2:     return GL_INT;
            case ShaderDataType::Int3:     return GL_INT;
            case ShaderDataType::Int4:     return GL_INT;
            case ShaderDataType::Bool:     return GL_BOOL;
        }

        VI_CORE_ASSERT(false, "Unknown ShaderDataType!")
        return 0;
    }


    OpenGLVertexArray::OpenGLVertexArray() {
        VI_PROFILE_FUNCTION();

        glCreateVertexArrays(1, &mRendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray() {
        VI_PROFILE_FUNCTION();

        glDeleteVertexArrays(1, &mRendererID);
    }

    void OpenGLVertexArray::bind() const {
        VI_PROFILE_FUNCTION();

        glBindVertexArray(mRendererID);
    }

    void OpenGLVertexArray::unbind() const {
        VI_PROFILE_FUNCTION();

        glBindVertexArray(0);
    }

    void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) {
        VI_PROFILE_FUNCTION();

        VI_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex Buffer has no layout!")

        glBindVertexArray(mRendererID);
        vertexBuffer->bind();

        const auto& layout = vertexBuffer->getLayout();
        for (const auto& element : layout)
        {
            switch (element.mType)
            {
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4:
                {
                    glEnableVertexAttribArray(mVertexBufferIndex);
                    glVertexAttribPointer(mVertexBufferIndex, element.getComponentCount(), ShaderDataTypeToOpenGLBaseType(element.mType), element.mNormalized ? GL_TRUE : GL_FALSE, layout.getStride(), (const void*)element.mOffset);
                    mVertexBufferIndex++;
                    break;
                }
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                case ShaderDataType::Bool:
                {
                    glEnableVertexAttribArray(mVertexBufferIndex);
                    glVertexAttribIPointer(mVertexBufferIndex, element.getComponentCount(), ShaderDataTypeToOpenGLBaseType(element.mType), layout.getStride(), (const void*)element.mOffset);
                    mVertexBufferIndex++;
                    break;
                }
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4:
                {
                    uint8_t count = element.getComponentCount();
                    for (uint8_t i = 0; i < count; i++)
                    {
                        glEnableVertexAttribArray(mVertexBufferIndex);
                        glVertexAttribPointer(mVertexBufferIndex, count, ShaderDataTypeToOpenGLBaseType(element.mType), element.mNormalized ? GL_TRUE : GL_FALSE, layout.getStride(), (const void*)(element.mOffset + sizeof(float) * count * i));
                        glVertexAttribDivisor(mVertexBufferIndex, 1);
                        mVertexBufferIndex++;
                    }
                    break;
                }
                default:
                    VI_CORE_ASSERT(false, "Unknown ShaderDataType!");
            }
        }

    }

    void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer> &indexBuffer) {
        VI_PROFILE_FUNCTION();

        glBindVertexArray(mRendererID);
        indexBuffer->bind();

        mIndexBuffer = indexBuffer;
    }


}
