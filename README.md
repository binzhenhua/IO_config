# IO_config
C++ use iostream to read and write config file in  Linux in Singleton
This is a fork from sakishum "Cppconfig".
I make it to a static library and make it thread safe in Singleton.

## About the name

The original name of this program was "C++ read config", it was later changed to "Cppconfig" by sakishum.

I change the Name to "IOConfig", because it use iostream to read and write config file.

## Compiling

Any C++98/C++11 compiler should work. If your compiler has the C++98/C++11 features that are available in Visual Studio 2010 then it will work. 

There are multiple compilation choices:
* g++ 4.4 (and above)
* clang++ 2.9 (and above)

## Example

You can make a static lib in the project directory.
Use IOConfig as a static lib as the example in example directory.
