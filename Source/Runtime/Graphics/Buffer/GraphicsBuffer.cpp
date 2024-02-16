#include "GraphicsBuffer.h"

namespace Dream
{
	GraphicsBuffer::~GraphicsBuffer()
	{
	}
	GraphicsBuffer::GraphicsBuffer(const GraphicsBufferDesc& desc, GraphicsDevice* pDevice) :
		GraphicsDeviceObject(pDevice),mUsages(desc.Usage), mSubItemCount(desc.SubItemCount), mSubItemSize(desc.SubItemSizeInBytes), mTotalSize(desc.SubItemCount* desc.SubItemSizeInBytes),mMemory(desc.pMemory)
	{

	}
}
