#include "header.h"

/*     Pre:  AprioriTrie
*     Post:  Single transaction data entered into aprioriTrie.
*  Purpose:  Inserts single transaction into apriortrie
*************************************************************************/
void addTransaction( AprioriTrie* aprioriTrie )
{
    std::stringstream ss;
    std::string str;
    ItemID itemID;
    LinkedList<ItemID> transaction;

    std::cout << "Enter ItemIDs in transaction, separated by spaces:\n";
    std::getline( std::cin, str );

    int strLength = str.length();

    for( int i = 0; i < strLength; ++i )
    {
        if( str[i] != ' ' && !isdigit( str[i] ) )
        {
            std::cout << "Invalid Input! Aborting 'Add Transaction'\n";
            return;
        }
    }

    ss << str;

    while( ss >> itemID )
    {
        transaction.insert( itemID );
    }

    aprioriTrie->addTransaction( transaction );
}

/*     Pre:  Empty aprioriTrie.
*     Post:  Aprioritrie loaded
*  Purpose:  Reads user input, loads file into aprioritrie based on input.
*************************************************************************/
bool loadAprioriTrie( AprioriTrie* aprioriTrie )
{
    std::string fileName;
    std::cout << "Enter name of data file: \n";
    std::cin >> fileName;

    std::ifstream fileIn( fileName );

    if( !fileIn.good() )
    {
        std::cout << "File not found or is corrupt! Exiting...\n";
        return false;
    }

    int transactionID,
        oldTransactionID = 1,
        transactionCount = 0;
    ItemID itemID;

    LinkedList<ItemID> itemIDs;

    while( fileIn >> transactionID )
    {
        fileIn >> itemID;

        //if we are still on the same transaction
        if( transactionID == oldTransactionID )
        {
            itemIDs.insert( itemID );
        }
        else //if we are done with a transaction.
        {
            ++transactionCount;
            aprioriTrie->addTransaction( itemIDs );
            itemIDs = LinkedList<ItemID>(); //should be faster than calling the clear function.
            itemIDs.insert( itemID );
        }

        oldTransactionID = transactionID;
    }

    ++transactionCount;
    aprioriTrie->addTransaction( itemIDs );

    std::stringstream ss;

    tm timeBuffer;
    std::chrono::system_clock::time_point currTime = std::chrono::system_clock::now();
    time_t currTimeTimeT = std::chrono::system_clock::to_time_t( currTime );
    localtime_s( &timeBuffer, &currTimeTimeT );
    ss << std::put_time( &timeBuffer, "[%c]  Session Started." );

    std::cout << "Loaded " << transactionCount << " transactions!\n";
    ss << "  Loaded " << transactionCount << " transactions!\n";

    std::ofstream outFile( FILE_OUT_NAME, std::ofstream::app );
    outFile << ss.str();
    outFile.close();

    return true;
}

/*     Pre:  Loaded aprioriTrie
*     Post:  User input read, operations performed.
*  Purpose:  Menu that reads user input and perform operations.
*************************************************************************/
void mainLoop( AprioriTrie* aprioriTrie )
{
    char input = ' ';

    while( input != '4' )
    {
        std::cout << "\n\t1. Report Frequent Itemsets\n\t2. Report all Itemsets\n\t3. Add Transaction\n\t4. Exit\n\n" <<
            "Input: ";
        //Read input
        std::cin >> input;
        std::cout << std::endl;
        std::string str;
        std::getline( std::cin, str );

        //Assess input
        switch( input )
        {
            case '1':
                reportFrequentItemsets( aprioriTrie );
                break;
            case '2':

                aprioriTrie->reportAll();
                break;
            case '3':
                addTransaction( aprioriTrie );
                break;
            case '4':
                break;
            default:
                std::cout << "Invalid input.\n\n";
                break;
        }
    }
}

/*     Pre:  Loaded aprioritrie
*     Post:  Requested data is displayed.
*  Purpose:  Reads user input and displays requested data from aprioriTrie.
*************************************************************************/
void reportFrequentItemsets( AprioriTrie* aprioriTrie )
{
    std::stringstream ss;
    std::string str;
    LinkedList<ItemID> transaction;

    std::cout << "Enter Support Threshold (0.0 - 1.0 float), Minimum Itemset Size,\n" <<
        "      and Maximum Itemset Size, separated by spaces:\n";
    std::getline( std::cin, str );

    int strLength = str.length(),
        spaceCounter = 0;
    bool putInDecimalPoint = false;

    for( int i = 0; i < strLength; ++i )
    {
        if( str[i] == ' ' )
        {
            ++spaceCounter;

            if( spaceCounter > MAX_SPACES_FOR_FREQUENT_ITEMSET_INPUT )
            {
                std::cout << "Invalid Input! Aborting 'Report Frequent Itemsets'\n";
                return;
            }
        }
        else if( !isdigit( str[i] ) )
        {
            if( spaceCounter == 0 && str[i] == '.' && !putInDecimalPoint )
            {
                putInDecimalPoint = true;
            }
            else
            {
                std::cout << "Invalid Input! Aborting 'Report Frequent Itemsets'\n";
                return;
            }
        }
    }

    ss << str;

    float supportThresholdPercentage;
    int minItemsetSize,
        maxItemsetSize;

    ss >> supportThresholdPercentage;
    ss >> minItemsetSize;
    ss >> maxItemsetSize;

    aprioriTrie->reportIfFrequent( supportThresholdPercentage, minItemsetSize, maxItemsetSize );
}

