// Exchange for working solution
#ifndef MINPRIORITYQUEUE_H
#define MINPRIORITYQUEUE_H

// NOTE: You may not include any other libraries!
#include <unordered_map>
#include <vector>
#include <utility> // Has pair and swap

using namespace std;

template <typename T>
class MinPriorityQueue
{
	// For the mandatory running times below:
	//
	// n is the number of strings in the MinPriorityQueue.
	//
	// Assume the operations of map are O(1) time 
	// (they are average case, but not worst-case).

	public:
		// Creates an empty MinPriorityQueue
		MinPriorityQueue()
		{
			H.clear();
			I.clear();
		}

		// Returns the number of elements in the MinPriorityQueue.
		//
		// Must run in O(1) time.
		int size()
		{
			return H.size();
		}

		// Pushes a new element x with priority p
		// into the MinPriorityQueue.
		//
		// Must run in O(log(n)) time. 
		void push(T x, int p)
		{
			pair<T, int> couple;						// Create a pair to hold the value and priority
			couple.first = x;
			couple.second = p;
			H.push_back(couple);						// Add the pair to the end of the heap vector
			int updIndex = bubbleUp(H.size() - 1);		// Bubble up the last index
			I[x] = updIndex;							// Update the index map
		}

		// Returns the string at the front of the MinPriorityQueue.
		// Undefined behavior if the MinPriorityQueue is empty.
		// 
		// Must run in O(1) time.
		T front()
		{
			return (H[0].first);		// The min is always the "root", index 0
		}

		// Removes the string at the front of the MinPriorityQueue.
		// Undefined behavior if the MinPriorityQueue is empty.
		//
		// Must run in O(log(n)) time. 
		void pop()
		{
			if (H.size() == 0) {
				//cout << "There are no elements to pop." << endl;
				return;
			}

			T eraseLater = H[0].first;

			if (H.size() == 1) {
				H.pop_back();
				I.erase(eraseLater);
				return;
			}

			int sizeH = H.size();

			pair<T, int> couple = H[sizeH - 1];
			T tempT = H[sizeH - 1].first;
			H.pop_back();

			H[0] = couple;

			I.erase(eraseLater);

			int updIndex = bubbleDown(0);

			I[tempT] = updIndex;
		}

		// If s is in the MinPriorityQueue 
		// with current priority at least new_p,  
		// then changes the priority of x to new_p.
		// Undefined behavior otherwise.
		//
		// Must run in O(log(n)) time. 
		void decrease_key(T x, int new_p)
		{
			int sizeH = H.size();
			if (new_p < 0)
			{
				cout << "Invalid key decrease, please input a positive integer." << endl;
				return;
			}

			if (I.find(x) == I.end())
				return;

			int updIndex = 0;
			int foundInd = I[x];
			int old_p = H[foundInd].second;
			if (new_p < old_p)
			{
				H[foundInd].second = new_p;
				updIndex = bubbleUp(foundInd);
				I[x] = updIndex;
			}
			else
			{
				//H[foundInd].second = new_p;
				//updIndex = bubbleDown(foundInd);
				//I[x] = updIndex;
				cout << "This code doesn't increase the priority." << endl;
				return;
			}
		}

	private:
		// You don't need any other instance variables,
		// but you can add some if you want to.
		unordered_map<T, int> I; // Maps elements to indices in H.
		vector< pair<T, int> > H;     // The array containing the heap.

		int parent(int i)
		{
			return (i - 1) / 2;
		}

		int childLeft(int i)
		{
			return 2 * i + 1;
		}

		int childRight(int i)
		{
			return 2 * i + 2;
		}

		int bubbleUp(int index)
		{
			int i = index;

			while (i > 0 && H[i].second < H[parent(i)].second) 	// while the second member is less than its parent
			{											// which is a parent violation
				// Before swapping, update the map 'I' for both items being moved.
				I[H[i].first] = parent(i);		// The child item is moving to the parent's index
				I[H[parent(i)].first] = i;  	// The parent item is moving to the child's index

				swap(H[i], H[parent(i)]);	// swap the two couples			
				i = parent(i);
			}
			return i;
		}

		int bubbleDown(int index)
		{
			int i = index;
			int sizeH = H.size();

			while (childLeft(i) < sizeH)		// Check that at least there's a left child
			{
				int smallerChildInd = childLeft(i);
				if (childRight(i) < sizeH && H[childRight(i)].second < H[childLeft(i)].second)
				{
					smallerChildInd = childRight(i);
				}
				if (H[i].second > H[smallerChildInd].second)
				{
					I[H[i].first] = smallerChildInd;
					I[H[smallerChildInd].first] = i;
					swap(H[i], H[smallerChildInd]);
					i = smallerChildInd;
				}
				else
				{
					break;
				}
			}

			return i;
		}

};

#endif 
