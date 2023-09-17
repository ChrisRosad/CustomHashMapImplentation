/*
* Name: Christopher Rosado, 2001956343, Assignment Five
* Description: The following algorithm takes in a file with 
* categories and key words for each category, puts all the key 
* words into a hashmap and the category names into a vector. 
* Then it takes in a abstract that needs to be catagorized. The
* algorithm will take in each word of the abstract, compare it to the 
* hashmap, the word matches with one of the keywords within the hashmap
* the algorithm will decrement a unique amount of words value for the 
* appropriate category and then will set the keyword's value to zero,
* thus making it impossible to match with that word again. When complete 
* it will check each of the unique amount of words values and if they 
* are eqivalent to zero then the matching category will output, but
* if none are equivalent to zero then the program will output "SQF Problem"
* meaning that the abstract does not fall into any of the categories.
* Input: a file with amount of test cases, amount of categories, key words
* and name of categories and the abstract for the program to check. 
* Output: Output number of test case and the following categories the
* abstract falls under.
*/
#include "hashMap.h"
#include "hashMap.cpp"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

int main(){
    //vectors for matching Category and unique words needed for that category
    vector<string> Catagory;
    vector<int> UniqueWords;
    //strings for each section of the loops for inserting
    string Catagories = "", TmpString = "", sString = "";
    //integer values meant to retreive different values or be used as a tmp value to 
    // insert to a vector or hashmap.
    int TestCases = 0, AmntOfCats = 0, AmntOfWrds = 0, Unique = 0, index = 0;
    //boolean to prevent SQF problem from appearing
    bool AnyHit = true;
    cin>>TestCases;
    //Big loop for each test case

    for(int i = 0; i < TestCases; i++){
        hashMap<string, int> CatChooser;
        cin>>AmntOfCats;

        //Outer Loop for amount of catagories per test case
        for(int j = 0; j < AmntOfCats; j++){
            cin>>Catagories>>AmntOfWrds>>Unique;
            Catagory.push_back(Catagories);
            UniqueWords.push_back(Unique);
        
            //inputting the words into the hashmap
            for(int k = 0; k < AmntOfWrds; k++){
                cin>>TmpString;
                //converting each of the words into all lower case to prevent
                // capital lettering within writing from false negatives within
                // hashmaps
                for(int m = 0; m < TmpString.size(); m++){
                    TmpString[m] = tolower(TmpString[m]);
                }
                CatChooser[TmpString] = j + 1;
            }
        }
        //Takes away empty line within beginning of each of the files
        getline(cin, TmpString);

        while(getline(cin, TmpString)){
            //checks for empty string, indicating end of the abstract
            if(TmpString == ""){
                break;
            }
                stringstream ss(TmpString);
                ss>>sString;
                //while loop for checking each word in the abstract to the entire hashmap
                //creates a new false entry if it appears. Setting its value to zero
                while(!ss.fail()){
                    //sets string to be all lowercase, preventing false negatives
                    for(int m = 0; m < sString.size(); m++){
                        sString[m] = tolower(sString[m]);
                    }
                    //checks if string is not a false positive
                    // if it doesn't equal zero it will decrement 
                    // the unique words vector at the appropriate spot
                    // and will set the value to zero so it doesn't get 
                    // hit again
                    if(CatChooser[sString] != 0){
                        index = CatChooser[sString];
                        UniqueWords[index - 1]--;
                        CatChooser[sString] = 0;
                    }
                    ss>>sString;
                }
        }
            cout<<"Test case "<<i + 1<<endl;
            for(int i = 0; i < AmntOfCats; i++){
                if(UniqueWords[i] == 0){
                    cout<<Catagory[i]<<endl;
                    AnyHit = false;
                }
            }
            if(AnyHit)
                cout<<"SQF problem\n";
            AnyHit = true;
            Catagory.clear();
            UniqueWords.clear();
    }
return 0;
}