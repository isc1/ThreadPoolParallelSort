// ThreadPoolParallelSort -- by Claude Opus 4.
// Where totalCores is (probably) the number of cpu cores, allocate totalCores-1 threads to sort a vector
// sort a vector of random numbers.  This is a basic example of Qt multithreading.

// main.cpp
#include <QCoreApplication>
#include <QThreadPool>
#include <QRunnable>
#include <QThread>
#include <QDebug>
#include <QElapsedTimer>
#include <QMutex>
#include <vector>
#include <algorithm>
#include <random>
#include <atomic>

// Global mutex for thread-safe printing
QMutex printMutex;

// Task to sort a chunk of the vector
class SortTask : public QRunnable {
private:
    std::vector<int>* data;
    int startIndex;
    int endIndex;
    int taskId;

public:
    SortTask(std::vector<int>* vec, int start, int end, int id)
        : data(vec), startIndex(start), endIndex(end), taskId(id) {
        setAutoDelete(true);  // QThreadPool will delete this after run()
    }

    void run() override {
        {
            QMutexLocker locker(&printMutex);
            qDebug() << "[Thread" << QThread::currentThreadId() << "] Task" << taskId
                     << "sorting range [" << startIndex << "-" << endIndex << ")";
        }

        // Sort this chunk of the vector
        std::sort(data->begin() + startIndex, data->begin() + endIndex);

        {
            QMutexLocker locker(&printMutex);
            qDebug() << "[Thread" << QThread::currentThreadId() << "] Task" << taskId
                     << "completed sorting";
        }
    }
};

// Task to merge sorted chunks
class MergeTask : public QRunnable {
private:
    std::vector<int>* data;
    int start1, end1;  // First sorted range
    int start2, end2;  // Second sorted range
    int taskId;

public:
    MergeTask(std::vector<int>* vec, int s1, int e1, int s2, int e2, int id)
        : data(vec), start1(s1), end1(e1), start2(s2), end2(e2), taskId(id) {
        setAutoDelete(true);
    }

    void run() override {
        {
            QMutexLocker locker(&printMutex);
            qDebug() << "[Thread" << QThread::currentThreadId() << "] Merge Task" << taskId
                     << "merging ranges [" << start1 << "-" << end1 << ") and ["
                     << start2 << "-" << end2 << ")";
        }

        // Create temporary vector for merged result
        std::vector<int> temp;
        temp.reserve(end2 - start1);

        // Merge the two sorted ranges
        std::merge(data->begin() + start1, data->begin() + end1,
                   data->begin() + start2, data->begin() + end2,
                   std::back_inserter(temp));

        // Copy back to original vector
        std::copy(temp.begin(), temp.end(), data->begin() + start1);

        {
            QMutexLocker locker(&printMutex);
            qDebug() << "[Thread" << QThread::currentThreadId() << "] Merge Task" << taskId
                     << "completed";
        }
    }
};

class ParallelSorter {
private:
    QThreadPool* threadPool;
    std::vector<int>* data;

public:
    ParallelSorter(std::vector<int>* vec) : data(vec) {
        threadPool = new QThreadPool();
        int totalCores = QThread::idealThreadCount();
        int usableCores = std::max(1, totalCores - 1);  // Leave 1 core for OS

        threadPool->setMaxThreadCount(usableCores);

        qDebug() << "System has" << totalCores << "cores";
        qDebug() << "Using" << usableCores << "threads for sorting";
        qDebug() << "Main thread ID:" << QThread::currentThreadId();
    }

    ~ParallelSorter() {
        delete threadPool;
    }

