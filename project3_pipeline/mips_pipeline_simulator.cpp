// Control Harzard (stall problem) <problem 1>
// 1) atp / antp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <bitset>
#include <map>

using namespace std;

// 01. Basic Data
string pc = "0x400000";

map<int, string> reg = {
    {0, "0x0"}, {1, "0x0"}, {2, "0x0"}, {3, "0x0"}, {4, "0x0"}, {5, "0x0"},
    {6, "0x0"}, {7, "0x0"}, {8, "0x0"}, {9, "0x0"}, {10, "0x0"}, {11, "0x0"},
    {12, "0x0"}, {13, "0x0"}, {14, "0x0"}, {15, "0x0"}, {16, "0x0"}, {17, "0x0"},
    {18, "0x0"}, {19, "0x0"}, {20, "0x0"}, {21, "0x0"}, {22, "0x0"}, {23, "0x0"},
    {24, "0x0"}, {25, "0x0"}, {26, "0x0"}, {27, "0x0"}, {28, "0x0"}, {29, "0x0"},
    {30, "0x0"}, {31, "0x0"}
};

map <string, string> memo = {

};

void makememo(int textsize, int datasize, map<string, string>* memo, ifstream& infile, int n){
    
    string inssa = "0x400000";
    string dasa = "0x10000000";
    string insca = inssa;
    string daca = dasa;
    string li;
    if (n==-1 || n >= textsize){
        for (int i = 0 ; i < textsize ; i++){
            getline(infile, li);
            (*memo)[insca] = li;
            
            int temp1 = stoi(insca, nullptr, 16);
            temp1 += 4;
            stringstream ss;
            ss << hex << "0x" << temp1;
            insca = ss.str();
        }
    }
    
    else if (0 < n < textsize){
        for (int i = 0 ; i < n ; i++){
            getline(infile, li);
            (*memo)[insca] = li;
            
            int temp1 = stoi(insca, nullptr, 16);
            temp1 += 4;
            stringstream ss;
            ss << hex << "0x" << temp1;
            insca = ss.str();      
        }
    }   
    
    for (int i = 0 ; i < datasize ; i++){
        getline(infile, li);
        (*memo)[daca] = li;

        int temp2 = stoi(daca, nullptr, 16);
        temp2 +=4;
        stringstream ss2;
        ss2 << hex << "0x" << temp2;
        daca = ss2.str();
    }
    // file pointer
    infile.clear();
    infile.seekg(0, ios::beg); 
}

// state register (data)
map<string, string> IFID={
    {"ins32", ""},
    {"pc", ""}
};

map<string, string> IDEX = {
    {"pc", ""},
    {"rs", ""},
    {"rt", ""},
    {"imm", ""},
    {"rd", ""},
    {"op", ""},
    {"jtar", ""},
    {"shamt", ""},
    {"funct", ""}
};

map<string, string> EXMEM = {
    {"pc", ""},
    {"alu", ""},
    {"tar", ""},
    {"op", ""},
    {"rd", ""},
    {"rt", ""},
    {"bit", ""},
    {"funct", ""}
};

map<string, string> MEMWB = {
    {"pc", ""},
    {"alu", ""},
    {"mem", ""},
    {"op", ""},
    {"rd", ""},
    {"rt", ""},
    {"bit", ""}
};

map<string, string> WBS = {
    {"pc", ""}
};

// Mux
map <string, bool> Mux = {    
    {"control", false},
    {"regread", false},
    {"memload", false},
    {"regwrite", false},
    {"stall", false},
    {"forwardrs", false},
    {"forwardrt", false},
    {"keep", true},
    {"unj", false},
    {"flush", false},
    {"bstall", false}
};

// 02. Cout Functions
int cycle = 0;

void cpipe(){

    cout << "==== Cycle: " << cycle << " ====" << endl;
    string pipe = '{' + IFID["pc"] + '|' + IDEX["pc"] + '|' + EXMEM["pc"] + '|' + MEMWB["pc"] + '|' + WBS["pc"] +'}';
    // noop는 따로 다시 생각해보자! 
    
    cout <<"Current pipeline PC state:" << endl;
    cout << pipe << endl << endl;
}

void compipe(){

    cout << "==== Completion cycle: " << cycle << " ====" << endl;
    cout << endl;
    string pipe = '{' + IFID["pc"] + '|' + IDEX["pc"] + '|' + EXMEM["pc"] + '|' + MEMWB["pc"] + '|' + WBS["pc"] +'}';
    // noop는 따로 다시 생각해보자! 
    
    cout <<"Current pipeline PC state:" << endl;
    cout << pipe << endl << endl;
}

void cpr(){
    cout << "Current register values:" << endl;
    cout << "------------------------------" << endl;
    cout << "PC: " << pc << endl;
    cout << "Registers:" << endl;
    for (int i = 0 ; i < 32 ; i++){
    cout << 'R' << i << ": " << reg[i] << endl;
    }
    cout << endl;
}

void cm(map<string, string>&memo, string addr1, string addr2){
    
    cout << "Memory content [" << addr1 << ".." << addr2 << "]:" << endl;
    cout << "---------------------------" << endl;
    
    string addr = addr1;
    int temp;

    while (addr != addr2){
        if (memo.find(addr) == memo.end()){
            cout << addr << ": " << "0x0" << endl;
            temp = stoi(addr, nullptr, 16);
            temp += 4;
            stringstream ss;
            ss << hex << "0x" << temp;
            addr = ss.str();
        }
        else{
            cout << addr << ": " << memo[addr] << endl;
            temp = stoi(addr, nullptr, 16);
            temp += 4;
            stringstream ss;            
            ss << hex << "0x" << temp;
            addr = ss.str();       
        }
    }
    if (memo.find(addr) == memo.end()){
        cout << addr << ": " << "0x0" << endl;
    }
    else{
        cout << addr << ": " << memo[addr] << endl;
    }
    cout << endl;
}


