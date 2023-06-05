

__kernel void parser(const int input, const int unput, __global int *output) {
  int a = input;
  int b = unput;
  int c = a + b;
  output[0] = c;
}