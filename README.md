# Kijang

## Tested Compilers

- GCC 64bit
- MSVC 2019 (Windows SDK 10.0.19041.0)

## Libraries used

### GPL v2 License

- [libfreenect](https://github.com/OpenKinect/libfreenect) v0.6.2
- [libfreenect2](https://github.com/OpenKinect/libfreenect2) v0.2.1

## Others

- [Kinect for Windows v1.8](https://www.microsoft.com/en-us/download/details.aspx?id=40278)
- [Kinect for Windows v2.0](https://www.microsoft.com/en-sg/download/details.aspx?id=44561)

## Compilation Instructions

### Windows

1. Install [Kinect for Windows v1.8](https://www.microsoft.com/en-us/download/details.aspx?id=40278) and [Kinect for Windows v2.0](https://www.microsoft.com/en-sg/download/details.aspx?id=44561).
1. Create a 'libs' folder in the project root directory.

### Linux

1. Create a 'libs' folder in the project root directory.
2. Find and download the corresponding libfreenect libraries and libfreenect2 libraries from [libfreenect-libs](https://github.com/paxriel/libfreenect-libs).
3. Extract the files in the two libraries into libs/libfreenect and libs/libfreenect2 respectively.
