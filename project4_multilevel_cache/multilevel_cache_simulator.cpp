#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <bitset>
#include <map>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

using namespace std;

unsigned long total = 0;
unsigned long read = 0;
unsigned long write = 0;
unsigned long l1rm = 0;
unsigned long l2rm = 0;
unsigned long l1wm = 0;
unsigned long l2wm = 0;
unsigned long l1ce = 0;
unsigned long l2ce = 0;
unsigned long l1de = 0;
unsigned long l2de = 0;


class Cache{
private:
    unsigned long c; // capacity (1)
    unsigned long a;  // associativity (1)
    unsigned long c2;  // capacity (2)
    unsigned long a2;  // associativity (2)
    unsigned long b;  // block size
    bool s;  // lru or random
    
    unsigned long bo; // block offset bit
    unsigned long ix; // index bit (1)
    unsigned long tg; // tag bit (1)
    unsigned long ix2; // index bit (2)
    unsigned long tg2; // tag bit (2)
    
    // level 1
    vector<vector<string>> mycache;
    vector<int> vcache;
    vector<vector<bool>> dccache;
    
    // level 2
    vector<vector<string>> mycache2;
    vector<int> vcache2;
    vector<vector<bool>> dccache2;
    
    
public:
    Cache(unsigned long C, unsigned long A, unsigned long B, bool S){
       
        // Calculate Field (bo, ix, tg)
        c2 = C;
        a2 = A;
        c = c2/4;
        if (a2 <=2){
            a = a2;
        }
        else if (a2 > 2){
            a = a2/4; 
        }
        b = B;
        s = S;  
        
        bo = (unsigned long)log2(b); // block offset bit
        ix = (unsigned long)log2((c*pow(2,10))/(b*a)); // index bit
        tg = 48 - bo - ix; // tag bit
        
        unsigned long indexRow = (unsigned long)pow(2, ix);

        // make mycache (1)
        mycache.resize((unsigned long) indexRow);
        for (unsigned long i = 0 ; i < indexRow ; i++){
            mycache[i].resize(a, "0x0");
        }
        // make vcache (1)
        vcache.resize((unsigned long) indexRow, 0);
        // make dccache (1)
        dccache.resize((unsigned long) indexRow);
        for (unsigned long w = 0 ; w < indexRow ; w++){
            dccache[w].resize(a, false);
        }
        
        
        ix2 = (unsigned long)log2((c2*pow(2,10))/(b*a2)); // index bit (2)
        tg2 = 48 - bo - ix2; // tag bit (2)        
        unsigned long indexRow2 = (unsigned long)pow(2, ix2);
        
        // make mycache2 (2)
        mycache2.resize((unsigned long) indexRow2);
        for (unsigned long i = 0 ; i < indexRow2 ; i++){
            mycache2[i].resize(a2, "0x0");
        }
        // make vcache2 (2)
        vcache2.resize((unsigned long) indexRow2, 0);
        // make dccache2 (2)
        dccache2.resize((unsigned long) indexRow2);
        for (unsigned long w = 0 ; w < indexRow2 ; w++){
            dccache2[w].resize(a2, false);
        }        
    }
    
