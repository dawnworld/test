#include <iostream>

using namespace std;

#define __NO_STD_VECTOR // Use cl::vector and cl::string and
#define __NO_STD_STRING // not STL versions, more on this later
#include <OpenCL/OpenCL.h>


#define DATA_SIZE (1024*1240)

const char *KernelSource = "\n"		      \
                            "__kernel void square(                    \n" \
                            "   __global float* input,                \n" \
                            "   __global float* output,               \n" \
                            "   const unsigned int count)             \n" \
                            "{                                        \n" \
                            "   int i = get_global_id(0);             \n" \
                            "   if(i < count)                         \n" \
                            "       output[i] = input[i] * input[i];  \n" \
                            "}                                        \n" \
                            "\n";

int main(int argc, char* argv[])
{
    int devType = CL_DEVICE_TYPE_GPU;

    if(argc > 1) {
        devType = CL_DEVICE_TYPE_CPU;
        cout << "Using: CL_DEVICE_TYPE_CPU" << endl;
    } else {
        cout << "Using: CL_DEVICE_TYPE_GPU" << endl;
    }

    cl_int err;     // error code returned from api calls

    size_t global;  // global domain size for our calculation
    size_t local;   // local domain size for our calculation

    cl_platform_id cpPlatform; // OpenCL platform
    cl_device_id device_id;    // compute device id
    cl_context context;        // compute context
    cl_command_queue commands; // compute command queue
    cl_program program;        // compute program
    cl_kernel kernel;          // compute kernel

    // Connect to a compute device
    err = clGetPlatformIDs(1, &cpPlatform, NULL);
    if (err != CL_SUCCESS) {
        cerr << "Error: Failed to find a platform!" << endl;
        return EXIT_FAILURE;
    }

    // Get a device of the appropriate type
    err = clGetDeviceIDs(cpPlatform, devType, 1, &device_id, NULL);
    if (err != CL_SUCCESS) {
        cerr << "Error: Failed to create a device group!" << endl;
        return EXIT_FAILURE;
    }

    // Create a compute context
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    if (!context) {
        cerr << "Error: Failed to create a compute context!" << endl;
        return EXIT_FAILURE;
    }

    // Create a command commands
    commands = clCreateCommandQueue(context, device_id, 0, &err);
    if (!commands) {
        cerr << "Error: Failed to create a command commands!" << endl;
        return EXIT_FAILURE;
    }

    // Create the compute program from the source buffer
    program = clCreateProgramWithSource(context, 1, 
            (const char **) &KernelSource,
            NULL, &err);
    if (!program) {
        cerr << "Error: Failed to create compute program!" << endl;
        return EXIT_FAILURE;
    }

    // Build the program executable
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        size_t len;
        char buffer[2048];

        cerr << "Error: Failed to build program executable!" << endl;
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG,
                sizeof(buffer), buffer, &len);
        cerr << buffer << endl;
        exit(1);
    }

    // Create the compute kernel in the program
    kernel = clCreateKernel(program, "square", &err);
    if (!kernel || err != CL_SUCCESS) {
        cerr << "Error: Failed to create compute kernel!" << endl;
        exit(1);
    }

    // create data for the run
    float* data = new float[DATA_SIZE];    // original data set given to device
    float* results = new float[DATA_SIZE]; // results returned from device
    unsigned int correct;               // number of correct results returned
    cl_mem input;                       // device memory used for the input array
    cl_mem output;                      // device memory used for the output array

    // Fill the vector with random float values
    unsigned int count = DATA_SIZE;
    for(int i = 0; i < count; i++)
        data[i] = rand() / (float)RAND_MAX;

    // Create the device memory vectors
    //
    input = clCreateBuffer(context,  CL_MEM_READ_ONLY,  
            sizeof(float) * count, NULL, NULL);
    output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
            sizeof(float) * count, NULL, NULL);
    if (!input || !output) {
        cerr << "Error: Failed to allocate device memory!" << endl;
        exit(1);
    }   

    // Transfer the input vector into device memory
    err = clEnqueueWriteBuffer(commands, input, 
            CL_TRUE, 0, sizeof(float) * count, 
            data, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        cerr << "Error: Failed to write to source array!" << endl;
        exit(1);
    }

    // Set the arguments to the compute kernel
    err = 0;
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
    err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &count);
    if (err != CL_SUCCESS) {
        cerr << "Error: Failed to set kernel arguments! " << err << endl;
        exit(1);
    }

    // Get the maximum work group size for executing the kernel on the device
    err = clGetKernelWorkGroupInfo(kernel, device_id, 
            CL_KERNEL_WORK_GROUP_SIZE, 
            sizeof(local), &local, NULL);
    if (err != CL_SUCCESS) {
        cerr << "Error: Failed to retrieve kernel work group info! "
            <<  err << endl;
        exit(1);
    }

    // Execute the kernel over the vector using the 
    // maximum number of work group items for this device
    global = count;
    err = clEnqueueNDRangeKernel(commands, kernel, 
            1, NULL, &global, &local, 
            0, NULL, NULL);
    if (err) {
        cerr << "Error: Failed to execute kernel!" << endl;
        return EXIT_FAILURE;
    }

    // Wait for all commands to complete
    clFinish(commands);

    // Read back the results from the device to verify the output
    //
    err = clEnqueueReadBuffer( commands, output,
            CL_TRUE, 0, sizeof(float) * count,
            results, 0, NULL, NULL ); 
    if (err != CL_SUCCESS) {
        cerr << "Error: Failed to read output array! " <<  err << endl;
        exit(1);
    }

    // Validate our results
    //
    correct = 0;
    for(int i = 0; i < count; i++) {
        if(results[i] == data[i] * data[i])
            correct++;
    }

    // Print a brief summary detailing the results
    cout << "Computed " << correct << "/" << count << " correct values" << endl;
    cout << "Computed " << 100.f * (float)correct/(float)count
        << "% correct values" << endl;

    // Shutdown and cleanup
    delete [] data; delete [] results;

    clReleaseMemObject(input);
    clReleaseMemObject(output);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);

    return 0;
}
