#include <iostream> // for cout & cin 
#include <stdio.h> //for rand and srand 
#include <string.h> //for memset

using namespace std;

/* Leaset Recently Used */ 
void LRU(int nFrames, int nPages, int *seq, int len); 

/* Most Freq. Used */
void MFU(int nFrames, int nPages, int *seq, int len);

/* Optimal Algorithm */
void OPT(int nFrames, int nPages, int *seq, int len); 

/* Leaset Freq. Used */
void LFU(int nFrames, int nPages, int *seq, int len);

/* First In First Out */ 
void FIFO(int nFrames, int nPages, int *seq, int len); 

int main() { 
    int seed;
    //int nPages;
    //int nFrames;
    //int lenOfSeq; 
    //int *seq;
    int algo_number;
    int choose_another_alog; 

	//cout << "This Program Simulate Page Replacement Alogrithms\n"; 
  	//cout << "Enter Number Of Pages : "; cin >> nPages; 
  	//cout << "Enter Number Of Frames : "; cin >> nFrames; 
  	//cout << "Enter Length Of Sequence : "; cin >> lenOfSeq;  
    //cout << "Enter The Seed Of sreand (seed): "; cin >> seed; 
    //srand(seed); 

    //seq = new int[lenOfSeq]; 

    //for(int i=0; i<lenOfSeq; i++) { 
    //    seq[i] = rand() % (nPages + 1); // 0<= rand() <= nPages 
    //}

	//int seq[] = {1,2,3,1,4,5,2,1,2,6,7,3,2}; 
	int seq[] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1}; 
	int lenOfSeq = sizeof(seq)/sizeof(int); 
	int nFrames = 3; 
	int nPages = 7; 


    do { 
		cout<<"\n--------------------------------------"
			<<"\nTo Run An Alogrithm Press Its Nubmer\n"
			<<"\n1. FIFO Algorithm"
			<<"\n2. LRU Algorithm"
			<<"\n3. LFU Algorithm"
			<<"\n4. MFU Algorithm"
			<<"\n5. Optimal Algorithm" 
			<<"\n--------------------------------------\n" 
			<< "Enter Algorithm Number: "; 
		cin >> algo_number; 

		cout << endl; 
		cout << "Seq. is: " << endl; 
		for(int i=0; i<lenOfSeq; i++) cout << seq[i] << " "; 
		cout << endl << endl; 

		switch(algo_number) 
		{ 
			case 1:FIFO(nFrames, nPages, seq, lenOfSeq); break; 
			case 2: LRU(nFrames, nPages, seq, lenOfSeq); break; 
			case 3: LFU(nFrames, nPages, seq, lenOfSeq); break; 
			case 4: MFU(nFrames, nPages, seq, lenOfSeq); break; 
			case 5: OPT(nFrames, nPages, seq, lenOfSeq); break; 
			default: 
				cout << "Enter Valid Nubmer" << endl; 
		}

		cout << endl; 
		cout <<"To Try Another Algorithm Press (1), To Exit Press (0) : "; 
		cin >> choose_another_alog;
    } while(choose_another_alog == 1); 

    return 0; 
}


/***********************************************
				UTILS SECTION
***********************************************/

bool find(int *arr, int size, int element) 
{ 
	bool isFound = false; 

	for(int i=0; i<size; i++) 
	{
		if(arr[i] == element) 
		{ 
			isFound = true; 
			break; 
		}
	}
	return isFound; 
}

void replace(int *arr, int size, int ele1, int ele2) 
{ 
	for(int i=0; i<size; i++) 
	{
		if(arr[i] == ele1) 
		{
			arr[i] = ele2; 
			break; 
		}
	}
}


/***********************************************
				LRU SECTION
***********************************************/

int min(int *map, int size) 
{ 
	int min = 0;
	
	/* this starts from 1 as we mad min the zero index element */ 
	for(int i=1; i<= size; i++) 
	{
		if(map[min] > map[i])  
		{
			min = i; 
		}
	}
	return min; 
}

