#include <stdio.h>
#include <OpenCL/OpenCL.h>
#include <time.h>
#include <string>
#include <fstream>

#define MALLOC_COUNT(A) (malloc_count++, malloc(A))
#define FREE_COUNT(A)   (malloc_count--, free(A), A = NULL)
#define BUF_SIZE 2

#define DEBUG 1

static int malloc_count = 0;
// wati event complete
int waitForEventComplete(cl_event *event)
{
    cl_int status = CL_SUCCESS;
    cl_int eventStatus = CL_QUEUED;
    while(eventStatus != CL_COMPLETE)
    {
        status = clGetEventInfo(*event, CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(cl_int), &eventStatus, NULL);
    }
    status = clReleaseEvent(*event);
    return 0;
}

int convertToString(const char* filename, std::string &s)
{
    size_t size;
    char*  str;

    std::fstream f(filename, (std::fstream::in | std::fstream::binary));
    if(!f.is_open())
    {
        printf(" open file %s error\n", filename);
        return -1;
    }

    size_t fileSize;
    f.seekg(0, std::fstream::end);
    size = fileSize = (size_t)f.tellg();
    f.seekg(0, std::fstream::beg);

    str = new char[size + 1];
    if(!str)
    {
        f.close();
        return -1;
    }

    f.read(str, fileSize);
    f.close();
    str[size] = '\0';

    s = str;
    delete[] str;
    return 0;
}
int main()
{
    // create thress buffers in host memory
    float *buf1 = 0, *buf2 = 0, *buf = 0;
    buf1 = (float *)MALLOC_COUNT(BUF_SIZE * sizeof(float));
    buf2 = (float *)MALLOC_COUNT(BUF_SIZE * sizeof(float));
    buf = (float *)MALLOC_COUNT(BUF_SIZE * sizeof(float));

    int i;
    srand((unsigned) time(NULL));
    for(i = 0; i < BUF_SIZE; i++)
        buf1[i] = rand() % 65535;

    srand((unsigned) time(NULL) + 1000);
    for(i = 0; i < BUF_SIZE; i++)
        buf2[i] = rand() % 65535;

    for(i = 0; i < BUF_SIZE; i++)
        buf[i] = buf1[i] + buf2[i];

#if DEBUG
    printf(" buf head is %f\n", buf[0]);
#endif

    cl_uint status;
    cl_platform_id platform;

    // create platform
    status = clGetPlatformIDs(1, &platform, NULL);

    cl_device_id device;
    
    // create GPU device
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    // create context
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);

    // create command queue
    cl_command_queue queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, NULL);

    // create three OpenCL memory object
    cl_mem clBuf1 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            BUF_SIZE * sizeof(cl_float), buf1, NULL);

    cl_mem clBuf2 = clCreateBuffer(context, CL_MEM_READ_ONLY,
            BUF_SIZE * sizeof(cl_float), NULL, NULL);

    cl_event writeEvent;
    status = clEnqueueWriteBuffer(queue, clBuf2, 1, 0, BUF_SIZE * sizeof(cl_float), buf2, 0, 0, 0);

#if DEBUG
    printf(" before wait\n");
#endif

    status = clFlush(queue);
    waitForEventComplete(&writeEvent);
    //clWaitForEvents();

    cl_mem buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, BUF_SIZE * sizeof(cl_float), NULL, NULL);

    const char* filename = "add.cl";
    std::string sourceStr;
    status = convertToString(filename, sourceStr);

    const char* source = sourceStr.c_str();
    size_t sourceSize[] = { strlen(source) };

#if DEBUG
    printf(" kernel source :%s\n", source);
#endif

    // create program object
    cl_program program = clCreateProgramWithSource(context, 1, &source, sourceSize, NULL);

    // compile program object
    status = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if(status != 0)
    {
        printf("clBuild failed:%d\n", status);
        char tbuf[0x10000];
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0x10000, tbuf, NULL);
        printf("\n%s\n", tbuf);
        return -1;
    }

    // create kernel object
    cl_kernel kernel = clCreateKernel(program, "vecadd", NULL);
    
    // set kernel parameter
    cl_int clnum = BUF_SIZE;
    clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*) &clBuf1);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*) &clBuf2);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*) &buffer);

    cl_event ev;
    size_t global_work_size = BUF_SIZE;
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size, NULL, 0, NULL, &ev);
    status = clFlush(queue);
    waitForEventComplete(&ev);

    // copy data to memory
    cl_float *ptr;
    cl_event mapEvent;

    ptr = (cl_float *)clEnqueueMapBuffer(queue, buffer, CL_TRUE, CL_MAP_READ, 0, BUF_SIZE * sizeof(cl_float),
            0, NULL, NULL, NULL);

    status = clFlush(queue);
    waitForEventComplete(&mapEvent);

    // comparing CPU result to check GPU result
    if(!memcmp(buf, ptr, BUF_SIZE))
        printf("Verify passed\n");
    else
        printf("Verify failed\n");

    if(buf) FREE_COUNT(buf);
    if(buf1) FREE_COUNT(buf1);
    if(buf2) FREE_COUNT(buf2);

    clReleaseMemObject(clBuf1);
    clReleaseMemObject(clBuf2);
    clReleaseMemObject(buffer);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    
//    cl_context context;
//    cl_device_id * devices;
//    char* devname;
//    size_t cb;
//    
//    context = clCreateContextFromType(NULL, CL_DEVICE_TYPE_DEFAULT, NULL, NULL, NULL);
//    
//    if(context == 0)
//    {
//        printf("can not get GPU context\n");
//        return -1;
//    }
//    
//    // get a list of devices
//    
//    clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &cb);
//    devices = (cl_device_id *) malloc(cb);
//    clGetContextInfo(context, CL_CONTEXT_DEVICES, cb, devices, 0);
//    
//    // show the name of the first device
//    clGetDeviceInfo(devices[0],CL_DEVICE_NAME, 0, NULL, &cb);
//    devname = (char*) malloc(cb);
//    clGetDeviceInfo(devices[0], CL_DEVICE_NAME, cb, devname, 0);
//    printf("Device: %s\n", devname);
//    
//    // release everything
//    free(devname);
//    free(devices);
//    clReleaseContext(context);
    

    return 0;
}
