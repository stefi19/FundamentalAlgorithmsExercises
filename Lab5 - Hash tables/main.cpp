#include <iostream>
#include <string.h>

#include <cstring> // For strncpy_s
#include <ctime>   // For srand and rand
#include <fstream> // For file operations

using namespace std;

const int TABLESIZE = 10007; // Prime number near 10000 for optimal distribution
#define _CRT_SECURE_NO_WARNINGS


typedef enum slotStatus
{
    SlotEmpty, SlotOccupied, SlotRemoved
}SlotStatus;

typedef struct {
    int id;
    char name[30];
    SlotStatus status;
} Entry;

Entry** createHashTable() {
    int i;
    Entry** hashTable = (Entry**)malloc(TABLESIZE * sizeof(Entry*));
    for (i = 0; i < TABLESIZE; i++) {
        hashTable[i] = (Entry*)malloc(sizeof(Entry));
        hashTable[i]->status = SlotEmpty;//initially, all are empty
    }
    return hashTable;
}

/*int hashFunction(int key)
{
    return key % TABLESIZE;
}*/

int qudraticPobringComputeHash(int id, int attempt, int size) {
    return (id + attempt * attempt) % size;
}

bool insertEntry(Entry** hashTable, int id, char const name[30])
{
    int attempt = 0, index;
    while (attempt < TABLESIZE)
    {
        index = qudraticPobringComputeHash(id,attempt,TABLESIZE);
        if (hashTable[index]->status==SlotEmpty||hashTable[index]->status==SlotRemoved)
        {
            hashTable[index]->id = id;
            strncpy_s(hashTable[index]->name, sizeof(hashTable[index]->name), name, _TRUNCATE);
            //strncpy(hashTable[index]->name, name, sizeof(hashTable[index]->name) - 1);
            hashTable[index]->name[sizeof(hashTable[index]->name) - 1] = '\0';
            hashTable[index]->status = SlotOccupied;
            return true;
        }
        attempt++;
    }
    return false;//overflow
}

bool searchEntry(Entry** hashTable, int id, int &attempts)
{
    int attempt = 0, index;
    attempts = 0;
    while (attempt < TABLESIZE)
    {
        attempts++;
        index = qudraticPobringComputeHash(id, attempt, TABLESIZE);
        if (hashTable[index]->status==SlotEmpty)
            return false;
        if (hashTable[index]->id == id && hashTable[index]->status==SlotOccupied)
            return true; //found it, yessss
        attempt++;
    }
    return false;//not found after all the elements were probed
}

bool deleteEntry(Entry** hashTable, int id)
{
    int attempt = 0, index;
    while (attempt < TABLESIZE)
    {
        index = qudraticPobringComputeHash(id, attempt, TABLESIZE);
        if (hashTable[index]->status==SlotEmpty)
            return false;
        if (hashTable[index]->status == SlotOccupied&&hashTable[index]->id==id)
        {
            hashTable[index]->status = SlotRemoved;
            return true;
        }
        attempt++;
    }
    return false;
}

void displayTableContentsForDemo(Entry** hashTable) 
{
    cout << "Displaying a subset of the hash table contents:" << endl;
    for (int i = 0; i < 10; i++) { // Only display a subset of the table
        if (hashTable[i]->status == SlotEmpty) 
        {
            cout << "Index " << i << ": Empty" << endl;
        }
        else 
            if (hashTable[i]->status == SlotOccupied) 
            {
                cout << "Index " << i << ": ID " << hashTable[i]->id << ", Name " << hashTable[i]->name << endl;
            }
            else   
                if (hashTable[i]->status == SlotRemoved) 
                {
                    cout << "Index " << i << ": Removed" << endl;
                }
    }
}

const int DEMO_TABLE_SIZE = 10;

int demoHashFunction(int id, int attempt) 
{
    return (id + attempt * attempt) % DEMO_TABLE_SIZE;
}

