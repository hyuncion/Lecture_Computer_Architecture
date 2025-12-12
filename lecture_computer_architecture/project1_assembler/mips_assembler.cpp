#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bitset>
#include <map>

using namespace std;


// 1. print text section size

int textsize(istream& file){
    
    // main location
    string l;
    int ml = 0;
    while (getline(file, l)){
        if (l.find("main") != string::npos){
            break;
        }
        else {
            ml += 1;
        }
    }    

    // file pointer
    file.clear();
    file.seekg(0, ios::beg);    
    
    // text size
    string line;
    int textsize = 0;
    
    for (int i = 0 ; i <= ml ; ++i){
        getline(file, line);
    }
    
    while (getline(file, line)){
        if (line.find("addiu") != string::npos){
            textsize += 4;
        }
        else if (line.find("addu") != string::npos){
            textsize += 4;
        }
        else if (line.find("and") != string::npos){
            textsize += 4;
        }                 
        else if (line.find("andi") != string::npos){
            textsize += 4;
        }         
        else if (line.find("beq") != string::npos){
            textsize += 4;
        }          
        else if (line.find("bne") != string::npos){
            textsize += 4;
        }          
        else if (line.find("j") != string::npos){
            textsize += 4;
        }          
        else if (line.find("jal") != string::npos){
            textsize += 4;
        }          
        else if (line.find("jr") != string::npos){
            textsize += 4;
        }          
        else if (line.find("lui") != string::npos){
            textsize += 4;
        }          
        else if (line.find("lw") != string::npos){
            textsize += 4;
        }
        else if (line.find("la") != string::npos){
            textsize += 8;
        }       
        else if (line.find("nor") != string::npos){
            textsize += 4;
        }    
        else if (line.find("or") != string::npos){
            textsize += 4;
        }
        else if (line.find("ori") != string::npos){
            textsize += 4;
        }        
        else if (line.find("sltiu") != string::npos){
            textsize += 4;
        }        
        else if (line.find("sltu") != string::npos){
            textsize += 4;
        }        
        else if (line.find("sll") != string::npos){
            textsize += 4;
        }        
        else if (line.find("srl") != string::npos){
            textsize += 4;
        }
        else if (line.find("sw") != string::npos){
            textsize += 4;
        }         
        else if (line.find("subu") != string::npos){
            textsize += 4;
        }        
        else if (line.find("lb") != string::npos){
            textsize += 4;
        }        
        else if (line.find("sb") != string::npos){
            textsize += 4;
        }        
    }
    
    // file pointer
    file.clear();
    file.seekg(0, ios::beg);  
    
    return textsize;
}

// 2. print data section size

int datasize(istream& file){
    
    // data size
    string line2;
    int datasize = 0;
    while (getline(file, line2)){
        if (line2.find("word") != string::npos){
            datasize += 4;
        }
    }
    
    // file pointer
    file.clear();
    file.seekg(0, ios::beg);
    
    return datasize;
}


// 3. print instruction with hexadecimal code

// 3.0 dividng instruction

string divins(string line){
    string result;
    if (line.find("addiu") != string::npos){
        result = "addiu";
    }
    else if (line.find("addu") != string::npos){
        result = "addu";
    }
    else if (line.find("andi") != string::npos){
        result = "andi";
    }                 
    else if (line.find("and") != string::npos){
        result = "and";
    }         
    else if (line.find("beq") != string::npos){
        result = "beq";
    }          
    else if (line.find("bne") != string::npos){
        result = "bne";
    }          
    else if (line.find("jal") != string::npos){
        result = "jal";
    }          
    else if (line.find("jr") != string::npos){
        result = "jr";
    }          
    else if (line.find("j") != string::npos){
        result = "j";
    }          
    else if (line.find("lui") != string::npos){
        result = "lui";
    }          
    else if (line.find("lw") != string::npos){
        result = "lw";
    }
    else if (line.find("la") != string::npos){
        result = "la";
    }       
    else if (line.find("nor") != string::npos){
        result = "nor";
    }    
    else if (line.find("ori") != string::npos){
        result = "ori";
    }
    else if (line.find("or") != string::npos){
        result = "or";
    }        
    else if (line.find("sltiu") != string::npos){
        result = "sltiu";
    }        
    else if (line.find("sltu") != string::npos){
        result = "sltu";
    }        
    else if (line.find("sll") != string::npos){
        result = "sll";
    }        
    else if (line.find("srl") != string::npos){
        result = "srl";
    }
    else if (line.find("sw") != string::npos){
        result = "sw";
    }         
    else if (line.find("subu") != string::npos){
        result = "subu";
    }        
    else if (line.find("lb") != string::npos){
        result = "lb";
    }        
    else if (line.find("sb") != string::npos){
        result = "sb";
    }
  
    return result;
}

