#ifndef UBERMENSCHENAMOGUS228_ALGS_H
#define UBERMENSCHENAMOGUS228_ALGS_H
#include <header.h>
#include <functional>
template <class T>
int find(const std::vector<T>& v,const T& s){
    for(int i = 0; i < v.size(); ++i){
        if(v[i] == s)
            return i;
    }
    return -1;
}

int find (const std::vector<std::string>& v, const char* s);

int find(const std::string& s,const char s0);

int find(const std::string& s,const std::string& s0);

template <class T>
int binarySearch(std::vector<T>& v,const T x, int leftoffset = 0, int rightoffset = 0) 
{
  int l = leftoffset;
  int r = v.size() - 1 - rightoffset;
  while (l <= r) {
    int mid = l + (r - l) / 2;
    if(v[mid] == x)
      return mid;
    else if(v[mid] < x)
      l = mid + 1;
    else
      r = mid - 1;
  }
  return -1;
}

template <class T>
void insert_sort(std::vector<T>& a, std::function<bool(T&, T&)> comp = [](T& x, T& y){return (x < y);}){    //insertion sort, O(n^2)
    for(long unsigned int i = 1; i < a.size(); ++i){
        for(int j = i-1; j >= 0 && !comp(a[j],a[j+1]); --j)
            swap(a[j],a[j+1]);
    }
}  //insertion sort, O(n^2), по возрастанию



template <class T>
void merge(std::vector<T>& v,std::vector<T>& v1, std::vector<T>& v2, std::function<bool(T&,T&)> comp){
    v.clear();
    long unsigned int i = 0, j = 0;
    while(i < v1.size() && j < v2.size()){
        if(comp(v1[i],v2[j])){
            v.push_back(v1[i]);
            i++;
        }
        else{
            v.push_back(v2[j]);
            j++;
        }
    }
    if(i < v1.size()){
        for(long unsigned int k = i; k < v1.size();++k)
            v.push_back(v1[k]);
    }
    else if(j < v2.size()){
        for(long unsigned int k = j; k < v2.size();++k)
            v.push_back(v2[k]);
    }
} // Вспомогательная функция merge sort
template <class T>
void merge_sort(std::vector<T>& v, std::function<bool(T&, T&)> comp = [](T& x, T& y){return (x < y);}){   // Merge Sort, O(n * log(n))
    if(v.size() == 2){
        if(!comp(v[0],v[1])){
            swap(v[0],v[1]);
        }
    }
    else if(v.size() > 2){
        std::vector<T> v1(v.begin(), v.begin() + (v.size() / 2));
        std::vector<T> v2(v.begin() + (v.size() / 2), v.end());
        merge_sort(v1,comp);
        merge_sort(v2,comp);
        merge(v,v1,v2,comp);
    }
} // Merge Sort, O(n * log(n)), по возрастанию, O(n) - память

template <class T>
int partition(std::vector<T>& v, int p, int r){
    int x = v[p];
    int i = p;
    int j = r;
    while(true){
        while(v[j] <= x && j >= 0)
            j--;
        while(v[i] >= x && i < v.size())
            i++;
        if(i < j)
            swap(v[i],v[j]);
        else
            return j;
    }
}
template <class T>
void quickSort(std::vector<T>& v, int p, int r){
    if(p < r){
        int q = partition(v,p,r);
        quickSort(v,p,q);
        quickSort(v,q+1,r);
    }
}
template <class T>
void quickSort(std::vector<T>& v){
    int p = 0, r = v.size()-1;
    quickSort(v,p,r);
}


#endif //UBERMENSCHENAMOGUS228_ALGS_H