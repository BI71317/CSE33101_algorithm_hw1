#include <iostream>
#include <fstream>
#include <chrono> 
#define MAX_SIZE 2500000
#define INF 1000000000
#define SWAP(a, b) do { auto temp = (a); (a) = (b); (b) = temp; } while (0)
using namespace std;

typedef long long ll;

// array that is required by sort functions
// that cannot sort in-place.
ll not_in_place[MAX_SIZE];

// array that contains original unsorted data 
// and after done sort functions, contains sorted data.
ll input_array[MAX_SIZE];

void merge(ll list[], ll left, ll mid, ll right){
    // copy all value into temporary space
    // from two sub array, left to mid and mid + 1 to right,
    // they are sorted each.
    ll size = right - left + 1;
    for(ll i = left; i <= mid; i++){
        not_in_place[i] = list[i];
    }
    for(ll i = mid + 1; i <= right; i++){
        not_in_place[i + size] = list[i];
    }
    not_in_place[mid + 1] = INF;
    not_in_place[size + right + 1] = INF;
    // As given each data range is 0 ~ 999,999, I set
    // sentinel value as INF = 1e9. 
    // If data range varies, sentinel value must
    // changed either.
    ll i = left, j = mid + 1, cur = left;
    // until one sub array is empty, 
    // compare front value and insert in list.
    while(cur <= right){
        if(not_in_place[i] <= not_in_place[j + size]){
            list[cur++] = not_in_place[i++];
        }
        else{
            list[cur++] = not_in_place[j + size];
            j++;
        }
    }
}

void merge_sort(ll list[], ll left, ll right){
    ll mid = -1;
    if(left < right){
        mid = (left + right) / 2;
        merge_sort(list, left, mid);
        merge_sort(list, mid + 1, right);
        merge(list, left, mid, right);
    }
}

void wrapper_merge_sort(ll list[], ll size){
    merge_sort(list, 1, size);
}

void down_heap(ll list[], ll cur, ll left, ll right){
    // 1-based index criteria
    // To reduce call and stack frame, 
    // implemented with loop, not in recursive way.
    ll offset = left - 1, size = right - left + 1;
    while(1){
        ll largest = cur;
        ll L = 2 * cur;
        ll R = 2 * cur + 1;
        if(cur * 2 <= size && list[L + offset] > list[largest + offset])
            largest = L;
        if(cur * 2 + 1 <= size && list[R + offset] > list[largest + offset])
            largest = R;
        if(largest == cur) break;
    
        swap(list[cur + offset], list[largest + offset]);
        cur = largest;
    }
}

void heap_sort(ll list[], ll left, ll right){
    // from lowest index inner node, heapify.
    ll size = right - left + 1;
    for(ll i = size / 2; i > 0; i--){
        down_heap(list, i, left, right);
    }
    // heap sort
    for(ll i = right; i > left; i--){
        swap(list[left], list[i]);
        // remove max val 
        down_heap(list, 1, left, i - 1);
        // lower array size, and heapify first index.
    }
}

void wrapper_heap_sort(ll list[], ll size){
    heap_sort(list, 1, size);
}

void bubble_sort(ll list[], ll size){
    for(ll i = size; i > 0; i--){
        // i is where this iteration, sorted 
        // item will exist.
        for(ll j = 1; j < i; j++){
            // from 1 to i, swap adjacent index.
            if(list[j] > list[j + 1])
                swap(list[j], list[j + 1]);
        }
    }
}

void insertion_sort(ll list[], ll left, ll right){
    // from the beginning of index,
    // choose where to insert in sorted array.
    for(ll i = left + 1; i <= right; i++) {
        // i is index where this iteration,
        // item in that index will be inserted
        // into sorted array 1 ~ i - 1.
        ll key = list[i], j = i - 1;
        while(j > left - 1 && list[j] > key){
            list[j + 1] = list[j];
            j--;
        }
        list[j + 1] = key;
    }
}

void wrapper_insertion_sort(ll list[], ll size){
    insertion_sort(list, 1, size);
}

