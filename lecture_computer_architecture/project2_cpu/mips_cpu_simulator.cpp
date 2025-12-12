#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <bitset>
#include <map>

using namespace std;

string pc = "0x400000";

map<int, string> reg = {
    {0, "0x0"}, {1, "0x0"}, {2, "0x0"}, {3, "0x0"}, {4, "0x0"}, {5, "0x0"},
    {6, "0x0"}, {7, "0x0"}, {8, "0x0"}, {9, "0x0"}, {10, "0x0"}, {11, "0x0"},
    {12, "0x0"}, {13, "0x0"}, {14, "0x0"}, {15, "0x0"}, {16, "0x0"}, {17, "0x0"},
    {18, "0x0"}, {19, "0x0"}, {20, "0x0"}, {21, "0x0"}, {22, "0x0"}, {23, "0x0"},
    {24, "0x0"}, {25, "0x0"}, {26, "0x0"}, {27, "0x0"}, {28, "0x0"}, {29, "0x0"},
    {30, "0x0"}, {31, "0x0"}
};

// memory map
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

// cout pc & register
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

// cout memory
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

// Combine while loop!!
void dmoption(bool d, bool m, string addr1, string addr2){
    if(d){
        cpr();
        if(m){
            cm(memo, addr1, addr2);
        }        
    }  
}

void pcplus(){
    int temp = stoi(pc, nullptr, 16);
    temp += 4;
    stringstream ss;            
    ss << hex << "0x" << temp;
    pc = ss.str();   
}

// Each Instruction Function

void addu(string rs, string rt, string rd){
    pcplus();
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    int d = stoi(rd, nullptr, 2);
    unsigned long regs = stoul(reg[s], nullptr, 16);
    unsigned long regt = stoul(reg[t], nullptr, 16);
    unsigned long regd = regs + regt;
    stringstream ss;
    ss << hex << "0x" << regd;
    reg[d] = ss.str();    
}

void andd(string rs, string rt, string rd){
    pcplus();
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    int d = stoi(rd, nullptr, 2);
    int regs = stoi(reg[s], nullptr, 16);
    int regt = stoi(reg[t], nullptr, 16);
    bitset<32> sbit(regs);
    bitset<32> tbit(regt);
    bitset<32> dbit = sbit & tbit;
    unsigned long ud = dbit.to_ulong();
    stringstream ss;
    ss << hex << "0x" << ud;
    reg[d] = ss.str();  
}

void jr(string rs){
    int s = stoi(rs, nullptr, 2);
    pc = reg[s];
}

void nor(string rs, string rt, string rd){
    pcplus();
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    int d = stoi(rd, nullptr, 2);
    int regs = stoi(reg[s], nullptr, 16);
    int regt = stoi(reg[t], nullptr, 16);
    bitset<32> sbit(regs);
    bitset<32> tbit(regt);
    bitset<32> dbit = ~(sbit | tbit);
    unsigned long ud = dbit.to_ulong();
    stringstream ss;
    ss << hex << "0x" << ud;
    reg[d] = ss.str(); 
}

void orr(string rs, string rt, string rd){
    pcplus();
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    int d = stoi(rd, nullptr, 2);
    int regs = stoi(reg[s], nullptr, 16);
    int regt = stoi(reg[t], nullptr, 16);
    bitset<32> sbit(regs);
    bitset<32> tbit(regt);
    bitset<32> dbit = sbit | tbit;
    unsigned long ud = dbit.to_ulong();
    stringstream ss;
    ss << hex << "0x" << ud;
    reg[d] = ss.str();   
}

void sll(string rt, string rd, string shamt){
    pcplus();
    int t = stoi(rt, nullptr, 2);
    int d = stoi(rd, nullptr, 2);
    int s = stoi(shamt, nullptr, 2);
    int regt = stoi(reg[t], nullptr, 16);    
    bitset<32> tbit(regt);
    bitset<32> dbit = tbit << s;
    unsigned long ud = dbit.to_ulong();
    stringstream ss;
    ss << hex << "0x" << ud;
    reg[d] = ss.str();        
}

void sltu(string rs, string rt, string rd){
    pcplus();
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    int d = stoi(rd, nullptr, 2);
    unsigned long regs = stoul(reg[s], nullptr, 16);
    unsigned long regt = stoul(reg[t], nullptr, 16);
    if (regs < regt){
        reg[d] = "0x1";
    }
    else{
        reg[d] = "0x0";
    }
}

