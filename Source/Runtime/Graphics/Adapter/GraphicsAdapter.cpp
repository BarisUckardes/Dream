#include "GraphicsAdapter.h"

namespace Dream
{
    GraphicsAdapter::~GraphicsAdapter()
    {
    }
    GraphicsAdapter::GraphicsAdapter(const GraphicsAdapterDesc& desc) :
        mProductName(desc.ProductName), mVendorName(desc.VendorName), mSupportGeometryShader(desc.bSupportGeometryShader), mSupportComputeShader(desc.bSupportComputeShader),
        mColorAttachmentCount(desc.ColorAttachmentCount), mShaderTextureCount(desc.ShaderTextureCount), mShaderSamplerCount(desc.ShaderSamplerCount), mShaderBufferCount(desc.ShaderBufferCount),
        mMemory(desc.Memory),mImageWidth(desc.ImageWidth),mImageHeight(desc.ImageHeight),mImageDepth(desc.ImageDepth), mOwnerInstance(desc.pInstance)
    {

    }
    GraphicsDevice* GraphicsAdapter::CreateDevice(const GraphicsDeviceDesc* pDesc)
    {
        //Validations
        DEV_ASSERT(pDesc->pOwnerAdapter != nullptr, "GraphicsDevice", "Cannot give nullptr to GraphicsAdapter");

        GraphicsDevice* pDevice = CreateDeviceCore(pDesc);

        mDevices.push_back(pDevice);

        return pDevice;
    }
}
