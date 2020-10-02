#include "mapch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Mana {

    RendererAPI* RenderCommand::s_rendererAPI = new OpenGLRendererAPI;

}