    void C2Read(string block){         
        string index = block.substr(48-bo-ix2, ix2);
        string tag = block.substr(0, tg2);
        unsigned long long in = stoull(index, nullptr, 2);
        
        if (vcache2[in] == 1){ // index hit
            auto it = find(mycache2[in].begin(), mycache2[in].end(), tag);
            if (it == mycache2[in].end()){ // tag miss (L1)
                l2rm += 1;
                auto itt = find(mycache2[in].begin(), mycache2[in].end(), "0x0");
                if(itt == mycache2[in].end()){
                    if (s == false){
                        random_device rd;
                        mt19937 generator(rd());
                        uniform_int_distribution<int> distribution(0, a2-1);
                        int ran = distribution(generator);
                        mycache2[in].at(ran) = tag; //## eviction
                        if (dccache2[in][ran] == true){
                            l2de +=1;
                            dccache2[in].at(ran) = false;
                        }
                        else if (dccache2[in][ran] == false){
                            l2ce += 1;
                        }
                    }
                    else if (s == true){
                        for (int i = a2-1 ; i>0 ; i--){
                            mycache2[in][i] = mycache2[in][i-1];
                        }
                        mycache2[in].at(0) = tag; //## eviction
                        if (dccache2[in][0] == true){
                            l2de += 1;
                            dccache2[in].at(0) = false;
                        }
                        else if (dccache2[in][0] == false){
                            l2ce += 1;
                        }
                    }
                }
                else if(itt != mycache2[in].end()){
                    for (int i = a2 -1 ; i > 0 ; i--){
                        mycache2[in][i] = mycache2[in][i-1];
                    }
                    mycache2[in].at(0) = tag;
                }
            }
            else if (it != mycache2[in].end()){ // tag hit
                int x = distance(mycache2[in].begin(), it);
                swap(mycache2[in][x], mycache2[in][0]);
            }
        }
        else if (vcache2[in] == 0){ // index miss (L1)
            l2rm += 1;
            mycache2[in].at(0) = tag;
            vcache2[in] = 1;
        }                    
    }   
    
    
    void CRead(string block){         
        string index = block.substr(48-bo-ix, ix);
        string tag = block.substr(0, tg);
        unsigned long long in = stoull(index, nullptr, 2);        
        if (vcache[in] == 1){ // index hit
            auto it = find(mycache[in].begin(), mycache[in].end(), tag);
            if (it == mycache[in].end()){ // tag miss (L1)
                l1rm += 1;
                auto itt = find(mycache[in].begin(), mycache[in].end(), "0x0");
                if (itt == mycache[in].end()){ // no empty
                    if (s == false){ // random
                        random_device rd;
                        mt19937 generator(rd());
                        uniform_int_distribution<int> distribution(0, a-1);
                        int ran = distribution(generator);
                        mycache[in].at(ran) = tag; //## eviction
                        if (dccache[in][ran] == true){
                            l1de += 1;
                            dccache[in][ran] = false;
                        }
                        else if (dccache[in][ran] == false){
                            l1ce += 1;
                        }
                    }
                    else if (s == true){ // lru
                        for (int i = a-1 ; i > 0 ; i--){
                            mycache[in][i] = mycache[in][i-1];
                        }
                        mycache[in].at(0) = tag; //## eviction
                        if (dccache[in][0] == true){
                            l1de += 1;
                            dccache[in].at(0) = false;
                        }
                        else if (dccache[in][0] == false){
                            l1ce += 1;
                        }  
                    }
                }
                else if (itt != mycache[in].end()){ // has "0x0"
                    for (int i = a-1 ; i > 0 ; i--){
                        mycache[in][i] = mycache[in][i-1];
                    }
                    mycache[in].at(0) = tag;
                }
                C2Read(block);
            }
            else if (it != mycache[in].end()){ // tag hit
                int x = distance(mycache[in].begin(), it);
                swap(mycache[in][x], mycache[in][0]);
            }
        }
        else if (vcache[in] == 0){ // index miss (L1)            

            l1rm += 1;
            mycache[in].at(0) = tag;
            vcache[in] = 1;
            C2Read(block);
        }               
    }
    