// 03. Basic Functions
void pcplus(){
    int temp = stoi(pc, nullptr, 16);
    temp += 4;
    stringstream ss;            
    ss << hex << "0x" << temp;
    pc = ss.str();   
}

// 04. Instruction Functions

void addu(string rs, string rt){
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    unsigned long regs = stoul(reg[s], nullptr, 16);
    if (Mux["forwardrs"] == true){
        regs = stoul(EXMEM["alu"], nullptr, 16);
        Mux["forwardrs"] = false;
    }
    unsigned long regt = stoul(reg[t], nullptr, 16);
    if (Mux["forwardrt"] == true){
        regt = stoul(EXMEM["alu"], nullptr, 16);
        Mux["forwardrt"] = false;
    }
    unsigned long regd = regs + regt;
    stringstream ss;
    ss << hex << "0x" << regd;
    EXMEM["alu"] = ss.str();
}

void andd(string rs, string rt){
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    unsigned long regs = stoul(reg[s], nullptr, 16);
    if (Mux["forwardrs"] == true){
        regs = stoul(EXMEM["alu"], nullptr, 16);
        Mux["forwardrs"] = false;
    }
    unsigned long regt = stoul(reg[t], nullptr, 16);
    if (Mux["forwardrt"] == true){
        regt = stoul(EXMEM["alu"], nullptr, 16);
        Mux["forwardrt"] = false;
    }   
    bitset<32> sbit(regs);
    bitset<32> tbit(regt);
    bitset<32> dbit = sbit & tbit;
    unsigned long ud = dbit.to_ulong();
    stringstream ss;
    ss << hex << "0x" << ud;
    EXMEM["alu"] = ss.str();  
}

void nor(string rs, string rt){
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    unsigned long regs = stoul(reg[s], nullptr, 16);
    if (Mux["forwardrs"] == true){
        regs = stoul(EXMEM["alu"], nullptr, 16);
        Mux["forwardrs"] = false;
    }
    unsigned long regt = stoul(reg[t], nullptr, 16);
    if (Mux["forwardrt"] == true){
        regt = stoul(EXMEM["alu"], nullptr, 16);
        Mux["forwardrt"] = false;
    }   
    bitset<32> sbit(regs);
    bitset<32> tbit(regt);
    bitset<32> dbit = ~(sbit | tbit);
    unsigned long ud = dbit.to_ulong();
    stringstream ss;
    ss << hex << "0x" << ud;
    EXMEM["alu"] = ss.str(); 
}

void orr(string rs, string rt){
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    unsigned long regs = stoul(reg[s], nullptr, 16);
    if (Mux["forwardrs"] == true){
        regs = stoul(EXMEM["alu"], nullptr, 16);
        Mux["forwardrs"] = false;
    }
    unsigned long regt = stoul(reg[t], nullptr, 16);
    if (Mux["forwardrt"] == true){
        regt = stoul(EXMEM["alu"], nullptr, 16);
        Mux["forwardrt"] = false;
    }   
    bitset<32> sbit(regs);
    bitset<32> tbit(regt);
    bitset<32> dbit = sbit | tbit;
    unsigned long ud = dbit.to_ulong();
    stringstream ss;
    ss << hex << "0x" << ud;
    EXMEM["alu"] = ss.str();   
}

void sll(string rt, string shamt){
    int t = stoi(rt, nullptr, 2);
    int s = stoi(shamt, nullptr, 2);
    unsigned long regt = stoul(reg[t], nullptr, 16);
    if (Mux["forwardrt"] == true){
        regt = stoul(EXMEM["alu"], nullptr, 16);
        Mux["forwardrt"] = false;
    }      
    bitset<32> tbit(regt);
    bitset<32> dbit = tbit << s;
    unsigned long ud = dbit.to_ulong();
    stringstream ss;
    ss << hex << "0x" << ud;
    EXMEM["alu"] = ss.str();        
}

void sltu(string rs, string rt){
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    unsigned long regs = stoul(reg[s], nullptr, 16);
    if (Mux["forwardrs"] == true){
        regs = stoul(EXMEM["alu"], nullptr, 16);
        Mux["forwardrs"] = false;
    }
    unsigned long regt = stoul(reg[t], nullptr, 16);
    if (Mux["forwardrt"] == true){
        regt = stoul(EXMEM["alu"], nullptr, 16);
        Mux["forwardrt"] = false;
    } 
    if (regs < regt){
        EXMEM["alu"] = "0x1";
    }
    else{
        EXMEM["alu"] = "0x0";
    }
}

void srl(string rt, string shamt){
    int t = stoi(rt, nullptr, 2);
    int s = stoi(shamt, nullptr, 2);
    unsigned long regt = stoul(reg[t], nullptr, 16);
    if (Mux["forwardrt"] == true){
        regt = stoul(EXMEM["alu"], nullptr, 16);
        Mux["forwardrt"] = false;
    }   
    bitset<32> tbit(regt);
    bitset<32> dbit = tbit >> s;
    unsigned long ud = dbit.to_ulong();
    stringstream ss;
    ss << hex << "0x" << ud;
    EXMEM["alu"] = ss.str();           
}