/*     Pre:  Empty aprioriTrie. filename of dataset.
*     Post:  Dataset data entered into aprioriTrie.
*  Purpose:  Inserts dataset data into aprioriTrie. (ONLY USED FOR TESTING)
*************************************************************************/
void fillAprioriTrie( AprioriTrie* aprioriTrie, const std::string FILE_NAME )
{
    std::ifstream fileIn( FILE_NAME );

    int transactionID,
        oldTransactionID = 1;
    ItemID itemID;
    LinkedList<ItemID> itemIDs;

    while( fileIn >> transactionID )
    {
        fileIn >> itemID;

        //if we are still on the same transaction
        if( transactionID == oldTransactionID )
        {
            itemIDs.insert( itemID );
        }
        else //if we are done with a transaction.
        {
            aprioriTrie->addTransaction( itemIDs );
            itemIDs = LinkedList<ItemID>(); //should be faster than calling the clear function.
        }

        oldTransactionID = transactionID;
    }

    aprioriTrie->addTransaction( itemIDs );
}

/*     Pre:  Nothing.
*     Post:  All test data is output to csv files.
*  Purpose:  Runs tests, times them and outputs data to csv files. (ONLY USED FOR TESTING)
*************************************************************************/
void runTests()
{
    double deltaTime;
    TimerSystem timer;
    
    for( int currDataSet = 0; currDataSet < DATASET_COUNT; ++currDataSet )
    {
        AprioriTrie* aprioriTrie = new AprioriTrie;
        timer.startClock();
        fillAprioriTrie( aprioriTrie, DATASET_NAMES[currDataSet] );
        deltaTime = timer.getTime();
        std::ofstream fileOut( OUTPUT_CSV_FILE, std::ios_base::app );
        std::ofstream fileOutSumm( SUMMARY_CSV_FILE, std::ios_base::app );
        fileOut << std::fixed << std::setprecision(5);
        fileOutSumm << std::fixed << std::setprecision(5);
        fileOut << "\"" << DATASET_NAMES[currDataSet] << "\" processed in " << deltaTime << " secs\n";
        fileOutSumm << "\"" << DATASET_NAMES[currDataSet] << "\" processed in " << deltaTime << " secs\n";
        
        for( int currSupportThresholdIndex = 0; currSupportThresholdIndex < DIFFERENT_SUPPORT_THRESHOLD_COUNT; ++currSupportThresholdIndex )
        {
            fileOut << "Dataset: " << DATASET_NAMES[currDataSet] << ",Minimum Support Threshold: " << SUPPORT_THRESHOLDS[currSupportThresholdIndex] << "\n";
            fileOutSumm << "Dataset: " << DATASET_NAMES[currDataSet] << ",Minimum Support Threshold: " << SUPPORT_THRESHOLDS[currSupportThresholdIndex] << "\n";
            fileOut << "Itemset Size,Number of Itemsets Generated,Run1,Run2,Run3,Average\n";
            fileOutSumm << "Itemset Size,Number of Itemsets Generated,Time Needed\n";
            for( int currGeneratedItemsetSize = 1; currGeneratedItemsetSize <= MAX_FREQUENT_ITEMSET_SIZE; ++currGeneratedItemsetSize )
            {
                fileOut << currGeneratedItemsetSize << ",";
                fileOutSumm << currGeneratedItemsetSize << ",";
                double sumTime = 0.0;
                gOutputCount = 0;
                
                for( int currTestCount = 0; currTestCount < TEST_COUNT; ++currTestCount )
                {
                    timer.startClock();

                    aprioriTrie->reportIfFrequent( SUPPORT_THRESHOLDS[currSupportThresholdIndex], currGeneratedItemsetSize, currGeneratedItemsetSize );

                    deltaTime = timer.getTime();
                    sumTime += deltaTime;

                    if( currTestCount == 0 )
                    {
                        fileOut << gOutputCount << ",";
                        fileOutSumm << gOutputCount << ",";
                    }

                    fileOut << deltaTime << ",";
                }

                fileOut << sumTime / TEST_COUNT << "\n";
                fileOutSumm << sumTime / TEST_COUNT << "\n";
            }

            fileOut << "\n";
            fileOutSumm << "\n";
        }

        fileOut << "\n\n";
        fileOutSumm << "\n\n";

        fileOut.close();
        fileOutSumm.close();

        std::cout << "Done with file: " << DATASET_NAMES[currDataSet] << "\n";

        delete aprioriTrie;
        std::cout << "Moving to next one if available.\n";
    }
}