    void C2Write(string block){
        string index = block.substr(48-bo-ix, ix2);
        string tag = block.substr(0, tg2);
        unsigned long long in = stoull(index, nullptr, 2);
        
        if (vcache2[in] == 1){ // index hit
            auto it = find(mycache2[in].begin(), mycache2[in].end(), tag);
            if (it == mycache2[in].end()){ // tag miss (L1)
                l2wm += 1;
                auto itt = find(mycache2[in].begin(), mycache2[in].end(), "0x0");
                if (itt == mycache2[in].end()){
                    if (s == false){  // random
                        random_device rd;
                        mt19937 generator(rd());
                        uniform_int_distribution<int> distribution(0, a2-1);
                        int ran = distribution(generator);
                        mycache2[in].at(ran) = tag; // dirty (l2-mem) & eviction
                        if (dccache2[in][ran] == true){
                            l2de += 1;
                            dccache2[in].at(ran) = false;
                        }
                        else if (dccache2[in][ran] == false){
                            l2ce += 1;
                        }
                        // dccache2[in].at(ran) = true;
                    }
                    else if (s == true){  // lru
                        for(int i = a2-1 ; i>0 ; i--){
                            mycache2[in][i] = mycache2[in][i-1];
                        }
                        mycache2[in].at(0) = tag; // dirty (l2-mem) & eviction
                        if (dccache2[in][0] == true){
                            l2de +=1;
                            dccache2[in].at(0) = false;
                        }
                        else if (dccache2[in][0] == false){
                            l2ce +=1;
                        }
                        // dccache2[in].at(0) = true;
                    }
                }
                else if(itt != mycache2[in].end()){
                    for (int i = a2 - 1 ; i > 0 ; i--){
                        mycache2[in][i] = mycache2[in][i-1];
                    }
                    mycache2[in].at(0) = tag; // dirty (l2-mem)
                    // dccache2[in].at(0) = true;
                }
            }
            else if (it != mycache2[in].end()){ // tag hit
                int x = distance(mycache2[in].begin(), it);
                swap(mycache2[in][x], mycache2[in][0]);
                dccache2[in].at(0) = true;
            }
        }
        else if (vcache2[in] == 0){ // index miss (L1)
            l2wm += 1;
            mycache2[in].at(0) = tag; // dirty (l2-mem)
            vcache2[in] = 1;
            // dccache2[in].at(0) = true;
        }        
    }
    
    void CWrite(string block){
        string index = block.substr(48-bo-ix, ix);
        string tag = block.substr(0, tg);
        unsigned long long in = stoull(index, nullptr, 2);
        if (vcache[in] == 1){ // index hit
            auto it = find(mycache[in].begin(), mycache[in].end(), tag);
            if (it == mycache[in].end()){ // tag miss (L1)
                l1wm += 1;
                auto itt = find(mycache[in].begin(), mycache[in].end(), "0x0");
                if (itt == mycache[in].end()){
                    if (s == false){ // random
                        random_device rd;
                        mt19937 generator(rd());
                        uniform_int_distribution<int> distribution(0, a-1);
                        int ran = distribution(generator);
                        //@@
                        string block2 = mycache[in].at(ran) + index + block.substr(48-bo-ix);
                        string tt = block2.substr(0, tg2);
                        auto ittt = find(mycache2[in].begin(), mycache2[in].end(), tt);
                        if (ittt != mycache2[in].end()){
                            dccache2[in][find(mycache2[in].begin(), mycache2[in].end(), tt) - mycache2[in].begin()] = true;
                        }
                        //@@                         
                        mycache[in].at(ran) = tag;
                        if (dccache[in][ran] == true){
                            l1de += 1;
                            dccache[in].at(ran) = false;
                        }
                        else if (dccache[in][ran] == false){
                            l1ce +=1;
                        }
                        // dccache[in].at(ran) = true;
                    }
                    else if (s == true){ // lru
                        //@@
                        string block2 = mycache[in].at(a-1) + index + block.substr(48-bo-ix);
                        string tt = block2.substr(0, tg2);
                        auto ittt = find(mycache2[in].begin(), mycache2[in].end(), tt);
                        if (ittt != mycache2[in].end()){
                            dccache2[in][find(mycache2[in].begin(), mycache2[in].end(), tt) - mycache2[in].begin()] = true;
                        }
                        //@@                      
                        for(int i = a-1 ; i>0 ; i--){
                            mycache[in][i] = mycache[in][i-1];
                        }
                        mycache[in].at(0) = tag;
                        if (dccache[in][0] == true){
                            l1de += 1;
                            dccache[in].at(0) = false;
                        }
                        else if (dccache[in][0] == false){
                            l1ce += 1;
                        }
                        // dccache[in].at(0) = true;
                    }
                }
                else if(itt != mycache[in].end()){
                    for (int i = a-1 ; i > 0 ; i--){
                        mycache[in][i] = mycache[in][i-1];
                    }
                    mycache[in].at(0) = tag;
                    // dccache[in].at(0) = true;
                }
                C2Write(block);
            }
            else if (it != mycache[in].end()){ // tag hit
                int x = distance(mycache[in].begin(), it);
                swap(mycache[in][x], mycache[in][0]); //
                dccache[in].at(0) = true;
            }
        }
        else if (vcache[in] == 0){ // index miss (L1)
            l1wm += 1;
            mycache[in].at(0) = tag;
            vcache[in] = 1;
            // dccache[in].at(0) = true;
            C2Write(block);
        }        
    }
};


