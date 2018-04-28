__kernel
void addone(__global float* data, const int n)
{
    int index = get_global_id(0);
     
    if (index < n) {
        data[index] += 1.0f;
    }
}
