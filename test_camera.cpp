
// THIS CODE IS UNTESTED

#include <iostream>

// for delay function.
#include <chrono>
#include <map>
#include <string>
#include <thread>

#include "rhobot.h"

#include <jetson-inference/depthNet.h>

// load mono depth network
depthNet* net = depthNet::Create();

// depthNet re-uses the same memory for the depth field,
// so you only need to do this once (not every frame)
float* depth_field = net->GetDepthField();
const int depth_width = net->GetDepthWidth();
const int depth_height = net->GetDepthHeight();

// delay function
inline void delay(double s) { std::this_thread::sleep_for(std::chrono::duration<double>(s)); }

int main()
{
    
    std::cout << "Camera test running." << std::endl;

    //RhoBot rhobot;

    //rhobot.start();

    uchar3* img = NUL;
    input->Capture(&img);  // assumes you have created an input videoSource stream
    net->Process(img, input->GetWidth(), input->GetHeight());

    // wait for the GPU to finish processing
    CUDA(cudaDeviceSynchronize()); 
	
    // you can now safely access depth_field from the CPU (or GPU)
    for( int y=0; y < depth_height; y++ )
        for( int x=0; x < depth_width; x++ )
	       printf("depth x=%i y=%i -> %f\n", x, y, depth_map[y * depth_width + x]);

}