// hexa code -> binary code
string hex2bin(string hexline){
    
    unsigned long long decimal = stoull(hexline, nullptr, 16);
    bitset<48> binary(decimal);
    return binary.to_string();
}


int main(int argc, char* argv[]){
    // 0 : runfile, 1 : capacity, 2 : associativity, 3 : block size, 4 : lru/random, 5 : file
    unsigned long c;
    unsigned long a;
    unsigned long b;
    bool s;    // true : lru || false : random
    
    for (int i = 0 ; i < argc ; i++){
        if (strcmp(argv[i], "-c") == 0){
            c = stoul(argv[i+1]);
            i++;
        }
        else if (strcmp(argv[i], "-a") == 0){
            a = stoul(argv[i+1]);
            i++;
        }
        else if (strcmp(argv[i], "-b") == 0){
            b = stoul(argv[i+1]);
            i++;
        }
        else if (strcmp(argv[i], "-lru") == 0){
            s = true;
        }
        else if (strcmp(argv[i], "-random") == 0){
            s = false;
        }
    }
    // open file
    ifstream infile(argv[argc - 1]);
    if (!infile.is_open()){
        cout << "Couldn't open file" << endl;
        return 1;
    }

    
    string line;
    string hexa;
    string bline;
    unsigned long p;
    
    Cache MyC(c, a, b, s);
    
    while (getline(infile, line)){
        // Read
        if (line.find("R") != string::npos){
            // scaling
            p = line.find("R");
            hexa = line.substr(p+2);
            read += 1;
            total += 1;
            bline = hex2bin(hexa);
            
            MyC.CRead(bline);
        }
        // Write
        else if (line.find("W") != string::npos){
            // scaling
            p = line.find("W");
            hexa = line.substr(p+2);
            write +=1;
            total += 1;
            bline = hex2bin(hexa);
            
            MyC.CWrite(bline);
        }
    }
    

    
    unsigned long a1;
    if (a <= 2){
        a1 = a;
    }
    else if (a > 2){
        a1 = a/4;
    }
    
    string name = argv[argc - 1];
    int h = name.find(".out");
    string name2 = name.substr(0, h);
    string filename = name2+'_'+to_string(c)+'_'+to_string(a)+'_'+to_string(b) +".out";
    ofstream ofile(filename);

    // cout
    ofile << "--General Stats--" << endl;
    ofile << "L1 Capacity:    " << c/4 << endl;
    ofile << "L1 way: " << a1 << endl;
    ofile << "L2 Capacuty:    " << c << endl;
    ofile << "L2 way:    " << a << endl;
    ofile << "Block Size:    " << b << endl;
    ofile << "Total accesses: " << total << endl;
    ofile << "Read accesses: " << read << endl;
    ofile << "Write accesses: " << write << endl;
    ofile << "L1 Read misses: " << l1rm << endl;
    ofile << "L2 Read misses: " << l2rm << endl;
    ofile << "L1 Write misses: " << l1wm << endl;
    ofile << "L2 Write misses: " << l2wm << endl;
    ofile << "L1 Read miss rate: " << (l1rm*100/read) << "%" << endl;
    ofile << "L2 Read miss rate: " << (l2rm*100/l1rm) << "%" << endl;
    ofile << "L1 Write miss rate: " << (l1wm*100/write) << "%" << endl;
    ofile << "L2 write miss rate: " << (l2wm*100/l1wm) << "%" << endl;
    ofile << "L1 Clean eviction: " << l1ce << endl;
    ofile << "L2 clean eviction: " << l2ce << endl;
    ofile << "L1 dirty eviction: " << l1de << endl;
    ofile << "L2 dirty eviction: " << l2de << endl;
    
    // close file
    infile.close();
    ofile.close();
    
    return 0;
}
