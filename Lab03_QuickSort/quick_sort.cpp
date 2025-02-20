#include "quick_sort.h"

#include "catch2.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>
#pragma comment(linker,"/STACK:200000000000")

namespace lab03
{
    int partition(int* vector, int left, int right, Operation* assignments, Operation* comparations)//O(n)
    {
        int pivot = vector[right], i = left - 1, j, aux;
        //the pivot element is the rightmost element 
        //i keeps track of the position where elements are smaller or equal to the pivot
        for (j = left; j < right; j++)
        {
            if (vector[j] <= pivot)//the current element is smaller or equal than the pivot
            {
                i++; //move the boundary of smaller elements
                //move the smaller elements to the left side of the boundary
                aux = vector[i];
                vector[i] = vector[j];
                vector[j] = aux;
                if (assignments)
                    assignments->count(3);
            }
            if (comparations)
                comparations->count();
        }
        //i+1 is the pivot's position
        aux = vector[i+1];
        vector[i+1] = vector[right];
        vector[right] = aux;
        if (assignments)
            assignments->count(3);
        return i + 1;//return the index of the pivot
    }

    void quickSort(int* vector, int left, int right, Operation* assignments, Operation* comparations)//worst case is O(n2), else O(nlogn)
    {
        int p;
        if (left < right) //the current subarray has more than one element
        {
            p = partition(vector, left, right, assignments, comparations); //get the position of the pivot
            quickSort(vector, left, p - 1, assignments, comparations);
            quickSort(vector, p + 1, right, assignments, comparations);
        }
    }

    void insertionSort(int* vector, int left, int right, Operation* assignments, Operation* comparisons)//O(n) best case, O(n2) worst case
    {
        int i, j, aux;
        for (i = left+1; i <= right; i++)
        {
            aux = vector[i];
            if (assignments)
                assignments->count();
            j = i - 1;
            if (comparisons)
                comparisons->count();
            while (j >= left && aux < vector[j])
            {
                vector[j + 1] = vector[j];
                if (assignments)
                    assignments->count();
                if (comparisons)
                    comparisons->count();
                j--;
            }
            vector[j + 1] = aux;
            if (assignments)
                assignments->count();
        }
    }

    void hybridizedQuickSort(int* vector, int n, int th,  int left, int right, Operation* assignments, Operation* comparisons)
    {
        if (left < right)
        {
            if (right - left + 1 <= th)
            {
                insertionSort(vector, left, right, assignments, comparisons);
            }
            else
            {
                int p = partition(vector, left, right,assignments,comparisons);
                hybridizedQuickSort(vector, n, th, left, p - 1, assignments, comparisons);
                hybridizedQuickSort(vector, n, th, p + 1, right, assignments, comparisons);;
            }
        }
    }

    int treshholdFind(Profiler& profiler)
    {
        int vector[10000], i, vector2[10000];
        int tr, minop=99999999;
        FillRandomArray(vector, 10000);
        for (i = 5; i <= 50; i++)
        {
            CopyArray(vector2, vector, 10000);
            Operation comparationsHybridized = profiler.createOperation("hydridCompatisons", i);
            Operation assignmentssHybridized = profiler.createOperation("hydridAssignments", i);
            hybridizedQuickSort(vector2, 10000, i, 0, 9999, &comparationsHybridized, &assignmentssHybridized);
            if (comparationsHybridized.get() + assignmentssHybridized.get() < minop)
            {
                tr = i;
                minop = comparationsHybridized.get() + assignmentssHybridized.get();
            }
        }
        profiler.addSeries("HybridTotalOperations", "HybridAssignments", "HybridComparisons");
        profiler.createGroup("Threshold Analysis", "HybridTotalOperations");
        return minop;
    }

