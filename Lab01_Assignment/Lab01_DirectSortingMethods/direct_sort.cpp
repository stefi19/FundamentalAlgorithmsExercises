#include "direct_sort.h"

#include "catch2.hpp"

#include <iostream>

//in the zip folder i attached with the assignment, i've put a document with all the explanations of the  algorithms

namespace lab01
{

    void bubbleSort(int* vector, int size, Operation* assignments, Operation* comparisons)
    {
        int poz, i, aux, notSorted, p;
        notSorted = 1;
        p = size - 1; 
        while (notSorted == 1)
        {
            poz = p;
            notSorted = 0;
            for (i = 0; i < poz; i++) //all the numbers after the last swap are ordered already
            {
                if (vector[i] > vector[i + 1])
                {
                    notSorted = 1;
                    p = i;
                    aux = vector[i];
                    vector[i] = vector[i + 1];
                    vector[i + 1] = aux;
                    if (assignments)
                        assignments->count(3);
                }
                if (comparisons)
                    comparisons->count();
            }
        }
    }

    void selectionSort(int* vector, int size, Operation* assignments, Operation* comparisons)
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

    void insertionSort(int* vector, int size, Operation* assignments, Operation* comparisons)
    {
        int i, j, aux;
        for (i = 1; i < size; i++)
        {
            aux = vector[i];
            j = i - 1;
            if (comparisons)
                comparisons->count();
            while (j >= 0 && aux < vector[j])
            {
                vector[j + 1] = vector[j];
                if (comparisons)
                    comparisons->count();
                j--;
            }
            vector[j + 1] = aux;
            if (assignments)
                assignments->count();
        }
    }

    void findPoz(int* vector, int size, int element, int& index, Operation* assignments, Operation* comparisons)
    {
        int left, right, middle, found = 0;
        left = 0;
        right = size - 1;
        while (found == 0 && left <= right)
        {
            middle = (left + right) / 2;
            if (vector[middle] == element)
            {
                found = 1;
                index = middle;
            }
            else
            {
                if (vector[middle] < element)
                    left = middle + 1;
                else
                    right = middle - 1;
                if(comparisons)
                    comparisons->count();
            }
            if(comparisons)
                comparisons->count();

        }
        if(found==0)
            index = left;
    }

    void insertAtPoz(int* vector, int size, int element, int indexWhere, Operation* assignments, Operation* comparisons)
    {
        int i;
        for (i = size - 1; i > indexWhere; i--)
        {
            vector[i] = vector[i - 1];
            if(assignments)
                assignments->count();
        }
        vector[indexWhere] = element;
        if(assignments)
            assignments->count();
    }

