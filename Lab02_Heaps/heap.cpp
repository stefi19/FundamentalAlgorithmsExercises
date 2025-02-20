#include "heap.h"

#include "catch2.hpp"

#include <iostream>

namespace lab02
{
    void selectionSortIterative(int* vector, int size, Operation* assignments, Operation* comparisons)
    {
        int i, j, minI, aux;
        for (i = 0; i < size - 1; i++)
        {
            minI = i;
            for (j = i + 1; j < size; j++)
            {
                if (comparisons)
                    comparisons->count();
                if (vector[j] < vector[minI])
                {
                    minI = j;
                }
            }
            aux = vector[i];
            vector[i] = vector[minI];
            vector[minI] = aux;
            if (assignments)
                assignments->count(3);
        }
    }

    void selectionSortRecursive(int* vector, int size, Operation* assignments, Operation* comparisons, int i, int j, int minI)
    {
        int aux;
        if (i < size - 1)
        {
            //minI = i;
            //j = i + 1;
            if (j < size)
            {
                if (comparisons)
                    comparisons->count();
                if (vector[j] < vector[minI])
                    minI = j;
                selectionSortRecursive(vector, size, assignments, comparisons, i, j + 1, minI);
            }
            else
            {
                aux = vector[i];
                vector[i] = vector[minI];
                vector[minI] = aux;
                if (assignments)
                    assignments->count(3);
                i++;
                minI = i;
                j = i + 1;
                selectionSortRecursive(vector, size, assignments, comparisons, i, j, minI);
            }
        }
    }