void subu(string rs, string rt){
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    unsigned long regs = stoul(reg[s], nullptr, 16);
    if (Mux["forwardrs"] == true){
        regs = stoul(EXMEM["alu"], nullptr, 16);
        Mux["forwardrs"] = false;
    }
    unsigned long regt = stoul(reg[t], nullptr, 16);
    if (Mux["forwardrt"] == true){
        regt = stoul(EXMEM["alu"], nullptr, 16);
        Mux["forwardrt"] = false;
    } 
    unsigned long regd = regs - regt;
    bitset<32> dbit(regd);
    stringstream ss;
    ss << hex << "0x" << dbit.to_ulong();
    EXMEM["alu"] = ss.str();         
}

void addiu(string rs, string imm){
    int s = stoi(rs, nullptr, 2);
    unsigned long regs = stoul(reg[s], nullptr, 16);
    if (Mux["forwardrs"] == true){
        regs = stoul(EXMEM["alu"], nullptr, 16);
        Mux["forwardrs"] = false;
    }
    if (imm[0] == '1'){
        string o = "";
        for (int i = 0 ; i < imm.length() ; i++){
            if (imm[i] == '1'){
                o = o + '0';
            }
            else{
                o = o + '1';
            }
        }
        long long ill = (-1) * (stoll(o, nullptr, 2) + 1);
        unsigned long regt = regs + ill;
        bitset<32> tbit(regt);
        stringstream ss;
        ss << hex << "0x" << tbit.to_ulong();
        EXMEM["alu"] = ss.str();
    }
    else{
        unsigned long iul = stoul(imm, nullptr, 2);
        unsigned long regt = regs + iul;
        bitset<32> tbit(regt);
        stringstream ss;
        ss << hex << "0x" << tbit.to_ulong();
        EXMEM["alu"] = ss.str(); 
    }        
}

void lui(string imm){
    string temp = imm + "0000000000000000";
    bitset<32> tbit(temp);
    stringstream ss;
    ss << hex << "0x" << tbit.to_ulong();
    EXMEM["alu"] = ss.str(); 
}

void andi(string rs, string imm){
    int s = stoi(rs, nullptr, 2);
    unsigned long regs = stoul(reg[s], nullptr, 16);
    if (Mux["forwardrs"] == true){
        regs = stoul(EXMEM["alu"], nullptr, 16);
        Mux["forwardrs"] = false;
    }
    bitset<32> sbit(regs);
    bitset<32> ibit(imm);
    bitset<32> tbit = sbit & ibit;
    unsigned long ut = tbit.to_ulong();
    stringstream ss;
    ss << hex << "0x" << ut;
    EXMEM["alu"] = ss.str();
}

void ori(string rs, string imm){
    int s = stoi(rs, nullptr, 2);
    unsigned long regs = stoul(reg[s], nullptr, 16);
    if (Mux["forwardrs"] == true){
        regs = stoul(EXMEM["alu"], nullptr, 16);
        Mux["forwardrs"] = false;
    }
    bitset<32> sbit(regs);
    bitset<32> ibit(imm);
    bitset<32> tbit = sbit | ibit;
    unsigned long ut = tbit.to_ulong();
    stringstream ss;
    ss << hex << "0x" << ut;
    EXMEM["alu"] = ss.str(); 
}

void sltiu(string rs, string imm){
    int s = stoi(rs, nullptr, 2);
    if (imm[0] == '1'){
          imm = "1111111111111111" + imm;
    }
    unsigned long im = stoul(imm, nullptr, 2);
    unsigned long regs = stoul(reg[s], nullptr, 16);
    if (Mux["forwardrs"] == true){
        regs = stoul(EXMEM["alu"], nullptr, 16);
        Mux["forwardrs"] = false;
    }
    if (regs < im){
        EXMEM["alu"] = "0x1";
    }
    else{
        EXMEM["alu"] = "0x0";
    }       
}



void beq(string rs, string rt, string offset){

    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    if (Mux["forwardrs"] == true || Mux["forwardrt"] == true){
        if(Mux["forwardrs"] == true & Mux["forwardrt"] == false){
            string regs = EXMEM["alu"];
            if (regs == reg[t]){
                Mux["control"] = true;
            }            
        }
        else if(Mux["forwardrs"] == false & Mux["forwardrt"] == true){
            string regt = EXMEM["alu"];
            if (reg[s] == regt){
                Mux["control"] = true;
            }
        }
        else if(Mux["forwardrs"] == true & Mux["forwardrt"] == true){
            Mux["control"] = true;
        }
    }
    else{
        if (reg[s] == reg[t]){
            Mux["control"] = true;
        }
    }
    Mux["forwardrs"] = false;
    Mux["forwardrt"] = false;
}

void bne(string rs, string rt, string offset){
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    if (Mux["forwardrs"] == true || Mux["forwardrt"] == true){
        if(Mux["forwardrs"] == true & Mux["forwardrt"] == false){
            string regs = EXMEM["alu"];          
            if (regs != reg[t]){
                Mux["control"] = true;
            }            
        }
        else if(Mux["forwardrs"] == false & Mux["forwardrt"] == true){
            string regt = EXMEM["alu"];         
            if (reg[s] != regt){
                Mux["control"] = true;
            }
        }
        else if(Mux["forwardrs"] == true & Mux["forwardrt"] == true){
            Mux["control"] = false;
        }
    }
    else{
        if (reg[s] != reg[t]){
            Mux["control"] = true;
        }
    }
    Mux["forwardrs"] = false;
    Mux["forwardrt"] = false;
}

