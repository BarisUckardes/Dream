#include "GraphicsQueue.h"

namespace Dream
{
    GraphicsQueue::~GraphicsQueue()
    {

    }
    GraphicsQueue::GraphicsQueue(const GraphicsQueueDesc& desc,GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),mType(desc.Type)
    {

    }
}
