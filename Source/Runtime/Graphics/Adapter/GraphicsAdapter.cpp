#include "GraphicsAdapter.h"

namespace Dream
{
    GraphicsAdapter::~GraphicsAdapter()
    {

    }
    GraphicsDevice* GraphicsAdapter::CreateDevice(const GraphicsDeviceDesc& desc)
    {
        GraphicsDevice* pDevice = new GraphicsDevice(desc,this);
        mDevices.push_back(pDevice);
        return pDevice;
    }
    GraphicsAdapter::GraphicsAdapter(const GraphicsAdapterDesc desc) :
        mProductName(desc.ProductName), mVendorName(desc.VendorName), mSupportGeometryShader(desc.bSupportGeometryShader), mSupportComputeShader(desc.bSupportComputeShader),
        mColorAttachmentCount(desc.ColorAttachmentCount), mShaderTextureCount(desc.ShaderTextureCount), mShaderSamplerCount(desc.ShaderSamplerCount), mShaderBufferCount(desc.ShaderBufferCount),
        mDeviceMemory(desc.DeviceMemory), mImageDimensions(), mOwnerInstance(desc.pInstance),mPhysicalDevice(desc.PhysicalDevice)
    {
        mImageDimensions[0] = desc.ImageDimensions[0];
        mImageDimensions[1] = desc.ImageDimensions[1];
        mImageDimensions[2] = desc.ImageDimensions[2];
    }
    void GraphicsAdapter::_OnDeviceDeleted(GraphicsDevice* pDevice)
    {
        const auto findIterator = std::find(mDevices.begin(), mDevices.end(), pDevice);
        if (findIterator == mDevices.end())
            return;

        mDevices.erase(findIterator);
    }
}