void lw(string rs, string rt, string imm){
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    long long sll = stoll(reg[s], nullptr, 16);
    if (Mux["forwardrs"] == true){
        sll = stoll(EXMEM["alu"], nullptr, 16);
        Mux["forwardrs"] = false;
    }
    long long ill;
    if (imm[0] == '1'){
        string o = "";
        for (int i = 0 ; i < imm.length() ; i++){
            if (imm[i] == '0'){
                o = o + '1';
            }
            else{
                o = o + '0';
            }        
        }
        ill = (-1) * (stoll(o, nullptr, 2) + 1);
    }
    else{
        ill = stoll(imm, nullptr, 2);
    }

    long long tll = sll + ill;
    stringstream ss;
    ss << hex << "0x" << tll;
    string addr = ss.str();
    EXMEM["alu"] = addr;
}

void sw(string rs, string rt, string offset){
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    long long sll = stoll(reg[s], nullptr, 16);
    if (Mux["forwardrs"] == true){
        sll = stoll(EXMEM["alu"], nullptr, 16);
        Mux["forwardrs"] = false;
    }    
    long long oll;
    if(offset[0] = '1'){
        string o = "";
        for (int i = 0 ; i < offset.length() ; i++){
            if (offset[i] == '0'){
                o = o + '1';
            }
            else{
                o = o + '0';
            }          
        }
        oll = (-1)*(stoll(o, nullptr, 2) + 1);
    }    
    else{
        oll = stoll(offset, nullptr, 2);
    }
    long long tll = sll + oll;
    stringstream ss;
    ss << hex << "0x" << tll;
    string addr = ss.str();
    EXMEM["alu"] = addr;
}

void lb(string rs, string rt, string offset){
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    if (offset[0] == '1'){
        string o = "";
        for (int i = 0 ; i < offset.length() ; i++){
            if (offset[i] == '0'){
                o = o + '1';
            }
            else{
                o = o + '0';
            }        
        }
        long long off = (-1) * (stoll(o, nullptr, 2) + 1);
        int w = (((-1)*off) / 4) + 1;
        int x = ((-1)*off) % 4;
        int xx;
        if (x==0){
            xx = 0;
        }
        else if (x==1){
            xx = 6;
        }
        else if (x==2){
            xx = 4;
        }
        else{
            xx = 2;
        }
        long long tarmem = stoll(reg[s], nullptr, 16) - (w * 4);
        // 여기서 (read before write) 해야 하지 않나..? (재확인 필수!!)
        if (Mux["forwardrs"] == true){
            tarmem = stoll(EXMEM["alu"], nullptr, 16) - (w * 4);
            Mux["forwardrs"] = false;
        }
        stringstream ss;
        ss << hex << "0x" << tarmem;
        EXMEM["alu"] = ss.str();
        EXMEM["bit"] = to_string(xx);
    }
    else {
        long long off = stoll(offset, nullptr, 2);
        int w = off / 4;
        int x = 2*(off % 4);
        long long tarmem = stoll(reg[s], nullptr, 16) + (w * 4);
        if (Mux["forwardrs"] == true){
            tarmem = stoll(EXMEM["alu"], nullptr, 16) + (w * 4);
            Mux["forwardrs"] = false;
        }
        stringstream ss;
        ss << hex << "0x" << tarmem;
        EXMEM["alu"] = ss.str();
        EXMEM["bit"] = to_string(x);
    }   
}

void sb(string rs, string rt, string offset){
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    if (offset[0] == '1'){
        string o = "";
        for (int i = 0 ; i < offset.length() ; i++){
            if (offset[i] == '0'){
                o = o + '1';
            }
            else{
                o = o + '0';
            }
        }
        long long off = (-1) * (stoll(o, nullptr, 2) +1);
        int w = (((-1)*off) / 4) + 1;
        int x = ((-1)*off) %4;
        int xx;
        if (x==0){
            xx = 0;
        }
        else if (x==1){
            xx = 6;
        }
        else if (x==2){
            xx = 4;
        }
        else{
            xx = 2;
        }
        long long tarmem = stoll(reg[s], nullptr, 16) - (w*4);
        if (Mux["forwardrs"] == true){
            tarmem = stoll(EXMEM["alu"], nullptr, 16) - (w*4);
            Mux["forwardrs"] = false;
        }
        stringstream ss;
        ss << hex << "0x" << tarmem;
        EXMEM["alu"] = ss.str();
        EXMEM["bit"] = to_string(xx);
    }
    else{
        long long off = stoll(offset, nullptr, 2);
        int w = off / 4;
        int x = 2*(off % 4);
        long long tarmem = stoll(reg[s], nullptr, 16) + (w*4);
        if (Mux["forwardrs"] == true){
            tarmem = stoll(EXMEM["alu"], nullptr, 16) + (w*4);
            Mux["forwardrs"] = false;
        }
        stringstream ss;
        ss << hex << "0x" << tarmem;
        EXMEM["alu"] = ss.str();
        EXMEM["bit"] =  to_string(x);
    }  
}

// 05. Pipeline Functions
void IF(string fadr){
    cycle += 1;

    // 이걸 어떻게 해야하나..?
    if (Mux["stall"] == false & Mux["unj"] == false & Mux["flush"] == false & Mux["bstall"] == false){
        if (pc == "" || pc == fadr){
            pc = fadr;
            IFID["pc"] = "";
            return;
        }
        string line = memo[pc];
        unsigned long long hex = stoull(line, nullptr, 16);
        bitset<32> linebits(hex);
        string bits = linebits.to_string();    
        IFID["pc"] = pc;
        IFID["ins32"] = bits;
        if (pc.find("0x") != string::npos || memo[pc] != "0x0"){
            pcplus();
            if (memo.find(pc) == memo.end() & pc != fadr){
                pc = "";
            }
        }
    }
    
    if (Mux["unj"] == true){
        Mux["unj"] = false;
    }
    
    if (Mux["stall"] == true){
        Mux["stall"] = false;
        Mux["regread"] = false;
        Mux["memload"] = false;
    }
    if (Mux["flush"] == true){
        Mux["flush"] = false;
    }
    if (Mux["bstall"] == true){
        Mux["bstall"] = false;
        IFID["pc"] = "";
        IFID["ins32"] = "";
    }
    
}

