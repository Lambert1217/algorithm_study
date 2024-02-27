#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

void sort__(vector<int> &arr);
void bubbleSort(vector<int> &arr);
void selectionSort(vector<int> &arr);
void insertionSort(vector<int> &arr);
void shellSort(vector<int> &arr); // 希尔排序(缩小增量排序)  元素多时速度快   O(n^1.5)
void shellInsert(vector<int> &arr, int start, int gap);
void countSort(vector<int> &arr);  // 计数排序
void bucketSort(vector<int> &arr); // 桶排序
void heapSort(vector<int> &arr);   // 堆排序
void buildheap(vector<int> &arr, int start, int end);
void merge(vector<int> &arr, int left, int right);
void mergeSort(vector<int> &arr, int left, int right); // 归并排序
void quickSort(vector<int> &arr, int left, int right);

int main(void)
{
    vector<int> arr = {155, 55, 144, 20, 22, 114, 10, 66, 88, 201};

    quickSort(arr, 0, arr.size() - 1);
    for (int &it : arr)
    {
        cout << it << " ";
    }
    cout << endl;
    return 0;
}

void sort__(vector<int> &arr)
{
    sort(arr.begin(), arr.end());
}

void bubbleSort(vector<int> &arr)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                // 交换arr[j]和arr[j+1]
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void selectionSort(vector<int> &arr)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i)
    {
        int index = i;
        for (int j = i + 1; j < n; j++)
        {
            if (arr[index] > arr[j])
            {
                index = j;
            }
        }
        if (index != i)
        {
            swap(arr[index], arr[i]);
        }
    }
}

void insertionSort(vector<int> &arr)
{
    int n = arr.size();
    for (int i = 1; i < n; ++i)
    {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && key > arr[j])
        {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void shellSort(vector<int> &arr)
{
    for (int gap = arr.size() / 2; gap > 0; gap /= 2)
    {
        for (int i = 0; i < gap; ++i)
        {
            shellInsert(arr, i, gap);
        }
    }
}

void shellInsert(vector<int> &arr, int start, int gap)
{
    int n = arr.size();
    for (int i = start + gap; i < n; i += gap)
    {
        int key = arr[i];
        int j = i - gap;
        while (j >= start && key > arr[j])
        {
            arr[j + gap] = arr[j];
            j -= gap;
        }
        arr[j + gap] = key;
    }
}

void countSort(vector<int> &arr)
{
    map<int, int> temp;
    for (int &it : arr)
    {
        temp[it]++;
    }
    arr.clear();
    for (auto &it : temp)
    {
        while (it.second)
        {
            arr.push_back(it.first);
            it.second--;
        }
    }
    // const int max = *max_element(arr.begin(), arr.end());
    // int tempArray[max + 1] = {0};
    // for (int &x : arr)
    // {
    //     ++tempArray[x];
    // }
    // arr.clear();
    // for (int i = 0; i <= max; ++i)
    // {
    //     while (tempArray[i] > 0)
    //     {
    //         arr.push_back(i);
    //         tempArray[i]--;
    //     }
    // }
}

void bucketSort(vector<int> &arr)
{
    // 时间(快排) : m(桶的数量)   O(N+C)    C --> (n*(logn-logm))
    // 空间 ： O(N+M)  N --> 桶的空间    M --> 排序空间
    // 稳定性 ：不稳定
    int arr_size = arr.size();
    int max = *max_element(arr.begin(), arr.end()); // n
    int min = *min_element(arr.begin(), arr.end()); // n
    int Bucketnum = (max - min + 1) / arr_size + 1; // m
    vector<vector<int>> Bucket(Bucketnum);
    for (int &it : arr) // n
    {
        int index = (it - min + 1) / arr_size;
        Bucket[index].push_back(it);
    }
    for (auto &it : Bucket) // m
    {
        sort(it.begin(), it.end()); // 快排  n/mlogn/m
    }
    arr.clear();
    for (auto &it : Bucket)
    {
        for (auto &x : it)
        {
            arr.push_back(x);
        }
    }
}

void heapSort(vector<int> &arr)
{
    int arrSize = arr.size();
    for (int i = arrSize / 2 - 1; i >= 0; i--) // 先构建大顶堆
    {
        buildheap(arr, i, arrSize - 1);
    }
    for (int i = arrSize - 1; i > 0; i--) // 依次将堆顶放入数组尾部
    {
        swap(arr[0], arr[i]);
        buildheap(arr, 0, i - 1);
    }
}

void buildheap(vector<int> &arr, int start, int end)
{
    int cur = start;
    int lc = 2 * cur + 1;
    for (; lc <= end; cur = lc, lc = 2 * cur + 1)
    {
        if (lc < end && arr[lc] < arr[lc + 1])
            lc++;
        if (arr[lc] > arr[cur])
            swap(arr[lc], arr[cur]);
        else
            break;
    }
}

void merge(vector<int> &arr, int left, int right)
{
    int *temp = new int[right - left + 1];
    int mid = (right + left) / 2;
    int i = left, j = mid + 1, index = 0;
    while (i <= mid && j <= right)
    {
        if (arr[i] < arr[j])
        {
            temp[index++] = arr[i++];
        }
        else
        {
            temp[index++] = arr[j++];
        }
    }
    while (i <= mid)
        temp[index++] = arr[i++];
    while (j <= right)
        temp[index++] = arr[j++];
    for (int i = 0; i < index; i++)
    {
        arr[left + i] = temp[i];
    }
    delete[] temp;
}

void mergeSort(vector<int> &arr, int left, int right)
{
    if (left >= right)
        return;
    int mid = (right + left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, right);
}

void quickSort(std::vector<int> &arr, int left, int right)
{
    if (left < right)
    {
        int key = arr[left];
        int i = left, j = right;
        bool b = true;
        while (i != j)
        {
            if (b)
            {
                if (key > arr[j])
                {
                    arr[i++] = arr[j];
                    b = !b;
                }
                else
                    j--;
            }
            else
            {
                if (key < arr[i])
                {
                    arr[j--] = arr[i];
                    b = !b;
                }
                else
                    i++;
            }
        }
        arr[i] = key;
        quickSort(arr, left, i - 1);
        quickSort(arr, i + 1, right);
    }
    else
    {
        return;
    }
}
