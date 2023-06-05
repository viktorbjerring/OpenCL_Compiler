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
        std::vector<cl::Platform> _all_platforms;
        
        std::vector<cl::Device> _all_devices;
        cl::Platform _selected_platform;
        cl::Device _selected_device;
        cl::Context* _context = nullptr;
        mutable cl::CommandQueue* _cmdQueue = nullptr;
      public:
        // Rule of six because c++ likes you to add a ton of code :)
        Context() {
            cl::Platform::get(&_all_platforms);
        }
        ~Context() {
            if(_context != nullptr) {
                delete _context;
            }
            
            if(_cmdQueue != nullptr) {
                delete _cmdQueue;
            }
        }
        Context(const Context& toCopy) {
            _all_platforms = toCopy._all_platforms;
            _selected_platform = toCopy._selected_platform;
            _all_devices = toCopy._all_devices;
            _selected_device = toCopy._selected_device;
            
            if(toCopy._context != nullptr) {
                _context = new cl::Context();
                *_context = *toCopy._context;
            }
        }
        Context(Context&& toMove) {
            _all_platforms = toMove._all_platforms;
            _selected_platform = toMove._selected_platform;
            _all_devices = toMove._all_devices;
            _selected_device = toMove._selected_device;
            _context = toMove._context;
            toMove._context = nullptr;
            _cmdQueue = toMove._cmdQueue;
            toMove._cmdQueue = nullptr;
        }
        
        Context& operator = (const Context& toCopy) {
            _all_platforms = toCopy._all_platforms;
            _selected_platform = toCopy._selected_platform;
            _all_devices = toCopy._all_devices;
            _selected_device = toCopy._selected_device;
            
            if(toCopy._context != nullptr) {
                _context = new cl::Context();
                *_context = *toCopy._context;
            }
            
            return *this;
        }
        
        
        Context& operator = (Context&& toMove) {
            _all_platforms = toMove._all_platforms;
            _selected_platform = toMove._selected_platform;
            _all_devices = toMove._all_devices;
            _selected_device = toMove._selected_device;
            _context = toMove._context;
            toMove._context = nullptr;
            _cmdQueue = toMove._cmdQueue;
            toMove._cmdQueue = nullptr;
            return *this;
        }
        
        
        static Context autoGenerate() {
            Context toReturn;
            
            if(toReturn.getPlatformCount() == 0) {
                std::cerr << " No platforms found. Check OpenCL installation!\n";
                return toReturn;
            }
            
            toReturn.selectPlatform(0);
            std::cout << "Using platform: " << toReturn._selected_platform.getInfo<CL_PLATFORM_NAME>() << "\n";
            
            if(toReturn.getDeviceCount() == 0) {
                std::cerr << " No devices found. Check OpenCL installation!\n";
                return toReturn;
            }
            
            toReturn.selectDevice(0);
            std::cout << "Using device: " << toReturn._selected_device.getInfo<CL_DEVICE_NAME>() << "\n";
            return toReturn;
        }
        
        size_t getPlatformCount() const {
            return _all_platforms.size();
        }
        
        void selectPlatform(size_t index) {
            _selected_platform = _all_platforms[index];
            // get default device (CPUs, GPUs) of the default platform
            _selected_platform.getDevices(CL_DEVICE_TYPE_ALL, &_all_devices);
        }
        
        size_t getDeviceCount() const {
            return _all_devices.size();
        }
        
        
        void selectDevice(size_t index) {
            _selected_device = _all_devices[index];
            _context = new cl::Context({_selected_device});
        }
        
        cl::Device getDevice() const {
            return _selected_device;
        }
        
        cl::Platform getPlatform() const {
            return _selected_platform;
        }
        
        cl::Context& operator()() const {
            return *_context;
        }
        
        cl::CommandQueue& getContextQueue() const {
            if(_cmdQueue == nullptr) {
                _cmdQueue = new cl::CommandQueue(*_context, _selected_device);
            }
            
            return *_cmdQueue;
        }
    };
}

#endif /* B8C6AC71_366E_4961_8330_99FECC1EA506 */