bool demoInsertEntry(Entry** hashTable, int id, const char name[30]) 
{
    int attempt = 0, index;
    while (attempt < DEMO_TABLE_SIZE) 
    {
        index = demoHashFunction(id, attempt);
        if (hashTable[index]->status == SlotEmpty || hashTable[index]->status == SlotRemoved) 
        {
            hashTable[index]->id = id;
            strncpy_s(hashTable[index]->name, sizeof(hashTable[index]->name), name, _TRUNCATE);
            hashTable[index]->status = SlotOccupied;
            return true;
        }
        attempt++;
    }
    return false; // Table overflow
}
int demoSearchEntry(Entry** hashTable, int id, int& attempts) 
{
    int attempt = 0, index;
    attempts = 0;
    while (attempt < DEMO_TABLE_SIZE) 
    { 
        attempts++;
        index = demoHashFunction(id, attempt);
        if (hashTable[index]->status == SlotEmpty)
            return false; // Not found
        if (hashTable[index]->id == id && hashTable[index]->status == SlotOccupied)
            return true; // Found
        attempt++;
    }
    return false; // Not found after all attempts
}


void demo(Entry** table)
{
    //insert
    /*insertEntry(table, 1, "Alice");
    insertEntry(table, 2, "Bob");
    insertEntry(table, 3, "Charlie");
    insertEntry(table, 3, "Raluca");
    insertEntry(table, 4, "David");
    insertEntry(table, 5, "Stefi");
    insertEntry(table, 6, "Ale");
   // insertEntry(table, 6, "Raluca");
    displayTableContentsForDemo(table);*/
    demoInsertEntry(table, 1, "Alice");
    demoInsertEntry(table, 11, "Bob");
    demoInsertEntry(table, 21, "Charlie");
    cout << "Table after inserting colliding values:\n";
    displayTableContentsForDemo(table);

    deleteEntry(table, 1);
    cout << "\nTable after deleting ID 1 (Alice):\n";
    displayTableContentsForDemo(table);


    //search entries
    int attempts;
    cout << "Searching for ID 11: " << (demoSearchEntry(table, 11, attempts) ? "Found" : "Not found") << " (Attempts: " << attempts << ")" << endl;
    cout << "Searching for ID 1: " << (demoSearchEntry(table, 1, attempts) ? "Found" : "Not found") << " (Attempts: " << attempts << ")" << endl;
    cout << "Searching for ID 5: " << (demoSearchEntry(table, 5, attempts) ? "Found" : "Not found") << " (Attempts: " << attempts << ")" << endl;
    cout << "Searching for ID 10: " << (demoSearchEntry(table, 10, attempts) ? "Found" : "Not found") << " (Attempts: " << attempts << ")" << endl;

    //delete
    deleteEntry(table, 3);
    deleteEntry(table, 5);
    displayTableContentsForDemo(table);

}

// Writes results to CSV file
void writeResultsToCSV(const char* filename, float fillFactor, float avgEffortFound, int maxEffortFound, float avgEffortNotFound, int maxEffortNotFound) 
{
    ofstream file;
    file.open(filename, ios::app); // Open in append mode
    if (file.is_open()) 
    {
        file << fillFactor << "," << avgEffortFound << "," << maxEffortFound << "," << avgEffortNotFound << "," << maxEffortNotFound << endl;
        file.close();
    }
    else 
    {
        cerr << "Unable to open file for writing." << endl;
    }
}

void performanceEvaluation(Entry** hashTable, float fillFactor, const char* filename)
{
    int numberOfEntries = static_cast<int>(fillFactor * TABLESIZE), insertedIDs[TABLESIZE],insertedCount=0, foundTotalAttepts=0, notFoundTotalAttempts=0,maxFoundAttempts=0, maxNotFoundAttempts=0, attemmpts;
    srand(static_cast<unsigned int>(time(0)));
    int i, id, j;
    bool unique;
    for (i = 0; i < numberOfEntries; i++)
    {
        id = rand() % 100000 + 1;
        unique = true;
        for (j = 0; j < insertedCount; j++)
        {
            if (insertedIDs[j] == id)
            {
                unique = false;
                break;
            }
        }
        if (unique)
        {
            insertedIDs[insertedCount++] = id;
            insertEntry(hashTable, id, "Random");
        }
        else
            i--; //retry if ID is not unique
    }

    //search for 1500 IDs that are in the table
    for (i = 0; i < 1500; i++)
    {
        id = insertedIDs[rand() % insertedCount];
        if (searchEntry(hashTable, id, attemmpts))
        {
            foundTotalAttepts = foundTotalAttepts + attemmpts;
            maxFoundAttempts = max(maxFoundAttempts, attemmpts);
        }
    }

    //search for 1500 IDs that are not in the table
    for (int i = 0; i < 1500; i++) 
    {
        int id = rand() % 100000 + 100000; // Generate ID outside the inserted range
        if (!searchEntry(hashTable, id, attemmpts)) 
        {
            notFoundTotalAttempts += attemmpts;
            maxNotFoundAttempts = max(maxNotFoundAttempts, attemmpts);
        }
    }
    // Display results
    cout << "Fill Factor: " << fillFactor << endl;
    cout << "Avg. Effort (Found): " << static_cast<float>(foundTotalAttepts) / 1500 << endl;
    cout << "Max. Effort (Found): " << maxFoundAttempts << endl;
    cout << "Avg. Effort (Not Found): " << static_cast<float>(notFoundTotalAttempts) / 1500 << endl;
    cout << "Max. Effort (Not Found): " << maxNotFoundAttempts << endl << endl;

    // Calculate averages
    float avgEffortFound = static_cast<float>(foundTotalAttepts) / 1500;
    float avgEffortNotFound = static_cast<float>(notFoundTotalAttempts) / 1500;

    // Write to CSV
    writeResultsToCSV(filename, fillFactor, avgEffortFound, maxFoundAttempts, avgEffortNotFound, maxNotFoundAttempts);
}

