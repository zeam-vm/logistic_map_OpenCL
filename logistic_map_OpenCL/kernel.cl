// Simple OpenCL kernel that squares an input array.
// This code is stored in a file called mykernel.cl.
// You can name your kernel file as you would name any other
// file.  Use .cl as the file extension for all kernel
// source files.

// Kernel block.

#define INLINE

#ifdef INLINE
kernel void logisticsmap(
                         global int *mu,
                         global int *p,
                         global int *input,
                         global int *output)
{
    size_t i = get_global_id(0);
    output[i] = input[i];
    output[i] = mu[i] * output[i] * (output[i] + 1) % p[i];
    output[i] = mu[i] * output[i] * (output[i] + 1) % p[i];
    output[i] = mu[i] * output[i] * (output[i] + 1) % p[i];
    output[i] = mu[i] * output[i] * (output[i] + 1) % p[i];
    output[i] = mu[i] * output[i] * (output[i] + 1) % p[i];
    output[i] = mu[i] * output[i] * (output[i] + 1) % p[i];
    output[i] = mu[i] * output[i] * (output[i] + 1) % p[i];
    output[i] = mu[i] * output[i] * (output[i] + 1) % p[i];
    output[i] = mu[i] * output[i] * (output[i] + 1) % p[i];
    output[i] = mu[i] * output[i] * (output[i] + 1) % p[i];
}
#else
kernel void logisticsmap(
                         global int *mu,
                         global int *p,
                         global int *input,
                         global int *output)
{
    size_t i = get_global_id(0);
    output[i] = input[i];
    for(int n = 0; n < 10; n++) {
        output[i] = mu[i] * output[i] * (output[i] + 1) % p[i];
    }
}
#endif
