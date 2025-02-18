# Ray Tracing Project

本项目基于 [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) 系列书籍的原理与实现，利用 C++ 实现光线追踪算法，生成具有真实感的渲染效果。

## 项目结构

- **bin/**  
  存放编译生成的可执行文件以及项目运行过程中产生的 PPM 图片效果。  
  例如，通过运行 `RayTracingInOneWeekend.exe > image.ppm` 即可生成 PPM 文件。

- **include/**  
  存放项目所用到的所有头文件（`.h` 或 `.hpp`），用于声明各个类、函数和常量。

- **src/**  
  存放项目的 C++ 源代码文件（`.cpp`），包含光线追踪核心算法、场景构建、材质、光照等实现。

- **CMakeLists.txt**  
  已经配置好项目的构建系统，使用 CMake 进行编译。只需通过 CMake 配置和生成对应平台的工程即可。

## 编译与使用

1. **编译项目**  
   在项目根目录下，使用以下命令进行构建（需已安装 CMake）：
   ```bash
   mkdir build
   cd build
   cmake ..
   ```
   构建成功后，生成的可执行文件会放在`bin/` 目录下
 2. **生成渲染图片**
    进入`bin/`目录，在命令行执行：
    ```bash
    RayTracingInOneWeekend.exe > image.ppm
    ```
    这样会将程序输出的 PPM 格式图片重定向到`image.ppm`文件中。你可以使用支持 PPM 格式的图片查看器打开该文件查看渲染效果。

## 项目说明

- 本项目使用 C++17 标准实现光线追踪，核心算法和数据结构参考了 *Ray Tracing in One Weekend* 系列书籍。
- 项目中使用 CMake 作为构建系统，可跨平台编译。
- PPM 文件格式采用纯文本格式（ASCII），方便调试和后续处理。

## 参考资料

- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