void ID(bool a){
    // Load & Use Data Harzard
    
    if (Mux["stall"] == false & Mux["flush"] == false & Mux["bstall"] == false){
        IDEX["pc"] = IFID["pc"];
        string i = IFID["ins32"];
        IDEX["rs"] = "";
        IDEX["rt"] = "";
        IDEX["rd"] = "";
        IDEX["imm"] = "";
        IDEX["jtar"] = "";
        IDEX["shamt"] = "";
        IDEX["funct"] = "";
        
        if(i != ""){
            IDEX["op"] = i.substr(0,6);
        }
        else{
            IDEX["op"] = "";
            return;
        }
        
        // R format
        if(IDEX["op"] == "000000"){
            IDEX["rs"] = i.substr(6,5);
            IDEX["rt"] = i.substr(11,5);
            IDEX["rd"] = i.substr(16, 5);
            IDEX["shamt"] = i.substr(21,5);
            IDEX["funct"] = i.substr(26,6);
            // jr
            if (IDEX["funct"] == "001000"){
                int s = stoi(IDEX["rs"], nullptr, 2);
                pc = reg[s];
                // one cycle stall
                IFID["ins32"] = "";
                IFID["pc"] = "";
            }
        }
        // J format
        else if(IDEX["op"] == "000010" || IDEX["op"] == "000011"){
            IDEX["jtar"] = i.substr(6,26);
            string jumptar = "0000" + IDEX["jtar"] + "00";
            bitset<32> jbit(jumptar);
            stringstream ss;
            ss << hex << "0x" << jbit.to_ulong();
            string jpc = ss.str();
            pc = jpc;
            // one cycle stall
            Mux["unj"] = true;
            IFID["ins32"] = "";
            IFID["pc"] = "";        
        }
        // I format
        else{
            IDEX["rs"] = i.substr(6,5);
            IDEX["rt"] = i.substr(11,5);
            IDEX["imm"] = i.substr(16,16);
            if (IDEX["op"] == "000100"){ // beq
                string temp1 = IDEX["imm"] + "00";
                unsigned long tempul = stoul(temp1, nullptr, 2);
                unsigned long pcul = stoul(IFID["pc"], nullptr, 16);
                unsigned long temp2 = tempul + pcul + 4;
                stringstream ss;
                ss << hex << "0x" << temp2;
                IDEX["jtar"] = ss.str();
                if (a){
                    Mux["bstall"] = true;
                    pc = IDEX["jtar"];
                } 
            }
            else if (IDEX["op"] == "000101"){ // bne
                string temp1 = IDEX["imm"] + "00";
                unsigned long tempul = stoul(temp1, nullptr, 2);
                unsigned long pcul = stoul(IFID["pc"], nullptr, 16);
                unsigned long temp2 = tempul + pcul + 4;
                stringstream ss;
                ss << hex << "0x" << temp2;
                IDEX["jtar"] = ss.str();
                if (a){
                    Mux["bstall"] = true;
                    pc = IDEX["jtar"];
                }                 
            }
        }
    }
}