/* Least Recently Used */ 
void LRU(int nFrames, int nPages, int *seq, int len)
{ 
	int missCount = 0, printFlag; 

	int map[nPages+1]; // if No. of pages 7 we need 8 places to store numbers from 0 up to 7 
	for(int i=0; i<=nPages; i++) map[i] = len; // filling the map array with numbers out of the range of expected mapping values    
	// memset(map, len, sizeof(map));
	
	//real frames array, with size of nFrames, at the very begining of the program
	//the filling indicator (framesSize = 0)  
	int frames[nFrames], framesSize = 0; 
	for(int i=0; i<nFrames; i++) frames[i] = -1; // puting a nubmer of of the range of possible values as know that values[0-n]  
	
	bool isFound; 

	for(int i=0; i<len; i++) 
	{
		printFlag = 1; 
		//find the element of the sequence in the frames 
		isFound = find(frames, framesSize, seq[i]);
		//if the frames array still not compelety filled up 
		if(framesSize < nFrames) 
		{ 		
			//Hit, element was found 
			if(isFound == true) 
			{ 
				//update mp value with the new insertion index in sequence array 
				map[seq[i]] = i;
				//don't print anything 
				printFlag = 0;
			}
			//Miss
			else 
			{ 
				//new element insertion. 
				frames[framesSize] = seq[i];
				framesSize++; 
				//change map[seq[i]] => say map of 5 to ith index, map[5] = i instead of len 
				map[seq[i]] = i;
				missCount++; //add +1 to total miss 
			}  
		}
		
		//array is filled, we need to earse an element to insert our new one in case of miss  
		else 
		{
			//Hit 
			if(isFound == true) 
			{ 
				//update map value for the ith element in sequence array with its values (note: that's what meant by mapping)
				map[seq[i]] = i;
				//print no thing 
				printFlag = 0; 
			} 
			// 2. element isn't in the frames insert it and increase total miss counter 
			// get the element that need to be removed and replace it with the new element
			else 
			{
				//find the element with minimum index value (least recently used) the one with the loweset map[x] value 
				// this element will be replaced by the newer one 
				int lru = min(map, nPages); 
				//set the map value for lru element to be out of range 
				map[lru] = len;
				//replace the lru by seq[i] in the frames array 
				replace(frames, nFrames, lru, seq[i]); 
				//update the newer element index value in map array 
				map[seq[i]] = i; 
				//miss count is increased 
				missCount++;
			}
		}

		//if the case was miss print the array to see the change 
		if(printFlag) 
		{
			for(int j=0; j<nFrames; j++) 
			{
				if(frames[j] == -1) cout << "# "; 
				else cout << frames[j] << " "; 
			}
			cout << endl;
		}
	}
	
	cout << "LRU total miss = " << missCount << endl; 
} 

/***********************************************
				MFU SECTION
***********************************************/
/**
 *this algo. we use to method the first one is the traditional mru, in case there's no tie between to element 
 *or more(elements to be erased from the frames array) 
 *if there's a tie we use MRL method to avoid this situation 
 */ 

//traditonal case
//this function returns an array (a pointer to array of size 2 
int *_max(int *freq, int size) 
{ 
	//this function takes the freq array and get the max, if there is anther element with 
	//the same max. flag is triggered to use MFU method 
	
	/* max starts from index zero 
	 * flag is an indicator to tie situation if there's two element with the same Freq to be removed 
	 * this flag is used to trigger the second method (MRL) 
	 */ 
	int max = 0;
	int flag = 0;
	
	/* i starts from i as i took max with zero value */ 
	for(int i=1; i<=size; i++) 
	{ 
		/* if the ith element freq > than recent max element value 
		 * change max to be i and update the flag with = 0 
		 */ 
		if(freq[i] > freq[max]) 
		{
			max = i; 
			flag = 0; 
		}
		/*if the ith element freq == the max element freq this a tie update flag with one 
		*/ 
		else if(freq[i] == freq[max]) { 
			flag = 1;
		}
	}
	/*making an array to return the two values of the max and flag as array pointer */ 
	int *max_flag = new int[2]; // won't work, unless the memory were dynamically allocated with new operator or c functions
	max_flag[0] = max; 
	max_flag[1] = flag; 
	return max_flag; 
}

//mru case 
int mru(int *map, int size)
{
	int max = 0; 
	for(int i=0; i<=size; i++) { 
	//if the ith element was entered after the max element, update max = i; 
		if(map[i] > map[max]) max = i; 
	}
	return max; 
}