void srl(string rt, string rd, string shamt){
    pcplus();
    int t = stoi(rt, nullptr, 2);
    int d = stoi(rd, nullptr, 2);
    int s = stoi(shamt, nullptr, 2);
    int regt = stoi(reg[t], nullptr, 16);    
    bitset<32> tbit(regt);
    bitset<32> dbit = tbit >> s;
    unsigned long ud = dbit.to_ulong();
    stringstream ss;
    ss << hex << "0x" << ud;
    reg[d] = ss.str();           
}

void subu(string rs, string rt, string rd){
    pcplus();
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    int d = stoi(rd, nullptr, 2);
    unsigned long regs = stoul(reg[s], nullptr, 16);
    unsigned long regt = stoul(reg[t], nullptr, 16);
    unsigned long regd = regs - regt;
    bitset<32> dbit(regd);
    stringstream ss;
    ss << hex << "0x" << dbit.to_ulong();
    reg[d] = ss.str();         
}

void addiu(string rs, string rt, string imm){
    pcplus();
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    unsigned long regs = stoul(reg[s], nullptr, 16);
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
        reg[t] = ss.str();
    }
    else{
        unsigned long iul = stoul(imm, nullptr, 2);
        unsigned long regt = regs + iul;
        bitset<32> tbit(regt);
        stringstream ss;
        ss << hex << "0x" << tbit.to_ulong();
        reg[t] = ss.str(); 
    }        
}

void andi(string rs, string rt, string imm){
    pcplus();
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    int regs = stoi(reg[s], nullptr, 16);
    bitset<32> sbit(regs);
    bitset<32> ibit(imm);
    bitset<32> tbit = sbit & ibit;
    unsigned long ut = tbit.to_ulong();
    stringstream ss;
    ss << hex << "0x" << ut;
    reg[t] = ss.str();
}
// @@
void beq(string rs, string rt, string offset){
    pcplus();
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    if (reg[s] == reg[t]){
        string temp1 = offset + "00";
        unsigned long tempul = stoul(temp1, nullptr, 2);
        unsigned long pcul = stoul(pc, nullptr, 16);
        unsigned long temp2 = tempul + pcul;
        stringstream ss;
        ss << hex << "0x" << temp2;
        pc = ss.str();
    }
}
// @@
void bne(string rs, string rt, string offset){
    pcplus();
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    if (reg[s] != reg[t]){
        string temp1 = offset + "00";
        unsigned long tempul = stoul(temp1, nullptr, 2);
        unsigned long pcul = stoul(pc, nullptr, 16);
        unsigned long temp2 = tempul + pcul;
        stringstream ss;
        ss << hex << "0x" << temp2;
        pc = ss.str();       
    }
}

void j(string target){
    string jumptar = "0000" + target + "00";
    bitset<32> jbit(jumptar);
    stringstream ss;
    ss << hex << "0x" << jbit.to_ulong();
    string jpc = ss.str();
    pc = jpc;
}

void jal(string target){
    pcplus();
    reg[31] = pc;
    string jumptar = "0000" + target + "00";
    bitset<32> jbit(jumptar);
    stringstream ss;
    ss << hex << "0x" << jbit.to_ulong();
    string jpc = ss.str();
    pc = jpc;
}

void lui(string rt, string imm){
    pcplus();
    int t = stoi(rt, nullptr, 2);
    string temp = imm + "0000000000000000";
    bitset<32> tbit(temp);
    stringstream ss;
    ss << hex << "0x" << tbit.to_ulong();
    reg[t] = ss.str(); 
}

void lw(string rs, string rt, string imm){
    pcplus();
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    long long sll = stoll(reg[s], nullptr, 16);
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
    if (memo.find(addr) !=  memo.end()){
        reg[t] = memo[addr];
    }
    else{
        reg[t] = "0x0";
    } 
}

void ori(string rs, string rt, string imm){
    pcplus();
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    unsigned long regs = stoul(reg[s], nullptr, 16);
    bitset<32> sbit(regs);
    bitset<32> ibit(imm);
    bitset<32> tbit = sbit | ibit;
    unsigned long ut = tbit.to_ulong();
    stringstream ss;
    ss << hex << "0x" << ut;
    reg[t] = ss.str();       
}