void selection_sort(ll list[], ll size){
    // select smallest value from unsorted
    // sub array. then, repeat 2 ~ n again.
    ll key;
    for(ll i = 1; i <= size; i++){
        // i is where smallest value from
        // unsorted data is inserted.
        key = i;
        // from i to size, find smallest value
        for(ll j = i + 1; j <= size; j++){
            if(list[j] < list[key])
                key = j;
        }
        // if it is not key, exchange it.
        if(i != key){
            swap(list[i], list[key]);
        }
    }
}

ll partition(ll list[], ll left, ll right){
    ll x = list[right]; // pivot
    ll cursor = left; // cursor
    for(ll i = left; i < right; i++){
        if(list[i] <= x){
            swap(list[i], list[cursor++]);
        }
    }
    swap(list[cursor], list[right]);
    return cursor;
}

void quick_sort(ll list[], ll left, ll right){
    if(left >= right) return;
    ll mid = partition(list, left, right);
    quick_sort(list, left, mid - 1);
    quick_sort(list, mid + 1, right);
}

void wrapper_quick_sort(ll list[], ll size){
    quick_sort(list, 1, size);
}


ll binary_search_with_gaps(ll key, ll S[], int len) {
    ll low = 1, high = len;
    while (low <= high) {
        ll mid = (low + high) / 2;

        if (S[mid] == INF) {
            ll l = mid - 1, r = mid + 1;
            while (l >= low && S[l] == INF) l--;
            while (r <= high && S[r] == INF) r++;

            if (l < low && r > high) return low; 
            else if (l < low) {
                if (key <= S[r]) return r;
                else low = r + 1;
            }
            else if (r > high) {
                if (key > S[l]) return l + 1;
                else high = l - 1;
            }
            else {
                if (key <= S[l]) high = l - 1;
                else if (key > S[r]) low = r + 1;
                else return l + 1;
            }
        } else {
            if (key <= S[mid]) high = mid - 1;
            else low = mid + 1;
        }
    }
    return low;
}

void insert_with_gap(ll pos, ll S[], ll& current_len) {
    ll gapIndex = pos;
    while (gapIndex <= current_len && S[gapIndex] != INF) gapIndex++;

    for (int i = gapIndex; i > pos; --i) {
        S[i] = S[i - 1];
    }
    S[pos] = INF;
    if (gapIndex > current_len) current_len = gapIndex;
}

ll temp[MAX_SIZE];

void rebalance(ll S[], ll prev_len, ll& current_len) {
    ll new_len = min(current_len * 2, MAX_SIZE * 1LL);

    for (ll i = 1; i <= new_len; ++i) temp[i] = INF;

    ll count = 0; // # of inserted
    for (ll i = 1; i <= prev_len; ++i) {
        if (S[i] != INF) count++;
    }

    ll spacing = (new_len - count) / count;
    ll idx = 1;
    for (ll i = 1; i <= prev_len; ++i) {
        if (S[i] != INF) {
            if (idx <= new_len) temp[idx] = S[i];
            idx += spacing;
            if (idx > new_len) idx = new_len;
        }
    }

    for (ll i = 1; i <= new_len; ++i) S[i] = temp[i];
    current_len = new_len;

}

/**
 * Main function implementing Library Sort on the array 'list' of length 'size' (1-indexed).
 * After sorting, the sorted values are written back into 'list[1..size]'.
 */
void library_sort(ll list[], ll size) {
    // already sort is done
    if (size <= 1) return;
    double epsilon = 0.9;
    ll S_size = (ll)((1 + epsilon) * size + 50);

    for (ll i = 1; i <= S_size; ++i) {
        not_in_place[i] = INF;
    }

    ll capacity = S_size;
    ll current_len = 1;
    ll elements_to_insert = 1;
    ll pos = 1;

    not_in_place[1] = list[pos++];
    if (pos <= size) {
        ll prev_len = current_len;
        current_len = min((ll)((2 + 2 * epsilon) * elements_to_insert) + 5, capacity);
        rebalance(not_in_place, prev_len, current_len);
        elements_to_insert *= 2;
    }

    while (pos <= size) {
        ll round_inserts = min(elements_to_insert, size - pos + 1);
        for (ll i = 0; i < round_inserts; ++i) {
            ll key = list[pos++];
            ll ins_index = binary_search_with_gaps(key, not_in_place, current_len);
            if (ins_index > current_len) ins_index = current_len;
            if (not_in_place[ins_index] != INF) {
                insert_with_gap(ins_index, not_in_place, current_len);
            }
            not_in_place[ins_index] = key;
        }
        if (pos > size) break;
        ll prev_len = current_len;
        current_len = min((ll)((2 + 2 * epsilon) * elements_to_insert), capacity);
        rebalance(not_in_place, prev_len, current_len);
        elements_to_insert *= 2;
    }

    ll out_index = 1;
    for (ll i = 1; i <= capacity && out_index <= size; ++i) {
        if (not_in_place[i] != INF) {
            list[out_index++] = not_in_place[i];
        }
    }
}

