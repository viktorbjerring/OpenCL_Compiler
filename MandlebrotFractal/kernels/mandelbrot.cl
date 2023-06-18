#define LOG_2 0.69314718055994530941723212145818f

inline uchar3 interpolate(uchar3 c1, uchar3 c2, float ratio) {
  return (uchar3)(c1.s0 * (1 - ratio) + c2.s0 * ratio,
                  c1.s1 * (1 - ratio) + c2.s1 * ratio,
                  c1.s2 * (1 - ratio) + c2.s2 * ratio);
}

// inline uchar3 colorPalette(const float iter) {
//   float iter_norm = (iter);
//   if (iter_norm < 0.25f) {
//     return interpolate((uchar3)(0, 0, 0), (uchar3)(0, 125, 255),
//                        iter_norm * 4.0f);
//   } else if (iter_norm < 0.5f) {
//     return interpolate((uchar3)(0, 125, 255), (uchar3)(255, 255, 255),
//                        (iter_norm - 0.25f) * 4.0f);
//   } else if (iter_norm < 0.75f) {
//     return interpolate((uchar3)(255, 255, 255), (uchar3)(246, 146, 30),
//                        (iter_norm - 0.5f) * 4.0f);
//   } else if (iter_norm < 1.0f) {
//     return interpolate((uchar3)(246, 146, 30), (uchar3)(0, 0, 0),
//                        (iter_norm - 0.75f) * 4.0f);
//   } else {
//     return (uchar3)(0, 0, 0);
//   }
// }

inline uchar3 colorPaletteCrazy(const float iter) {
  float iter_norm = (iter);
  return interpolate((uchar3)(0, 0, 0), (uchar3)(0, 125, 255),
                     iter_norm * 4.0f) *
             (uchar)(iter_norm < 0.25f) +
         interpolate((uchar3)(0, 125, 255), (uchar3)(255, 255, 255),
                     (iter_norm - 0.25f) * 4.0f) *
             (uchar)(iter_norm < 0.5f) +
         interpolate((uchar3)(255, 255, 255), (uchar3)(246, 146, 30),
                     (iter_norm - 0.5f) * 4.0f) *
             (uchar)(iter_norm < 0.75f) +
         interpolate((uchar3)(246, 146, 30), (uchar3)(0, 0, 0),
                     (iter_norm - 0.75f) * 4.0f) *
             (uchar)(iter_norm < 1.0f);
}

inline uchar3 colorPalette(const float iter) {
  float iter_norm = (iter);
  return interpolate((uchar3)(0, 0, 0), (uchar3)(0, 125, 255),
                     iter_norm * 4.0f) *
             (uchar)(iter_norm < 0.25f) +
         interpolate((uchar3)(0, 125, 255), (uchar3)(255, 255, 255),
                     (iter_norm - 0.25f) * 4.0f) *
             (uchar)(iter_norm < 0.5f) * (uchar)(iter_norm >= 0.25f) +
         interpolate((uchar3)(255, 255, 255), (uchar3)(246, 146, 30),
                     (iter_norm - 0.5f) * 4.0f) *
             (uchar)(iter_norm < 0.75f) * (uchar)(iter_norm >= 0.5f) +
         interpolate((uchar3)(246, 146, 30), (uchar3)(0, 0, 0),
                     (iter_norm - 0.75f) * 4.0f) *
             (uchar)(iter_norm < 1.0f) * (uchar)(iter_norm >= 0.75f);
}

// Functionlaity for LCH color space.
#define EPSILON (216.0f / 24389.0f)
#define KAPPA (24389.0f / 27.0f)
#define DELTA (6.0f / 29.0f)

inline uchar gamma_compression(float linear) {
  float v;
  if (linear <= 0.00313066844250060782371) {
    v = 3294.6 * linear;
  } else {
    v = (pow(linear * 1.055f, 1.0f / 2.4f) - 0.055f) * 255.0f;
  };
  v = round(v);
  if (v > 255) {
    return 255;
  } else if (v < 0) {
    return 0;
  } else {
    return (int)v;
  }
}

inline float f_inv(float t) {
  if (t > DELTA) {
    return pow(t, 3.0f);
  } else {
    return 3 * DELTA * DELTA * DELTA * (t - (4.0f / 29.0f));
  }
}

inline uchar3 lch_to_rgb(const float3 lch) {
  // Convert to lab:
  float l = lch.s0;
  float a = lch.s1 * cos(lch.s2);
  float b = lch.s1 * sin(lch.s2);
  // convert to xyz
  float fy = (l + 16.0f) / 116.0f;
  float fx = (a / 500.0f) + fy;
  float fz = fy - (b / 200.0f);

  float x; // = f_inv(fx);
  float y; // = f_inv(fy);
  float z; // = f_inv(fz);
  if (pow(fx, 3.0f) > EPSILON) {
    x = pow(fx, 3.0f);
  } else {
    x = (116.0f * fx - 16.0f) / KAPPA;
  }
  if (l > KAPPA * EPSILON) {
    y = pow((l + 16.0f) / 116.0f, 3.0f);
  } else {
    y = l / KAPPA;
  }
  if (pow(fz, 3.0f) > EPSILON) {
    z = pow(fz, 3.0f);
  } else {
    z = (116.0f * fz - 16.0f) / KAPPA;
  }

  // Convert to rgb
  float sr = x * 3.2404542f + y * -1.5371385f + z * -0.4985314;
  float sg = x * -0.9692660f + y * 1.8760108f + z * 0.0415560f;
  float sb = x * 0.0556434f - y * -0.2040259f + z * 1.0572252f;

  return (uchar3)(gamma_compression(sr), gamma_compression(sg),
                  gamma_compression(sb));
}

