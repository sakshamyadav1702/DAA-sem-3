#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    char question[120];
    char options[4][40];
    int correctIndex;
} Question;

int sessionScore = 0;

int getIntInput(const char *prompt, int min, int max) {
    int value, result, c;

    while (1) {
        printf("%s", prompt);
        result = scanf("%d", &value);
        while ((c = getchar()) != '\n' && c != EOF);

        if (result == EOF) {
            printf("\nNo more input available. Exiting SortQuest.\n");
            exit(0);
        }
        if (result != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Invalid choice. Please enter a number between %d and %d.\n", min, max);
            continue;
        }
        return value;
    }
}

void pauseScreen(void) {
    int c;
    printf("\nPress Enter to continue...");
    while ((c = getchar()) != '\n' && c != EOF);
}

void printBanner(const char *title) {
    printf("\n========================================\n%s\n========================================\n", title);
}

void displayRange(int arr[], int start, int end) {
    int i;
    for (i = start; i <= end; i++) printf("%d ", arr[i]);
    printf("\n");
}

void displayArray(int arr[], int n) {
    displayRange(arr, 0, n - 1);
}

void generateArray(int arr[], int n) {
    int i, j, duplicate;
    for (i = 0; i < n; i++) {
        do {
            arr[i] = rand() % 99 + 1;
            duplicate = 0;
            for (j = 0; j < i; j++)
                if (arr[j] == arr[i]) duplicate = 1;
        } while (duplicate);
    }
}

void swapInts(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void printLearningInfo(const char *best, const char *average, const char *worst,
                       const char *space, const char *advantage, const char *disadvantage) {
    printf("\nBest Case Complexity : %s\n", best);
    printf("Average Case Complexity : %s\n", average);
    printf("Worst Case Complexity : %s\n", worst);
    printf("Space Complexity : %s\n", space);
    printf("\nAdvantage: %s\n", advantage);
    printf("Disadvantage: %s\n", disadvantage);
}

void showLevelSummary(const char *algoName, int correct, int wrong,
                      int arr[], int n, const char *worstCase) {
    int score = correct * 10;
    int total = (correct + wrong) * 10;

    printf("\n===================================\n");
    printf("LEVEL COMPLETE\n");
    printf("===================================\n");
    printf("Algorithm: %s\n", algoName);
    printf("Correct Answers: %d\n", correct);
    printf("Wrong Answers: %d\n", wrong);
    printf("Score: %d / %d\n", score, total);
    printf("\nFinal Sorted Array:\n");
    displayArray(arr, n);
    printf("\nWorst Case Complexity:\n%s\n", worstCase);
    printf("===================================\n");

    sessionScore += score;
    pauseScreen();
}

void learnBubbleSort(void) {
    int arr[] = {5, 3, 8, 2};
    int n = 4, i, j, swapped;

    printBanner("BUBBLE SORT");
    printf("What it does:\n"
           "Bubble Sort repeatedly compares adjacent elements and swaps\n"
           "them if they are in the wrong order. With every pass the\n"
           "largest remaining element 'bubbles' up to its correct place.\n\n"
           "Example Array:\n");
    displayArray(arr, n);
    printf("\n");

    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        for (j = 0; j < n - 1 - i; j++) {
            printf("Compare %d and %d\n", arr[j], arr[j + 1]);
            if (arr[j] > arr[j + 1]) {
                printf("%d > %d, therefore swap.\n", arr[j], arr[j + 1]);
                swapInts(&arr[j], &arr[j + 1]);
                swapped = 1;
            } else {
                printf("No swap required.\n");
            }
            displayArray(arr, n);
            printf("\n");
        }
        if (!swapped) break;
    }

    printf("Sorted Array:\n");
    displayArray(arr, n);
    printLearningInfo("O(n)      (optimized version)", "O(n^2)", "O(n^2)", "O(1)",
                      "Very simple to understand and implement.",
                      "Very slow on large datasets.");
    pauseScreen();
}