void tim_sort(ll list[], ll size){
    // 어떻게 적절하게 각 run, 그러니까 부분 배열의 크기를 최적화할 수 있을까?
    ll run = 64;
    for(ll i = 1; i <= size; i += run){
        if(i + run - 1 > size){
            insertion_sort(list, i, size);
            break;
        }
        insertion_sort(list, i, i + run - 1);
    }
    
    for(ll i = run; i <= size; i <<= 1){
        // run size of this iteration : i
        // then merge 1 to i - 1, i to 2 * i - 1
        for(ll j = 1; j <= size; j += 2 * i){
            ll left = j;
            ll mid = min(j + i - 1, size);
            ll right = min(j + 2 * i - 1, size);
            if(mid < right){
                merge(list, left, mid, right);
            }
        }
    }
}

void cocktail_shaker_sort(ll list[], ll size){
    /*
    cocktail shaker sort is bubble sort in ambidirectional.

    unlike bubble sort sorted in only one direction,

    cocktail shaker sort back and forth.

    */
    ll left = 1, right = size;
    bool swapped = 0;
    while(left < right){
        // from left to right, push max value in 
        // unsorted array, to right.
        for(ll i = left; i < right; i++){
            if(list[i] > list[i + 1]){
                swap(list[i], list[i + 1]);
                swapped = 1;
            }
        }
        // after pusing, index at right is sorted.
        if(!swapped)    break;
        swapped = 0;
        right--;

        // after then, lower right and from right to left,
        // this time push minimum value in unsorted array to left. 
        for(ll i = right; i > left; i--){
            if(list[i] < list[i - 1]){
                swap(list[i], list[i - 1]);
                swapped = 1;
            }
        }

        if(!swapped)    break;
        swapped = 0;
        left++;
        // after pusing minimum value to left,
        // increase left index such that lowering
        // investigation range.
    }
}

void comb_sort(ll list[], ll size){
    // 적절한 shrink 값은?
    double shrink = 1.3;
    ll gap = size;
    bool swapped = 1;
    while(gap > 1 || swapped){
        // while gap is larger than one, must iterate.
        // after gap became 1, it is just like bubble sort
        // but if no swap happens, break;
        gap = ll(gap * 1.0 / shrink);
        if(!gap)    gap = 1;
        swapped = 0;

        for(ll i = 1; i + gap <= size; i++){
            if(list[i] > list[i + gap]){
                swap(list[i], list[i + gap]);
                swapped = 1;
            }
        }
    }
}

void tournament_sort(ll list[], ll size){
    // tournament sort is not in-place sort algorithm. 
    // using heap's lowest index inner node is n / 2,
    // shift all nodes size - 1. then
    // it all become leaf node.
    for(ll i = 1; i <= size; i++){
        // from N to N + N - 1, every input data in
        not_in_place[size - 1 + i] = list[i];
    }
    for(ll i = size - 1; i > 0; i--){
        not_in_place[i] = min(not_in_place[i * 2], not_in_place[i * 2 + 1]);
    }
    for(ll i = 1; i <= size; i++){
        list[i] = not_in_place[1];
        ll cur = 1;
        while(cur < size){
            // down heap
            // checking left first let this sort stable.
            if(not_in_place[cur] == not_in_place[cur * 2]){
                cur = cur * 2;
            }
            else{
                cur = cur * 2 + 1;
            } 
        }
        // size index is where leaf node starts. 
        // after cur < size, it is leaf node index.
        not_in_place[cur] = INF;
        while(cur > 1){
            cur >>= 1;
            not_in_place[cur] = min(not_in_place[cur * 2], not_in_place[cur * 2 + 1]);
        }
    }
}

