# OpenCL Compiler

This repository is covering the research and development project made with Aarhus University, and is an experiment into using OpenCL to use hardware acceleration for compilation.

This repository is testing implementations in OpenCL, once with a usual use case for hardware acceleration - in this case drawing Mandlebrot fractals, and once with one segment of a compiler - the lexer.

The repository contains parts of the project, and how to navigate to project is explained here:

- The folder `CompilerLexer` contains the OpenCL lexer project. More info about it can be seen in the section [OpenCL Lexer](#opencl-lexer).
- The `Data` folder contains data collected while doing tests for the project. Both scripts and the raw data is very utilitarian and cluttered and is manly used for generating the tables and graphs shown in the report made in this project.
- In the `doc` folder is the plant UML source code used for generating the UML diagrams for this project.
- The Mandelbrot fractal generator created for this project is located in the folder `MandelbrotFractal`. More info about this project can be seen under the section [Mandelbrot fractal plotter](#mandelbrot-fractal-plotter).

## OpenCL Lexer

The lexer is as mentioned implemented in OpenCL. The host application is made in C++ and the build system used is CMake. The source code kan be found in the folder [CompilerLexer/src](CompilerLexer/src). There is some old source code for a partial frontend implemented early in the project under [CompilerLexer/oldSrc](CompilerLexer/oldSrc). The kernel implemented in OpenCL can be found at [CompilerLexer/src/kernels/lexer.cl](CompilerLexer/src/kernels/lexer.cl). A modified version of the kernel to run on the Xilinx Zynq UltraScale+ platform can be seen at [CompilerLexer/src/kernels/lexer_xilinx.cl](CompilerLexer/src/kernels/lexer_xilinx.cl) but is not used by the program by default.

### Prerequisites

Before building the project, a working C++ compiler, CMake and a working OpenCL runtime must be available. CMake must also be able to find OpenCL.

### Build the Project

Building the project is done using CMake where the `CMakeLists.txt` located at [CompilerLexer/CMakeLists.txt](CompilerLexer/CMakeLists.txt), and then building using the build system the project was configured with.

### Running the Executable

Due to how the program looks for the kernel. The program must be run from the same directory as the kernel file is placed. CMake will automatically place a copy of the kernel in the folder where the executable is placed when build.

## Mandelbrot fractal plotter

The Mandelbrot plotter is implemented in OpenCL and the host application in C++. The kernels implemented for the Mandelbrot plotter can be found in the file [MandelbrotFractal/kernels/mandelbrot.cl](MandelbrotFractal/kernels/mandelbrot.cl). The host source files is places directly in [MandelbrotFractal](MandelbrotFractal) and its subsequent folders.

### Prerequisites

Before building the project, a working C++ compiler, CMake and a working OpenCL runtime must be available. CMake must also be able to find OpenCL. In addition to this, this project also uses LibPNG to save the generated images. This also needs to be findable by CMake.

### Build the Project

Building the project is done using CMake where the `CMakeLists.txt` located at [MandelbrotFractal/CMakeLists.txt](MandelbrotFractal/CMakeLists.txt), and then building using the build system the project was configured with.

### Running the Executable

Due to how the program looks for the kernel. The program must be run from the same directory as the kernel file is placed. CMake will automatically place a copy of the kernel in the folder where the executable is placed when build.
