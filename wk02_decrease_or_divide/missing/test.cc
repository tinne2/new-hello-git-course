#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <random>
#include <set>
#include <string> 

#include "test.hh"
using namespace std;

const int LIMIT = 50;
const int MAX_RANDOM_START_VALUE = 200;
int getRandom(int upper)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, upper);
	// To generate a number:
	return dis(gen);
}

std::vector<int>* getRandomArray(unsigned int size)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, MAX_RANDOM_START_VALUE);
    auto s = new std::vector<int>(size+1,0);
    std::iota(s->begin(),s->end(),dis(gen));

    // remove a random middle element from the vector if there are more than 3 elements
    if(size-2 > 2){
        std::uniform_int_distribution dis2(1,(int)(size-2));
        auto to_remove_it = s->begin()+dis2(gen);
        s->erase(to_remove_it);
    }
    return s;
}

std::vector<int>* getSequence(int size)
{
    auto v = new vector<int>(size);
    const int rnd = getRandom(size);
    std::iota(v->begin(),v->end(),rnd);
    return v;
}

void test(int size, bool isIter, bool isRnd)
{
    std::vector<int>* arr = isRnd ? getRandomArray(size) : getSequence(size);
	// print out the array
    int i = 0;
    for (auto iter = arr->begin(); iter != arr->end(); ++iter,++i) {
        if (i>LIMIT){
            cout << "...";
            break;
        }
        cout << *iter<< (iter+1 == arr->end() ? "" : ", ");
    }
	cout << endl;

    // student's missing calculated - our missing2 for comparison
    const int missing = isIter ? searchSmallestMissingIteration(&(*arr)[0], size) : searchSmallestMissing(&(*arr)[0], 0, size-1);
    delete arr;
    cout << (missing == NO_VALUE_MISSING?"No value":std::to_string(missing)) << " missing!" << endl;
}

int main(int argc, char *argv[])
{
    // size must be given as the first command line param
    if (argc >= 2)
    {
        // if the second command line param exists, then this is recursion
        bool isIter = argc > 2 ? false : true;
        char *p;
        long l_size = strtol(argv[1], &p, 10);
        int size = (int)l_size;
        test(size, isIter, true);
        return EXIT_SUCCESS;
	}
    // running the default tests for both functions
    bool isIter = true;
    cout << "testing missing.cc"<<endl;
    std::vector<int> sizes ={10, 100, 1000, 10000};
    for (unsigned int size : sizes){
        test(size, isIter, false);
        test(size, isIter, true);
    }
    cout << endl << "testing missing2.cc" << endl;
    isIter = false;
    for (unsigned int size : sizes){
        test(size, isIter, false);
        test(size, isIter, true);
    }
}