// 3.1 instruction to binary code (basic)

map<string, string> fmap = {
    {"addiu", "i"},
    {"addu", "r"},
    {"and", "r"},
    {"andi", "i"},
    {"beq", "i"},
    {"bne", "i"},
    {"j", "j"},
    {"jal", "j"},
    {"jr", "r"},
    {"lui", "i"},
    {"lw", "i"},
    {"nor", "r"},
    {"or", "r"},
    {"ori", "i"},
    {"sltiu", "i"},
    {"sltu", "r"},
    {"sll", "r"},
    {"srl", "r"},
    {"sw", "i"},
    {"subu", "r"},
    {"lb", "i"},
    {"sb", "i"},
    {"la", "ps"}
};


map<string, string> opmap = {
    {"addiu", "001001"},
    {"addu", "000000"},
    {"and", "000000"},
    {"andi", "001100"},
    {"beq", "000100"}, 
    {"bne", "000101"},
    {"j", "000010"}, 
    {"jal", "000011"}, 
    {"jr", "000000"}, 
    {"lui", "001111"}, 
    {"lw", "100011"}, 
    {"nor", "000000"},
    {"or", "000000"},
    {"ori", "001101"},
    {"sltiu", "001011"},
    {"sltu", "000000"},
    {"sll", "000000"},
    {"srl", "000000"},
    {"sw", "101011"},
    {"subu", "000000"},
    {"lb", "100000"},
    {"sb", "101000"}
};

map<string, string> rfuncmap = {
    {"addu", "100001"},
    {"and", "100100"},
    {"jr", "001000"},
    {"nor", "100111"},
    {"or", "100101"},
    {"sll", "000000"},
    {"sltu", "101011"},
    {"srl", "000010"},
    {"subu", "100011"}
};

// 3.2 register to binary code

map<string, string> regmap = {
    {"$0", "00000"},
    {"$1", "00001"},
    {"$2", "00010"},
    {"$3", "00011"},
    {"$4", "00100"},
    {"$5", "00101"},
    {"$6", "00110"},
    {"$7", "00111"},
    {"$8", "01000"},
    {"$9", "01001"},
    {"$10", "01010"},
    {"$11", "01011"},
    {"$12", "01100"},
    {"$13", "01101"},
    {"$14", "01110"},
    {"$15", "01111"},
    {"$16", "10000"},
    {"$17", "10001"},
    {"$18", "10010"},
    {"$19", "10011"},
    {"$20", "10100"},
    {"$21", "10101"},
    {"$22", "10110"},
    {"$23", "10111"},
    {"$24", "11000"},
    {"$25", "11001"},
    {"$26", "11010"},
    {"$27", "11011"},
    {"$28", "11100"},
    {"$29", "11101"},
    {"$30", "11110"},
    {"$31", "11111"}
};

// 3.3 constant to binary code

string D2B(int dec){

	bitset<16> bits(dec);
	string result = bits.to_string();
	return result;
}

// 3.5 demical code to hexademical code

string D2H(int num){
    stringstream ss;
    ss << hex << num;
    string str_hex = ss.str();
    str_hex = "0x" + str_hex;
    return str_hex;
}

// 3.6 binary code to hexadecimal code (later!!)

string B2H(string B){
    bitset<32> b(B);
    int i = static_cast<int>(b.to_ulong());
    string H = D2H(i);
    return H;
}

// (+) isint

bool isint(char i){
    if (i == '0' || i == '1' || i == '2' || i == '3' || i == '4' || i == '5' || i == '6' || i == '7' || i == '8' || i == '9'){
    
        return true;
    }
    else{
    
        return false;
    }
}


// 4. print value with hexadecimal code

// small alphabet!

string small(string line){
    string temp = line;
    for (int i = 0 ; i < line.length() ; i++){
        if (isdigit(line[i])){
            continue;
        }
        else if (isupper(line[i])){
            char t = tolower(line[i]);
            temp.replace(i, 1, string(1,t));
        }      
    }
    return temp;
}