/* Most Freq. Used */ 
void MFU(int nFrames, int nPages, int *seq, int len) 
{
	/* 
	Find the element with the highest freq. of hits and replace it with the newer element.
	-If there's two elements have the same highest freq. use MRU to resolve this situation.
	*/
	int missCount = 0, printFlag; 

	//freq array to be used in the first approach the regular one
	int freq[nPages+1];
	//setting the starts from first element in the array to the last one with 0 value
	//feel free to use for loop to fill the array, difference is in speed 
	memset(freq, 0, sizeof(freq)); 
	 
	//MRU array to be used in the second appraoch 
	int MRU[nPages+1]; 
	//fill the MRU with -1 as indicator of emptencess 
	for(int i=0; i<=nPages; i++) MRU[i] = -1; 

	//frames array 
	int frames[nFrames], framesSize = 0;
	//fill the frames with -1 also or any value out the range of used numbers, ex: could be nPages + 1;  
	memset(frames, -1, sizeof(frames)); 

	bool isFound; 

	for(int i=0; i<len; i++) { 
		//lookup the element to be inserted if it already in the frames array 
		isFound = find(frames, framesSize, seq[i]); 
		printFlag = 1; 
		// if the frames has not filled yet
		if(framesSize < nFrames) {
			//Hit: element to be inserted exist in the frames
			if(isFound == true) 
			{
				//increase the appereance (MFU) by one 
				freq[seq[i]]++; 
				//update the MRU with the newer enternece index 
				MRU[seq[i]] = i;
				//print no thing 
				printFlag = 0;  
			}
			//Miss: element doesn't exist 
			else 
			{
				//miss insert this element in the frames 
				frames[framesSize++] = seq[i];  
				//appereance = 1; 
				freq[seq[i]] = 1;
				//MRU[x] = i; instead of -1 
				MRU[seq[i]] = i; 
				//increaes miss counter 
				missCount++; 
			}
		}
		// else, if the frames has been filled 
		// if the hit increase element freq, 
		// if miss find MFU element, erase the MFU element, if there's two of them erase the MRU one
		// and insert the newer one 
		else 
		{
			//Hit
			if(isFound == true) 
			{ 
				freq[seq[i]]++;
				MRU[seq[i]] = i;
				printFlag = 0;   
			}
			//Miss
			else
			{
				//there's a need to remove an element 
				//1.st check the regular method if available to use 
				int *max_flag = _max(freq, nPages);
				
				//if flag == 1 // use the second case MRU
				if(max_flag[1]) { 
					//use MRU
					int x_mru = mru(MRU, nPages); 
					MRU[x_mru] = -1; 
					freq[x_mru] = 0; 
					replace(frames, nFrames, x_mru, seq[i]); 
				}
				//if not use MFU 
				else 
				{ 
					//use MFU
					MRU[max_flag[0]] = -1; 
					freq[max_flag[0]] = 0;
					replace(frames, nFrames, max_flag[0], seq[i]); 
				}
				//make freq[x] = 1 instead of -1
				freq[seq[i]] = 1;
				//MRU = i instead of -1 
				MRU[seq[i]] = i; 
				missCount++;  
			}
		}
		//if there's a change print the new frames array 
		if(printFlag)
		{
			for(int j=0; j<nFrames; j++) 
			{
				//print # instead of -1 in case of empty frame field 
				if(frames[j] == -1) cout << "# "; 
				else cout << frames[j] << " "; 
			}
			cout << endl;
		}

	}

	cout << "MFU total miss = " << missCount << endl;  
}

/***********************************************
				LFU SECTION
***********************************************/
/*
 *There're two approaches to handle LFU, just similar to MFU, with difference in using LRU instead MRU 
 *In case there's no tie between the elements (meaning there's no two element, or more with the same 
 *min freq use LFU 
 *use LRU in caes of tie 
 */
 
/* returns an array of size two */ 
int *lfu(int *freq, int size)
{ 
	int min = 0, flag = 0;
	
	for(int i=1; i<=size; i++) 
	{ 
		//if freq[i] not equal to zero, meaning the element as appereaed at least once 
		if(freq[i] != 0)
		{ 
			//if freq[min] == 0, update the min to be i immediately 
			if(freq[min] == 0)
			{ 
				min = i; 
				continue; 
			}
			//if min element was already appeared once 
			//check if freq[i] < freq[min]
			//if tree update min = i, flag = 0; 
			else
			if(freq[i] < freq[min])
			{ 
				min = i; 
				flag = 0; 
			}
			//if not check if the min and i has the same times of appeareances[tie]
			//update flag with = 1; 
			else 
			if(freq[i] == freq[min])
			{ 
				flag = 1;  
			}
		}
	}

	//return the min_flag array pointer 
	int *min_flag = new int[2]; 
	min_flag[0] = min; 
	min_flag[1] = flag; 
	return min_flag; 
}