void EX(){

    if (Mux["flush"] == true){
        return;
    }

    // data forwarding (reg read) -load use
    if(IDEX["op"] == "000010" || IDEX["op"] == "000011" || IDEX["op"] ==  ""){
        Mux["regread"] = false;
    }
    else{
        Mux["regread"] = true;
    }
    if (Mux["memload"] == true & Mux["regread"] == true){
        if( EXMEM["rt"] == IDEX["rs"] || EXMEM["rt"] == IDEX["rt"] ){
            Mux["stall"] = true;
        }
    }
    if (Mux["stall"] == false){
        EXMEM["pc"] = IDEX["pc"]; 
    }
    else{
        EXMEM["pc"] = ""; 
    }
    // data forwarding (reg read) - read before write
    if (Mux["regwrite"] == true & Mux["regread"] == true){
        // R-R
        if ((EXMEM["op"] == "000000" & EXMEM["funct"] != "001000") & (IDEX["op"] == "000000" || IDEX["op"] == "000100" || IDEX["op"] == "000101")){
        // jr O jr X
            if (EXMEM["rd"] == IDEX["rs"] || EXMEM["rd"] == IDEX["rt"]){
                if (EXMEM["rd"] == IDEX["rs"]){
                    Mux["forwardrs"] = true;  // rs = EXMEM["alu"]
                }
                if (EXMEM["rd"] == IDEX["rt"]){
                    Mux["forwardrt"] = true;  // rt = EXMEM["alu"]
                }
            }
        }
        // R-I
        else if ((EXMEM["op"] == "000000" & EXMEM["funct"] != "001000") & (IDEX["op"] == "001001" || IDEX["op"] == "001100" || IDEX["op"] == "001101" || IDEX["op"] == "001011" || IDEX["op"] == "100011" || IDEX["op"] == "101011" || IDEX["op"] == "100000" || IDEX["op"] == "101000" )){
            if(EXMEM["rd"] == IDEX["rs"]){
                Mux["forwardrs"] = true;
            }
        }
        // I-R
        else if ((EXMEM["op"] == "001001" || EXMEM["op"] == "001100" || EXMEM["op"] == "001101" || EXMEM["op"] == "001011" || EXMEM["op"] == "001111") & (IDEX["op"] == "000000" || IDEX["op"] == "000100" || IDEX["op"] == "000101")){
        // lui O jr O
            if(EXMEM["rt"] == IDEX["rs"]){
                Mux["forwardrs"] = true;
            }
            if(EXMEM["rt"] == IDEX["rt"]){
                Mux["forwardrt"] = true;
            }
        }
        // I-I
        else if((EXMEM["op"] == "001001" || EXMEM["op"] == "001100" || EXMEM["op"] == "001101" || EXMEM["op"] == "001011" || EXMEM["op"] == "001111") & (IDEX["op"] == "001001" || IDEX["op"] == "001100" || IDEX["op"] == "001101" || IDEX["op"] == "001011" || IDEX["op"] == "100011" || IDEX["op"] == "101011" || IDEX["op"] == "100000" || IDEX["op"] == "101000")){
            if (EXMEM["rt"] == IDEX["rs"]){      
                Mux["forwardrs"] = true;
            }
        }  
    }
    else{
        EXMEM["alu"] = "";
    }
    // Come Back ROUTINE!!

    EXMEM["tar"] = "";
    EXMEM["op"] = "";
    EXMEM["rd"] = "";
    EXMEM["rt"] = "";
    EXMEM["bit"] = "";
    
    if (Mux["stall"] == false){
    
        if (IDEX["op"] == "000000"){
    
            if (IDEX["funct"] == "100001"){
                addu(IDEX["rs"], IDEX["rt"]);
                EXMEM["op"] = IDEX["op"];
                EXMEM["rd"] = IDEX["rd"];
            }
            else if(IDEX["funct"] == "100100"){
                andd(IDEX["rs"], IDEX["rt"]);       
                EXMEM["op"] = IDEX["op"];
                EXMEM["rd"] = IDEX["rd"];               
            }
        
            else if (IDEX["funct"] == "001000"){ // jr
                EXMEM["op"] = IDEX["op"];
                EXMEM["funct"] = IDEX["funct"];
            }
        
            else if (IDEX["funct"] == "100111"){
                nor(IDEX["rs"], IDEX["rt"]);  
                EXMEM["op"] = IDEX["op"];
                EXMEM["rd"] = IDEX["rd"];            
            }
            else if (IDEX["funct"] == "100101"){
                orr(IDEX["rs"], IDEX["rt"]);
                EXMEM["op"] = IDEX["op"];
                EXMEM["rd"] = IDEX["rd"];            
            }
            else if (IDEX["funct"] == "000000"){
                sll(IDEX["rt"], IDEX["shamt"]);
                EXMEM["op"] = IDEX["op"];
                EXMEM["rd"] = IDEX["rd"];            
            }
            else if (IDEX["funct"] == "101011"){
                sltu(IDEX["rs"], IDEX["rt"]);
                EXMEM["op"] = IDEX["op"];
                EXMEM["rd"] = IDEX["rd"];            
            }
            else if (IDEX["funct"] == "000010"){
                srl(IDEX["rt"], IDEX["shamt"]);  
                EXMEM["op"] = IDEX["op"];
                EXMEM["rd"] = IDEX["rd"]; 
            }
            else if (IDEX["funct"] == "100011"){
                subu(IDEX["rs"], IDEX["rt"]);
                EXMEM["op"] = IDEX["op"];
                EXMEM["rd"] = IDEX["rd"];
            }
        }
        else if (IDEX["op"] == "001001"){
            addiu(IDEX["rs"], IDEX["imm"]);
            EXMEM["op"] = IDEX["op"];
            EXMEM["rt"] = IDEX["rt"];
        }
        else if (IDEX["op"] == "001100"){
            andi(IDEX["rs"], IDEX["imm"]);
            EXMEM["op"] = IDEX["op"];
            EXMEM["rt"] = IDEX["rt"];
        }
        else if (IDEX["op"] == "000100"){    // beq
            beq(IDEX["rs"], IDEX["rt"], IDEX["imm"]);
            EXMEM["tar"] = IDEX["jtar"];
            EXMEM["op"] = IDEX["op"];
        }
        else if (IDEX["op"] == "000101"){    // bne
            bne(IDEX["rs"], IDEX["rt"], IDEX["imm"]);
            EXMEM["tar"] = IDEX["jtar"];  
            EXMEM["op"] = IDEX["op"];
        }
        else if(IDEX["op"] == "000010"){  // j
            EXMEM["op"] = IDEX["op"];
        }
        else if(IDEX["op"] == "000011"){  // jal
            EXMEM["op"] = IDEX["op"];    
        }
        else if(IDEX["op"] == "001111"){
            lui(IDEX["imm"]);  
            EXMEM["op"] = IDEX["op"];
            EXMEM["rt"] = IDEX["rt"];
        }
        
        else if(IDEX["op"] == "001101"){
            ori(IDEX["rs"], IDEX["imm"]); 
            EXMEM["op"] = IDEX["op"];  
            EXMEM["rt"] = IDEX["rt"]; 
        }
        else if(IDEX["op"] == "001011"){
            sltiu(IDEX["rs"], IDEX["imm"]);  
            EXMEM["op"] = IDEX["op"];  
            EXMEM["rt"] = IDEX["rt"];
        }
        else if(IDEX["op"] == "100011"){
            lw(IDEX["rs"], IDEX["rt"], IDEX["imm"]);  // imm == offset
            EXMEM["op"] = IDEX["op"];
            EXMEM["rt"] = IDEX["rt"];
        }
        else if(IDEX["op"] == "101011"){
            sw(IDEX["rs"], IDEX["rt"], IDEX["imm"]);  // imm == offset  
            EXMEM["op"] = IDEX["op"];
            EXMEM["rt"] = IDEX["rt"];
        }
        else if(IDEX["op"] == "100000"){
            lb(IDEX["rs"], IDEX["rt"], IDEX["imm"]);  // imm == offset 
            EXMEM["op"] = IDEX["op"];
            EXMEM["rt"] = IDEX["rt"];
        }
        else if(IDEX["op"] == "101000"){
            sb(IDEX["rs"], IDEX["rt"], IDEX["imm"]);  // imm == offset  
            EXMEM["op"] = IDEX["op"];
            EXMEM["rt"] = IDEX["rt"];
        }
        
    }
}

