#include"../openCL/context.hpp"
#include"../openCL/program.hpp"

#ifndef D7DB11BE_85B5_429F_AE2D_CDC86781A449
#define D7DB11BE_85B5_429F_AE2D_CDC86781A449
namespace fractal {
    enum class ColorPalette {
        basic,
        lch,
    };
    class Mandelbrot {
      private:
        open_cl::Context _context;
        open_cl::Program _prog;
        cl::Kernel _mandelbrot;
        cl::Kernel _apply_basic_palette;
        cl::Kernel _apply_lch_palette;
        cl::Kernel _normalize_data;
        cl::Kernel _generate_row_histogram;
        cl::Kernel _consolidate_histogram;
        cl::Kernel _apply_histogram_data;
        float _x_min = -2.00f;
        float _x_max = 0.47f;
        float _y_min = -1.12f;
        float _y_max = 1.12f;
        float _exponent = 1.5f;
        int _max_iterations = 5000;
        ColorPalette _palette = ColorPalette::basic;
        size_t _data_count;
        int _img_size;
        uint8_t* _img_data;
      public:
        Mandelbrot() = default;
        
        Mandelbrot(open_cl::Context& context) {
            setContext(context);
        }
        ~Mandelbrot() {
            delete [] _img_data;
        }
        
        inline void setContext(open_cl::Context& context) {
            _context = context;
            _prog = open_cl::Program(_context);
            _prog.open("mandelbrot.cl");
            _mandelbrot = _prog("mandelbrot");
            _normalize_data = _prog("normalize_data");
            _apply_basic_palette = _prog("apply_basic_palette");
            _apply_lch_palette = _prog("apply_lch_palette");
            _generate_row_histogram = _prog("generate_row_histogram");
            _consolidate_histogram = _prog("consolidate_histogram");
            _apply_histogram_data = _prog("apply_histogram_data");
        }
        
        void setExponent(float exponent) {
            _exponent = exponent;
        }
        
        float getExponent() const {
            return _exponent;
        }
        
        void setColorPalette(ColorPalette palette) {
            _palette = palette;
        }
        
        ColorPalette getColorPalette() const {
            return _palette;
        }
        
        void setImageSize(const int size) {
            _img_size = size;
            _data_count = _img_size * _img_size * 3;
            _img_data = new uint8_t[_data_count];
        }
        
        int getImageSize() const {
            return _img_size;
        }
        
        size_t getDataSize() const {
            return _data_count;
        }
        
        uint8_t* getDataContext() const {
            return _img_data;
        }
        
        void setBounds(float x_min, float x_max, float y_min, float y_max) {
            _x_min = x_min;
            _x_max = x_max;
            _y_min = y_min;
            _y_max = y_max;
        }
        
        bool operator()(bool useHistogram = true) {
            cl::Buffer img_mem_obj(_context(), CL_MEM_WRITE_ONLY, _data_count * sizeof(uint8_t));
            cl::Buffer data_mem_obj(_context(), CL_MEM_READ_WRITE, _img_size * _img_size * sizeof(float));
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
            auto ret = _context.getContextQueue().enqueueNDRangeKernel(_mandelbrot, cl::NullRange, cl::NDRange(_img_size * _img_size));
            
            if(ret) {
                std::cerr << "Error in queue at mandelbrot kernel: " << ret << std::endl;
                return false;
            }
            
            if(!useHistogram) {
                _normalize_data.setArg(0, _max_iterations);
                _normalize_data.setArg(1, _exponent);
                _normalize_data.setArg(2, data_mem_obj);
                ret = _context.getContextQueue().enqueueNDRangeKernel(_normalize_data, cl::NullRange, cl::NDRange(_img_size * _img_size));
                
                if(ret) {
                    std::cerr << "Error in queue at normalize kernel: " << ret << std::endl;
                    return false;
                }
            } else {
                cl::Buffer hist_mem_obj(_context(), CL_MEM_READ_WRITE, _img_size * _max_iterations * sizeof(int));
                _generate_row_histogram.setArg(0, _max_iterations);
                _generate_row_histogram.setArg(1, _img_size);
                _generate_row_histogram.setArg(2, data_mem_obj);
                _generate_row_histogram.setArg(3, hist_mem_obj);
                ret = _context.getContextQueue().enqueueNDRangeKernel(_generate_row_histogram, cl::NullRange, cl::NDRange(_img_size));
                
                if(ret) {
                    std::cerr << "Error in queue at generate row kernel: " << ret << std::endl;
                    return false;
                }
                
                _consolidate_histogram.setArg(0, _max_iterations);
                _consolidate_histogram.setArg(1, _img_size);
                _consolidate_histogram.setArg(2, hist_mem_obj);
                ret = _context.getContextQueue().enqueueNDRangeKernel(_consolidate_histogram, cl::NullRange, cl::NDRange(_max_iterations));
                
                if(ret) {
                    std::cerr << "Error in queue at consolidate kernel: " << ret << std::endl;
                    return false;
                }
                
                _apply_histogram_data.setArg(0, _img_size * _img_size);
                _apply_histogram_data.setArg(1, _exponent);
                _apply_histogram_data.setArg(2, data_mem_obj);
                _apply_histogram_data.setArg(3, hist_mem_obj);
                // read result from GPU to here
                ret = _context.getContextQueue().enqueueNDRangeKernel(_apply_histogram_data, cl::NullRange, cl::NDRange(_img_size * _img_size));
                
                if(ret) {
                    std::cerr << "Error in queue at apply hist kernel: " << ret << std::endl;
                    return false;
                }
            }
            
            switch(_palette) {
                case ColorPalette::basic: {
                        // Apply the basic color pallette for now.
                        _apply_basic_palette.setArg(0, data_mem_obj);
                        _apply_basic_palette.setArg(1, img_mem_obj);
                        ret = _context.getContextQueue().enqueueNDRangeKernel(_apply_basic_palette, cl::NullRange, cl::NDRange(_img_size * _img_size));
                        
                        if(ret) {
                            std::cerr << "Error in queue at color kernel: " << ret << std::endl;
                            return false;
                        }
                    }
                    break;
                    
                case ColorPalette::lch: {
                        // Apply the basic color pallette for now.
                        _apply_lch_palette.setArg(0, data_mem_obj);
                        _apply_lch_palette.setArg(1, img_mem_obj);
                        ret = _context.getContextQueue().enqueueNDRangeKernel(_apply_lch_palette, cl::NullRange, cl::NDRange(_img_size * _img_size));
                        
                        if(ret) {
                            std::cerr << "Error in queue at color kernel: " << ret << std::endl;
                            return false;
                        }
                    }
                    break;
            }
            
            // read result from GPU to here
            ret = _context.getContextQueue().enqueueReadBuffer(img_mem_obj, CL_TRUE, 0, _data_count * sizeof(uint8_t), _img_data);
            
            if(ret) {
                std::cerr << "Error in queue at read: " << ret << std::endl;
                return false;
            }
            
            ret = _context.getContextQueue().finish();
            
            if(ret) {
                std::cerr << "Error in queue at finish: " << ret << std::endl;
                return false;
            }
            
            return true;
        }
    };
}

#endif /* D7DB11BE_85B5_429F_AE2D_CDC86781A449 */