/* returns an integer value */ 
int lru(int *lru, int*freq, int min_freq, int size)
{ 
	int min = 0; 
	/*
	 *here we eleminate the element with the same min freq that was discovered in lfu function
	 *we choose to delete the LRU one from among this set of elements 
	 */
	for(int i=1; i<=size; i++) 
	{
		//if by chance min was the page with min freq 
		if(freq[min] == min_freq)
		{ 
			//if we find the min freq we need then to find the min LRU
			if(lru[min] > lru[i])
			{ 
				min = i; 
			}
		}
		//if not change the min to i until we find the min freq 
		else 
		{ 
			min = i; 
		}
	}
	
	return min; 
}

/* Leaset Freq. Used */
void LFU(int nFrames, int nPages, int *seq, int len) 
{ 
	int missCount = 0, printFlag;
	
	//freq array 
	int freq[nPages + 1];
	//filling it with zero values 
	for(int i=0; i<=nPages; i++) freq[i] = 0;
	//memset(freq, 0, sizeof(freq)); 
	 
	int LRU[nPages + 1]; 
	for(int i=0; i<=nPages; i++) LRU[i] = len;
	//memset(LRU, len, sizeof(LRU)); 

	int frames[nFrames], framesSize = 0; 
	for(int i=0; i<=nFrames; i++) frames[i] = -1;
	//memset(frames, -1, sizeof(frames)); 

	bool isFound; 

	for(int i=0; i<len; i++) 
	{ 
		printFlag = 1; 
		
		isFound = find(frames, framesSize, seq[i]);  

		if(framesSize < nFrames) 
		{
			//hit
			if(isFound == true) 
			{ 
				freq[seq[i]]++;
				LRU[seq[i]] = i;
				printFlag = 0;  
			}	
			//miss 
			else 
			{ 
				frames[framesSize++] = seq[i];  
				freq[seq[i]]++;
				LRU[seq[i]] = i;
				missCount++; 
			}
		}
		else
		{
			//hit 
			if(isFound == true)
			{ 
				freq[seq[i]]++; 
				LRU[seq[i]] = i;
				printFlag = 0;
			}
			//miss
			else 
			{ 
				//try LFU if there's two elements with the leaset freq. use LRU. 
				int *min_flag = lfu(freq, nPages); 
				//if flag is 0 use LFU 
				if(min_flag[1] == 0) 
				{
					// cout << "flag = 0" << endl; 
					freq[min_flag[0]] = 0; 
					LRU[min_flag[0]] = len; 
					replace(frames, nFrames, min_flag[0], seq[i]); 
					freq[seq[i]]++; 
					LRU[seq[i]] = i; 	
				}
				//use LRU 
				else 
				{ 
					// cout << "flag = 1"<<endl; 
					int x = lru(LRU, freq, freq[min_flag[0]], nPages); 
					freq[x] = 0;
					LRU[x] = len; 
					replace(frames, nFrames, x, seq[i]);
					freq[seq[i]]++; 
					LRU[seq[i]] = i; 	
				}
				missCount++; 
			} 
		}
		if(printFlag)
		{
			for(int j=0; j<nFrames; j++) {
				if(frames[j] == -1) cout << "# "; 
				else cout << frames[j] << " "; 
			}
			// cout << "\tFreq. : "; 
			// for(int j=0; j<nFrames; j++) {
			// 	if(frames[j] == -1) cout << "# "; 
			// 	else cout << freq[frames[j]] << " "; 
			// }
			// cout << "\tLRU. : "; 
			// for(int j=0; j<nFrames; j++) {	
			// 	if(frames[j] == -1) cout << "# "; 
			// 	else cout << LRU[frames[j]] << " ";
			// }
			cout << endl;
		} 
	}

	cout << "LFU total miss = " << missCount << endl; 
}


/***********************************************
				OPT SECTION
***********************************************/

/* Optimal 
 * in optimal approach will look up each element and its appereances in the future(in to be come indices of seq array) 
 * to delete an element there're exactly 2 cases
 * 1. to remove the one with no, or with the farest appereance in the future 
 * 2. in case of there're to candidate to be delete delete any one of them, randomly  
 */
