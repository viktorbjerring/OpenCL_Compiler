

#ifdef DO_TEST
#include <vector>
#include <array>
#include <iostream>
#include <fstream>

#include "openCL/context.hpp"
#include "fractal/mandelbrotTest.hpp"

#ifndef GENERATE_TEST_DATA
    #include "TestData.hpp"
#endif

int main(int argc, char* argv[]) {
    // The image size;
    constexpr size_t img_size = 512;
    // Autogenerate the context for now.
    // My autogenerater is quite dum right now.
    // Just takes platform index 0, device index 0.
    std::cout << "Platform count:" << open_cl::Context::getPlatformCount() << "\nAvailable platforms:\n";
    
    for(auto plat : open_cl::Context::getAllPlatforms()) {
        auto info = plat.getInfo<CL_PLATFORM_NAME>();
        std::cout << info << "\n";
        std::vector<cl::Device> all_devices;
        plat.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
        
        for(auto device : all_devices) {
            auto info = device.getInfo<CL_DEVICE_NAME>();
            std::cout << info << "\n";
            std::vector<cl::Device> all_devices;
        }
    }
    
    // auto context = open_cl::Context::autoGenerate();
    open_cl::Context context;
    bool found = false;
    found = context.selectPlatformByName("NVIDIA CUDA");
    
    if(!found) {
        std::cerr << "Could not find the platform specified platform.\n";
        exit(1);
    }
    
    found = context.selectDeviceByName("NVIDIA GeForce RTX 3060 Laptop GPU");
    
    if(!found) {
        std::cerr << "Could not find the platform specified device.\n";
        exit(1);
    }
    
    // Generate the mandelbrot OpenCL program
    // and run it.
    auto mandelbrot = fractal::MandelbrotTest(context);
    mandelbrot.setImageSize(img_size);
    
    if(argc > 1) {
        mandelbrot.setWorkGroupSize(std::atoi(argv[1]));
    }
    
    // To only build part of the image
    // uncomment this.
    // mandelbrot.setBounds(-1.3825f, -0.765f, -0.56f, 0.0f);
    if(!mandelbrot()) {
        exit(1);
    }
    
    constexpr size_t end = img_size * img_size;
    constexpr size_t count = 100;
    auto data = mandelbrot.getDataContext();
#ifdef GENERATE_TEST_DATA
    std::cout << "constexpr float startTest[" << count << "] = {";
    
    for(size_t i = 0; i < count; ++i) {
        std::cout << data[i] << "f,\n";
    }
    
    std::cout << "};\n";
    std::cout << "constexpr float endTest[" << count << "] = {";
    
    for(size_t i = end - count; i < end; ++i) {
        std::cout << data[i] << "f,\n";
    }
    
    std::cout << "};\n";
#else
    
    for(size_t i = 0; i < count; ++i) {
        if((data[i] - startTest[i]) > 0.00001f || (data[i] - startTest[i]) < -0.00001f) {
            std::cerr << "Error in start test at index: " << i << ", data was: " << data[i] << ", startTest was: " << startTest[i] << "\n";
            exit(1);
        }
    }
    
    for(size_t i = 0; i < count; ++i) {
        if(data[end - count + i] -  endTest[i] > 0.00001f || data[end - count + i] -  endTest[i] < -0.00001f) {
            std::cerr << "Error in end test at index: " << i << ", data was: " << data[end - count + i] << ", endTest was: " << endTest[i] << "\n";
            exit(1);
        }
    }
    
    std::cout << "Test was successful!\n";
#endif
}

# else

#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include "png/png.hpp"

#include "openCL/context.hpp"
#include "fractal/mandelbrot.hpp"

int main(void) {
    // The image size;
    constexpr size_t img_size = 1024;
    // Autogenerate the context for now.
    // My autogenerater is quite dum right now.
    // Just takes platform index 0, device index 0.
    std::cout << "Platform count:" << open_cl::Context::getPlatformCount() << "\nAvailable platforms:\n";
    
    for(auto plat : open_cl::Context::getAllPlatforms()) {
        auto info = plat.getInfo<CL_PLATFORM_NAME>();
        std::cout << info << "\n";
        std::vector<cl::Device> all_devices;
        plat.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
        
        for(auto device : all_devices) {
            auto info = device.getInfo<CL_DEVICE_NAME>();
            std::cout << info << "\n";
            std::vector<cl::Device> all_devices;
        }
    }
    
    auto context = open_cl::Context::autoGenerate();
    // Generate the mandelbrot OpenCL program
    // and run it.
    auto mandelbrot = fractal::Mandelbrot(context);
    mandelbrot.setImageSize(img_size);
    mandelbrot.setColorPalette(fractal::ColorPalette::lch);
    
    // To only build part of the image
    // uncomment this.
    // mandelbrot.setBounds(-1.3825f, -0.765f, -0.56f, 0.0f);
    if(!mandelbrot(true)) {
        exit(1);
    }
    
    std::cout << "Image generation done. Saving image...\n";
    // Save the mandelbrot plot as an image.
    png::PNG img;
    img.setPictureProperties(img_size, img_size, png::ColorType::rgb, 8);
    png::RGBData<uint8_t>* data_ptr = static_cast<png::RGBData<uint8_t>*>(img.getPixelData());
    data_ptr->setDataContext(mandelbrot.getDataContext());
    img.save("test.png");
    return 0;
}
#endif