#include "VideoRenderer.h"
#include "VKEngineRenderer.h"

VideoRenderer::VideoRenderer()
	: m_width(0)
	, m_height(0)
	, m_backingWidth(0)
	, m_backingHeight(0)
	, m_params(0)
	, m_rotation(0)
	, isDirty(false)
	, isProgramChanged(false)
{

}

VideoRenderer::~VideoRenderer()
{

}

std::unique_ptr<VideoRenderer> VideoRenderer::create(int type)
{
	return std::unique_ptr<VideoRenderer>(std::make_unique<VKEngineRenderer>());
}
