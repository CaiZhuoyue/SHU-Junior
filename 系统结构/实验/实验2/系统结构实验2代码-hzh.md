系统结构实验2



```c++
#include <stdio.h> 
#include <omp.h> 
#include <time.h> 
#include <iostream> using namespace std;

int main() 
{ 
    omp_set_num_threads(8);
    #pragma omp parallel 
    {
        int tid = omp_get_thread_num();
        printf("Hello World from OMP thread %d\n", tid); 
        if (!tid) 
        {
            int nthreads = omp_get_num_threads();
            printf("Number of threads is %d\n", nthreads); 
        }

    }
}
```



```c++
#include<stdio.h> 
#include<omp.h> 
#include<time.h> 
#include<iostream> 
#include<fstream> 
using namespace std;

int main() 
{ 
    omp_set_num_threads(8); 
#pragma omp parallel for
	for (size_t i = 0; i < 14; i++)
	{
		int tid = omp_get_thread_num();
        printf("Hello World from OMP thread %d\n", tid);
    }
}
```



```c++
#include<stdio.h> 
#include<omp.h> 
#include<time.h> 
#include <mutex>
#include<iostream> 
#include<fstream> 
using namespace std;

mutex mtx;

int main()
{ 
    omp_set_num_threads(8);
	int x = 10, y = 1;
#pragma omp parallel while 
    for (;x > y;)
    {
        mtx.lock();
        x--;
        mtx.unlock();
        int tid = omp_get_thread_num();
        printf("Hello World from OMP thread %d\n", tid);
	}
}

```



```c++
#include<stdio.h>
#include<omp.h>
#include<time.h> 
#include<iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>

#define random(x) (rand() % x)

// 向量初始化
void arrayRandInit(int n, float a[]) {
    for (size_t i = 0; i < n; i++)
    	a[i] = random(10000000);
}

// 并⾏加法
void mtx_add_p(int n, float a[], float b[], float rs[]) { #pragma omp parallel for
    for (size_t i = 0; i < n; i++)
        rs[i] = a[i] + b[i];
}

// 并行乘法
void mtx_multiply_p(int n, float a[], float b[], float rs[]) { #pragma omp parallel for
    for (size_t i = 0; i < n; i++)
        rs[i] = a[i] * b[i];
}

// ⾮并⾏加法和乘法
void mtx_add(int n, float a[], float b[], float rs[]) {
    for (size_t i = 0; i < n; i++)
        rs[i] = a[i] + b[i];
}

void mtx_multiply(int n, float a[], float b[], float rs[]) {
    for (size_t i = 0; i < n; i++)
        rs[i] = a[i] * b[i];
}

int main() {

    float duration; clock_t s, f;

    int n = 10000010;

    float *A, *B, *C, *D;
    A = new float[n];
    B = new float[n];
    C = new float[n];
    D = new float[n];

    arrayRandInit(n, A),arrayRandInit(n, B);

    s = clock();
    mtx_add(n, A, B, C);
    f = clock();
    duration = (double)(f - s) / CLOCKS_PER_SEC;
    std::cout << "no paralle add:" << duration << '\n';

    s = clock();
    mtx_add_p(n, A, B, C);
    f = clock();
    duration = (double)(f - s) / CLOCKS_PER_SEC;
    std::cout << "paralle add:" << duration << '\n';

    s = clock();
    mtx_multiply(n, A, B, C);
    f = clock();
    duration = (double)(f - s) / CLOCKS_PER_SEC;
    std::cout << "no paralle multiply:" << duration << '\n';

    s = clock();
    mtx_multiply_p(n, A, B, C);
    f = clock(); duration = (double)(f - s) / CLOCKS_PER_SEC; 	  std::cout << "paralle multiply:" << duration << '\n';

    std::cout << "d1:" << s << f << '\n';
    return 0;

}
```

