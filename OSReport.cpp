#include <iostream> // for cout & cin 
#include <stdio.h> //for rand and srand 
#include <string.h> //for memset

using namespace std;

/*
** Author: Muhammad Abulmajd
** Helwan - Operating System
*/

/* Leaset Recently Used */ 
void LRU(int nFrames, int nPages, int *seq, int len); 
/* Most Freq. Used */
void MFU(int nFrames, int nPages, int *seq, int len);
/* Optimal Algorithm */
void OPT(int nFrames, int nPages, int *seq, int len); 
/* Leaset Freq. Used */
void LFU(int nFrames, int nPages, int *seq, int len);
/* Second Chance Algorithm */ 
void SCA(int nFrames, int nPages, int *seq, int len); 
/* First In First Out */ 
void FIFO(int nFrames, int nPages, int *seq, int len); 

int main() { 
    int seed, nPages, nFrames, lenOfSeq, *seq, algo_number, choose_another_alog; 

  	cout << "This Program Simulate Page Replacement Alogrithms\n"; 
  	cout << "Enter Number Of Pages : "; cin >> nPages; 
  	cout << "Enter Number Of Frames : "; cin >> nFrames; 
  	cout << "Enter Length Of Sequence : "; cin >> lenOfSeq;  
    cout << "Enter The Seed Of sreand (seed): "; cin >> seed; 
    srand(seed); 

    seq = new int[lenOfSeq]; 

    for(int i=0; i<lenOfSeq; i++) { 
        seq[i] = rand() % (nPages + 1); // 0<= rand() <= nPages 
    }


    do { 
		cout<<"\n--------------------------------------"
		<<"\nTo Run An Alogrithm Press Its Nubmer\n"
		<<"\n1. FIFO Algorithm"
		<<"\n2. LRU Algorithm"
		<<"\n3. LFU Algorithm"
		<<"\n4. MFU Algorithm"
		<<"\n5. Optimal Algorithm"
		<<"\n6. Second Chance Algorithm"; 	
		cout<<"\n--------------------------------------\n"; 
		cout << "Enter Algorithm Number: "; cin >> algo_number; 

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
			case 6: SCA(nFrames, nPages, seq, lenOfSeq); break; 
			default: 
				cout << "Enter Valid Nubmer" << endl; 
		}

		cout << endl; 
		cout <<"To Try Another Algorithm Press (1), To Exit Press (0) : "; 
		cin >> choose_another_alog;
    } while(choose_another_alog == 1); 

    return 0; 
}


//////////////////////////////////////////////////////
//utils functions that used in all algortithms
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

///////////////////////////////////////////////
//	LRU SECTION
///////////////////////////////////////////////

