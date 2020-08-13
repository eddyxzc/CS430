
#include <iostream>
#include<random>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;


void InsertSort(int *A, int p, int q) {
    for (int i = p; i < q; i++) {
        for (int j = i + 1; j > p && A[j] < A[j - 1]; j--) {
            std::swap(A[j], A[j - 1]);
        }
    }
}


int Partition(int *A, int p, int r) {
    //int key = pvalue;//
    int key = A[r];
    int i = p;
    for (int j = p; j < r; j++) {
        if (A[j] <= key) {

            std::swap(A[i], A[j]);
            i++;
        }
    }
    std::swap(A[i], A[r]);
    return i;
}

int GetRandIndex(int low, int high) {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(low, high);
    return dist(mt);
}


// return kth smallest value in A[p,q], GP is 5,3,or 7
template<int GP>
int SelectKth(int *A, int p, int q, int k) {
    int nitems = GP;// items in one group, may be 3,5 or 7 items in one froup
    if (q - p < nitems) {
        // there's only one group, do insert sorting directly and return
        InsertSort(A, p, q);
        return A[p + k - 1];
    } else {
        // We have to divide the whole array to several gropus
        int pqlength = q - p + 1;// Length of A[p,q].
        int pqEnd = p + pqlength / nitems - 1;// this is the end index of A[p,q],num of items inA[pqend,q]
        // is less than ntimes, they are not enough to form a group, they are excluded from finding median
        // process

        int ngroup = pqlength / nitems;// number of groups
        for (int igroup = 0; igroup < ngroup; igroup++) {
            int istart = igroup * nitems + p;
            int iend = istart + nitems - 1;
            InsertSort(A, istart, iend);
            std::swap(A[p + igroup], A[p + igroup * nitems + nitems / 2]);// swap the median to the first position
        }

        // now we need to find median of the median.
        // The median found in last step is located in A[p,p+ngroup-1], total of ngroup elements.


        int kMoM = ngroup;// even number,we take the lower median, e.g. we have 4 groups, than median is the 2nd  item.
        if (kMoM % 2 != 0) kMoM = ngroup / 2 + 1;
        else kMoM = ngroup / 2;
        // odd number,e.g. we have 5 groups, than median is the 3rd  item.


        int MoM = SelectKth<GP>(A, p, p + ngroup - 1, kMoM);// MOM is the median of medians
        // Partition use A[q] as the partition value, so we have to swap the MOM to A[q]
        //int iMoM = Partition(A, p, q);// the index of MOM in A[p,q]

        //Here we need one more step to find the index of MoM, MoM is just the value
        // to use partition function we have to swap the MoM to A[q];
        for (size_t i = p; i <= q; i++) {
            if (A[i] == MoM) {
                std::swap(A[i], A[q]);
                break;
            }
        }
        int iMoM = Partition(A, p, q);
        int MOMisKth = iMoM - p + 1;// MOM is the MOMisKth smallest of A[p,q]

        if (k == MOMisKth) {// MoM is the kth smallest number
            return MoM;
        } else if (k < MOMisKth) {
            // find kth smallest number in A[p,iMoM-1];
            return SelectKth<GP>(A, p, iMoM - 1, k);
        } else {
            // find kth smallest number in A[iMoM+1,q];
            // note the k is k-(iMOM+1) now
            //return SelectKth<GP>(A,  iMoM + 1,q, k-iMoM-1);
            return SelectKth<GP>(A, iMoM + 1, q, k - MOMisKth);
        }
    }

}

int RandomPartition(int *A, int p, int r) {
    int rdIndex = GetRandIndex(p, r);
    //cout << "RandomPartition" << rdIndex << endl;
    std::swap(A[rdIndex], A[r]);
    int partitionIndex = Partition(A, p, r);
    return partitionIndex;
}