void sltiu(string rs, string rt, string imm){
    pcplus();
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    if (imm[0] == '1'){
          imm = "1111111111111111" + imm;
    }
    unsigned long im = stoul(imm, nullptr, 2);
    unsigned long regs = stoul(reg[s], nullptr, 16);
    if (regs < im){
        reg[t] = "0x1";
    }
    else{
        reg[t] = "0x0";
    }       
}

void sw(string rs, string rt, string offset){
    pcplus();
    int s = stoi(rs, nullptr, 2);
    int t = stoi(rt, nullptr, 2);
    long long sll = stoll(reg[s], nullptr, 16);
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
    memo[addr] = reg[t];
}

void lb(string rs, string rt, string offset){
    pcplus();
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
        stringstream ss;
        ss << hex << "0x" << tarmem;
        string target = ss.str();
        cout << target << endl;
        string temp = memo[target].substr(2);
        while(temp.length() != 8){
            temp = '0' + temp;
        }
        cout << temp << endl;
        temp = temp.substr(xx, 2);
        temp = "0x"+temp;
        reg[t] = temp;
        cout << temp << endl;
        
    }
    else {
        long long off = stoll(offset, nullptr, 2);
        int w = off / 4;
        int x = 2*(off % 4);
        long long tarmem = stoll(reg[s], nullptr, 16) + (w * 4);
        stringstream ss;
        ss << hex << "0x" << tarmem;
        string target = ss.str();
        string temp = memo[target].substr(x+2,2);
        temp = "0x" + temp;
        reg[t] = temp;
    }   
}

void sb(string rs, string rt, string offset){
    pcplus();
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
        stringstream ss;
        ss << hex << "0x" << tarmem;
        string target = ss.str();
        string temp = memo[target].substr(2);
        while(temp.length() != 8){
            temp = '0' + temp;
        }
        temp = temp.substr(xx, 2);
        string temp2 = reg[t].substr(2);
        if (temp2.length() > 2){
            temp2 = temp2.substr(temp2.length()-2, 2);
        }
        else if (temp2.length() == 1){
            temp2 = '0' + temp2;
        }
        
        if (xx==0){
            temp = temp2 + temp.substr(2);
        }
        else if (xx == 6){
            temp = temp.substr(0,6) + temp2;
        }
        else if (xx == 4){
            temp = temp.substr(0,4) + temp2 + temp.substr(6, 2);
        }
        else if (xx == 2){
            temp = temp.substr(0,2) + temp2 + temp.substr(4);
        }
        while(temp[0] == '0'){
            temp = temp.substr(1);
        }
        memo[target] = "0x" + temp;   
    }
    else{
        long long off = stoll(offset, nullptr, 2);
        int w = off / 4;
        int x = 2*(off % 4);
        long long tarmem = stoll(reg[s], nullptr, 16) + (w*4);
        stringstream ss;
        ss << hex << "0x" << tarmem;
        string target = ss.str();
        string temp = memo[target].substr(2);
        string temp2 = reg[t].substr(2);
        if(temp2.length() > 2){
            temp2 = temp2.substr(temp2.length()-2,2);
        }
        else if (temp2.length() == 1){
            temp2 = '0' + temp2;
        }
        if (x == 0){
            temp = temp2 + temp.substr(2);
        }
        else if (x == 2){
            temp = temp.substr(0, 2) + temp2 + temp.substr(4);
        }
        else if (x == 4){
            temp = temp.substr(0, 4) + temp2 + temp.substr(6);
        }
        else if (x == 6){
            temp = temp.substr(0, 6) + temp2;
        }
        memo[target] = "0x" + temp;
    }  
}