    void heapify_sink(int* vector, int index, int size, Operation* assignments, Operation* comparisons) //O(logn)
    {
        int left, right, largest;
        left = 2 * index + 1;
        right = 2 * index + 2;
        largest = index; //the current node is the largest
        if (left<size && vector[left]>vector[largest]) //left child exists and is greater
            largest = left;
        if (left < size && comparisons)
            comparisons->count();
        if (right<size && vector[right]>vector[largest]) //right child exists and is greater
            largest = right;
        if (right < size && comparisons)
            comparisons->count();
        if (largest != index) //the largest is not the current node
        {
            int aux;
            aux = vector[index];
            vector[index] = vector[largest];
            vector[largest] = aux;
            if (assignments)
                assignments->count(3);
            heapify_sink(vector, largest, size, assignments, comparisons); //continue with the affected tree
        }
    }

    void buildHeap_BottomUp(int* vector, int size, Operation* assignments, Operation* comparisons)//O(n)
    {
        //the last non-leaf node is size/2 - 1
        int i;
        for (i = size / 2 - 1; i >= 0; i--)
        {
            heapify_sink(vector, i, size, assignments, comparisons);
        }
        //the complexity is O(n) because the height of a heap is logn and a node at height h takes at most logh to be in the right place
        //so, at height 0 -> n/2 nodes require 0 movement; at height 1 -> n/4 nodes require 1 movement and so on
        //after making the sum, the total will be n/2 * 0 + n/4 * 1 + n/8 * 2 +...+ 1 * logn
    }

    void heapSort(int* vector, int size, Operation* assignments, Operation* comparisons)//O(nlogn)
    {
        //extract de max element (root) and heapify
        //instead of creating a new array, we can just move it at the end of the existing one and reducing the size
        //first, create heap, if it's not already created
        buildHeap_BottomUp(vector, size, assignments, comparisons);
        int i, aux;
        for (i = size - 1; i >= 1; i--)
        {
            aux = vector[i];
            vector[i] = vector[0];
            vector[0] = aux;
            if (assignments)
                assignments->count(3);
            heapify_sink(vector, 0, i, assignments, comparisons);
        }
    }

    int randomPartition(int* vector, int left, int right, Operation* assignments, Operation* comparisons)
    {
        int pr = left + rand() % (right - left + 1); //random from left and right
        int aux;
        //move the pivot element to the end
        aux = vector[pr];
        vector[pr] = vector[right];
        vector[right] = aux;
        if (assignments)
            assignments->count(3);

        //perform the partition
        int p = vector[right], i=left-1, j;
        for (j = left; j < right; j++)
        {
            if (vector[j] < p)
            {
                i++;
                aux = vector[i];
                vector[i] = vector[j];
                vector[j] = aux;
                if (assignments)
                    assignments->count(3);
            }
            if (comparisons)
                comparisons->count();
        }
        aux = vector[i+1];
        vector[i+1] = vector[right];
        vector[right] = aux;
        if (assignments)
            assignments->count(3);
        return i + 1;
    }

    void quickSelect(int* vector, int n, int left, int right, Operation* assignments, Operation* comparisons)
    {
        if (left < right)
        {
            int pr = randomPartition(vector, left, right, assignments, comparisons);
            quickSelect(vector, n, left, pr - 1, assignments, comparisons);
            quickSelect(vector, n, pr + 1, right, assignments, comparisons);
        }
    }

    int quickSelect2(int* vector, int left, int right, int k, Operation* assignments, Operation* comparisons)
    {
        if (comparisons)
            comparisons->count();
        if (left <= right)
        {
            int pivot = randomPartition(vector, left, right, assignments, comparisons);

            if (comparisons)
                comparisons->count();
            if (pivot == k)
                return vector[pivot];

            if (comparisons)
                comparisons->count();
            if (pivot > k)
                return quickSelect2(vector, left, pivot - 1, k, assignments, comparisons);
            else
                return quickSelect2(vector, pivot + 1, right, k, assignments, comparisons);
        }

        return -1;
    }


    void copyVector(int* vector1, int* vector2, int size)
    {
        int i;
        for (i = 0; i < size; i++)
            vector1[i] = vector2[i];
    }

    void printVector(int* vector, int size)
    {
        printf("The vector, now, is: ");
        int i;
        for (i = 0; i < size; i++)
            printf("%d, ", vector[i]);
        printf("\n");
    }