int RandomSelectIth(int* A, int p, int r, int i)
{
    if (p == r)// only one element
        return A[p];
    int q = RandomPartition(A, p, r);
    int k = q - p + 1;
    if (i == k)
    {
        return A[q];
    }
    else if (i<k)
    {
        return RandomSelectIth(A, p, q - 1, i);
    }else{
        return RandomSelectIth(A, q+1, r, i-k);
    }
}

int MedianPartition(int *A, int p, int r) {
    int pivotValue = SelectKth<5>(A, p, r, (r - p + 1 + 1) / 2);
    for (size_t i = p; i <= r; i++) {
        if (A[i] == pivotValue) {
            std::swap(A[i], A[r]);
            break;
        }
    }
    int pivotIndex = Partition(A, p, r);
    cout << p << " " << r << " " << pivotIndex << endl;
    return pivotIndex;
}

//template function for quickSort with partition function as a argument PART
template<typename PART>
void QuickSort(int *A, int low, int high, PART parter) {
    //cout << "QuickSort " << low << " " << high << endl; debug
    typedef PART Partition;
    if (high <= low)
        return;
    int pivot = parter(A, low, high);
    QuickSort<Partition>(A, low, pivot - 1, parter);
    QuickSort<Partition>(A, pivot + 1, high, parter);
}


int main(int argc, char* argv[]) {

     cout<< "input a data file name"<<endl;
     string datafile;
     cin>>datafile;
    std::ifstream infstream(datafile);// input text file
    if (infstream.fail())
    {
        cout<<"open file failed!"<<endl;
        infstream.close();
        return -1;
    }
    vector<int> res;
    string line;
    while (getline(infstream, line)) {
        stringstream ss; //input stream
        ss << line; //put one line
        if (!ss.eof()) {
            int temp;
            while (ss >> temp) // get a int
                res.push_back(temp); //store in vector
        }
    }
    infstream.close();
    cout<<"data loaded."<<endl;
    //sort(res.begin(),res.end());debug
    cout<<"order statistics kth:"<<endl;

    int k=0;
    cin>>k;
    if (k<0||k>res.size())
    {
        cout<<"order statistics out range!"<<endl;
        return -1;
    }
    cout<<"choose a median group, 3,5 or 7, or type 0 to choose random select:"<<endl;
    cout<<"type 9 to run all 4 methods"<<endl;
    int group=0;
    cin>>group;


    int* A= new int[res.size()];
    for (int i = 0; i < res.size(); ++i) {
        A[i]=res[i];
    }
    int n=res.size();

    switch (group) {
        case 3:{
            auto median = SelectKth<3>(A, 0, n-1, k);
            cout<<"group 3 "<<k<<" th order statistics is "<<median<<endl;
            break;
        }
        case 5:{
            auto median = SelectKth<5>(A, 0, n-1, k);
            cout<<"group 5 "<<k<<" th order statistics is "<<median<<endl;
            break;
        } case 7:{
            auto median = SelectKth<7>(A, 0, n-1, k);
            cout<<"group 7 "<<k<<" th order statistics is "<<median<<endl;
            break;
        } case 0:{
            auto median = RandomSelectIth(A, 0, n-1, k);
            cout<<"random select"<<k<<" th order statistics is "<<median<<endl;
            break;
        }
        case 9:{
            auto median = SelectKth<3>(A, 0, n-1, k);
            cout<<"group 3 "<<k<<" th order statistics is "<<median<<endl;
            median = SelectKth<5>(A, 0, n-1, k);
            cout<<"group 5 "<<k<<" th order statistics is "<<median<<endl;
            median = SelectKth<7>(A, 0, n-1, k);
            cout<<"group 7 "<<k<<" th order statistics is "<<median<<endl;
            median = RandomSelectIth(A, 0, n-1, k);
            cout<<"random select"<<k<<" th order statistics is "<<median<<endl;
            break;
        }
        default: {
            cout << "input is wrong!" << endl;
            return -1;
        }
    }

    delete [] A;
    return 0;
}