void detect(bool d, bool m, string addr1, string addr2){
    
    if (memo.find(pc) == memo.end()){
        return;
    }
    string line = memo[pc];
    // 16진법 string > 2진법 string
    unsigned long long hex = stoull(line, nullptr, 16);
    bitset<32> linebits(hex);
    string li = linebits.to_string();
    // 앞의 opcode
    string opcode = li.substr(0,6);  
    
    if (opcode == "000000"){
        string func = li.substr(26, 6);
        if (func == "100001"){
            string rs = li.substr(6, 5);
            string rt = li.substr(11, 5);
            string rd = li.substr(16, 5);
            addu(rs, rt, rd);
        }
        else if (func == "100100"){
            string rs = li.substr(6, 5);
            string rt = li.substr(11, 5);
            string rd = li.substr(16, 5);
            andd(rs, rt, rd);
        }    
        else if (func == "001000"){
            string rs = li.substr(6, 5);
            jr(rs);
        }      
        else if (func == "100111"){
            string rs = li.substr(6, 5);
            string rt = li.substr(11, 5);
            string rd = li.substr(16, 5);
            nor(rs, rt, rd);
        }      
        else if (func == "100101"){
            string rs = li.substr(6, 5);
            string rt = li.substr(11, 5);
            string rd = li.substr(16, 5);    
            orr(rs, rt, rd);
        }      
        else if (func == "000000"){
            string rt = li.substr(11, 5);
            string rd = li.substr(16, 5);
            string shamt = li.substr(21, 5);
            sll(rt, rd, shamt);
        }      
        else if (func == "101011"){
            string rs = li.substr(6, 5);
            string rt = li.substr(11, 5);
            string rd = li.substr(16, 5);
            sltu(rs, rt, rd);
        }      
        else if (func == "000010"){
            string rt = li.substr(11, 5);
            string rd = li.substr(16, 5);
            string shamt = li.substr(21, 5);
            srl(rt, rd, shamt);
        }      
        else if (func == "100011"){
            string rs = li.substr(6, 5);
            string rt = li.substr(11, 5);
            string rd = li.substr(16, 5);
            subu(rs, rt, rd);
        }    
    }
    else if(opcode == "001001"){
        string rs = li.substr(6, 5);
        string rt = li.substr(11, 5);
        string imm = li.substr(16, 16);
        addiu(rs, rt, imm);
    }
    else if(opcode == "001100"){
        string rs = li.substr(6, 5);
        string rt = li.substr(11, 5);
        string imm = li.substr(16, 16);
        andi(rs, rt, imm);
    }    
    else if(opcode == "000100"){
        string rs = li.substr(6, 5);
        string rt = li.substr(11, 5);
        string offset = li.substr(16, 16);
        beq(rs, rt, offset);
    }   
    else if(opcode == "000101"){
        string rs = li.substr(6, 5);
        string rt = li.substr(11, 5);
        string offset = li.substr(16, 16);
        bne(rs, rt, offset);
    }    
    else if(opcode == "000010"){
        string target = li.substr(6, 26);
        j(target);
    }    
    else if(opcode == "000011"){
        string target = li.substr(6, 26);
        jal(target);
    }    
    else if(opcode == "001111"){
        string rt = li.substr(11, 5);
        string imm = li.substr(16, 16);
        lui(rt, imm);
    }    
    else if(opcode == "100011"){
        string rs = li.substr(6, 5);
        string rt = li.substr(11, 5);
        string imm = li.substr(16, 16);
        lw(rs, rt, imm);
    }    
    else if(opcode == "001101"){
        string rs = li.substr(6, 5);
        string rt = li.substr(11, 5);
        string imm = li.substr(16, 16);
        ori(rs, rt, imm);
    }    
    else if(opcode == "001011"){
        string rs = li.substr(6, 5);
        string rt = li.substr(11, 5);
        string imm = li.substr(16, 16);
        sltiu(rs, rt, imm);
    }    
    else if(opcode == "101011"){
        string rs = li.substr(6, 5);
        string rt = li.substr(11, 5);
        string offset = li.substr(16, 16);
        sw(rs, rt, offset);
    }    
    else if(opcode == "100000"){
        string rs = li.substr(6, 5);
        string rt = li.substr(11, 5);
        string offset = li.substr(16, 16);
        lb(rs, rt, offset);
    }    
    else if(opcode == "101000"){
        string rs = li.substr(6, 5);
        string rt = li.substr(11, 5);
        string offset = li.substr(16, 16);
        sb(rs, rt, offset);
    }
    
    dmoption(d, m, addr1, addr2);
}

// Combine
void Combine(int textsize, bool d, int n, bool m, string addr1, string addr2){
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
            detect(d, m, addr1, addr2);
        }
        if (!d){
            cpr();
            if(m){
                cm(memo, addr1, addr2);
            }
        }        
    }
    else if (n == 0){
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
            detect(d, m, addr1, addr2);
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
    // 0 : runfile, 1 : m, 2 : d, 3 : n, 4 : file
    string addr1 = "";
    string addr2 = "";
    bool m = false;
    bool d = false;
    int n = -1;
    for (int i = 0 ; i < argc ; i++){
        if (strcmp(argv[i], "-m") == 0){
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
    Combine(textsize, d, n, m, addr1, addr2); 
    // close file
    infile.close();
    return 0;
}