void learnSelectionSort(void) {
    int arr[] = {8, 4, 2, 7, 1};
    int n = 5, i, j, minIdx;

    printBanner("SELECTION SORT");
    printf("What it does:\n"
           "Selection Sort repeatedly finds the smallest element from the\n"
           "unsorted part of the array and swaps it to the front of that\n"
           "unsorted part.\n\n"
           "Example Array:\n");
    displayArray(arr, n);
    printf("\n");

    for (i = 0; i < n - 1; i++) {
        printf("Current unsorted section:\n");
        displayRange(arr, i, n - 1);

        minIdx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[minIdx]) minIdx = j;

        printf("Smallest value in this section: %d\n", arr[minIdx]);
        if (minIdx != i) {
            printf("Swap %d with %d\n", arr[minIdx], arr[i]);
            swapInts(&arr[i], &arr[minIdx]);
        }
        displayArray(arr, n);
        printf("\n");
    }

    printf("Sorted Array:\n");
    displayArray(arr, n);
    printLearningInfo("O(n^2)", "O(n^2)", "O(n^2)", "O(1)",
                      "Performs very few swaps compared to Bubble Sort.",
                      "Still O(n^2) even if the array is already sorted.");
    pauseScreen();
}

void learnInsertionSort(void) {
    int arr[] = {3, 7, 4, 8, 2};
    int n = 5, i, j, key;

    printBanner("INSERTION SORT");
    printf("What it does:\n"
           "Insertion Sort builds a sorted portion of the array one\n"
           "element at a time, inserting each new element into its\n"
           "correct position among the already sorted elements.\n\n"
           "Example Array:\n");
    displayArray(arr, n);
    printf("\n");

    for (i = 1; i < n; i++) {
        printf("Sorted portion:\n");
        displayRange(arr, 0, i - 1);
        key = arr[i];
        printf("Current element:\n%d\n", key);

        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;

        printf("Array after insertion:\n");
        displayArray(arr, n);
        printf("\n");
    }

    printf("Sorted Array:\n");
    displayArray(arr, n);
    printLearningInfo("O(n)     (already sorted array)", "O(n^2)", "O(n^2)", "O(1)",
                      "Very efficient for small or nearly sorted arrays.",
                      "Inefficient for large, randomly ordered arrays.");
    pauseScreen();
}

void mergeDemo(int arr[], int l, int m, int r) {
    int left[50], right[50];
    int n1 = m - l + 1, n2 = r - m;
    int i = 0, j = 0, k = l;

    for (i = 0; i < n1; i++) left[i] = arr[l + i];
    for (j = 0; j < n2; j++) right[j] = arr[m + 1 + j];

    i = j = 0;
    while (i < n1 && j < n2)
        arr[k++] = (left[i] <= right[j]) ? left[i++] : right[j++];
    while (i < n1) arr[k++] = left[i++];
    while (j < n2) arr[k++] = right[j++];

    printf("Merge result: ");
    displayRange(arr, l, r);
    printf("\n");
}

void mergeSortDemo(int arr[], int l, int r) {
    int m;
    if (l >= r) return;

    m = (l + r) / 2;
    printf("Divide: ");
    displayRange(arr, l, r);
    printf("   into : ");
    displayRange(arr, l, m);
    printf("   and  : ");
    displayRange(arr, m + 1, r);
    printf("\n");

    mergeSortDemo(arr, l, m);
    mergeSortDemo(arr, m + 1, r);
    mergeDemo(arr, l, m, r);
}

void learnMergeSort(void) {
    int arr[] = {8, 3, 5, 1};
    int n = 4;

    printBanner("MERGE SORT");
    printf("What it does:\n"
           "Merge Sort uses the Divide and Conquer strategy. The array is\n"
           "split in half again and again until each piece has only one\n"
           "element, then the pieces are merged back together in sorted\n"
           "order.\n\n"
           "Example Array:\n");
    displayArray(arr, n);
    printf("\n");

    mergeSortDemo(arr, 0, n - 1);

    printf("Sorted Array:\n");
    displayArray(arr, n);
    printLearningInfo("O(n log n)", "O(n log n)", "O(n log n)", "O(n)",
                      "Guaranteed O(n log n) performance, always stable.",
                      "Requires extra memory for the temporary arrays.");
    pauseScreen();
}

int partitionDemo(int arr[], int low, int high) {
    int pivot = arr[high], i = low - 1, j, value;

    printf("Pivot = %d\n", pivot);
    for (j = low; j < high; j++) {
        value = arr[j];
        if (arr[j] < pivot) {
            swapInts(&arr[++i], &arr[j]);
            printf("%d < pivot(%d) -> place LEFT\n", value, pivot);
        } else {
            printf("%d >= pivot(%d) -> place RIGHT\n", value, pivot);
        }
    }

    swapInts(&arr[i + 1], &arr[high]);
    printf("Partitioned Array: ");
    displayRange(arr, low, high);
    printf("Pivot %d is now at its correct sorted position.\n\n", arr[i + 1]);
    return i + 1;
}