    void binaryInsertionSort(int* vector, int size, Operation* assignments, Operation* comparisons)
    {
        int i, aux, indexWhere;
        for (i = 1; i < size; i++)
        {
            aux = vector[i];
            findPoz(vector, i, aux, indexWhere, assignments, comparisons);
            insertAtPoz(vector, i + 1, aux, indexWhere, assignments, comparisons);
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
        printf("The vector, now, is: ");
        int i;
        for (i = 0; i < size; i++)
            printf("%d, ", vector[i]);
        printf("\n");
    }

    void demonstrate(int size)
    {
        int v[10] = { 8,4,3,6,2,7,19,19,0,12 }, copyV[10];
        copyVector(copyV, v, 10);
        printVector(copyV, 10);
        bubbleSort(copyV, 10);
        printf("Bubble sort -> ");
        printVector(copyV, 10);
        copyVector(copyV, v, 10);
        insertionSort(copyV, 10);
        printf("Insertion sort -> ");
        printVector(copyV, 10);
        copyVector(copyV, v, 10);
        selectionSort(copyV, 10);
        printf("Selection sort -> ");
        printVector(copyV, 10);
        copyVector(copyV, v, 10);
        binaryInsertionSort(copyV, 10);
        printf("Binary insertion sort -> ");
        printVector(copyV, 10);
    }

    TEST_CASE("Direct sorting methods")
    {
        int data1[100], copyD[100];
        FillRandomArray(data1, 100);
        copyVector(copyD, data1, 100);
        binaryInsertionSort(copyD, 100);
        REQUIRE(IsSorted(copyD, 100));
        copyVector(copyD, data1, 100);
        insertionSort(copyD, 100);
        REQUIRE(IsSorted(copyD, 100));
        copyVector(copyD, data1, 100);
        selectionSort(copyD, 100);
        REQUIRE(IsSorted(copyD, 100));
        copyVector(copyD, data1, 100);
        bubbleSort(copyD, 100);
        REQUIRE(IsSorted(copyD, 100));

        int data2[10] = { 1,2,3,4,5,6,7,8,9,10 };
        copyVector(copyD, data2, 10);
        binaryInsertionSort(copyD, 10);
        REQUIRE(IsSorted(copyD, 10));
        copyVector(copyD, data2, 10);
        insertionSort(copyD, 10);
        REQUIRE(IsSorted(copyD, 10));
        copyVector(copyD, data2, 10);
        selectionSort(copyD, 10);
        REQUIRE(IsSorted(copyD, 10));
        copyVector(copyD, data2, 10);
        bubbleSort(copyD, 10);
        REQUIRE(IsSorted(copyD, 10));

        int data3[10] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
        copyVector(copyD, data3, 10);
        binaryInsertionSort(copyD, 10);
        REQUIRE(IsSorted(copyD, 10));
        copyVector(copyD, data3, 10);
        insertionSort(copyD, 10);
        REQUIRE(IsSorted(copyD, 10));
        copyVector(copyD, data3, 10);
        selectionSort(copyD, 10);
        REQUIRE(IsSorted(copyD, 10));
        copyVector(copyD, data3, 10);
        bubbleSort(copyD, 10);
        REQUIRE(IsSorted(copyD, 10));
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
                    Operation bubble_assignments = profiler.createOperation("bubbleSort_Assignments", size);
                    Operation bubble_comparisons = profiler.createOperation("bubbleSort_Comparisons", size);

                    Operation insertion_assignments = profiler.createOperation("insertionSort_Assignments", size);
                    Operation insertion_comparisons = profiler.createOperation("insertionSort_Comparisons", size);

                    Operation selection_assignments = profiler.createOperation("selectionSort_Assignments", size);
                    Operation selection_comparisons = profiler.createOperation("selectionSort_Comparisons", size);

                    Operation binaryInsertion_assignments = profiler.createOperation("binaryInsertionSort_Assignments", size);
                    Operation binaryInsertion_comparisons = profiler.createOperation("binaryInsertionSort_Comparisons", size);

                    FillRandomArray(v1, size, 10, 50000, false, UNSORTED);
                    copyVector(v2, v1, size);
                    bubbleSort(v2, size, &bubble_assignments, &bubble_comparisons);
                    copyVector(v2, v1, size);
                    insertionSort(v2, size, &insertion_assignments, &insertion_comparisons);
                    copyVector(v2, v1, size);
                    selectionSort(v2, size, &selection_assignments, &selection_comparisons);
                    copyVector(v2, v1, size);
                    binaryInsertionSort(v2, size, &binaryInsertion_assignments, &binaryInsertion_comparisons);
                }
            }
            profiler.divideValues("bubbleSort_Assignments", 5);
            profiler.divideValues("bubbleSort_Comparisons", 5);
            profiler.divideValues("insertionSort_Assignments", 5);
            profiler.divideValues("insertionSort_Comparisons", 5);
            profiler.divideValues("selectionSort_Assignments", 5);
            profiler.divideValues("selectionSort_Comparisons", 5);
            profiler.divideValues("binaryInsertionSort_Assignments", 5);
            profiler.divideValues("binaryInsertionSort_Comparisons", 5);

            profiler.createGroup("Assignments", "bubbleSort_Assignments", "insertionSort_Assignments", "selectionSort_Assignments", "binaryInsertionSort_Assignments");
            profiler.createGroup("Comparisons", "bubbleSort_Comparisons", "insertionSort_Comparisons", "selectionSort_Comparisons", "binaryInsertionSort_Comparisons");

            profiler.addSeries("bubbleSort_Total", "bubbleSort_Assignments", "bubbleSort_Comparisons");
            profiler.addSeries("insertionSort_Total", "insertionSort_Assignments", "insertionSort_Comparisons");
            profiler.addSeries("selectionSort_Total", "selectionSort_Assignments", "selectionSort_Comparisons");
            profiler.addSeries("binaryInsertionSort_Total", "binaryInsertionSort_Assignments", "binaryInsertionSort_Comparisons");

            profiler.createGroup("Total", "bubbleSort_Total", "insertionSort_Total", "selectionSort_Total", "binaryInsertionSort_Total");