void evaluateAfterDeletion(Entry** hashTable) {
    int insertedIDs[TABLESIZE], insertedCount = 0, attempts,id,j, i;
    bool unique;
    // Fill the table up to a 0.99 fill factor
    int numEntries = static_cast<int>(0.99 * TABLESIZE);
    for (i = 0; i < numEntries; i++) 
    {
        id = rand() % 100000 + 1;
        // Ensure unique ID insertion
        unique = true;
        for (j = 0; j < insertedCount; j++) 
        {
            if (insertedIDs[j] == id) 
            {
                unique = false;
                break;
            }
        }
        if (unique) 
        {
            insertedIDs[insertedCount++] = id;
            insertEntry(hashTable, id, "Random");
        }
        else {
            i--; // Retry if ID is not unique
        }
    }

    // Delete entries until fill factor reduces to 0.8
    int deleteCount = numEntries - static_cast<int>(0.8 * TABLESIZE);
    for (i = 0; i < deleteCount; i++) 
    {
        deleteEntry(hashTable, insertedIDs[i]);
    }

    // Evaluate search performance with 1500 found and 1500 not-found cases
    int foundTotalAttempts = 0, notFoundTotalAttempts = 0, maxFoundAttempts = 0, maxNotFoundAttempts = 0;

    for (i = 0; i < 1500; i++) 
    {
        id = insertedIDs[rand() % insertedCount];
        if (searchEntry(hashTable, id, attempts)) 
        {
            foundTotalAttempts += attempts;
            maxFoundAttempts = max(maxFoundAttempts, attempts);
        }
    }

    for (i = 0; i < 1500; i++) {
        id = rand() % 100000 + 100000;
        if (!searchEntry(hashTable, id, attempts)) 
        {
            notFoundTotalAttempts += attempts;
            maxNotFoundAttempts = max(maxNotFoundAttempts, attempts);
        }
    }

    // Display results after deletion
    cout << "After Deletion (Fill Factor 0.8):" << endl;
    cout << "Avg. Effort (Found): " << static_cast<float>(foundTotalAttempts) / 1500 << endl;
    cout << "Max. Effort (Found): " << maxFoundAttempts << endl;
    cout << "Avg. Effort (Not Found): " << static_cast<float>(notFoundTotalAttempts) / 1500 << endl;
    cout << "Max. Effort (Not Found): " << maxNotFoundAttempts << endl << endl;
}

int main()
{
    Entry** hashTable = createHashTable();
    demo(hashTable);
    const char* filename = "hash_table_performance.csv";
    ofstream file;
    file.open(filename);
    if (file.is_open()) 
    {
        file << "Filling factor,Avg. Effort found,Max. Effort found,Avg. Effort not-found,Max. Effort not-found" << endl;
        file.close();
    }

    // Evaluate performance for different fill factors
    float fillFactors[] = { 0.8, 0.85, 0.9, 0.95, 0.99 };
    for (float fillFactor : fillFactors) {
        performanceEvaluation(hashTable, fillFactor, filename);

        // Reset the hash table between evaluations
        for (int i = 0; i < TABLESIZE; i++) {
            hashTable[i]->status = SlotEmpty;
        }
    }

    // Evaluate search performance after deletion
    evaluateAfterDeletion(hashTable);

    //clean
    int i;
    for (i = 0; i < TABLESIZE; i++) 
    {
        delete hashTable[i];
    }
    delete[] hashTable;
}