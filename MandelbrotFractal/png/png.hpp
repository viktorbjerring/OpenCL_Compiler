#include <png.h>
#include <string>

#ifndef D8C9964B_4002_4E8C_B76F_B947E5AB4A18
#define D8C9964B_4002_4E8C_B76F_B947E5AB4A18
namespace png {
    using pixelDataPtr_t = void*;
    enum class ColorType {
        gray = PNG_COLOR_TYPE_GRAY,
        palette = PNG_COLOR_TYPE_PALETTE,
        rgb = PNG_COLOR_TYPE_RGB,
        rgb_alpha = PNG_COLOR_TYPE_RGB_ALPHA,
        gray_alpha = PNG_COLOR_TYPE_GRAY_ALPHA,
    };
    class PixelData {
      protected:
        png_byte** _pixels;
        size_t _width = 0;
        size_t _height = 0;
      public:
        virtual const ColorType GetColorType() const = 0;
        virtual int GetBitDepth() const = 0;
        virtual png_byte** GetAllPixels() {
            return _pixels;
        }
        virtual void setDimensions(size_t width, size_t height) = 0;
        virtual size_t getWidth() const {
            return _width;
        }
        virtual size_t getHeight() const {
            return _height;
        }
        virtual void setDataContext(void* origBuffer) = 0;
    };
    template <class T>
    class RGBData : public PixelData {
      private:
        T** _real_pixels;
        
      public:
        ~RGBData() {
            for(int i = 0; i < _height; i++) {
                delete [] _real_pixels[i];
            }
            
            delete [] _real_pixels;
        }
        virtual const ColorType GetColorType() const override {
            return ColorType::rgb;
        }
        virtual int GetBitDepth() const override {
            return sizeof(T) * 8;
        }
        void setDimensions(size_t width, size_t height) override {
            _height = height;
            _width = width;
            _real_pixels = new T*[_height];
            
            for(int i = 0; i < _height; i++) {
                _real_pixels[i] = new T[_width * 3];
            }
            
            _pixels = reinterpret_cast<png_byte**>(_real_pixels);
        }
        virtual void setDataContext(void* origBuffer) {
            T* origBufferPtr = reinterpret_cast<T*>(origBuffer);
            _real_pixels = new T*[_height];
            
            for(int i = 0; i < _height; i++) {
                _real_pixels[i] = &(origBufferPtr[_width * 3 * i]);
            }
            
            _pixels = reinterpret_cast<png_byte**>(_real_pixels);
        }
        T R(size_t indexX, size_t indexY) const {
            return _real_pixels[indexY][indexX * 3]
        }
        void R(size_t indexX, size_t indexY, T value) {
            _real_pixels[indexY][indexX * 3] = value;
        }
        T G(size_t indexX, size_t indexY) const {
            return _real_pixels[indexY][indexX * 3 + 1]
        }
        void G(size_t indexX, size_t indexY, T value) {
            _real_pixels[indexY][indexX * 3 + 1] = value;
        }
        T B(size_t indexX, size_t indexY) const {
            return _real_pixels[indexY][indexX * 3 + 2]
        }
        void B(size_t indexX, size_t indexY, T value) {
            _real_pixels[indexY][indexX * 3 + 2] = value;
        }
    };
    class PNG {
      private:
        PixelData* _pixelData = nullptr;
      public:
        PNG() = default;
        ~PNG() {
            if(_pixelData != nullptr) {
                delete _pixelData;
            }
        }
        PixelData* getPixelData() const {
            return _pixelData;
        }
        bool setPictureProperties(size_t width, size_t height, ColorType ct, int bitDepth) {
            switch(ct) {
                case ColorType::rgb:
                    if(bitDepth == 8) {
                        _pixelData = new RGBData<uint8_t>();
                        _pixelData->setDimensions(width, height);
                        return true;
                    } else if(bitDepth == 16) {
                        _pixelData = new RGBData<uint16_t>();
                        _pixelData->setDimensions(width, height);
                        return true;
                    } else {
                        return false;
                    }
                    
                default:
                    return false;
            }
            
            return true;
        }
        bool save(std::string path) {
            FILE* fp;
            png_structp png_ptr = NULL;
            png_infop info_ptr = NULL;
            int status = -1;
            fp = fopen(path.c_str(), "wb");
            
            if(! fp) {
                return false;
            }
            
            png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            
            if(png_ptr == NULL) {
                fclose(fp);
                return false;
            }
            
            info_ptr = png_create_info_struct(png_ptr);
            
            if(info_ptr == NULL) {
                png_destroy_write_struct(&png_ptr, &info_ptr);
                fclose(fp);
            }
            
            if(setjmp(png_jmpbuf(png_ptr))) {
                png_destroy_write_struct(&png_ptr, &info_ptr);
                fclose(fp);
            }
            
            png_set_IHDR(png_ptr,
                         info_ptr,
                         static_cast<png_uint_32>(_pixelData->getWidth()),
                         static_cast<png_uint_32>(_pixelData->getHeight()),
                         _pixelData->GetBitDepth(),
                         static_cast<int>(_pixelData->GetColorType()),
                         PNG_INTERLACE_NONE,
                         PNG_COMPRESSION_TYPE_DEFAULT,
                         PNG_FILTER_TYPE_DEFAULT);
            png_init_io(png_ptr, fp);
            png_set_rows(png_ptr, info_ptr, _pixelData->GetAllPixels());
            png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
            return true;
        }
        
    };
}

#endif /* D8C9964B_4002_4E8C_B76F_B947E5AB4A18 */