            break;
        case BEST:
                for (size = 100; size <= 10000; size += 100)
                {
                    Operation bubble_assignments = profiler.createOperation("bubbleSort_Assignments", size);
                    Operation bubble_comparisons = profiler.createOperation("bubbleSort_Comparisons", size);

                    Operation insertion_assignments = profiler.createOperation("insertionSort_Assignments", size);
                    Operation insertion_comparisons = profiler.createOperation("insertionSort_Comparisons", size);

                    Operation selection_assignments = profiler.createOperation("selectionSort_Assignments", size);
                    Operation selection_comparisons = profiler.createOperation("selectionSort_Comparisons", size);

                    Operation binaryInsertion_assignments = profiler.createOperation("binaryInsertionSort_Assignments", size);
                    Operation binaryInsertion_comparisons = profiler.createOperation("binaryInsertionSort_Comparisons", size);

                    FillRandomArray(v1, size, 10, 50000, false, 1); //sorted array
                    copyVector(v2, v1, size);
                    bubbleSort(v2, size, &bubble_assignments, &bubble_comparisons);
                    copyVector(v2, v1, size);
                    insertionSort(v2, size, &insertion_assignments, &insertion_comparisons);
                    copyVector(v2, v1, size);
                    selectionSort(v2, size, &selection_assignments, &selection_comparisons);
                    copyVector(v2, v1, size);
                    binaryInsertionSort(v2, size, &binaryInsertion_assignments, &binaryInsertion_comparisons);
                }

            profiler.createGroup("Assignments", "bubbleSort_Assignments", "insertionSort_Assignments", "selectionSort_Assignments", "binaryInsertionSort_Assignments");
            profiler.createGroup("Comparisons", "bubbleSort_Comparisons", "insertionSort_Comparisons", "selectionSort_Comparisons", "binaryInsertionSort_Comparisons");

            profiler.addSeries("bubbleSort_Total", "bubbleSort_Assignments", "bubbleSort_Comparisons");
            profiler.addSeries("insertionSort_Total", "insertionSort_Assignments", "insertionSort_Comparisons");
            profiler.addSeries("selectionSort_Total", "selectionSort_Assignments", "selectionSort_Comparisons");
            profiler.addSeries("binaryInsertionSort_Total", "binaryInsertionSort_Assignments", "binaryInsertionSort_Comparisons");

            profiler.createGroup("Total", "bubbleSort_Total", "insertionSort_Total", "selectionSort_Total", "binaryInsertionSort_Total");
            break;
        case WORST:
            for (size = 100; size <= 10000; size += 100)
            {
                    Operation bubble_assignments = profiler.createOperation("bubbleSort_Assignments", size);
                    Operation bubble_comparisons = profiler.createOperation("bubbleSort_Comparisons", size);

                    Operation insertion_assignments = profiler.createOperation("insertionSort_Assignments", size);
                    Operation insertion_comparisons = profiler.createOperation("insertionSort_Comparisons", size);

                    Operation selection_assignments = profiler.createOperation("selectionSort_Assignments", size);
                    Operation selection_comparisons = profiler.createOperation("selectionSort_Comparisons", size);

                    Operation binaryInsertion_assignments = profiler.createOperation("binaryInsertionSort_Assignments", size);
                    Operation binaryInsertion_comparisons = profiler.createOperation("binaryInsertionSort_Comparisons", size);

                    FillRandomArray(v1, size, 10, 50000, false, 2); //sorted in reverse order
                    copyVector(v2, v1, size);
                    bubbleSort(v2, size, &bubble_assignments, &bubble_comparisons);
                    copyVector(v2, v1, size);
                    insertionSort(v2, size, &insertion_assignments, &insertion_comparisons);
                    copyVector(v2, v1, size);
                    selectionSort(v2, size, &selection_assignments, &selection_comparisons);
                    copyVector(v2, v1, size);
                    binaryInsertionSort(v2, size, &binaryInsertion_assignments, &binaryInsertion_comparisons);
            }

            profiler.createGroup("Assignments", "bubbleSort_Assignments", "insertionSort_Assignments", "selectionSort_Assignments", "binaryInsertionSort_Assignments");
            profiler.createGroup("Comparisons", "bubbleSort_Comparisons", "insertionSort_Comparisons", "selectionSort_Comparisons", "binaryInsertionSort_Comparisons");

            profiler.addSeries("bubbleSort_Total", "bubbleSort_Assignments", "bubbleSort_Comparisons");
            profiler.addSeries("insertionSort_Total", "insertionSort_Assignments", "insertionSort_Comparisons");
            profiler.addSeries("selectionSort_Total", "selectionSort_Assignments", "selectionSort_Comparisons");
            profiler.addSeries("binaryInsertionSort_Total", "binaryInsertionSort_Assignments", "binaryInsertionSort_Comparisons");

            profiler.createGroup("Total", "bubbleSort_Total", "insertionSort_Total", "selectionSort_Total", "binaryInsertionSort_Total");
            break;
        }
    }
    void benchmark(Profiler& profiler, AnalysisCase whichCase)
    {
        // TODO: Write your benchmarks here, as explained in the readme
        // You should use profiler.startTimer/stopTimer for measuring and plotting measured time
    }

} // namespace lab01
