#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int sortedArray[10];

void evenSort(int arr[])
{
	int j=0, k=0;

	for (int i = 0; i < 10; i++)
	{
		if (arr[i] % 2 == 0) 
		{
			sortedArray[j]=arr[i];
			j++;
		}
	}

	for (int i = 0; i < 10; i++)
	{
		if (arr[i] % 2 != 0) 
		{
			sortedArray[j]=arr[i];
			j++;
		}
	}
}

void printArray(int arr[])
{
	int i;
	for (i = 0; i < 10; i++)
		cout << arr[i] << " ";
	cout << endl;
}


int main()
{
	int arr[] = {7, 4, 7, 8, 4, 6, 3, 9, 6, 11};
	evenSort(arr);
	printArray(sortedArray);
    return 0;
}
