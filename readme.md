# ThreadPoolParallelSort

ThreadPoolParallelSort -- by Claude Opus 4.
Where totalCores is (probably) the number of cpu cores, allocate totalCores-1 threads to sort a vector sort a vector of random numbers.  This is a basic example of Qt multithreading.

This program demonstrates:

Core Detection: Uses QThread::idealThreadCount() to detect available cores and creates cores-1 threads
Thread Pool Setup: Creates a QThreadPool with the calculated number of threads
Work Distribution:

Divides the vector into chunks (one per thread)
Each thread sorts its chunk independently
Then merges the sorted chunks back together


Thread-Safe Output: Uses QMutex to ensure qDebug() statements don't overlap
Performance Comparison: Times both parallel and single-threaded sorts to show the speedup

How to compile and run:
bashqmake
make
./threadpool_sort
Example output:
System has 8 cores
Using 7 threads for sorting
Main thread ID: 0x7f8b2c003600

Original vector (unsorted):
First 10 elements: QVector(423569, 894732, 123456, ...)

=== PHASE 1: Sorting chunks in parallel ===
[Thread 0x7f8b2c003700] Task 0 sorting range [0-142857)
[Thread 0x7f8b2c003800] Task 1 sorting range [142857-285714)
...

=== PHASE 2: Merging sorted chunks ===
[Thread 0x7f8b2c003700] Merge Task 0 merging ranges...

Vector is sorted: true

Parallel sort took: 145 ms
Single-threaded sort took: 423 ms
Speedup: 2.92x
The key concepts demonstrated:

QRunnable: Tasks that can be executed by thread pool
QThreadPool: Manages a pool of threads and distributes work
Automatic work distribution: Thread pool automatically assigns tasks to available threads
Parallel algorithm: Divide-and-conquer approach (sort chunks, then merge)

This gives you a foundation for applying the same concepts to your creature simulation!