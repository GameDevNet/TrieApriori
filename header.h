#pragma once

#ifndef HEADER_H_
#define HEADER_H_

#include "AprioriTrie.h"
#include "linkedList.h"
#include "timerSystem.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

const int MAX_SPACES_FOR_FREQUENT_ITEMSET_INPUT = 2,
          TEST_COUNT = 3,
          DIFFERENT_SUPPORT_THRESHOLD_COUNT = 5,
          ITEMSET_SIZES_TO_GENERATE = 25, //1-2-3-...25
          DATASET_COUNT = 27;
const float SUPPORT_THRESHOLDS[DIFFERENT_SUPPORT_THRESHOLD_COUNT] =
{
    0.0025f,
    0.005f,
    0.01f,
    0.1f,
    0.25f
};

const std::string OUTPUT_CSV_FILE = "RAW_RESULTS.csv",
                  SUMMARY_CSV_FILE = "SUMMARY_RESULTS.csv";
const std::string DATASET_NAMES[DATASET_COUNT] =
{
    "T5.N0.1K.D1K.txt",
    "T5.N0.1K.D10K.txt",
    "T5.N0.1K.D100K.txt",
    "T5.N0.5K.D1K.txt",
    "T5.N0.5K.D10K.txt",
    "T5.N0.5K.D100K.txt",
    "T5.N1K.D1K.txt",
    "T5.N1K.D10K.txt",
    "T5.N1K.D100K.txt",
    "T20.N0.1K.D1K.txt",
    "T20.N0.1K.D10K.txt",
    "T20.N0.1K.D100K.txt",
    "T20.N0.5K.D1K.txt",
    "T20.N0.5K.D10K.txt",
    "T20.N0.5K.D100K.txt",
    "T20.N1K.D1K.txt",
    "T20.N1K.D10K.txt",
    "T20.N1K.D100K.txt",
    "T25.N0.1K.D1K.txt",
    "T25.N0.1K.D10K.txt",
    "T25.N0.1K.D100K.txt",
    "T25.N0.5K.D1K.txt",
    "T25.N0.5K.D10K.txt",
    "T25.N0.5K.D100K.txt",
    "T25.N1K.D1K.txt",
    "T25.N1K.D10K.txt",
    "T25.N1K.D100K.txt"
};

void addTransaction( AprioriTrie* aprioriTrie );
bool loadAprioriTrie( AprioriTrie* aprioriTrie );
void mainLoop( AprioriTrie* aprioriTrie );
void reportFrequentItemsets( AprioriTrie* aprioriTrie );
void runTests();

#endif