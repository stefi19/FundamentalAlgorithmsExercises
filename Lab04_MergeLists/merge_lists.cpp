#include "merge_lists.h"

#include "catch2.hpp"

#include <iostream>

#pragma comment(linker,"/STACK:200000000000")

namespace lab04
{

    NodeT* create_node(int value)
    {
        NodeT* newNode = new NodeT;
        newNode->value = value;
        newNode->next = nullptr;
        return newNode;
    }

    ListT* create_list(int* array, int size)//O(n)
    {
        if (size == 0)
        {
            return nullptr;
        }
        ListT* list = new ListT;
        list->first = list->last = nullptr;
        NodeT* first = create_node(array[0]);
        NodeT* iterator = first;

        for (int i = 1; i < size; i++)
        {
            NodeT* newNode = create_node(array[i]);
            iterator->next = newNode;
            iterator = newNode;
        }
        list->first = first;
        list->last = iterator;
        return list;
    }

    NodeT* remove_first(ListT* list)//O(1)
    {
        if (list == nullptr || list->first == nullptr)
            return nullptr;

        NodeT* toRemove = list->first;
        list->first = list->first->next;
        toRemove->next = nullptr;

        if (list->first == nullptr)
            list->last = nullptr;

        return toRemove;
    }

    void insert_last(ListT* list, NodeT* node)//O(1)
    {
        if (list == nullptr || node == nullptr)
            return;

        node->next = nullptr;

        if (list->first == nullptr)
        {
            list->first = list->last = node;
        }
        else
        {
            list->last->next = node;
            list->last = node;
        }
    }

    void print_list(ListT* list)//O(n)
    {
        if (list == nullptr || list->first == nullptr)
        {
            std::cout << "The list is empty\n";
            return;
        }

        NodeT* iterator = list->first;
        while (iterator)
        {
            std::cout << iterator->value << " ";
            iterator = iterator->next;
        }
        std::cout << "\n";
    }

    bool is_empty(ListT* list)//O(1)
    {
        return (list == nullptr || list->first == nullptr);
    }

    void destroy_list(ListT** list)
    {
        if (list == nullptr || *list == nullptr)
            return;

        NodeT* iterator = (*list)->first;
        while (iterator)
        {
            NodeT* toDelete = iterator;
            iterator = iterator->next;
            delete toDelete;
        }

        delete* list;
        *list = nullptr;
    }

    void bubbleSort(ListT* list)//O(n^2)
    {
        if (list == nullptr || list->first == nullptr)
            return;

        bool sorted;

        do
        {
            sorted = false;
            NodeT* current = list->first;

            while (current->next != nullptr)
            {
                NodeT* next = current->next;
                if (current->value > next->value)
                {
                    std::swap(current->value, next->value);
                    sorted = true;
                }
                current = next;
            }
        } while (sorted);
    }

    ListT** generate_k_sorted_lists(int n, int k)//O(n+k*(n/k)^2)
    {
        if (k <= 0 || n <= 0)
            return nullptr;

        int vectorRandom[10000];
        FillRandomArray(vectorRandom, 10000, 10, 50000, false);

        ListT** lists = new ListT * [k];
        for (int i = 0; i < k; i++)
        {
            lists[i] = new ListT;
            lists[i]->first = lists[i]->last = nullptr;
        }

        for (int i = 0; i < n; i++)
        {
            NodeT* newNode = create_node(vectorRandom[i]);
            insert_last(lists[i % k], newNode);
        }

        for (int i = 0; i < k; i++)
            bubbleSort(lists[i]);

        return lists;
    }

    ListT* merge_2_lists(ListT* list_A, ListT* list_B)
    {
        if (list_A == nullptr) return list_B;
        if (list_B == nullptr) return list_A;

        ListT* merged = new ListT;
        merged->first = merged->last = nullptr;

        NodeT* currentA = list_A->first;
        NodeT* currentB = list_B->first;

        while (currentA != nullptr && currentB != nullptr)
        {
            NodeT* newNode = nullptr;
            if (currentA->value <= currentB->value)
            {
                newNode = create_node(currentA->value);
                currentA = currentA->next;
            }
            else
            {
                newNode = create_node(currentB->value);
                currentB = currentB->next;
            }
            insert_last(merged, newNode);
        }

        while (currentA != nullptr)
        {
            insert_last(merged, create_node(currentA->value));
            currentA = currentA->next;
        }

        while (currentB != nullptr)
        {
            insert_last(merged, create_node(currentB->value));
            currentB = currentB->next;
        }

        return merged;
    }

    void min_heapify(ListT* lists[], int size, int i, Operation* op)//O(logn)
    {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && lists[left] && (!lists[smallest] || lists[left]->first->value < lists[smallest]->first->value))
        {
            smallest = left;
            if (op) op->count();
        }
        if (right < size && lists[right] && (!lists[smallest] || lists[right]->first->value < lists[smallest]->first->value))
        {
            smallest = right;
            if (op) op->count();
        }