    void demonstrate(int size)
    {
        int v[10] = { 8, 4, 3, 6, 2, 7, 19, 19, 0, 12 }, copyV[10];

        // Copy and print original array
        copyVector(copyV, v, 10);
        printf("Original array: ");
        printVector(copyV, 10);

        // QuickSort
        copyVector(copyV, v, 10);
        quickSort(copyV, 0, 9, nullptr, nullptr);
        printf("QuickSort -> ");
        printVector(copyV, 10);

        // InsertionSort
        copyVector(copyV, v, 10);
        insertionSort(copyV, 0, 9, nullptr, nullptr);
        printf("InsertionSort -> ");
        printVector(copyV, 10);

        // Hybridized QuickSort (with threshold of 3)
        copyVector(copyV, v, 10);
      //  printf("The best threashold value is %d\n", treshholdFind(profiler));
        int threshold = 3;
        hybridizedQuickSort(copyV, 10, threshold, 0, 9, nullptr, nullptr);
        printf("Hybridized QuickSort -> ");
        printVector(copyV, 10);

        // HeapSort
        copyVector(copyV, v, 10);
        heapSort(copyV, 10, nullptr, nullptr);
        printf("HeapSort -> ");
        printVector(copyV, 10);

        // QuickSelect to find k-th smallest element
        int k = 3;  // looking for the 4th smallest element, because the index starts from 0
        copyVector(copyV, v, 10);
        int kthElement = quickSelect2(copyV, 0, 9, k, nullptr, nullptr);
        printf("QuickSelect (4th smallest element) -> %d\n", kthElement);
    }

    TEST_CASE("Quick sort")
    {
        int data1[100], copyD[100];
        FillRandomArray(data1, 100);

        // Testing quickSort
        copyVector(copyD, data1, 100);
        quickSort(copyD, 0, 99, nullptr, nullptr);
        REQUIRE(IsSorted(copyD, 100));

        // Testing insertionSort
        copyVector(copyD, data1, 100);
        insertionSort(copyD, 0, 99, nullptr, nullptr);
        REQUIRE(IsSorted(copyD, 100));

        // Testing hybridizedQuickSort
        copyVector(copyD, data1, 100);
        int threshold = 10;
        hybridizedQuickSort(copyD, 100, threshold, 0, 99, nullptr, nullptr);
        REQUIRE(IsSorted(copyD, 100));

        // Testing heapSort
        copyVector(copyD, data1, 100);
        heapSort(copyD, 100, nullptr, nullptr);
        REQUIRE(IsSorted(copyD, 100));
        
    }

