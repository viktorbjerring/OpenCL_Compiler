#ifdef __APPLE__
    #include <OpenCL/opencl.hpp>
#else
    #include <CL/cl.hpp>
#endif

#include <iostream>

#ifndef B8C6AC71_366E_4961_8330_99FECC1EA506
#define B8C6AC71_366E_4961_8330_99FECC1EA506

namespace open_cl {
    class Context {
      private:
        // get all platforms (drivers), e.g. NVIDIA
        static std::vector<cl::Platform> _all_platforms;
        
        std::vector<cl::Device> _all_devices;
        cl::Platform _selected_platform;
        cl::Device _selected_device;
        cl::Context* _context = nullptr;
        mutable cl::CommandQueue* _cmdQueue = nullptr;
      public:
        // Rule of six because c++ likes you to add a ton of code :)
        Context();
        ~Context();
        Context(const Context& toCopy);
        Context(Context&& toMove);
        Context& operator = (const Context& toCopy);
        Context& operator = (Context&& toMove);
        
        static Context autoGenerate();
        
        static size_t getPlatformCount();
        
        static std::vector<cl::Platform> getAllPlatforms();
        bool selectPlatformByName(const char* name);
        
        bool selectPlatformByName(const std::string name);
        
        void selectPlatform(size_t index);
        
        size_t getDeviceCount() const;
        
        std::vector<cl::Device> getAllDevices();
        
        void selectDevice(size_t index);
        
        bool selectDeviceByName(const char* name);
        
        bool selectDeviceByName(const std::string name);
        
        cl::Device getDevice() const;
        
        cl::Platform getPlatform() const;
        
        cl::Context& operator()() const;
        
        cl::CommandQueue& getContextQueue() const;
    };
}

#endif /* B8C6AC71_366E_4961_8330_99FECC1EA506 */
