#include"../openCL/context.hpp"
#include"../openCL/program.hpp"

#ifndef E77EFBD5_9EBA_4AAD_8CC2_D5B3F2780000
#define E77EFBD5_9EBA_4AAD_8CC2_D5B3F2780000

namespace fractal {
    class MandelbrotTest {
      private:
        open_cl::Context _context;
        open_cl::Program _prog;
        cl::Kernel _mandelbrot;
        float _x_min = -2.00f;
        float _x_max = 0.47f;
        float _y_min = -1.12f;
        float _y_max = 1.12f;
        float _exponent = 1.5f;
        int _max_iterations = 5000;
        int _img_size;
        cl::NDRange work_group_size = cl::NullRange;
        int _local_work_size;
        float* _img_data;
      public:
        MandelbrotTest() = default;
        
        MandelbrotTest(open_cl::Context& context) {
            setContext(context);
        }
        ~MandelbrotTest() {
            delete [] _img_data;
        }
        
        void setWorkGroupSize(int size) {
            if(size <= 0) {
                work_group_size = cl::NullRange;
            } else {
                work_group_size = cl::NDRange(size);
            }
        }
        
        inline void setContext(open_cl::Context& context) {
            _context = context;
            _prog = open_cl::Program(_context);
            _prog.open("mandelbrot.cl");
            _mandelbrot = _prog("mandelbrot");
        }
        
        void setExponent(float exponent) {
            _exponent = exponent;
        }
        
        float getExponent() const {
            return _exponent;
        }
        
        void setImageSize(const int size) {
            _img_size = size;
            _img_data = new float[ _img_size * _img_size * sizeof(float)];
        }
        
        int getImageSize() const {
            return _img_size;
        }
        
        float* getDataContext() const {
            return _img_data;
        }
        
        void setBounds(float x_min, float x_max, float y_min, float y_max) {
            _x_min = x_min;
            _x_max = x_max;
            _y_min = y_min;
            _y_max = y_max;
        }
        
        bool operator()(bool useHistogram = true) {
            cl::Buffer data_mem_obj(_context(), CL_MEM_WRITE_ONLY, _img_size * _img_size * sizeof(float));
            // // push write commands to queue
            // queue.enqueueWriteBuffer(a_mem_obj, CL_TRUE, 0, list_size * sizeof(int), A.data());
            // queue.enqueueWriteBuffer(b_mem_obj, CL_TRUE, 0, list_size * sizeof(int), B.data());
            // RUN ZE KERNEL
            // Set mandelbrot args
            _mandelbrot.setArg(0, _img_size);
            _mandelbrot.setArg(1, _img_size);
            _mandelbrot.setArg(2, _max_iterations);
            _mandelbrot.setArg(3, _x_min);
            _mandelbrot.setArg(4, _x_max);
            _mandelbrot.setArg(5, _y_min);
            _mandelbrot.setArg(6, _y_max);
            _mandelbrot.setArg(7, data_mem_obj);
            cl::Event evt;
            auto ret = _context.getContextQueue().enqueueNDRangeKernel(_mandelbrot, cl::NullRange, cl::NDRange((_img_size * _img_size)), work_group_size, NULL, &evt);
            evt.wait();
            
            if(ret) {
                std::cerr << "Error in queue at mandelbrot kernel: " << ret << std::endl;
                return false;
            }
            
            // read result from GPU to here
            ret = _context.getContextQueue().enqueueReadBuffer(data_mem_obj, CL_TRUE, 0, _img_size * _img_size * sizeof(float), _img_data);
            
            if(ret) {
                std::cerr << "Error in queue at read: " << ret << std::endl;
                return false;
            }
            
            ret = _context.getContextQueue().finish();
            
            if(ret) {
                std::cerr << "Error in queue at finish: " << ret << std::endl;
                return false;
            }
            
            auto start = evt.getProfilingInfo<CL_PROFILING_COMMAND_START>();
            auto end = evt.getProfilingInfo<CL_PROFILING_COMMAND_END>();
            auto nanosTotal = end - start;
            auto secondsTotal = nanosTotal / 1000000000;
            auto minutes = secondsTotal / 60;
            auto seconds = secondsTotal % 60;
            auto millis = (nanosTotal % 1000000000) / 1000000;
            auto micros = (nanosTotal % 1000000) / 1000;
            auto nanos = nanosTotal % 1000;
            std::cout << "Mandelbrot ran in: " << minutes << " minutes, " << seconds << " seconds, " << millis << " milliseconds, " << micros << " microseconds, " << nanos << " nanoseconds\n";
            std::cout << "Total nanoseconds:\n" << nanosTotal << "\n";
            return true;
        }
    };
}


#endif /* E77EFBD5_9EBA_4AAD_8CC2_D5B3F2780000 */
