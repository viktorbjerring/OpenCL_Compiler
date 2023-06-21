#include "context.hpp"

namespace open_cl {
    std::vector<cl::Platform> Context::_all_platforms;
    
    Context::Context() {
        cl::Platform::get(&_all_platforms);
    }
    
    Context::~Context() {
        if(_context != nullptr) {
            delete _context;
        }
        
        if(_cmdQueue != nullptr) {
            delete _cmdQueue;
        }
    }
    Context::Context(const Context& toCopy) {
        _selected_platform = toCopy._selected_platform;
        _all_devices = toCopy._all_devices;
        _selected_device = toCopy._selected_device;
        
        if(toCopy._context != nullptr) {
            _context = new cl::Context();
            *_context = *toCopy._context;
        }
    }
    Context::Context(Context&& toMove) {
        _selected_platform = toMove._selected_platform;
        _all_devices = toMove._all_devices;
        _selected_device = toMove._selected_device;
        _context = toMove._context;
        toMove._context = nullptr;
        _cmdQueue = toMove._cmdQueue;
        toMove._cmdQueue = nullptr;
    }
    
    Context& Context::operator = (const Context& toCopy) {
        _selected_platform = toCopy._selected_platform;
        _all_devices = toCopy._all_devices;
        _selected_device = toCopy._selected_device;
        
        if(toCopy._context != nullptr) {
            _context = new cl::Context();
            *_context = *toCopy._context;
        }
        
        return *this;
    }
    
    
    Context& Context::operator = (Context&& toMove) {
        _selected_platform = toMove._selected_platform;
        _all_devices = toMove._all_devices;
        _selected_device = toMove._selected_device;
        _context = toMove._context;
        toMove._context = nullptr;
        _cmdQueue = toMove._cmdQueue;
        toMove._cmdQueue = nullptr;
        return *this;
    }
    
    
    Context Context::autoGenerate() {
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
    
    size_t Context::getPlatformCount() {
        cl::Platform::get(&_all_platforms);
        return _all_platforms.size();
    }
    
    std::vector<cl::Platform> Context::getAllPlatforms() {
        cl::Platform::get(&_all_platforms);
        return _all_platforms;
    }
    
    void Context::selectPlatform(size_t index) {
        _selected_platform = _all_platforms[index];
        // get default device (CPUs, GPUs) of the default platform
        _selected_platform.getDevices(CL_DEVICE_TYPE_ALL, &_all_devices);
    }
    
    bool Context::selectPlatformByName(const char* name) {
        return selectPlatformByName(std::string(name));
    }
    
    bool Context::selectPlatformByName(const std::string name) {
        bool found = false;
        
        for(auto plat : _all_platforms) {
            if(plat.getInfo<CL_PLATFORM_NAME>() == name) {
                _selected_platform = plat;
                // get default device (CPUs, GPUs) of the default platform
                _selected_platform.getDevices(CL_DEVICE_TYPE_ALL, &_all_devices);
                found = true;
                break;
            }
        }
        
        return found;
    }
    
    size_t Context::getDeviceCount() const {
        return _all_devices.size();
    }
    
    
    void Context::selectDevice(size_t index) {
        _selected_device = _all_devices[index];
        _context = new cl::Context({_selected_device});
    }
    
    std::vector<cl::Device> Context::getAllDevices() {
        return _all_devices;
    }
    
    bool Context::selectDeviceByName(const char* name) {
        return selectDeviceByName(std::string(name));
    }
    
    bool Context::selectDeviceByName(const std::string name) {
        bool found = false;
        
        for(auto device : _all_devices) {
            if(device.getInfo<CL_DEVICE_NAME>() == name) {
                _selected_device = device;
                _context = new cl::Context({_selected_device});
                found = true;
                break;
            }
        }
        
        return found;
    }
    
    cl::Device Context::getDevice() const {
        return _selected_device;
    }
    
    cl::Platform Context::getPlatform() const {
        return _selected_platform;
    }
    
    cl::Context& Context::operator()() const {
        return *_context;
    }
    
    cl::CommandQueue& Context::getContextQueue() const {
        if(_cmdQueue == nullptr) {
            cl_queue_properties _queue_properties[] = {CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0};
            cl_int err;
            _cmdQueue = new cl::CommandQueue(*_context, _selected_device, _queue_properties, &err);
        }
        
        return *_cmdQueue;
    }
}