void intro_sort(ll list[], ll maxdepth, ll left, ll right){
    // intro sort does 3 sort.
    ll size = right - left + 1;
    if(size < 16){
        // when given sub array is smaller than 16, insertion sort.
        insertion_sort(list, left, right);
    }
    else if(!maxdepth){
        // 어떻게 maxdepth를 최적으로 고를 수 있는가?
        // when maxdepth = log_2(size) * 2 is done, heap sort
        heap_sort(list, left, right);
    }
    else{
        // default, quick sort.
        ll mid = partition(list, left, right);
        intro_sort(list, maxdepth - 1, left, mid - 1);
        intro_sort(list, maxdepth - 1, mid + 1, right);
    }
}

ll intLog2(ll n) {
    ll res = 0;
    while (n >>= 1) res++;
    return res;
}

void wrapper_intro_sort(ll list[], ll size){
    intro_sort(list, 2 * intLog2(size), 1, size);
}

enum SortType{
    UNKNOWN, BUBBLE, INSERTION, QUICK, MERGE, SELECTION, HEAP, LIBRARY, TIM, COCKTAIL_SHAKER, COMB, TOURNAMENT, INTRO
};

SortType get_sort_type(const string &str){
    if(str == "bubble") return BUBBLE;
    if(str == "insertion") return INSERTION;
    if(str == "quick") return QUICK;
    if(str == "merge") return MERGE;
    if(str == "heap") return HEAP;
    if(str == "selection") return SELECTION;
    if(str == "library")    return LIBRARY;
    if(str == "tim")    return TIM;
    if(str == "cocktail_shaker") return COCKTAIL_SHAKER;
    if(str == "comb")   return COMB;
    if(str == "tournament") return TOURNAMENT;
    if(str == "intro")   return INTRO;
    return UNKNOWN;
}

int main(int argc, char* argv[]){
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    if (argc != 3) {
        cerr << "error: use as ./sort.exe [sort_name] [input_file]" << endl;
        cerr << "for example: ./sort.exe quick input.txt" << endl;
        return 1;
    }
    string sort_type = argv[1];
    string input_filename = argv[2];
    ifstream infile(input_filename);
    if (!infile) {
        cerr << "impossible to open: " << input_filename << endl;
        return 1;
    }
    ll n;
    infile >> n;
    for(ll i = 1; i <= n; i++){
        infile >> input_array[i];
    }
    infile.close();

    void (*func_to_exec)(ll[], ll);
    switch (get_sort_type(sort_type)) {
        case BUBBLE: func_to_exec = bubble_sort; break;
        case INSERTION: func_to_exec = wrapper_insertion_sort; break;
        case QUICK: func_to_exec = wrapper_quick_sort; break;
        case SELECTION: func_to_exec = selection_sort; break;
        case HEAP: func_to_exec = wrapper_heap_sort; break;
        case MERGE: func_to_exec = wrapper_merge_sort; break;
        case LIBRARY: func_to_exec = library_sort; break;
        case TIM: func_to_exec = tim_sort; break;
        case COCKTAIL_SHAKER: func_to_exec = cocktail_shaker_sort; break;
        case COMB: func_to_exec = comb_sort; break;
        case TOURNAMENT: func_to_exec = tournament_sort; break;
        case INTRO: func_to_exec = wrapper_intro_sort; break;
        default: cerr << "UNKNOWN.\n"; break;
    }


    auto start = chrono::high_resolution_clock::now();

    func_to_exec(input_array, n);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "time take using " << sort_type << " sort: " << duration.count() << "ms\n";

    string file_name = sort_type;
    file_name += "output.txt";

    ofstream outfile(file_name);  
    for(ll i = 1; i <= n; i++){
        outfile << input_array[i] << ' ';
    }
    outfile << endl;
    outfile.close();

    // Every unsorted array datas are from
    // file. According to the argument of function,
    // after sort, write sorted value in output.txt.

    // 1. Evert array starts from index 1.

    // 2. every input file contains n + 1 data.
    // first data is array size, remaining n data are
    // unsorted array value.

    // 3. As using input data from file, all data are treated as
    // offline data.
    return 0;
}