// Load & Store Only!!
void MEM(bool a){
    // Data Forwarding (memread - load&use)
    if(EXMEM["op"] == "100011" || EXMEM["op"] == "100000"){
        Mux["memload"] = true;
    }
    else{
        Mux["memload"] = false;
    }
    
    if((EXMEM["op"] == "000000" & EXMEM["funct"] != "001000") || EXMEM["op"] == "001001" || EXMEM["op"] == "001100" || EXMEM["op"] == "001111" || EXMEM["op"] == "001101" || EXMEM["op"] == "001011"){
        EXMEM["funct"] = "";
        Mux["regwrite"] = true;
    }
    else{
        Mux["regwrite"] = false;
    }
    
    MEMWB["pc"] = EXMEM["pc"];
    MEMWB["alu"] = "";
    MEMWB["mem"] = ""; // load!
    MEMWB["bit"] = EXMEM["bit"];
    MEMWB["op"] = EXMEM["op"];
    MEMWB["rd"] = EXMEM["rd"];
    MEMWB["rt"] = EXMEM["rt"];
    
    // Conditional Branch
    if (EXMEM["op"] == "000100" || EXMEM["op"] == "000101"){
        if (Mux["control"] != a){
            if (Mux["control"] == true){ // not taken(a) > taken!!
                pc = EXMEM["tar"];
                Mux["control"] = false;
            }
            else if (Mux["control"] == false){ // taken(a) > not taken!!
                int temp = stoi(EXMEM["pc"], nullptr, 16);
                temp += 4;
                stringstream ss;            
                ss << hex << "0x" << temp;
                string p = ss.str();
                pc = p;
            }
            // three cycle stall & flush
            EXMEM["pc"] = "";
            EXMEM["alu"] = "";
            EXMEM["tar"] = "";
            EXMEM["op"] = "";
            EXMEM["rd"] = "";
            EXMEM["rs"] = "";
            IDEX["pc"] = "";
            IDEX["rs"] = "";
            IDEX["rt"] = "";
            IDEX["imm"] = "";
            IDEX["rd"] = "";
            IDEX["op"] = "";
            IDEX["jtar"] = "";
            IDEX["shamt"] = "";
            IDEX["funct"] = "";
            IFID["ins32"] = "";
            IFID["pc"] = "";
            Mux["flush"] = true;
        }
    }    
    if (EXMEM["op"] == "100011"){
        // lw
        if (memo.find(EXMEM["alu"]) != memo.end()){
            MEMWB["mem"] = memo[EXMEM["alu"]];
        }
        else {
            MEMWB["mem"] = "0x0";
        }    
    }
    else if (EXMEM["op"] == "101011"){
        // sw
        int t = stoi(EXMEM["rt"], nullptr, 2);
        memo[EXMEM["alu"]] = reg[t];
    }
    else if (EXMEM["op"] == "100000"){
        // lb
        string temp = memo[EXMEM["alu"]].substr(2);
        while(temp.length() != 8){
            temp = '0' + temp;
        }
        temp = temp.substr(stoi(EXMEM["bit"]), 2);
        MEMWB["mem"] = "0x" + temp;
    }
    else if (EXMEM["op"] == "101000"){        
        
        string temp = memo[EXMEM["alu"]].substr(2);
        while(temp.length() != 8){
            temp = '0' + temp;
        }
        int t = stoi(EXMEM["rt"], nullptr, 2);
        string temp2 = reg[t].substr(2);
        if (temp2.length() > 2){
            temp2 = temp2.substr(temp2.length()-2, 2);
        }
        else if (temp2.length() == 1){
            temp2 = '0' + temp2;
        }
        
        if (stoi(EXMEM["bit"])==0){
            temp = temp2 + temp.substr(2);
        }
        else if (stoi(EXMEM["bit"]) == 6){
            temp = temp.substr(0,6) + temp2;
        }
        else if (stoi(EXMEM["bit"]) == 4){
            temp = temp.substr(0,4) + temp2 + temp.substr(6);
        }
        else if (stoi(EXMEM["bit"]) == 2){
            temp = temp.substr(0,2) + temp2 + temp.substr(4);
        }
        while(temp[0] == '0'){
            temp = temp.substr(1);
        }
        memo[EXMEM["alu"]] = "0x" + temp;   
    }
    
    else{
        MEMWB["alu"] = EXMEM["alu"];
    }
}

