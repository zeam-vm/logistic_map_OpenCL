//
//  main.c
//  logistic_map_OpenCL
//
//  Created by 山崎進 on 2018/04/28.
//  Copyright © 2018年 山崎進. All rights reserved.
//

#include <stdio.h>
#include <OpenCL/opencl.h>

#define PLATFORM_MAX 4
#define DEVICE_MAX 4

void EC(cl_int result, const char *title) {
	if(result != CL_SUCCESS) {
		printf("Error: %s(%d)\n", title, result);
	}
}

static cl_int err = CL_SUCCESS;
void EC2(cl_int result, const char *title) {
    if(err != CL_SUCCESS) {
        printf("Error: %s(%d)\n", title, result);
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
    
    return EXIT_SUCCESS;
}