int min(int *map, int size) 
{ 
	int min = 0;

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

	int map[nPages+1]; 
	for(int i=0; i<=nPages; i++) map[i] = len;  
	// memset(map, len, sizeof(map));
	

	int frames[nFrames], framesSize = 0; 
	for(int i=0; i<nFrames; i++) frames[i] = -1;  

	bool isFound; 

	for(int i=0; i<len; i++) 
	{
		printFlag = 1; 
		
		isFound = find(frames, framesSize, seq[i]); // a pointer to the wanted element if exist, else to the end of the set

		if(framesSize < nFrames) 
		{ 		
			//Hit 
			if(isFound == true) 
			{ 
				map[seq[i]] = i;
				printFlag = 0;
			}
			//Miss
			else 
			{ 
				//new element insertion. 
				frames[framesSize] = seq[i];
				framesSize++; 

				map[seq[i]] = i;
				missCount++; //add +1 to total miss 
			}  
		}
		
		else 
		{
			//Hit 
			if(isFound == true) 
			{ 
				map[seq[i]] = i;
				printFlag = 0; 
			} 
			// 2. element isn't in the frames insert it and increase total miss counter 
			// get the element that need to be removed and replace it with the new element
			else 
			{
				int lru = min(map, nPages); 
				map[lru] = len;
				replace(frames, nFrames, lru, seq[i]); 
				map[seq[i]] = i; 
				missCount++;
			}
		}

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

///////////////////////////////////////////////
//	MFU SECTION
///////////////////////////////////////////////

int *_max(int *freq, int size)
{ 
	//this function takes the freq array and get the max, if there is anther element with 
	//the same max. flag is triggered to use LRU method 
	int max = 0;
	int flag = 0;

	// cout << "\n--------------------------------------- << endl; 

	for(int i=1; i<=size; i++) 
	{ 
		if(freq[i] > freq[max]) 
		{
			max = i; 
			flag = 0; 
		}
		else if(freq[i] == freq[max]) { 
			// cout << i << ", "; 
			flag = 1;
		}
	}

	// cout << "\nMax: " << max << " freq["<<max<<"] = " <<freq[max]<< " ,Flag: " << flag << endl; 
	// cout << "\n--------------------------------------- << endl; 

	int *max_flag = new int[2]; 
	max_flag[0] = max; 
	max_flag[1] = flag; 
	return max_flag; 
}

int mru(int *map, int size)
{
	int max = 0; 
	for(int i=0; i<=size; i++) { 
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

	int freq[nPages+1];
	memset(freq, 0, sizeof(freq)); 
	 
	int MRU[nPages+1]; 
	memset(MRU, -1, sizeof(MRU)); 

	int frames[nFrames], framesSize = 0; 
	memset(frames, -1, sizeof(frames)); 

	bool isFound; 

	for(int i=0; i<len; i++) { 
		// if the frames has not filled yet
		
		isFound = find(frames, framesSize, seq[i]); 
		printFlag = 1; 

		if(framesSize < nFrames) {
			//Hit: element to be inserted exist in the frames
			if(isFound == true) 
			{
				freq[seq[i]]++; 
				MRU[seq[i]] = i;
				printFlag = 0;  
			}
			//Miss: element doesn't exist 
			else 
			{ 
				frames[framesSize++] = seq[i];  
				freq[seq[i]] = 1;
				MRU[seq[i]] = i; 
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
				int *max_flag = _max(freq, nPages);
				
				if(max_flag[1]) { 
					//use MRU
					int x_mru = mru(MRU, nPages); 
					MRU[x_mru] = -1; 
					freq[x_mru] = 0; 
					replace(frames, nFrames, x_mru, seq[i]); 
				}
				else 
				{ 
					//use MFU
					MRU[max_flag[0]] = -1; 
					freq[max_flag[0]] = 0;
					replace(frames, nFrames, max_flag[0], seq[i]); 
				}
				freq[seq[i]] = 1;
				MRU[seq[i]] = i; 
				missCount++;  
			}
		}
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

	cout << "MFU total miss = " << missCount << endl;  
}

///////////////////////////////////////////////
//	LFU SECTION
///////////////////////////////////////////////

int *lfu(int *freq, int size)
{ 
	int min = 0, flag = 0;
	
	for(int i=1; i<=size; i++) 
	{ 
		if(freq[i] != 0)
		{ 
			if(freq[min] == 0)
			{ 
				min = i; 
				continue; 
			}
			else
			if(freq[i] < freq[min])
			{ 
				min = i; 
				flag = 0; 
			}
			else 
			if(freq[i] == freq[min])
			{ 
				flag = 1;  
			}
		}
	}

	int *min_flag = new int[2]; 
	min_flag[0] = min; 
	min_flag[1] = flag; 
	return min_flag; 
}
int lru(int *lru, int*freq, int min_freq, int size)
{ 
	// cout <<"-----------------"<<endl; 
	// cout << "Min freq: " << min_freq << endl; 
	int min = 0; 
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
	// cout <<"-----------------"<<endl; 
	return min; 
}

/* Leaset Freq. Used */
void LFU(int nFrames, int nPages, int *seq, int len) 
{ 
	int missCount = 0, printFlag;
	int freq[nPages + 1];
	memset(freq, 0, sizeof(freq)); 
	 
	int LRU[nPages + 1]; 
	memset(LRU, len, sizeof(LRU)); 

	int frames[nFrames], framesSize = 0; 
	memset(frames, -1, sizeof(frames)); 

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
				if(min_flag[1] == 0) 
				{
					// cout << "flag = 0" << endl; 
					freq[min_flag[0]] = 0; 
					LRU[min_flag[0]] = len; 
					replace(frames, nFrames, min_flag[0], seq[i]); 
					freq[seq[i]]++; 
					LRU[seq[i]] = i; 	
				}
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


///////////////////////////////////////////////
//	OPT SECTION
///////////////////////////////////////////////

/* Optimal */
void OPT(int nFrames, int nPages, int *seq, int len) 
{
	//choose the the farest element in appearance to delete 
	//if we have to element or more that will not appear use MRU element to delete 

	int missCount = 0; 
	int printFlag; 

	int nearest_index[nPages + 1];
	// memset(nearest_index, len, sizeof(nearest_index)); 
	for(int i=0; i<=nPages; i++) nearest_index[i] = len; 

	int MRU[nPages+1]; 
	memset(MRU, -1, sizeof(MRU)); 
	
	int frames[nFrames], framesSize = 0; 
	memset(frames, -1, sizeof(frames)); 

	bool isFound; 

	for(int i=0; i<len; i++)
	{ 
		printFlag = 1; 

		isFound = find(frames, framesSize, seq[i]);  
		
		//frames still has empty place 
		if(framesSize < nFrames)
		{

			//hit
			if(isFound == true) 
			{
				MRU[seq[i]] = i;
				printFlag = 0; 
			}
			//miss 
			else 
			{	
				frames[framesSize++] = seq[i]; 
				MRU[seq[i]] = i; 
				missCount++; 
			}
		}
		//frames is full 
		//we could use OPTIMAL or MRU here. 
		else 
		{
			//hit 
			if(isFound == true)
			{
				MRU[seq[i]] = i;
				printFlag = 0;  
			}
			//miss 
			else 
			{
				//easy way to find nearest element to use two for lps 
				//first one loop on frames and there inner one loops through seq form i - to len 
				// of the seq if we find the element in the inner loop we break and store its index 
				// the compare to find the farest one through its index  

				for(int j=0; j<nFrames; j++) 
				{
					int flag = 0; 
					for(int k=i+1; k<len; k++)
					{
						if(frames[j] == seq[k]) 
						{
							nearest_index[frames[j]] = k; 
							flag = 1; 
							break; 
						}
					}
					if(flag == 0) nearest_index[frames[j]] = len; 
				}

				int counter = 0;
				int max = frames[0]; 
				for(int j=1; j<nFrames; j++) 
				{
					if(nearest_index[frames[j]] > nearest_index[max])
					{
						max = frames[j]; 
					} 
					else 
					if(nearest_index[frames[j]] == nearest_index[max]) 
					{
						counter++; 
					}
				}


				//if we have only one element that have the highest index in seq or out the seq in
				//the range of (i : len) of seq we remove this element 
				//counter == 0 if the element with the highest index in seq is the first element in frames counter will not be increased. 
				// cout << "Counter : " << counter << endl; 

				if((counter == 1) || (counter == 0)) 
				{ 
					// cout<<"Counter == 1 or 0"<<endl;
					// cout<<"Max = " << max << endl;  
					replace(frames, nFrames, max, seq[i]);
					MRU[seq[i]] = i;
					MRU[max] = -1; 
					nearest_index[max] = len;
				}
				//if we have more the one element the out the seq in the range (i : len), We use 
				//MRU method to determined which element to be removed form the frames 
				else 
				{ 
					// cout <<"Two or more elements out of range"<<endl; 
					max = mru(MRU, nPages);
					replace(frames, nFrames, max, seq[i]);
					MRU[seq[i]] = i; 
					MRU[max] = -1; 
					nearest_index[max] = len;
				}
				missCount++; 
			}
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


///////////////////////////////////////////////
//	SCA SECTION
///////////////////////////////////////////////

int fifo(int *queue, int *ref_bit, int size)
{ 
	int min; 

	//continue until we find element that doesn't have second chance to be removed. 
	while(1) 
	{ 
		min = 0; 
		for(int i=0; i<=size; i++) 
		{ 
			if((queue[min] > queue[i]) && (ref_bit[i] != -1))
			{ 
				min = i; 
			}
		}
		//if ref_bit on, disable it. and loop again excluding it from the search next time by setting it to; 
		if(ref_bit[min] == 1) 
		{ 
			ref_bit[min] = -1; 
		}
		else break; 
	} 

	for(int i=0; i<=size; i++) 
	{ 
		if(ref_bit[i] == -1) ref_bit[i] = 0; 
	}

	return min; 
}

/*Second Chance Algorithm */ 
void SCA(int nFrames, int nPages, int *seq, int len)
{
	/* 
	Second chance algorithm is a FIFO variant, as we give the repeated elements that chance chance to 
	be in frames, 
	Ex: if we have page 2 in our frames and we require 2 again while the first still in the frames 
	we give the 2 second chance by enabling its second chance bit

	when removing pages/elements we search for the first entered (FI) and replace it, it have its second
	chance bit enable we disable it and look for the next in line and remove it.
	*/
	
	int missCount = 0, printFlag; 
	
	int ref_bit[nPages +1]; // ref_bit : Second Chance Bit; 
	memset(ref_bit, 0, sizeof(ref_bit)); 
	
	int queue[nPages + 1]; 
	// memset(queue, len, sizeof(queue)); // as len out of the range of indeces
	for(int i=0; i<=nPages; i++) queue[i] = len; 

	int frames[nFrames], framesSize = 0; 
	memset(frames, -1, sizeof(frames)); 

	bool isFound; 

	// set<int> frames; 
	// set<int>::iterator isFound; 

	for(int i=0; i<len; i++) 
	{ 
		printFlag = 1;

		isFound = find(frames, framesSize, seq[i]); 

		//frames is not full 
		if(framesSize < nFrames) 
		{
			//hit 
			if(isFound == true) 
			{ 
				ref_bit[seq[i]] = 1;
				printFlag = 0; 
			} 
			//miss
			else
			{ 
				frames[framesSize++] = seq[i];  
				queue[seq[i]] = i;
				missCount++;  
			}
		}
		//frames is full 
		else
		{ 
			//Hit 
			if(isFound == true)
			{ 
				ref_bit[seq[i]] = 1;
				printFlag = 0; 
			}
			//Miss
			else
			{
				int x = fifo(queue, ref_bit, nPages);
				queue[x] = len;

				replace(frames, nFrames, x, seq[i]); 
				
				queue[seq[i]] = i; 
				missCount++; 
			}
		}
		if(printFlag)
		{
			for(int j=0; j<nFrames; j++) {
				if(frames[j] == -1) cout << "# "; 
				else cout << frames[j] << " "; 
			}
			cout << endl; 
		}
	}

	cout << "Second Chance total miss = " << missCount << endl; 
}


///////////////////////////////////////////////
//	FIFO SECTION
///////////////////////////////////////////////

/* First In First Out */ 
void FIFO(int nFrames, int nPages, int *seq, int len) { 
	int missCount = 0, printFlag; 
	
	int queue[nPages + 1]; 
	for(int i=0; i<=nPages; i++) queue[i] = len; 

	int frames[nFrames], framesSize =0; 
	memset(frames, -1, sizeof(frames));  

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
