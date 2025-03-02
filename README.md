# DDS

### Install dependencies
* CMake >= 3.17
* Boost
* OpenSSL
```
$ sudo apt-get install make cmake yasm nasm pkg-config libssl-dev libboost-all-dev libx264-dev libavdevice-dev g++
```

* [FFMPEG](https://git.ffmpeg.org/ffmpeg.git) build 5.1 or later with at least libx264 enabled

  * Working solution is to build from source. Follow the official [guide](https://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu).
  * Configure ffmpeg with:
  
     ``` $ ./configure  --ld="g++" --enable-gpl --disable-programs --disable-static --enable-shared --enable-libx264```

* libopencv
```
$ sudo apt install libopencv-dev
```

* [websocketpp](https://github.com/zaphoyd/websocketpp)
```
$ git clone git@github.com:zaphoyd/websocketpp.git
$ cd websocketpp
$ mkdir build
$ cd build
$ cmake ..
$ sudo make install
```

### Building
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```
---
Rtmp module inspired by [cpp_media_server-1](https://github.com/grandi23/cpp_media_server-1) which is licensed under [MIT](https://opensource.org/licenses/MIT). Copyright (c) 2021 Alex.CR

The class contains a copy of [json](https://github.com/nlohmann/json) from Niels Lohmann which is licensed under [MIT](https://opensource.org/licenses/MIT). Copyright (c) 2013-2022 Niels Lohmann
