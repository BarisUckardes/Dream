#include "GraphicsDevice.h"

namespace Dream
{
    GraphicsDevice::~GraphicsDevice()
    {

    }
    GraphicsQueue* GraphicsDevice::CreateQueue(const GraphicsQueueDesc& desc)
    {
        //First validate if device can give queue
        if (!HasQueue(desc.Type))
            return nullptr;

        GraphicsQueue* pQueue = CreateQueueCore(desc);

        RegisterObject(pQueue);

        return pQueue;
    }
    GraphicsDevice::GraphicsDevice(const GraphicsDeviceDesc* pDesc)
    {
        mOwnerAdapter = pDesc->pOwnerAdapter;
    }
    void GraphicsDevice::RegisterObject(GraphicsDeviceObject* pObject)
    {
        mObjects.push_back(pObject);
    }
    void GraphicsDevice::RemoveObject(GraphicsDeviceObject* pObject)
    {
        auto findIterator = std::find(mObjects.begin(), mObjects.end(), pObject);
        if (findIterator == mObjects.end())
            return;

        mObjects.erase(findIterator);
    }
}
