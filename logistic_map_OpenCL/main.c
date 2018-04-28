//
//  main.c
//  logistic_map_OpenCL
//
//  Created by 山崎進 on 2018/04/28.
//  Copyright © 2018年 山崎進. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <OpenCL/opencl.h>

#define PLATFORM_MAX 4
#define DEVICE_MAX 4

void EC(cl_int result, const char *title) {
	if(result != CL_SUCCESS) {
		printf("Error: %s(%d)\n", title, result);
	}
}

static cl_int err = CL_SUCCESS;
void EC2(const char *title) {
    if(err != CL_SUCCESS) {
        printf("Error: %s(%d)\n", title, err);
    }
    err = CL_SUCCESS;
}


int main(int argc, const char * argv[]) {
    // プラットフォーム一覧を取得
	cl_platform_id platforms[PLATFORM_MAX];
	cl_uint platformCount;
	EC(clGetPlatformIDs(PLATFORM_MAX, platforms, &platformCount), "clGetPlatformIDs");
    if(platformCount == 0) {
        fprintf(stderr, "No Platform.\n");
        return EXIT_FAILURE;
    }
    
    // 見つかったプラットフォームの情報を表示
    for(int i = 0; i < platformCount; i++) {
        char vendor[100] = {0};
        char version[100] = {0};
        EC(clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, sizeof(vendor), vendor, NULL), "clGetPlatformInfo");
        EC(clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, sizeof(version), version, NULL), "clGetPlatformInfo");
        printf("Platform id: %p, Vendor: %s, Version: %s\n", platforms[i], vendor, version);
    }

    // デバイス一覧を取得
    cl_device_id devices[DEVICE_MAX];
    cl_uint deviceCount;
    EC(clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, DEVICE_MAX, devices, &deviceCount), "clGetDeviceIDs");
    if (deviceCount == 0) {
        fprintf(stderr, "No device.\n");
        return EXIT_FAILURE;
    }
    
    // 見つかったデバイスの情報を表示
    printf("%d device(s) found.\n", deviceCount);
    for(int i = 0; i < deviceCount; i++) {
        char name[100] = {0};
        size_t len;
        EC(clGetDeviceInfo(devices[i], CL_DEVICE_NAME, sizeof(name), name, &len), "clGetDeviceInfo");
        printf("Device id: %d, Name: %s\n", i, name);
    }
    
    // コンテキストの作成
    cl_context ctx = clCreateContext(NULL, 1, devices, NULL, NULL, &err);
    EC2("clCreateContext");
    
    // コンパイル済みclプログラムの読み込み
    const char* bitcode_path = "OpenCL/kernel.cl.gpu_32.bc";
    size_t len = strlen(bitcode_path);
    cl_program program = clCreateProgramWithBinary(ctx, 1, devices, &len, (const unsigned char**)&bitcode_path, NULL,&err);
    EC2("clCreateProgramWithBinary");
    
    // プログラムのビルド
    EC(clBuildProgram(program, 1, devices, NULL, NULL, NULL), "clBuildProgram");

    // カーネルの作成
    cl_kernel kernel = clCreateKernel(program, "addone", &err);
    EC2("clCreateKernel");
    
    // データを用意
    const int n = 1000;
    float *data = (float *)malloc(sizeof(float) * n);
    for(int i = 0; i < n; i++) {
        data[i] = (float)i;
    }

    // デバイスメモリを確保しつつデータをコピー
    cl_mem device_mem = clCreateBuffer(ctx, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float) * n, data, &err);
    EC2("clCreateBuffer");
    
    // カーネルの引数をセット
    EC(clSetKernelArg(kernel, 0, sizeof(cl_mem), &device_mem), "clSetKernelArg");
    EC(clSetKernelArg(kernel, 1, sizeof(int), &n), "clSetKernelArg");
    
    // コマンドキューの作成
    cl_command_queue q = clCreateCommandQueue(ctx, devices[0], 0, &err);
    EC2("clCreateCommandQueue");
    
    // カーネルの実行
    size_t global = n;
    EC(clEnqueueNDRangeKernel(q, kernel, 1, NULL, &global, NULL, 0, NULL, NULL), "clEnqueueNDRangeKernnel");
    
    // 結果を読み込み
    EC(clEnqueueReadBuffer(q, device_mem, CL_TRUE, 0, sizeof(float) * n, data, 0, NULL, NULL), "clEnqueueReadBuffer");
    
    // 結果の表示
    for(int i = 0; i < n; i++) {
        printf("%f, ", data[i]);
    }
    printf("\n");
    
    // コマンドキューの解放
    EC(clReleaseCommandQueue(q), "clReleaseCommandQueue");
    
    // デバイスメモリを解放
    EC(clReleaseMemObject(device_mem), "clReleaseMemObject");
    
    // カーネルの解放
    EC(clReleaseKernel(kernel), "clReleaseKernel");
    
    // プログラムの解放
    EC(clReleaseProgram(program), "clReleaseProgram");
    
    // コンテキストの解放
    EC(clReleaseContext(ctx), "clReleaseContext");
    
    printf("Done.\n");
    return EXIT_SUCCESS;
}