inline uchar3 lchPalette(const float iter) {
  float v = 1.0f - pow(sin((float)M_PI * iter), 2.0f);
  float3 LCH = (float3)(75.0f - (75.0f * v), 28.0f + (75.0f - (75.0f * v)),
                        fmod(pow(360.0f * iter, 0.8f), 360));
  return lch_to_rgb(LCH);
}

__kernel void mandelbrot(const int width, const int height,
                         const int max_iterations, const float x_min,
                         const float x_max, const float y_min,
                         const float y_max, __global float *data_out) {

  // Get the index of the current element to be processed
  int i = get_global_id(0);
  int x_cord = i % width;
  int y_cord = i / width;
  float x0 = ((float)x_cord) / ((float)width) * (x_max - x_min) + x_min;
  float y0 = ((float)y_cord) / ((float)height) * (y_max - y_min) + y_min;
  float x = 0.0;
  float y = 0.0;
  float x2 = 0.0;
  float y2 = 0.0;
  long k = 0;
  for (; k < max_iterations && x2 + y2 < (1 << 16); k++) {
    y = 2 * x * y + y0;
    x = x2 - y2 + x0;
    x2 = x * x;
    y2 = y * y;
  }

  float k_f = (float)k;
  if (k_f < max_iterations) {
    float log_zn = log(x * x + y * y) / 2;
    float nu = log(log_zn / LOG_2) / LOG_2;
    k_f = k_f + 1 - nu;
  }
  data_out[i] = k_f;
}

__kernel void normalize_data(const int max_iterations, const float exponent,
                             __global float *data) {
  // Get the index of the current element to be processed
  int i = get_global_id(0);
  data[i] = pow(data[i] / max_iterations, exponent);
}

// This function processes a row in the image and creates a histogram over that
// rows data and places it acordingly in the histData array. Since openCL only
// supports 1D arrays offset is calculated manually.
__kernel void generate_row_histogram(const int max_iterations,
                                     const int row_length,
                                     __global const float *data,
                                     __global int *histData) {
  // Get the index of the current element to be processed
  int i = get_global_id(0);
  int row_start_idx = i * row_length;
  int hist_start_idx = i * max_iterations;
  for (int k = 0; k < row_length; ++k) {
    histData[hist_start_idx + (int)data[row_start_idx + k] - 1]++;
  }
}

// This function consolidates the ealier generated histograms and puts the sum
// of them in the first "row" of all the histograms, so index 0 to
// max_iterations-1 will have the complete histogram.
__kernel void consolidate_histogram(const int max_iterations, const int row_num,
                                    __global int *histData) {
  // Get the index of the current element to be processed
  int i = get_global_id(0);
  int k = i + max_iterations;
  for (; k < row_num * max_iterations; k += max_iterations) {
    histData[i] += histData[k];
  }
}

__kernel void apply_histogram_data(const int total, const float exponent,
                                   __global float *interData,
                                   __global int *histData) {
  // Get the index of the current element to be processed
  int i = get_global_id(0);
  int data = 0;
  // The hisogram needs to go from 0% to 100%.
  // Therefore things are added togeather in a for loop.
  for (int k = 0; k < (int)interData[i]; ++k) {
    data += histData[k];
  }

  // If data equals total, then just add the last color in the palette.
  if (data < total) {
    // To get the next coler we need to add the next history entry which is
    // histData[(int)interData[i]].
    // By getting the next color we can interpolate the two colers and get
    // smooth transitions.
    float decimal_part = interData[i] - (float)((int)interData[i]);
    interData[i] = pow(
        ((float)data + (float)(histData[(int)interData[i]]) * decimal_part) /
            total,
        exponent);
  } else {
    interData[i] = 1;
  }
}

__kernel void apply_basic_palette(__global const float *data_in,
                                  __global uchar *img_out) {
  // Get the index of the current element to be processed
  int i = get_global_id(0);
  uchar3 color = colorPalette(data_in[i]);
  img_out[i * 3] = color.s0;
  img_out[i * 3 + 1] = color.s1;
  img_out[i * 3 + 2] = color.s2;
}

__kernel void apply_lch_palette(__global const float *data_in,
                                __global uchar *img_out) {
  // Get the index of the current element to be processed
  int i = get_global_id(0);
  uchar3 color = lchPalette(data_in[i]);
  img_out[i * 3] = color.s0;
  img_out[i * 3 + 1] = color.s1;
  img_out[i * 3 + 2] = color.s2;
}