// Load & R format etc..
void WB(){
    WBS["pc"] = MEMWB["pc"];
    if (MEMWB["op"] == "000000"){
        // R format (jr X)
        if (MEMWB["rd"] != ""){
            int d = stoi(MEMWB["rd"], nullptr, 2);
            reg[d] = MEMWB["alu"];
        }
    }
    else if (MEMWB["op"] == "100011"){
        // lw
        int t = stoi(MEMWB["rt"], nullptr, 2);
        reg[t] = MEMWB["mem"];   
    }
    else if (MEMWB["op"] == "100000"){
        // lb
        int t = stoi(MEMWB["rt"], nullptr, 2);
        reg[t] = MEMWB["mem"];
    }
    else if (MEMWB["op"] == "000011"){
        // jal
        int temp = stoi(WBS["pc"], nullptr, 16);
        temp += 4;
        stringstream ss;            
        ss << hex << "0x" << temp; 
        reg[31] = ss.str();
    }    
    else if (MEMWB["op"] != "101011" & MEMWB["op"] != "101000" & MEMWB["op"] != "000010" & MEMWB["op"] != "000101" & MEMWB["op"] != "000100"){
    // I format(sw, sb, j, bne, beq ... X))
        if (MEMWB["rt"] != ""){
            int t = stoi(MEMWB["rt"], nullptr, 2);
            reg[t] = MEMWB["alu"];
        }
    }
}


// Combine
void Combine(int textsize, bool a, bool d, bool p, int n, bool m, string addr1, string addr2){
    string faddr = "0x400000";
    
    if (n == -1 || n > textsize){
        for (int i = 0 ; i < textsize ; i++){
            int tempa = stoi(faddr, nullptr, 16);
            tempa += 4;
            stringstream ss;
            ss << hex << "0x" << tempa;
            faddr = ss.str();
        }
        
        while (pc != faddr){
            WB();
            MEM(a);
            EX();
            ID(a);  
            IF(faddr);    
            if (p){
                cpipe();
            }
            if (d){
                cpr();
                if (m){
                    cm(memo, addr1, addr2);
                }
            }
        }
        while (Mux["keep"] == true){
            WB();
            MEM(a);
            EX();
            ID(a);
            IF(faddr);
            if (IFID["pc"] == "" & IDEX["pc"] == "" & EXMEM["pc"] == "" & MEMWB["pc"] == "" & WBS["pc"] == ""){
                Mux["keep"] = false;
            }            
            if (p){
                if(Mux["keep"] == false){
                    compipe(); // completion
                }
                else if(Mux["keep"] == true){
                    cpipe();
                }
            }
            if (d){
                if(Mux["keep"] == false & !p){
                    compipe(); // completion
                }
                cpr();
                if (m){
                    cm(memo, addr1, addr2);
                }
            }            
        }
           
        if (!p & !d){
            compipe(); // completion
        }
        if (!d){
            cpr();
            if(m){
                cm(memo, addr1, addr2);
            }
        }        
    }
    
    
    
    
    
    else if (n == 0){
        compipe(); // completion
        cpr();        
        if(m){
            cm(memo, addr1, addr2);
        }
    }
    
    
    
    
    
    else if (0 < n < textsize){
        for (int i = 0 ; i < n ; i++){
            int tempb = stoi(faddr, nullptr, 16);
            tempb += 4;
            stringstream ss2;
            ss2 << hex << "0x" << tempb;
            faddr = ss2.str();
        }       
        while (pc != faddr){

            WB();
            MEM(a);
            EX();
            ID(a);
            IF(faddr);
            
            if (p){
                if(Mux["keep"] == false){
                    compipe(); // completion
                }
                else if(Mux["keep"] == true){
                    cpipe();
                }
            }
            if (d){
                cpr();
                if (m){
                    cm(memo, addr1, addr2);
                }
            }
        }
        while (Mux["keep"] == true){
            WB();
            MEM(a);
            EX();
            ID(a);
            IF(faddr);
            if (p){
                cpipe();
            }
            if (IFID["pc"] == "" & IDEX["pc"] == "" & EXMEM["pc"] == "" & MEMWB["pc"] == "" & WBS["pc"] == ""){
                Mux["keep"] = false;
            }
            if (d){
                if(Mux["keep"] == false & !p){
                    compipe(); // completion
                }
                cpr();
                if (m){
                    cm(memo, addr1, addr2);
                }
            }            
        }      
        
        if(!d & !p){
            compipe(); // completion
        }
        if (!d){
            cpr();            
            if(m){
                cm(memo, addr1, addr2);
            }
        }
    }
}




int main(int argc, char* argv[]){
    // 0 : runfile, 1 : a, 2 : m, 3 : d, 4 : p, 5 : n, 6 : file
    bool a;
    string addr1 = "";
    string addr2 = "";
    bool m = false;
    bool d = false;
    bool p = false;
    int n = -1;
    for (int i = 0 ; i < argc ; i++){
        if (strcmp(argv[i], "-atp") == 0){
            a = true;
        }
        else if (strcmp(argv[i], "-antp") == 0){
            a = false;
        }
        else if (strcmp(argv[i], "-m") == 0){
            string s(argv[i+1]);
            int d = s.find(":");
            addr1 = s.substr(0, d);
            addr2 = s.substr(d+1);
            m = true;
            i++;
        }
        else if (strcmp(argv[i], "-d") == 0){
            d = true;
        }
        else if (strcmp(argv[i], "-p") == 0){
            p = true;
        }
        else if (strcmp(argv[i], "-n") == 0){
            n = stoi(argv[i+1]);
            i++;
        }
    }
    // open file
    ifstream infile(argv[argc - 1]);
    if (!infile.is_open()){
        cout << "Couldn't open file" << endl;
        return 1;
    }
    // data processing
    string l;
    getline(infile, l);
    int textsize = stoi(l, nullptr, 16);
    getline(infile, l);
    int datasize = stoi(l, nullptr, 16);
    textsize = textsize/4;
    datasize = datasize/4;
    // make memory map
    makememo(textsize, datasize, &memo, infile, n);
        
    // main
    Combine(textsize, a, d, p, n, m, addr1, addr2);     
    
    // close file
    infile.close();
    return 0;
}