void quickSortDemo(int arr[], int low, int high) {
    int pi;
    if (low >= high) return;
    pi = partitionDemo(arr, low, high);
    quickSortDemo(arr, low, pi - 1);
    quickSortDemo(arr, pi + 1, high);
}

int partitionSolve(int arr[], int low, int high) {
    int pivot = arr[high], i = low - 1, j;
    for (j = low; j < high; j++)
        if (arr[j] < pivot) swapInts(&arr[++i], &arr[j]);
    swapInts(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSortSolve(int arr[], int low, int high) {
    int pi;
    if (low >= high) return;
    pi = partitionSolve(arr, low, high);
    quickSortSolve(arr, low, pi - 1);
    quickSortSolve(arr, pi + 1, high);
}

void learnQuickSort(void) {
    int arr[] = {7, 2, 9, 4, 1};
    int n = 5;

    printBanner("QUICK SORT");
    printf("What it does:\n"
           "Quick Sort picks a 'pivot' element and rearranges the array so\n"
           "that smaller elements move to its left and larger elements\n"
           "move to its right. This project always uses the LAST element\n"
           "of the current section as the pivot.\n\n"
           "Example Array:\n");
    displayArray(arr, n);
    printf("\n");

    quickSortDemo(arr, 0, n - 1);

    printf("Quick Sort now repeats this same partition process on the\n"
           "left partition and the right partition, recursively, until\n"
           "every partition has 0 or 1 elements left.\n\n"
           "Sorted Array:\n");
    displayArray(arr, n);
    printLearningInfo("O(n log n)", "O(n log n)", "O(n^2)",
                      "O(log n) typical, O(n) worst case (recursion)",
                      "Usually the fastest sorting algorithm in practice.",
                      "Can degrade to O(n^2) with a poor pivot choice.");
    pauseScreen();
}

void bubbleSortGame(void) {
    int arr[8], n = 5, i, j, swapped;
    int correct = 0, wrong = 0, choice, shouldSwap;

    generateArray(arr, n);
    printBanner("LEVEL 1 - BUBBLE SORT GAME");
    printf("Array:\n");
    displayArray(arr, n);

    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        for (j = 0; j < n - 1 - i; j++) {
            printf("\n%d and %d\n", arr[j], arr[j + 1]);
            printf("What should Bubble Sort do?\n1. Swap\n2. Don't Swap\n");
            choice = getIntInput("Your answer: ", 1, 2);
            shouldSwap = arr[j] > arr[j + 1];

            if ((choice == 1) == shouldSwap) {
                correct++;
                if (shouldSwap)
                    printf("Correct! %d > %d, so they must be swapped.\n", arr[j], arr[j + 1]);
                else
                    printf("Correct! %d is not greater than %d, so no swap is needed.\n", arr[j], arr[j + 1]);
            } else {
                wrong++;
                printf("Incorrect. Bubble Sort swaps adjacent elements when the\n"
                       "left element is greater than the right element.\n");
            }

            if (shouldSwap) {
                swapInts(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
            printf("Updated array:\n");
            displayArray(arr, n);
        }
        if (!swapped) break;
    }

    showLevelSummary("Bubble Sort", correct, wrong, arr, n, "O(n^2)");
}

void selectionSortGame(void) {
    int arr[8], n = 5, i, j, minIdx, userVal;
    int correct = 0, wrong = 0;

    generateArray(arr, n);
    printBanner("LEVEL 2 - SELECTION SORT GAME");
    printf("Array:\n");
    displayArray(arr, n);
    printf("\nSelection Sort searches for the smallest element in the\n"
           "unsorted part of the array during every step.\n");

    for (i = 0; i < n - 1; i++) {
        printf("\nCurrent unsorted section:\n");
        displayRange(arr, i, n - 1);

        minIdx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[minIdx]) minIdx = j;

        userVal = getIntInput("Which value is the smallest? ", 1, 99);
        if (userVal == arr[minIdx]) {
            correct++;
            printf("Correct!\n");
        } else {
            wrong++;
            printf("Incorrect. The smallest value in this section is %d.\n", arr[minIdx]);
        }

        if (minIdx != i) {
            printf("Swap %d with %d\n", arr[minIdx], arr[i]);
            swapInts(&arr[i], &arr[minIdx]);
        }
        printf("Array now:\n");
        displayArray(arr, n);
    }

    showLevelSummary("Selection Sort", correct, wrong, arr, n, "O(n^2)");
}

void insertionSortGame(void) {
    int arr[8], n = 5, i, j, key, correctPos, userPos;
    int correct = 0, wrong = 0;

    generateArray(arr, n);
    printBanner("LEVEL 3 - INSERTION SORT GAME");
    printf("Array:\n");
    displayArray(arr, n);
    printf("\nInsertion Sort keeps the left portion of the array sorted\n"
           "and inserts each new element into its correct position.\n");

    for (i = 1; i < n; i++) {
        printf("\nSorted portion:\n");
        displayRange(arr, 0, i - 1);
        key = arr[i];
        printf("Current element:\n%d\n", key);

        j = i - 1;
        while (j >= 0 && arr[j] > key) j--;
        correctPos = j + 1;

        printf("At what position should %d be inserted? (1 = leftmost)\n", key);
        userPos = getIntInput("Your answer: ", 1, i + 1);

        if (userPos - 1 == correctPos) {
            correct++;
            printf("Correct!\n");
        } else {
            wrong++;
            printf("Incorrect. %d should be inserted at position %d.\n", key, correctPos + 1);
        }

        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;

        printf("Array now:\n");
        displayArray(arr, n);
    }

    showLevelSummary("Insertion Sort", correct, wrong, arr, n, "O(n^2)");
}

void interactiveMergeArrays(int left[], int leftSize, int right[], int rightSize,
                            int result[], int *correct, int *wrong) {
    int i = 0, j = 0, k = 0, choice, smaller, chosen;

    while (i < leftSize && j < rightSize) {
        printf("\nMerge:\n%d and %d\n", left[i], right[j]);
        printf("Which number should come first?\n1. %d\n2. %d\n", left[i], right[j]);
        choice = getIntInput("Your answer: ", 1, 2);

        smaller = left[i] < right[j] ? left[i] : right[j];
        chosen = choice == 1 ? left[i] : right[j];

        if (chosen == smaller) {
            (*correct)++;
            printf("Correct!\n");
        } else {
            (*wrong)++;
            printf("Incorrect. %d should come first because it is smaller.\n", smaller);
        }

        result[k++] = (left[i] < right[j]) ? left[i++] : right[j++];
        printf("Merged so far: ");
        displayRange(result, 0, k - 1);
    }

    while (i < leftSize) result[k++] = left[i++];
    while (j < rightSize) result[k++] = right[j++];
}

void mergeSortGame(void) {
    int arr[4], leftSorted[2], rightSorted[2], finalArr[4];
    int correct = 0, wrong = 0;

    generateArray(arr, 4);
    printBanner("LEVEL 4 - MERGE SORT GAME");
    printf("Original:\n");
    displayArray(arr, 4);
    printf("\nFirst divide:\n%d %d | %d %d\n", arr[0], arr[1], arr[2], arr[3]);
    printf("\nThen:\n%d | %d | %d | %d\n", arr[0], arr[1], arr[2], arr[3]);

    printf("\n-- Merging the first pair --");
    interactiveMergeArrays(&arr[0], 1, &arr[1], 1, leftSorted, &correct, &wrong);
    printf("\n-- Merging the second pair --");
    interactiveMergeArrays(&arr[2], 1, &arr[3], 1, rightSorted, &correct, &wrong);

    printf("\nMerged pairs:\n%d %d | %d %d\n",
           leftSorted[0], leftSorted[1], rightSorted[0], rightSorted[1]);

    printf("\n-- Final merge --");
    interactiveMergeArrays(leftSorted, 2, rightSorted, 2, finalArr, &correct, &wrong);

    printf("\nFinal Sorted Array:\n");
    displayArray(finalArr, 4);
    showLevelSummary("Merge Sort", correct, wrong, finalArr, 4, "O(n log n)");
}

void quickSortGame(void) {
    int arr[8], leftArr[8], rightArr[8], partitioned[8], sortedArr[8];
    int n = 5, i, k = 0, pivot, leftCount = 0, rightCount = 0;
    int correct = 0, wrong = 0, choice, shouldBeLeft;

    generateArray(arr, n);
    printBanner("LEVEL 5 - QUICK SORT GAME");
    printf("Array:\n");
    displayArray(arr, n);

    pivot = arr[n - 1];
    printf("\nPivot = %d (last element)\n", pivot);

    for (i = 0; i < n - 1; i++) {
        printf("\nWhere should %d go relative to pivot %d?\n", arr[i], pivot);
        printf("1. Left\n2. Right\n");
        choice = getIntInput("Your answer: ", 1, 2);
        shouldBeLeft = arr[i] < pivot;

        if ((choice == 1) == shouldBeLeft) {
            correct++;
            printf("Correct! %d goes %s because %d %s %d.\n",
                   arr[i], shouldBeLeft ? "LEFT" : "RIGHT",
                   arr[i], shouldBeLeft ? "<" : ">", pivot);
        } else {
            wrong++;
            printf("Incorrect. %d goes %s because %d %s %d.\n",
                   arr[i], shouldBeLeft ? "LEFT" : "RIGHT",
                   arr[i], shouldBeLeft ? "<" : ">", pivot);
        }

        if (shouldBeLeft) leftArr[leftCount++] = arr[i];
        else rightArr[rightCount++] = arr[i];
    }

    for (i = 0; i < leftCount; i++) partitioned[k++] = leftArr[i];
    partitioned[k++] = pivot;
    for (i = 0; i < rightCount; i++) partitioned[k++] = rightArr[i];

    printf("\nPartitioned Array:\n");
    displayArray(partitioned, n);
    printf("\nQuick Sort now recursively repeats this same partitioning\n"
           "process on the left partition and the right partition until\n"
           "every partition has 0 or 1 elements left.\n");

    for (i = 0; i < n; i++) sortedArr[i] = arr[i];
    quickSortSolve(sortedArr, 0, n - 1);
    showLevelSummary("Quick Sort", correct, wrong, sortedArr, n, "O(n^2)");
}

void compareAlgorithms(void) {
    printBanner("COMPARE SORTING ALGORITHMS");
    printf("%-16s%-16s%-18s%-14s%-10s\n", "Algorithm", "Best Case", "Average Case", "Worst Case", "Space");
    printf("--------------------------------------------------------------------\n");
    printf("%-16s%-16s%-18s%-14s%-10s\n", "Bubble Sort",    "O(n)",       "O(n^2)",       "O(n^2)",     "O(1)");
    printf("%-16s%-16s%-18s%-14s%-10s\n", "Selection Sort", "O(n^2)",     "O(n^2)",       "O(n^2)",     "O(1)");
    printf("%-16s%-16s%-18s%-14s%-10s\n", "Insertion Sort", "O(n)",       "O(n^2)",       "O(n^2)",     "O(1)");
    printf("%-16s%-16s%-18s%-14s%-10s\n", "Merge Sort",     "O(n log n)", "O(n log n)",   "O(n log n)", "O(n)");
    printf("%-16s%-16s%-18s%-14s%-10s\n", "Quick Sort",     "O(n log n)", "O(n log n)",   "O(n^2)",     "O(log n)");
    printf("--------------------------------------------------------------------\n");
    printf("\nBubble Sort    : Easiest to understand but inefficient for large datasets.\n"
           "Selection Sort : Performs fewer swaps but is still O(n^2).\n"
           "Insertion Sort : Useful for small or nearly sorted data.\n"
           "Merge Sort     : Guaranteed O(n log n), but requires extra memory.\n"
           "Quick Sort     : Usually extremely fast, but can degrade to O(n^2).\n");
    pauseScreen();
}

void finalQuiz(void) {
    Question quiz[10] = {
        {"Which sorting algorithm repeatedly compares adjacent elements?", {"Merge Sort", "Bubble Sort", "Selection Sort", "Quick Sort"}, 1},
        {"Which algorithm uses the Divide and Conquer approach?", {"Bubble Sort", "Selection Sort", "Merge Sort", "Insertion Sort"}, 2},
        {"What is the worst-case time complexity of Merge Sort?", {"O(n)", "O(n^2)", "O(log n)", "O(n log n)"}, 3},
        {"Which sorting algorithm selects a pivot element?", {"Quick Sort", "Bubble Sort", "Selection Sort", "Insertion Sort"}, 0},
        {"Which algorithm works especially well on small or nearly sorted arrays?", {"Selection Sort", "Insertion Sort", "Merge Sort", "Quick Sort"}, 1},
        {"What is the space complexity of Merge Sort?", {"O(1)", "O(log n)", "O(n)", "O(n^2)"}, 2},
        {"Selection Sort works by repeatedly finding the:", {"Largest element in the sorted part", "Smallest element in the unsorted part", "Middle element", "Pivot element"}, 1},
        {"In this project, Quick Sort always chooses its pivot as the:", {"First element", "Middle element", "Last element", "Random element"}, 2},
        {"What is the worst-case time complexity of Quick Sort?", {"O(n log n)", "O(log n)", "O(n)", "O(n^2)"}, 3},
        {"Which of these algorithms has O(1) space complexity?", {"Merge Sort", "Insertion Sort", "None of these", "All of these"}, 1}
    };
    int i, choice, correctCount = 0, totalQuestions = 10;
    double percentage;

    printBanner("FINAL QUIZ");
    printf("Answer all %d questions. Enter 1, 2, 3 or 4 for your answer.\n", totalQuestions);

    for (i = 0; i < totalQuestions; i++) {
        printf("\nQ%d. %s\n", i + 1, quiz[i].question);
        printf("1. %s\n2. %s\n3. %s\n4. %s\n",
               quiz[i].options[0], quiz[i].options[1], quiz[i].options[2], quiz[i].options[3]);

        choice = getIntInput("Your answer: ", 1, 4);
        if (choice - 1 == quiz[i].correctIndex) {
            printf("Correct!\n");
            correctCount++;
        } else {
            printf("Incorrect. The correct answer was: %s\n", quiz[i].options[quiz[i].correctIndex]);
        }
    }

    percentage = correctCount * 100.0 / totalQuestions;
    printf("\n===================================\n"
           "FINAL QUIZ RESULT\n"
           "===================================\n");
    printf("Correct Answers: %d / %d\n", correctCount, totalQuestions);
    printf("Score: %.0f%%\n", percentage);

    if (percentage >= 90)
        printf("\nExcellent! You understand sorting algorithms very well!\n");
    else if (percentage >= 70)
        printf("\nGood job! You understand most sorting concepts.\n");
    else if (percentage >= 50)
        printf("\nNice attempt. Review a few algorithms and try again.\n");
    else
        printf("\nKeep practicing! Use the Learn section before trying again.\n");

    printf("===================================\n");
    pauseScreen();
}

void learnMenu(void) {
    int choice;
    do {
        printBanner("LEARN SORTING ALGORITHMS");
        printf("1. Bubble Sort\n2. Selection Sort\n3. Insertion Sort\n"
               "4. Merge Sort\n5. Quick Sort\n6. Back to Main Menu\n");
        choice = getIntInput("\nEnter your choice: ", 1, 6);

        switch (choice) {
            case 1: learnBubbleSort(); break;
            case 2: learnSelectionSort(); break;
            case 3: learnInsertionSort(); break;
            case 4: learnMergeSort(); break;
            case 5: learnQuickSort(); break;
        }
    } while (choice != 6);
}

void gameMenu(void) {
    int choice;
    do {
        printBanner("PLAY SORTING CHALLENGES");
        printf("Session Score so far: %d\n\n", sessionScore);
        printf("1. Level 1 - Bubble Sort Game\n2. Level 2 - Selection Sort Game\n"
               "3. Level 3 - Insertion Sort Game\n4. Level 4 - Merge Sort Game\n"
               "5. Level 5 - Quick Sort Game\n6. Back to Main Menu\n");
        choice = getIntInput("\nEnter your choice: ", 1, 6);

        switch (choice) {
            case 1: bubbleSortGame(); break;
            case 2: selectionSortGame(); break;
            case 3: insertionSortGame(); break;
            case 4: mergeSortGame(); break;
            case 5: quickSortGame(); break;
        }
    } while (choice != 6);
}

void mainMenu(void) {
    int choice;
    do {
        printf("\n==================================================\n"
               "                     SORTQUEST\n"
               "        Learn Sorting Algorithms by Playing\n"
               "==================================================\n"
               "1. Learn Sorting Algorithms\n2. Play Sorting Challenges\n"
               "3. Compare Sorting Algorithms\n4. Final Quiz\n5. Exit\n");

        choice = getIntInput("\nEnter your choice: ", 1, 5);
        switch (choice) {
            case 1: learnMenu(); break;
            case 2: gameMenu(); break;
            case 3: compareAlgorithms(); break;
            case 4: finalQuiz(); break;
            case 5:
                printf("\nSession Score (all games): %d\n", sessionScore);
                printf("Thank you for playing SortQuest. Keep practicing sorting!\n");
                break;
        }
    } while (choice != 5);
}

int main(void) {
    srand((unsigned int)time(NULL));
    mainMenu();
    return 0;
}