void V2H(istream& file, ofstream& ofile){

    string line;
    while(getline(file, line)){
    
        if (line.find(".word   ") != string::npos){
        
            int id = line.find(".word   ");
            string value = line.substr(id+8);
        
            if (value.find("0x") == string::npos){
                int iv = stoi(value);
                string hv = D2H(iv);
                ofile << hv << endl;
            }
            else { 
                ofile << small(value) << endl;
            }        
        }  
    }
    
    // file pointer
    file.clear();
    file.seekg(0, ios::beg); 
}



// insfunction
void insfunction(istream& file, string line, int linenum, int insnum, ofstream& ofile){
    string ins = divins(line);
    string format = fmap[ins];
    if (format == "j"){
    
        string op = opmap[ins];
        
        if (line.find("0x") != string::npos){
            string addr = line.substr(line.find("0x")+2);
            unsigned int add = std::stoul(addr, nullptr, 16);
            bitset<32> ad(add);
            bitset<26> address;
            for (int i = 0 ; i < 26 ; i++){
                address[i] = ad[i+2];
            }
            string jtar = address.to_string();
            string bin = op + jtar;
            ofile << B2H(bin) << endl;
        }
        
        else{
        
            string w;
            if (ins == "jal"){
                int in = line.find("jal ");
                w = (line.substr(in+4) + ':');
            }
            else if (ins == "j"){
                int in = line. find("j ");
                w = (line.substr(in +2) + ':');
            }
        
            file.clear();
            file.seekg(0, ios::beg);
            string e;
            int wnum = 0;               
            while(getline(file, e)){
            
                if (e.find(w) != string::npos){
                    if (w == "exit:" && w.length() != e.length()){
                        continue;
                    }
                    wnum += 1;
                    string addr = "0x400000";
                    int add = stoi(addr, nullptr, 16);
                    for (int i = 0 ; i < wnum ; i++){
                        add += 4;
                    }
                    bitset<32> addrbit(add);
                    bitset<26> jtarbit;
                    
                    for (int i = 0 ; i < 26 ; i++){
                        jtarbit[i] = addrbit[i+2];
                    }
                    
                    string jtar = jtarbit.to_string();
                    string bin = op + jtar;
                    ofile << B2H(bin) << endl;

                    break;
                }
                else if(e.find(".word") != string::npos){
                    continue;
                }
            
                else if (e.find("addiu") != string::npos || e.find("addu") != string::npos
                    || e.find("andi") != string::npos || e.find("and") != string::npos
                    || e.find("beq") != string::npos || e.find("bne") != string::npos
                    || e.find("jal") != string::npos || e.find("jr") != string::npos
                    || e.find("j") != string::npos || e.find("lui") != string::npos
                    || e.find("lw") != string::npos || e.find("nor") != string::npos
                    || e.find("ori") != string::npos
                    || e.find("or") != string::npos || e.find("sltiu") != string::npos
                    || e.find("sltu") != string::npos || e.find("sll") != string::npos
                    || e.find("srl") != string::npos || e.find("sw") != string::npos
                    || e.find("subu") != string::npos || e.find("lb") != string::npos
                    || e.find("sb") != string::npos){
                    wnum+=1;
                }
                else if(e.find("la") != string::npos){
                    wnum+=2;
                }
            }
            file.clear();
            file.seekg(0, ios::beg);
            for (int i = 0 ; i < linenum ; i++){
                getline(file, e);
            }
        }
    }
    else if (format == "i"){
        if(ins == "lb" || ins == "sb" || ins == "lw" || ins == "sw"){

            string op = opmap[ins];
            int a = 1;
            int fr = line.find("$");
            if (isint(line[fr+2])){
                a = 2;
            }                    
            string fre = line.substr(fr, a+1); 
            string rt = regmap[fre];
            
            int b = 1;
            if (isint(line[fr + a + 4])){
                b = 2;
            }
            string offset = line.substr(fr + a + 3, b);
            int dof;
            stringstream ss(offset);
            ss >> dof;
            bitset<16> binary(dof);
            string off = binary.to_string();         
            
            int c = 1;
            int sr = line.find("$", fr + 1);
            if (isint(line[sr + 2])){
                c = 2;
            }
            string sre = line.substr(sr, c+1);
            string rs = regmap[sre];
            
            string bin = op + rs + rt + off;
            ofile << B2H(bin) << endl;
        }
        
        else if(ins == "lui" || ins == "ori"){

            if (ins == "lui"){
                int a = 1;
                int fr = line.find("$");
                if (isint(line[fr+2])){
                    a = 2;
                }                    
                string fre = line.substr(fr, a+1); 
                string rt = regmap[fre];
                string temp = line.substr(fr+a+2);
                int imm = stoi(temp);
                string im = D2B(imm);
                string op = opmap["lui"];
                string bin = op + "00000" + rt + im;
                ofile << B2H(bin) << endl;
            }
            else{
                int a = 1;
                int b = 1;
                int fr = line.find("$");                    
                if (isint(line[fr+2])){
                    a = 2;
                }                    
                string fre = line.substr(fr, a+1); 
                string rt = regmap[fre];
                int sr = line.find("$", fr+1);
                if (isint(line[sr+2])){
                    b = 2;
                }                    
                string sre = line.substr(sr, b+1); 
                string rs = regmap[sre];                                               
                string temp = line.substr(sr+b+2);
                int imm = stoi(temp);
                string im = D2B(imm);
                string op = opmap["ori"];                   
                string bin = op + rs + rt + im; 
                ofile << B2H(bin) << endl;
            }
        }
        
        else{
            string op = opmap[ins];
        
            int a = 1;
            int b = 1;
            
            int fr = line.find("$");
            if (isint(line[fr+2])){
                a = 2;
            }
            
            
            string fre = line.substr(fr, a+1); 
                           
            int sr = line.find("$", fr+1);
            if (isint(line[sr+2])){
                b = 2;
            }   
            
            string sre = line.substr(sr, b+1);                
            
            string r1 = regmap[fre];
            string r2 = regmap[sre];
            
            if (ins == "beq" || ins == "bne"){ 
                
                file.clear();
                file.seekg(0, ios::beg);
                string p;
                int wnum = 0;
                string w = (line.substr(sr+b+3) + ":");
                while(getline(file, p)){
                    wnum += 1;
                    if(p.find(w) != string::npos){
                        if(line.length() == w.length()){
                            getline(file, p);
                        }
                        int num = wnum - linenum - 1;
                        string im = D2B(num);
                        string bin = op + r1 + r2 + im; 
                        ofile << B2H(bin) << endl;
                        break;
                    }
                }                
                
                // file pointer
                file.clear();
                file.seekg(0, ios::beg);
                for (int i=0 ; i < linenum ; i++){
                    getline(file, p);
                }              
            }
            else {
                string temp = line.substr(sr+b+2);
                int imm;
                if (temp.find("0x") != string::npos){
                    imm = stoi(temp, nullptr, 16);
                }
                else{
                    imm = stoi(temp);
                }
                string im = D2B(imm);
                string bin = op + r2 + r1 + im;
                ofile << B2H(bin) << endl;
            }
        }
    }
    else if (format == "r"){
        string op = opmap[ins];
        string func = rfuncmap[ins];
        if (ins == "jr"){
            int a = 1;
            int fr = line.find("$");
            if (isint(line[fr+2])){
                a = 2;
            }
            
            
            string fre = line.substr(fr, a+1); 
                          
            string rs = regmap[fre];
            string rt = "00000";
            string rd = "00000";
            string shamt = "00000";
            string bin = op + rs + rt + rd + shamt + func;
            ofile << B2H(bin) << endl;               
        }
        else if (ins == "srl" || ins == "sll"){
            string rs = "00000";
        
            int a = 1;
            int b = 1;
            
            int fr = line.find("$");
            if (isint(line[fr+2])){
                a = 2;
            }
            
            
            string fre = line.substr(fr, a+1); 
            
            int sr = line.find("$", fr+1);
            if (isint(line[sr+2])){
                b = 2;
            }   
            
            string sre = line.substr(sr, b+1); 
            
            string rd = regmap[fre];
            string rt = regmap[sre];
            
            string temp = line.substr(sr+b+3);

            int sh = stoi(temp);
            bitset<5> bit(sh);
            string shamt = bit.to_string();   
                      
            string bin = op + rs + rt + rd + shamt + func;
            ofile << B2H(bin) << endl; 
        }

        else {
        
            int a = 1;
            int b = 1;
            int c = 1;
            int fr = line.find("$");
            if (isint(line[fr+2])){
                a = 2;
            }
            
            
            string fre = line.substr(fr, a+1); 
            
            int sr = line.find("$", fr+1);
            if (isint(line[sr+2])){
                b = 2;
            }
            
            string sre = line.substr(sr, b+1); 
            
            int tr = line.find("$", sr+1);
            if (isint(line[tr+2])){
                c = 2;
            }
            
            string tre = line.substr(tr, c+1); 
            
            string rd = regmap[fre];
            string rs = regmap[sre];
            string rt = regmap[tre];
            string shamt = "00000";                                  
            string bin = op + rs + rt + rd + shamt + func;
            ofile << B2H(bin) << endl;
        }
        

    }
    else if (format =="ps"){
        if (ins == "la"){
            int a = 1;
            int fr = line.find("$");
            if (isint(line[fr+2])){
                a = 2;
            }
            string fre = line.substr(fr, a+1);               
            string re = regmap[fre];            
            
            
            int d = line.find(",");
            string w = (line.substr(d+2) + ":");
            
            file.clear();
            file.seekg(0, ios::beg);
            
            
            string e;
            int c = 0;
            
            while(getline(file, e)){
                if (e.find(w) != string::npos){
                    c-=1;
                    int t = e.find(".word");
                    string lu = "1000";
                    
                    string bin = opmap["lui"] + "00000" + re;
                    string Bin = B2H(bin) + lu;
                    ofile << Bin << endl;
                     
                       
                    string addr = "0x0000";
                    int add = stoi(addr, nullptr, 16);
                    for (int i = 0 ; i < c ; i++){
                        add += 4;
                    }
                    bitset<16> addrbit(add);  
                    string h = addrbit.to_string();                                            
                    bin = opmap["ori"] + re + re + h;
                    Bin = B2H(bin);
                    ofile << Bin << endl;
                    break;                        
                }
                else{
                    c +=1;
                }
            }
            
         // file pointer
            string po;
            while(getline(file, po)){
                if (po.find("la") != string::npos){
                    break;
                }                 
            }               
        }
    }  
}


