#include "GraphicsInstance.h"
#include <Runtime/Core/Core.h>

namespace Dream
{
#ifdef DREAM_DEBUG
    PFN_vkCreateDebugUtilsMessengerEXT debugMessengerCreateProc = NULL;
    PFN_vkDestroyDebugUtilsMessengerEXT debugMessengerDestroyProc = NULL;
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallBack(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        DEV_LOG("Vulkan", "%s", pCallbackData->pMessage);
        return VK_FALSE;
    }
#endif

    GraphicsInstance::GraphicsInstance(const GraphicsInstanceDesc& desc) : mInstance(VK_NULL_HANDLE)
    {
        struct ExtensionEntry
        {
            const char* pName;
            bool bSupported;
        };

        //Create requested extension array
        std::vector<ExtensionEntry> instanceRequestedExtensions;
        for (const std::string& extension : desc.Extensions)
            instanceRequestedExtensions.push_back({ extension.c_str(),false });

        //Get supported extension count
        unsigned int supportedExtensionCount = 0;
        DEV_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionCount, nullptr) == VK_SUCCESS,"VulkanInstance","Failed to get supported extension count");
        DEV_ASSERT(supportedExtensionCount > 0, "VulkanInstance", "No supported extensions found!");

        //Get supported extensions
        std::vector<VkExtensionProperties> supportedExtensions(supportedExtensionCount);
        DEV_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionCount, supportedExtensions.data()) == VK_SUCCESS, "VulkanInstace", "Failed to get supported extension properties");

        //Add required extensions
        instanceRequestedExtensions.push_back({ VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME ,false });
#ifdef DREAM_DEBUG
        instanceRequestedExtensions.push_back({ VK_EXT_DEBUG_REPORT_EXTENSION_NAME ,false });
        instanceRequestedExtensions.push_back({ VK_EXT_DEBUG_UTILS_EXTENSION_NAME ,false });
#endif

        //Check if extension names are supported
        for (unsigned int instanceExtensionIndex = 0; instanceExtensionIndex < instanceRequestedExtensions.size(); instanceExtensionIndex++)
        {
            ExtensionEntry& entry = instanceRequestedExtensions[instanceExtensionIndex];

            //Check if properties contains this extension
            bool bExtensionSupported = false;
            for (unsigned int supportedInstanceExtensionIndex = 0; supportedInstanceExtensionIndex < supportedExtensionCount; supportedInstanceExtensionIndex++)
            {
                const VkExtensionProperties& supportedInstanceExtensionProperty = supportedExtensions[supportedInstanceExtensionIndex];

                if (strcmp(entry.pName, supportedInstanceExtensionProperty.extensionName) == 0)
                {
                    bExtensionSupported = true;
                    break;
                }
            }

            entry.bSupported = bExtensionSupported;
        }

        //Filter extensions
        std::vector<const char*> selectedExtensions;
        std::vector<const char*> unsupportedExtensions;
        for (unsigned int i = 0; i < instanceRequestedExtensions.size(); i++)
        {
            ExtensionEntry& entry = instanceRequestedExtensions[i];
            if (entry.bSupported)
                selectedExtensions.push_back(entry.pName);
            else
                unsupportedExtensions.push_back(entry.pName);
        }

        //Get supported layer count
        unsigned int supportedLayerCount = 0;
        DEV_ASSERT(vkEnumerateInstanceLayerProperties(&supportedLayerCount, nullptr) == VK_SUCCESS, "VulkanInstance", "Failed to get supported layer count");
        DEV_ASSERT(supportedLayerCount > 0, "VulkanInstance", "No supported layers found");

        std::vector<VkLayerProperties> supportedLayers(supportedLayerCount);
        DEV_ASSERT(vkEnumerateInstanceLayerProperties(&supportedLayerCount, supportedLayers.data()) == VK_SUCCESS, "VulkanInstance", "Failed to get supported layers");

        //Get layers
        std::vector<const char*> selectedLayerNames = {};
        for (const std::string& layer : desc.Layers)
            selectedLayerNames.push_back(layer.c_str());

#ifdef DREAM_DEBUG
        selectedLayerNames.push_back("VK_LAYER_KHRONOS_validation");
#endif

        //Create instance
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Runtime";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Runtime";
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo instanceInfo = {};
        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceInfo.pApplicationInfo = &appInfo;
        instanceInfo.enabledExtensionCount = selectedExtensions.size();
        instanceInfo.ppEnabledExtensionNames = selectedExtensions.data();
        instanceInfo.enabledLayerCount = selectedLayerNames.size();
        instanceInfo.ppEnabledLayerNames = selectedLayerNames.data();

        DEV_ASSERT(vkCreateInstance(&instanceInfo, nullptr, &mInstance) == VK_SUCCESS, "VulkanInstance", "Failed to create instance!");

#ifdef DREAM_DEBUG
        //Get debug proc addresses
        debugMessengerCreateProc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mInstance, "vkCreateDebugUtilsMessengerEXT");
        debugMessengerDestroyProc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mInstance, "vkDestroyDebugUtilsMessengerEXT");

        DEV_ASSERT(debugMessengerCreateProc != nullptr, "VulkanInstance", "Failed to get vkCreateDebugUtilsMessengerEXT method!");
        DEV_ASSERT(debugMessengerDestroyProc != nullptr, "VulkanInstance", "Failed to get vkDestroyDebugUtilsMessengerEXT method!");

        //Create debug messenger
        VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo = {};
        debugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugMessengerInfo.pfnUserCallback = DebugCallBack;
        debugMessengerInfo.pUserData = nullptr;
        DEV_ASSERT(debugMessengerCreateProc(mInstance, &debugMessengerInfo, nullptr, &mDebugMessenger) == VK_SUCCESS, "VulkanInstance", "Failed to create VkDebugUtilsMessengerEXT");
#endif

        //Get physical device count
        unsigned int physicalDeviceCount = 0;
        DEV_ASSERT(vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, nullptr) == VK_SUCCESS, "VulkanInstance", "Failed to get physical device count");
        DEV_ASSERT(physicalDeviceCount > 0, "VulkanInstace", "No physical devices found!");

        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
        DEV_ASSERT(vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, physicalDevices.data()) == VK_SUCCESS, "VulkanInstace", "Failed to get physical devices");

        for (const VkPhysicalDevice physicalDevice : physicalDevices)
        {
            VkPhysicalDeviceMemoryProperties memoryProperties = {};
            vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

            VkPhysicalDeviceProperties deviceProperties = {};
            vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

            GraphicsAdapterDesc adapterDesc = {};
            adapterDesc.PhysicalDevice = physicalDevice;
            adapterDesc.ProductName = deviceProperties.deviceName;
            adapterDesc.VendorName = "N/A";
            adapterDesc.ImageDimensions[0] = deviceProperties.limits.maxImageDimension1D;
            adapterDesc.ImageDimensions[1] = deviceProperties.limits.maxImageDimension2D;
            adapterDesc.ImageDimensions[2] = deviceProperties.limits.maxImageDimension3D;
            adapterDesc.pInstance = this;

            GraphicsAdapter* pAdapter = new GraphicsAdapter(adapterDesc);
            mAdapters.push_back(pAdapter);
        }
        DEV_LOG("VulkanInstance", "Created instance!");
    }
    GraphicsInstance::~GraphicsInstance()
    {
#ifdef DREAM_DEBUG
        debugMessengerDestroyProc(mInstance, mDebugMessenger, nullptr);
#endif

        vkDestroyInstance(mInstance, nullptr);
        DEV_LOG("VulkanInstance", "Destroyed");
    }
}