    void performance(Profiler& profiler, AnalysisCase whichCase)
    {
        int vector1[10000], vector2[10000], vector3[10000], vector4[10000];
        int i, size;

        switch (whichCase)
        {
        case AVERAGE:
            for (i = 0; i < 5; i++)
            {
                for (size = 100; size < 10000; size += 100)
                {
                   // int tr= treshholdFind(profiler);

                    Operation quickSort_assignments = profiler.createOperation("quickSort_Assignments", size);
                    Operation quickSort_comparisons = profiler.createOperation("quickSort_Comparisons", size);

                    Operation heapSort_assignments = profiler.createOperation("heapSort_Assignments", size);
                    Operation heapSort_comparisons = profiler.createOperation("heapSort_Comparisons", size);

                    Operation hybridQuickSort_assignments = profiler.createOperation("hybridQuickSort_Assignments", size);
                    Operation hybridQuickSort_comparisons = profiler.createOperation("hybridQuickSort_Comparisons", size);

                  //  Operation quickSelect_assignments = profiler.createOperation("quickSelect_Assignments", size);
                  //  Operation quickselect_comparisons = profiler.createOperation("quickSelect_Comparisons", size);

                    FillRandomArray(vector1, size);
                    copyVector(vector2, vector1, size);
                    copyVector(vector3, vector1, size);
                    copyVector(vector4, vector1, size);

                    quickSort(vector1, 0, size - 1, &quickSort_assignments, &quickSort_comparisons);
                    heapSort(vector2, size, &heapSort_assignments, &heapSort_comparisons);
                    hybridizedQuickSort(vector3, size, 10, 0, size - 1, &hybridQuickSort_assignments, &hybridQuickSort_comparisons);
                  //  quickSelect(vector4, size, 0, size-1, &quickSelect_assignments, &quickselect_comparisons);
                }
            }

            profiler.divideValues("quickSort_Assignments", 5);
            profiler.divideValues("quickSort_Comparisons", 5);
            profiler.divideValues("heapSort_Assignments", 5);
            profiler.divideValues("heapSort_Comparisons", 5);
            profiler.divideValues("hybridQuickSort_Assignments", 5);
            profiler.divideValues("hybridQuickSort_Comparisons", 5);
            //profiler.divideValues("quickSelect_Assignments", 5);
            //profiler.divideValues("quickSelect_Comparisons", 5);

            profiler.addSeries("quickSort_Total", "quickSort_Assignments", "quickSort_Comparisons");
            profiler.addSeries("heapSort_Total", "heapSort_Assignments", "heapSort_Comparisons");
            profiler.addSeries("hybridQuickSort_Total", "hybridQuickSort_Assignments", "hybridQuickSort_Comparisons");
            //profiler.addSeries("quickSelect_Total", "quickSelect_Assignments", "quickSelect_Comparisons");

            profiler.createGroup("Assignments", "quickSort_Assignments", "heapSort_Assignments", "hybridQuickSort_Assignments");
            profiler.createGroup("Comparisons", "quickSort_Comparisons", "heapSort_Comparisons", "hybridQuickSort_Comparisons");
            profiler.createGroup("Total", "quickSort_Total", "heapSort_Total", "hybridQuickSort_Total");
            break;

        case WORST:
            for (size = 100; size < 10000; size += 100)
            {
                Operation quickSort_assignments = profiler.createOperation("quickSort_Assignments_Worst", size);
                Operation quickSort_comparisons = profiler.createOperation("quickSort_Comparisons_Worst", size);
                FillRandomArray(vector1, size, 10, 50000, false, ASCENDING);  // Sorted for worst case
                quickSort(vector1, 0, size - 1, &quickSort_assignments, &quickSort_comparisons);
            }
            profiler.createGroup("quickSort_Total_Worst", "quickSort_Assignments_Worst", "quickSort_Comparisons_Worst");
            break;

        case BEST:
            for (size = 100; size < 10000; size += 100)
            {
                Operation quickSort_assignments = profiler.createOperation("quickSort_Assignments_Best", size);
                Operation quickSort_comparisons = profiler.createOperation("quickSort_Comparisons_Best", size);

                FillRandomArray(vector1, size);  // Random data for best case
                quickSort(vector1, 0, size - 1, &quickSort_assignments, &quickSort_comparisons);
            }
            profiler.createGroup("quickSort_Total_Best", "quickSort_Assignments_Best", "quickSort_Comparisons_Best");
            break;
        }
    }

    void benchmark(Profiler& profiler, AnalysisCase whichCase) 
    {
        int vector1[10000], vector2[10000];
        int size;

        for (size = 100; size < 5000; size += 100) {
            // QuickSort benchmark
            profiler.startTimer("quickSort_Time", size);
            for (int test = 0; test < 1000; ++test) {
                FillRandomArray(vector1, size);
                quickSort(vector1, 0, size - 1, nullptr, nullptr);
            }
            profiler.stopTimer("quickSort_Time", size);

            // Hybridized QuickSort benchmark
            profiler.startTimer("hybridQuickSort_Time", size);
            for (int test = 0; test < 1000; ++test) {
                FillRandomArray(vector2, size);
                hybridizedQuickSort(vector2, size, 10, 0, size - 1, nullptr, nullptr);
            }
            profiler.stopTimer("hybridQuickSort_Time", size);
        }

        // Grouping results
        profiler.createGroup("Execution Time", "quickSort_Time", "hybridQuickSort_Time");
    }

} // namespace lab03