    void heapify_sink(int* vector, int index, int size, Operation* assignments, Operation* comparisons) //O(logn)
    {
        int left, right, largest;
        left = 2 * index + 1;
        right = 2 * index + 2;
        largest = index; //the current node is the largest
        if (left<size && vector[left]>vector[index]) //left child exists and is greater
            largest = left;
        if (left >= size && comparisons)
            comparisons->count();
        if (right<size && vector[right]>vector[largest]) //right child exists and is greater
            largest = right;
        if (right >= size && comparisons)
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

    void swim(int* vector, int size, Operation* assignments, Operation* comparisons) //O(logn)
    {
        int i, parent;
        i = size - 1;
        parent = (i - 1) / 2;
        while (i > 0 && vector[parent] < vector[i]) //move the child up until the parent is larger
        {
            if (comparisons)
                comparisons->count();
            int aux;
            aux = vector[parent];
            vector[parent] = vector[i];
            vector[i] = aux;
            if (assignments)
                assignments->count(3);
            i = parent;
            parent = (i - 1) / 2;
        }
        if (i > 0 && comparisons)
            comparisons->count();
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

    void buildHeap_TopDown(int* vector, int size, Operation* assignments, Operation* comparisons)//O(nlogn)
    {
        int i;
        for (i = 0; i < size; i++)
        {
            swim(vector, i, assignments, comparisons);
        }
        //in the worst case, the inserted element will move up to the root
        //the height is logn, so it will execute nlogn times (n because n elements)
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

    void copyVector(int* vector1, int* vector2, int size)
    {
        int i;
        for (i = 0; i < size; i++)
            vector1[i] = vector2[i];
    }

    void printVector(int* vector, int size)
    {
        int i;
        printf("The vector is: ");
        for (i = 0; i < size; i++)
        {
            printf("%d ", vector[i]);
        }
        printf("\n");
    }

    bool isHeap(int* vector, int size)
    {
        int i,left,right;
        for (i = 0; i < size / 2; i++)
        {
            left = 2 * i + 1;
            right = 2 * i + 2;
            if (left < size && vector[i] < vector[left])
                return false;
            if (right < size && vector[i] < vector[right])
                return false;
        }
        return true;
    }

    void demonstrate(int size)
    {
        // TODO: Write a small demo for each method here
        int data1[100], copyD[100];
        FillRandomArray(data1, 100);
        printVector(data1, 100);
        copyVector(copyD, data1, 100);
        selectionSortRecursive(copyD, 100,NULL,NULL,0,1,0);
        printVector(copyD, 100);
        copyVector(copyD, data1, 100);
        heapSort(copyD, 100);
        printVector(copyD, 100);
        copyVector(copyD, data1, 100);
        buildHeap_TopDown(copyD, 100);
        printVector(copyD, 100);
        copyVector(copyD, data1, 100);
        buildHeap_BottomUp(copyD, 100);
        printVector(copyD, 100);
       // REQUIRE(IsSorted(copyD, 100));
    }

    TEST_CASE("Heap methods")
    {
        int data1[100], copyD[100];
        FillRandomArray(data1, 100);
        copyVector(copyD, data1, 100);
        selectionSortIterative(copyD, 100,NULL,NULL);
        REQUIRE(IsSorted(copyD, 100));
        copyVector(copyD, data1, 100);
        selectionSortRecursive(copyD, 100,NULL,NULL,0,0,0);
        REQUIRE(IsSorted(copyD, 100));
        copyVector(copyD, data1, 100);
        heapSort(copyD, 100);
        REQUIRE(IsSorted(copyD, 100));
        copyVector(copyD, data1, 100);
        buildHeap_BottomUp(copyD,100);
        REQUIRE(isHeap(copyD, 100));
        copyVector(copyD, data1, 100);
        buildHeap_TopDown(copyD, 100);
        REQUIRE(isHeap(copyD, 100));
    }

    void performance(Profiler& profiler, AnalysisCase whichCase)
    {
        int* v1 = new int[10000];
        int* v2 = new int[10000];
        int i, size;
        switch (whichCase)
        {
        case AVERAGE:
            for (i = 0; i < 5; i++)
            {
                for (size = 100; size <= 10000; size += 100)
                {
                    Operation selection_assignments = profiler.createOperation("selectionSort_Assignments", size);
                    Operation selection_comparisons = profiler.createOperation("selectionSort_Comparisons", size);

                    Operation recursive_assignments = profiler.createOperation("recursiveSort_Assignments", size);
                    Operation recursive_comparisons = profiler.createOperation("recursiveSort_Comparisons", size);

                    Operation bottomUp_assignments = profiler.createOperation("bottomUp_Assignments", size);
                    Operation bottomUp_comparisons = profiler.createOperation("bottomUp_Comparisons", size);

                    Operation topDown_assignments = profiler.createOperation("topDown_Assignments", size);
                    Operation topDown_comparisons = profiler.createOperation("topDown_Comparisons", size);

                    Operation heapSort_assignments = profiler.createOperation("heapSort_Assignments", size);
                    Operation heapSort_comparisons = profiler.createOperation("heapSort_Comparisons", size);

                    FillRandomArray(v1, size, 10, 50000, false, UNSORTED);
                    copyVector(v2, v1, size);
                    selectionSortIterative(v2, size, &selection_assignments, &selection_comparisons);
                    copyVector(v2, v1, size);
                    selectionSortRecursive(v2, size, &recursive_assignments, &recursive_comparisons,0,0,0);
                    copyVector(v2, v1, size);
                    buildHeap_BottomUp(v2, size, &bottomUp_assignments, &bottomUp_assignments);
                    copyVector(v2, v1, size);
                    buildHeap_TopDown(v2, size, &topDown_assignments, &topDown_comparisons);
                    copyVector(v2, v1, size);
                    heapSort(v2, size, &heapSort_assignments, &heapSort_assignments);
                }
            }
            profiler.divideValues("selectionSort_Assignments", 5);
            profiler.divideValues("selectionSort_Comparisons", 5);
            profiler.divideValues("recursiveSort_Assignments", 5);
            profiler.divideValues("recursiveSort_Comparisons", 5);
            profiler.divideValues("bottomUp_Assignments", 5);
            profiler.divideValues("bottomUp_Comparisons", 5);
            profiler.divideValues("topDown_Assignments", 5);
            profiler.divideValues("topDown_Comparisons", 5);
            profiler.divideValues("heapSort_Assignments", 5);
            profiler.divideValues("heapSort_Comparisons", 5);

            profiler.createGroup("Assignments", "selectionSort_Assignments", "recursiveSort_Assignments", "bottomUp_Assignments", "topDown_Assignments", "heapSort_Assignments");
            profiler.createGroup("Comparisons", "selectionSort_Comparisons", "recursiveSort_Comparisons", "bottomUp_Comparisons", "topDown_Comparisons", "heapSort_Comparisons");

            profiler.addSeries("selectionSort_Total", "selectionSort_Assignments", "selectionSort_Comparisons");
            profiler.addSeries("recursiveSort_Total", "recursiveSort_Assignments", "recursiveSort_Comparisons");
            profiler.addSeries("bottomUp_Total", "bottomUp_Assignments", "bottomUp_Comparisons");
            profiler.addSeries("topDown_Total", "topDown_Assignments", "topDown_Comparisons");
            profiler.addSeries("heapSort_Total", "heapSort_Assignments", "heapSort_Comparisons");

            profiler.createGroup("Total", "selectionSort_Total", "recursiveSort_Total", "bottomUp_Total", "topDown_Total", "heapSort_Total");

            break;
        case WORST:
            for (size = 100; size <= 10000; size += 100)
            {
                Operation bottomUp_assignments = profiler.createOperation("bottomUp_Assignments", size);
                Operation bottomUp_comparisons = profiler.createOperation("bottomUp_Comparisons", size);

                Operation topDown_assignments = profiler.createOperation("topDown_Assignments", size);
                Operation topDown_comparisons = profiler.createOperation("topDown_Comparisons", size);

                FillRandomArray(v1, size, 10, 50000, false, 1);
                copyVector(v2, v1, size);
                //at bottom up, it's always the same complexity
                buildHeap_BottomUp(v2, size, &bottomUp_assignments, &bottomUp_assignments);
                copyVector(v2, v1, size);
                buildHeap_TopDown(v2, size, &topDown_assignments, &topDown_comparisons);
            }

            profiler.createGroup("Assignments", "bottomUp_Assignments", "topDown_Assignments");
            profiler.createGroup("Comparisons", "bottomUp_Comparisons", "topDown_Comparisons");

            profiler.addSeries("bottomUp_Total", "bottomUp_Assignments", "bottomUp_Comparisons");
            profiler.addSeries("topDown_Total", "topDown_Assignments", "topDown_Comparisons");

            profiler.createGroup("Total", "bottomUp_Total", "topDown_Total");
            break;
        }
    }

    void benchmark(Profiler& profiler, AnalysisCase whichCase)
    {
        // TODO: Write your benchmarks here, as explained in the readme
        // You should use profiler.startTimer/stopTimer for measuring and plotting measured time
        int vector1[10000], vector2[10000];
        int n;
        for (n = 100; n < 1000; n += 100) {
            profiler.startTimer("iterative_function", n);
            for (int i = 0; i < 1000; ++i) {
                FillRandomArray(vector1, n);
                selectionSortIterative(vector1, n,NULL,NULL);
            }
            profiler.stopTimer("iterative_function", n);
            profiler.startTimer("recursive_function", n);

            for (int i = 0; i < 1000; ++i) {
                FillRandomArray(vector2, n);
                selectionSortRecursive(vector2, n,NULL,NULL,0,0,0);
            }
            profiler.stopTimer("recursive_function", n);
        }
        profiler.createGroup("time", "iterative_function", "recursive_function");

    }

} // namespace lab02