void OPT(int nFrames, int nPages, int *seq, int len) 
{
	int missCount = 0; 
	int printFlag; 

	//future checking array, to choose the one with farest index
	int nearest_index[nPages + 1];
	// memset(nearest_index, len, sizeof(nearest_index)); 
	for(int i=0; i<=nPages; i++) nearest_index[i] = len; 
	
	int frames[nFrames], framesSize = 0; 
	for(int i=0; i<=nFrames; i++) frames[i] = -1; 
	//memset(frames, -1, sizeof(frames)); 

	bool isFound; 

	for(int i=0; i<len; i++)
	{ 
		printFlag = 1; 

		isFound = find(frames, framesSize, seq[i]);  
		
		//frames still has empty place 
		if(framesSize < nFrames)
		{
			//miss
			if(isFound != true) 
			{
				frames[framesSize++] = seq[i]; 
				missCount++; 
			}
			//do nothing in case of hit
			else printFlag = 0;  
		}
		//frames is full 
		//we could use OPTIMAL or random erasing here. 
		else 
		{
			//miss 
			if(isFound != true)
			{
				int flag = 0; 
				//loop for frames array 
				for(int j=0; j<nFrames; j++) 
				{
					flag = 0; 
					//loop for seq array 
					for(int k=i+1; k<len; k++)
					{
						//if the frame j is == seq [k]
						if(frames[j] == seq[k]) 
						{
							// freme-J index in nearest index array with k
							nearest_index[frames[j]] = k; 
							flag = 1; 
							break; 
						}
					}
					//if the element don't appear in the future update its nearest index [frame-J] = len 
					if(flag == 0) nearest_index[frames[j]] = len; 
				}

				//counter incase there's a tie 
				flag = -1;
				int max = frames[0];

				//find the one with farest apperance  
				for(int j=1; j<nFrames; j++) 
				{
					if(nearest_index[frames[j]] > nearest_index[max])
					{
						max = frames[j];
						flag = -1; 
					} 
					else 
					if(nearest_index[frames[j]] == nearest_index[max]) 
					{
						flag = j;
						//cout <<"J: " << j << endl;    
					}
				}
				//there's no tie 
				if(flag == -1)
				{ 
					replace(frames, nFrames, max, seq[i]); 
					//cout << "Max: " << max << endl; 
				}
				//there's a tie remove the flag element
				else {
					replace(frames, nFrames, frames[flag], seq[i]);
					//cout << "Flag: " << frames[flag] << endl;  
				}
				missCount++; 
			}
			else printFlag = 0; 
		}
		if(printFlag) 
		{
			// for(auto p=frames.begin(); p!=frames.end(); ++p) cout << *p << " "; 
			// cout << endl;
			for(int j=0; j<nFrames; j++) {
				if(frames[j] == -1) cout << "# "; 
				else cout << frames[j] << " "; 
			}
			cout << endl; 
		} 
	}	

	cout << "OPT total miss = " << missCount<< endl; 
}


/***********************************************
				FIFO SECTION
***********************************************/

/* First In First Out */ 
void FIFO(int nFrames, int nPages, int *seq, int len) { 
	int missCount = 0, printFlag; 
	
	//queue array
	int queue[nPages + 1]; 
	for(int i=0; i<=nPages; i++) queue[i] = len; 
	
	int frames[nFrames], framesSize =0; 
	for(int i=0; i<=nPages; i++) frames[i] = -1; 
	//memset(frames, -1, sizeof(frames));  

	bool isFound; 

	for(int i=0; i<len; i++) 
	{
		printFlag = 0; 

		isFound = find(frames, framesSize, seq[i]); 

		if(framesSize < nFrames) 
		{
			//MISS 
			if(isFound == false)
			{ 
				frames[framesSize++] = seq[i];
				queue[seq[i]] = i; 
				missCount++; 
				printFlag = 1; 
			}
		}
		else 
		{
			if(isFound == false) 
			{
				// we need to search for FI element in frames array 
				int firstIn = frames[0]; 
				for(int j=1; j<nFrames; j++) 
				{
					if(queue[firstIn] > queue[frames[j]])
						firstIn = frames[j]; 
				}
				queue[firstIn] = len; 
				replace(frames, nFrames, firstIn, seq[i]); 
				queue[seq[i]] = i; 
				missCount++; 	
				printFlag = 1; 
			}
		}
		if(printFlag)
		{
			for(int j=0;j<nFrames;j++) { 
				if(frames[j] == -1) cout << "# "; 
				else cout << frames[j] << " ";
			} 
			cout << endl;
		}
	}

	cout << "FIFO total miss = " << missCount << endl; 
}