    void parallelSort() {
        int vectorSize = data->size();
        int numThreads = threadPool->maxThreadCount();
        int chunkSize = vectorSize / numThreads;

        qDebug() << "=== PHASE 1: Sorting chunks in parallel ===";
        qDebug() << "Vector size:" << vectorSize;
        qDebug() << "Chunk size:" << chunkSize;

        // Phase 1: Sort chunks in parallel
        for (int i = 0; i < numThreads; i++) {
            int start = i * chunkSize;
            int end = (i == numThreads - 1) ? vectorSize : (i + 1) * chunkSize;

            SortTask* task = new SortTask(data, start, end, i);
            threadPool->start(task);
        }

        // Wait for all sorting tasks to complete
        threadPool->waitForDone();

        qDebug() << "=== PHASE 2: Merging sorted chunks ===";

        // Phase 2: Merge sorted chunks
        // We'll do a tree-like merge for efficiency
        int numChunks = numThreads;
        int mergeTaskId = 0;

        while (numChunks > 1) {
            int newNumChunks = 0;

            for (int i = 0; i < numChunks; i += 2) {
                if (i + 1 < numChunks) {
                    // Merge two adjacent chunks
                    int start1 = (i * vectorSize) / numThreads;
                    int end1 = ((i + 1) * vectorSize) / numThreads;
                    int start2 = end1;
                    int end2 = ((i + 2) * vectorSize) / numThreads;

                    if (i + 2 == numChunks) {
                        end2 = vectorSize;  // Last chunk might be larger
                    }

                    MergeTask* mergeTask = new MergeTask(data, start1, end1, start2, end2, mergeTaskId++);
                    threadPool->start(mergeTask);

                    newNumChunks++;
                }
            }

            threadPool->waitForDone();
            numChunks = newNumChunks;
        }

        qDebug() << "=== Sorting complete! ===";
    }
};

// Helper function to verify the vector is sorted
bool isSorted(const std::vector<int>& vec) {
    for (size_t i = 1; i < vec.size(); i++) {
        if (vec[i] < vec[i-1]) {
            return false;
        }
    }
    return true;
}

// Helper function to print first and last elements
void printSample(const std::vector<int>& vec, const QString& label) {
    QMutexLocker locker(&printMutex);
    qDebug() << label;

    // Create QVector and copy elements manually for Qt 5.13 compatibility
    int firstCount = std::min(10, (int)vec.size());
    QVector<int> first;
    for (int i = 0; i < firstCount; i++) {
        first.append(vec[i]);
    }
    qDebug() << "First 10 elements:" << first;

    int lastCount = std::min(10, (int)vec.size());
    QVector<int> last;
    for (size_t i = vec.size() - lastCount; i < vec.size(); i++) {
        last.append(vec[i]);
    }
    qDebug() << "Last 10 elements:" << last;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Create a vector of random integers
    const int VECTOR_SIZE = 1000000;  // 1 million integers

    std::vector<int> data(VECTOR_SIZE);

    // Fill with random numbers
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);

    qDebug() << "Generating" << VECTOR_SIZE << "random integers...";
    for (int i = 0; i < VECTOR_SIZE; i++) {
        data[i] = dis(gen);
    }

    printSample(data, "\nOriginal vector (unsorted):");

    // Time the parallel sort
    QElapsedTimer timer;
    timer.start();

    // Perform parallel sort
    ParallelSorter sorter(&data);
    sorter.parallelSort();

    qint64 parallelTime = timer.elapsed();

    // Verify the result
    bool sorted = isSorted(data);
    qDebug() << "\nVector is sorted:" << sorted;

    printSample(data, "\nSorted vector:");

    qDebug() << "\nParallel sort took:" << parallelTime << "ms";

    // For comparison, let's time a single-threaded sort
    qDebug() << "\nNow testing single-threaded sort for comparison...";

    // Regenerate random data
    for (int i = 0; i < VECTOR_SIZE; i++) {
        data[i] = dis(gen);
    }

    timer.restart();
    std::sort(data.begin(), data.end());
    qint64 singleThreadTime = timer.elapsed();

    qDebug() << "Single-threaded sort took:" << singleThreadTime << "ms";
    qDebug() << "Speedup:" << QString::number((double)singleThreadTime / parallelTime, 'f', 2) << "x";

    return 0;
}
