#include "ExecutionDevice.hpp"
using namespace fast;

cl::CommandQueue OpenCLDevice::getCommandQueue() {
    return getQueue(0);
}

cl::Device OpenCLDevice::getDevice() {
    return Context::getDevice(0);
}