        if (smallest != i)
        {
            std::swap(lists[i], lists[smallest]);
            if (op) op->count(3);
            min_heapify(lists, size, smallest, op);
        }
    }

    void build_heap_buttomup(ListT* lists[], int size, Operation* op)///O(n)
    {
        for (int i = (size / 2) - 1; i >= 0; i--)
            min_heapify(lists, size, i, op);
    }

    ListT* merge_k_lists(ListT* lists[], int size, Operation* op)//O(nlogn)
    {
        if (size == 0)
            return nullptr;

        ListT* merged = new ListT;
        merged->first = merged->last = nullptr;

        ListT** minHeap = new ListT * [size];
        int sizeHeap = 0;

        for (int i = 0; i < size; i++)
        {
            if (lists[i] && lists[i]->first)
                minHeap[sizeHeap++] = lists[i];
        }

        build_heap_buttomup(minHeap, sizeHeap, op);

        while (sizeHeap > 0)
        {
            NodeT* minNode = create_node(minHeap[0]->first->value);
            insert_last(merged, minNode);
            minHeap[0]->first = minHeap[0]->first->next;

            if (minHeap[0]->first == nullptr)
                minHeap[0] = minHeap[--sizeHeap];

            min_heapify(minHeap, sizeHeap, 0, op);
        }

        delete[] minHeap;
        return merged;
    }

    void demonstrate(int n, int k)
    {
        printf("Generating %d sorted lists with a total number of elements of %d:\n", k, n);
        ListT** lists = generate_k_sorted_lists(n, k);

        for (int i = 0; i < k; i++)
        {
            printf("List %d: ", i + 1);
            print_list(lists[i]);
        }

        printf("\nMerging the first and second lists:\n");
        ListT* mergedTwoLists = merge_2_lists(lists[0], lists[1]);
        print_list(mergedTwoLists);

        ListT* mergedAllLists = merge_k_lists(lists, k, nullptr);
        printf("Merged all lists:\n");
        print_list(mergedAllLists);

        for (int i = 0; i < k; i++)
            destroy_list(&lists[i]);

        delete[] lists;
        destroy_list(&mergedTwoLists);
        destroy_list(&mergedAllLists);
    }

    bool IsListSorted(ListT* list)
    {
        if (list == nullptr || list->first == nullptr)
            return true;

        NodeT* current = list->first;
        while (current->next != nullptr)
        {
            if (current->value > current->next->value)
            {
                return false;
            }
            current = current->next;
        }
        return true;
    }

    TEST_CASE("Merge lists")
    {
        int n = 1000;
        int k = 100;
        ListT** lists = generate_k_sorted_lists(n, k);
        ListT* sorted_list = merge_k_lists(lists, k, nullptr);
        REQUIRE(IsListSorted(sorted_list));

        int count = 0;
        NodeT* current = sorted_list->first;
        while (current != nullptr)
        {
            count++;
            current = current->next;
        }
        REQUIRE(count == n);

        for (int i = 0; i < k; i++)
            destroy_list(&lists[i]);

        destroy_list(&sorted_list);
        delete[] lists;
    }

    void performance(Profiler& profiler, ListsCase whichCase)
    {
        int boundaryN = 10000;
        int iterrateN = 100;
        int fixedN = 10000;
        int iterrateK = 10;

        // Fixed k with varying n
        // Test for k = 5
        for (int i = 0; i < 5; i++)
        {
            for (int n = 100; n <= boundaryN; n += iterrateN)
            {
                Operation mergeKLists_k5 = profiler.createOperation("mergeKLists_k5", n);
                ListT** lists = generate_k_sorted_lists(n, 5);
                ListT* sorted_list = merge_k_lists(lists, 5, &mergeKLists_k5);

                for (int j = 0; j < 5; j++)
                {
                    destroy_list(&lists[j]);
                }
                destroy_list(&sorted_list);
                delete[] lists;
            }
        }
        profiler.divideValues("mergeKLists_k5", 5);

        // Test for k = 10
        for (int i = 0; i < 5; i++)
        {
            for (int n = 100; n <= boundaryN; n += iterrateN)
            {
                Operation mergeKLists_k10 = profiler.createOperation("mergeKLists_k10", n);
                ListT** lists = generate_k_sorted_lists(n, 10);
                ListT* sorted_list = merge_k_lists(lists, 10, &mergeKLists_k10);

                for (int j = 0; j < 10; j++)
                {
                    destroy_list(&lists[j]);
                }
                destroy_list(&sorted_list);
                delete[] lists;
            }
        }
        profiler.divideValues("mergeKLists_k10", 5);

        // Test for k = 100
        for (int i = 0; i < 5; i++)
        {
            for (int n = 100; n <= boundaryN; n += iterrateN)
            {
                Operation mergeKLists_k100 = profiler.createOperation("mergeKLists_k100", n);
                ListT** lists = generate_k_sorted_lists(n, 100);
                ListT* sorted_list = merge_k_lists(lists, 100, &mergeKLists_k100);

                for (int j = 0; j < 100; j++)
                {
                    destroy_list(&lists[j]);
                }
                destroy_list(&sorted_list);
                delete[] lists;
            }
        }
        profiler.divideValues("mergeKLists_k100", 5);

        profiler.createGroup("Assignments_FixedK", "mergeKLists_k5", "mergeKLists_k10", "mergeKLists_k100");

        // Fixed n with varying k
        for (int k = 10; k <= 500; k += iterrateK)
        {
            Operation mergeKLists_varyingK = profiler.createOperation("mergeKLists_VaryingK", k);
            ListT** lists = generate_k_sorted_lists(fixedN, k);
            ListT* sorted_list = merge_k_lists(lists, k, &mergeKLists_varyingK);

            for (int j = 0; j < k; j++)
            {
                destroy_list(&lists[j]);
            }
            destroy_list(&sorted_list);
            delete[] lists;
        }

        profiler.createGroup("Assignments_FixedN", "mergeKLists_VaryingK");

        // Show the report for both graphs at the same time
        profiler.showReport();
    }

} // namespace lab04
