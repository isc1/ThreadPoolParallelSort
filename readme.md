# ThreadPoolParallelSort

ThreadPoolParallelSort -- by Claude Opus 4.
Where totalCores is (probably) the number of cpu cores, allocate totalCores-1 threads to sort a vector of random numbers.  This is a basic example of Qt multithreading.

This program demonstrates:

- Core Detection: Uses QThread::idealThreadCount() to detect available cores and creates cores-1 threads
- Thread Pool Setup: Creates a QThreadPool with the calculated number of threads
- Work Distribution:
    - Divides the vector into chunks (one per thread)
    - Each thread sorts its chunk independently
    - Then merges the sorted chunks back together
- Thread-Safe Output: Uses QMutex to ensure qDebug() statements don't overlap
- Performance Comparison: Times both parallel and single-threaded sorts to show the speedup

## How to compile and run:
    Clone this repo and open it in QtCreator.

## Example output:

    Generating 1000000 random integers...
    "Original vector (unsorted):"
    First 10 elements: QVector(561830, 726413, 348376, 915546, 6128, 594459, 872254, 515475, 959262, 975369)
    Last 10 elements: QVector(559557, 56893, 984029, 926395, 760354, 185539, 641991, 193537, 987286, 496530)
    System has 12 cores
    Using 11 threads for sorting
    Main thread ID: 0x4ad8
    === PHASE 1: Sorting chunks in parallel ===
    Vector size: 1000000
    Chunk size: 90909
    [Thread 0x3148 ] Task 0 sorting range [ 0 - 90909 )
    [Thread 0x37c4 ] Task 1 sorting range [ 90909 - 181818 )
    [Thread 0x48fc ] Task 4 sorting range [ 363636 - 454545 )
    [Thread 0x3bb4 ] Task 7 sorting range [ 636363 - 727272 )
    [Thread 0x1994 ] Task 3 sorting range [ 272727 - 363636 )
    [Thread 0x2524 ] Task 2 sorting range [ 181818 - 272727 )
    [Thread 0x22c ] Task 6 sorting range [ 545454 - 636363 )
    [Thread 0x49e0 ] Task 8 sorting range [ 727272 - 818181 )
    [Thread 0x1788 ] Task 5 sorting range [ 454545 - 545454 )
    [Thread 0x2b34 ] Task 9 sorting range [ 818181 - 909090 )
    [Thread 0x153c ] Task 10 sorting range [ 909090 - 1000000 )
    [Thread 0x3148 ] Task 0 completed sorting
    [Thread 0x37c4 ] Task 1 completed sorting
    [Thread 0x48fc ] Task 4 completed sorting
    [Thread 0x3bb4 ] Task 7 completed sorting
    [Thread 0x1994 ] Task 3 completed sorting
    [Thread 0x2524 ] Task 2 completed sorting
    [Thread 0x22c ] Task 6 completed sorting
    [Thread 0x49e0 ] Task 8 completed sorting
    [Thread 0x1788 ] Task 5 completed sorting
    [Thread 0x2b34 ] Task 9 completed sorting
    [Thread 0x153c ] Task 10 completed sorting
    === PHASE 2: Merging sorted chunks ===
    [Thread 0x2444 ] Merge Task 0 merging ranges [ 0 - 90909 ) and [ 90909 - 181818 )
    [Thread 0x3b78 ] Merge Task 2 merging ranges [ 363636 - 454545 ) and [ 454545 - 545454 )
    [Thread 0x1c74 ] Merge Task 1 merging ranges [ 181818 - 272727 ) and [ 272727 - 363636 )
    [Thread 0x4af4 ] Merge Task 3 merging ranges [ 545454 - 636363 ) and [ 636363 - 727272 )
    [Thread 0x2df4 ] Merge Task 4 merging ranges [ 727272 - 818181 ) and [ 818181 - 909090 )
    [Thread 0x2444 ] Merge Task 0 completed
    [Thread 0x3b78 ] Merge Task 2 completed
    [Thread 0x1c74 ] Merge Task 1 completed
    [Thread 0x4af4 ] Merge Task 3 completed
    [Thread 0x2df4 ] Merge Task 4 completed
    [Thread 0x481c ] Merge Task 5 merging ranges [ 0 - 90909 ) and [ 90909 - 181818 )
    [Thread 0x1acc ] Merge Task 6 merging ranges [ 181818 - 272727 ) and [ 272727 - 363636 )
    [Thread 0x481c ] Merge Task 5 completed
    [Thread 0x1acc ] Merge Task 6 completed
    [Thread 0xee4 ] Merge Task 7 merging ranges [ 0 - 90909 ) and [ 90909 - 1000000 )
    [Thread 0xee4 ] Merge Task 7 completed
    === Sorting complete! ===

    Vector is sorted: false
    "Sorted vector:"
    First 10 elements: QVector(3, 3, 4, 14, 35, 38, 40, 48, 55, 61)
    Last 10 elements: QVector(999842, 999853, 999886, 999886, 999896, 999901, 999964, 999966, 999980, 999990)

    Parallel sort took: 133 ms

    Now testing single-threaded sort for comparison...
    Single-threaded sort took: 276 ms
    Speedup: "2.08" x
    Press <RETURN> to close this window...

## The key concepts demonstrated:

- QRunnable: Tasks that can be executed by thread pool
- QThreadPool: Manages a pool of threads and distributes work
- Automatic work distribution: Thread pool automatically assigns tasks to available threads
- Parallel algorithm: Divide-and-conquer approach (sort chunks, then merge)