// 5. Combine all of them


void Combine(istream& file, ofstream& ofile){
    
    // line num
    int linenum = 0;
    int insnum = 0;
    
    // text size
    int ts;
    ts = textsize(file);
    string tsize;
    tsize = D2H(ts);
    ofile << tsize << endl;
    
    // data size
    int ds;
    ds = datasize(file);
    string dsize;
    dsize = D2H(ds);
    ofile << dsize << endl;
    
    // instruction hexademical code
    // 3.1 main location
    string l;
    int ml = 0;
    while (getline(file, l)){
        linenum += 1;
        if (l.find("main") != string::npos){
            break;
        }
        else {
            ml += 1;
        }
    }  
    
    // 3.2 file pointer
    file.clear();
    file.seekg(0, ios::beg);
    
    
    // 3.3 main operation
    string li;
    for (int i = 0 ; i <= ml ; ++i){
        getline(file, li);
    }
    
    string line;
    while(getline(file, line)){
        
        linenum +=1;
        
        // None > jump case
        if (line.find("addiu") == string::npos & line.find("addu") == string::npos
        & line.find("andi") == string::npos & line.find("and") == string::npos
        & line.find("beq") == string::npos & line.find("bne") == string::npos
        & line.find("jal") == string::npos & line.find("jr") == string::npos
        & line.find("j") == string::npos & line.find("lui") == string::npos
        & line.find("lw") == string::npos & line.find("la") == string::npos
        & line.find("nor") == string::npos & line.find("ori") == string::npos
        & line.find("or") == string::npos & line.find("sltiu") == string::npos
        & line.find("sltu") == string::npos & line.find("sll") == string::npos
        & line.find("srl") == string::npos & line.find("sw") == string::npos
        & line.find("subu") == string::npos & line.find("lb") == string::npos
        & line.find("sb") == string::npos){
            continue;
        }       
        
        else{           
            insnum += 1;
            insfunction(file, line, linenum, insnum, ofile);
                 
        }
    }  
      
    // file pointer
    file.clear();
    file.seekg(0, ios::beg);
          
    // value
    V2H(file, ofile);  
}


int main() {

    // open file
    ifstream ifile;
    ifile.open("sample.s");
    
    ofstream ofile("sample.o");

    if (!ifile.is_open()){
        cerr << "Error" << endl;
        return 1;
    }

    // implement

    Combine(ifile, ofile);

    // close file
    ifile.close();
    ofile.close();
    return 0;
}
