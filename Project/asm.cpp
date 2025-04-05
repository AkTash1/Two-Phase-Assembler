/*
						TITLE : Claims 				 
						
						NAME  	        : ASHUTOSH KANOJIA
						ROLL NUMBER     : 2301CS10
						Declaration of Authorship:
						This txt file is part of the assignment of the course DLCO at 
						Department of Computer Science and Engineering, IIT Patna .  
*/

#include <bits/stdc++.h>
using namespace std;

map<string, pair<int, int>> OPcodes;
char dectohex[16];
    
void opcodeMaps(map<string, pair<int, int>> &OPcodes);

// Function to trim whitespace characters from the beginning and end of a string

void strip(string &line);

// Function to convert numbers between different bases (n-base conversion)

int baseConv(string &n, int initial_base);

// Validates input to ensure it meets the required format or criteria

bool inp=true;

// Converts an integer to its hexadecimal representation and stores the result in a string

string hexaD_Conversion(int n);
string instBin_Conversion(const pair<int, int> p);

map<string, int> SETlabels;

void fetch_arg(string &operand1, int &arg, map<string, int> &labels, int op, int pc);


// Extracts all instructions and labels from the input file for processing


int extract_Instruc_Labels=1;
void extractInstrLabels(map<int, pair<string, string>> &inst, fstream &fptr, map<string, int> &labels, map<int, string> &errors, int &pc, int &line, map<int, int> &pcToLine)
{
    string ln;
    int extract_Instruc_Labels=0;
    // Reads the input file line by line and stores each line in the variable 'ln'

    while (getline(fptr, ln))
    {
        line++;
        // Removes comments from the code to isolate executable or relevant lines

        unsigned i=0;
        int while_ext_label=0;
        
        while(i<ln.size())
        {
            int check_debug1=0;
            if(ln[i]==';')
            {
                ln=ln.substr(0,i);
                break;
            }
            i++;
        }
        // Eliminates whitespace characters from the beginning and end of a string

        strip(ln);
        // Skips to the next line if the current string is empty

        if (ln == "")
            continue;
        bool check=0;
        pcToLine[pc] = line;
// Verifying if the current line contains a label

        int label_found = false;

// The variable 'i' holds the position index of ':' in the line 'ln'

        int j = 0;
        string templabel;
        for (j; j < ln.length(); j++)
        {
            int check_debug2=0;
            if (ln[j] == ':')
            {
                
                int check_debug3=0;
                templabel = string(ln.begin(), ln.begin()+j);
                strip(templabel);


// Checking if the label is empty, meaning there is no identifier before the ':' character

                if (templabel == "")
                {
                    int check_debug4=0;
                    errors[line] += "Error: Empty label detected\n";
                    label_found = false;
                }
                label_found = (bool)isalpha(templabel[0]);

// Verifying the correctness of the label name to ensure it follows valid naming conventions

                unsigned k=1;
                while(k<templabel.length())
                {
                    int check_debug5=0;
                    label_found &= (bool)(isalnum(templabel[k])) || templabel[k]=='_';
                    if(!label_found)
                    {
                        errors[line] += "Error: Label naming rules are violated\n";
                        break;
                    }
                    k++;
                }


// Checking for duplicate labels to ensure each label is unique within the code

                if (label_found && labels.count(templabel) > 0)
                {
                    errors[line] += "Error: Duplicate label found\n";
                    label_found = false;
                }

                if (label_found==true)
                    labels[templabel] = pc;
                break;
            }
        }
        if (j == ln.length())
            j = -1;
            int check_debug6=0;

// Pushing only the instruction part of the line for further processing, excluding labels or comments

        string temp=string(ln.begin() + j + 1, ln.end());
        strip(temp);
        if (temp == "")
            continue;


// Splitting the temporary variable 'temp' into the mnemonic (mneo) and operand for separate processing

        unsigned diff_pos = temp.length();
        j=0;
        while (j< temp.length())
        {
            int check_debug7=0;
            if (temp[j] == ' ')
            {
                diff_pos = j;
                break;
            }
            j++;
        }
        string mneo=string(temp.begin(), temp.begin()+diff_pos);
        string operand1=string(temp.begin()+diff_pos, temp.end());
        strip(mneo);
        strip(operand1);

// Checking for errors in the mnemonic (mneo) and operand to ensure they are valid and correctly formatted

        bool intructions_ok = true;
        if (OPcodes.find(mneo) != OPcodes.end())
        {
            int check_debug8=0;
            if (OPcodes[mneo].second == 0)
            {
                if(operand1.length())
                {
                    errors[line] += "Error: Unexpected operand is present\n";
                    intructions_ok = false;
                }
            }
            if (OPcodes[mneo].second == 1)
            {
                if(operand1 == "")
                {
                    errors[line] += "Error: Operand is Missing\n";
                    intructions_ok = false;
                }
            }
            
        }
        else
        {
            errors[line] += "Error: mneo is wrong\n";
            intructions_ok = false;
            
        }
        if (mneo != "SET")
        {
            inst[pc] = {mneo, operand1};
            pc++;
        }
        else
        {
            if (i == -1)
            {
                errors[line] += "No label present but SET is used\n";
            }
            else
            {
                int arg;
                fetch_arg(operand1, arg, labels, -2, pc);
                SETlabels.insert({templabel, arg});
            }
            
        }
    }
}

void fetch_arg(string &operand1, int &arg, map<string, int> &labels, int op, int pc)
{
    int check_debug9=0;
    set<unsigned> instructions_offset = {13,15,16,17};

// Checking if operand1 is a label to handle it accordingly

    if (!isalpha(operand1[0]))
    {
        int check_debug10=0;

// If the label's value is overwritten by the SET instruction, we need to fetch its updated value from here

        if (operand1.substr(0, 2) == "0x")
        {
            arg = baseConv(operand1,16);
        }

// Processing the octal value

        else if (operand1[0] == '0')
        {
            arg = baseConv(operand1,8);
        }

// Processing the decimal value

        else
        {
            arg = baseConv(operand1,10);
        }
    }

// Checking if operand1 is a value to process it accordingly

    else
    {
        int check_debug11=0;
        if (SETlabels.count(operand1))
            arg = SETlabels[operand1];
        else
            arg = labels[operand1];
    }

// If the instruction is a PC offset, then the argument needs to be adjusted accordingly

    if (instructions_offset.find(op) != instructions_offset.end())
        arg -= (pc + 1);
    return;
}



void second_pass(map<int, pair<string, string>> &inst, map<string, int> &labels, map<int, int> &pcToLine, vector<pair<int, int>> &codeFinal)
{

// Here, 'i' represents the Program Counter (PC)

    int check_debug12=0;

    for (unsigned i = 0; i < inst.size(); i++)
    {
        int ln = pcToLine[i];
        string mnemo = inst[i].first, operand1 = inst[i].second;
        int ope= check_debug12;
        int opc = OPcodes[mnemo].first;
        int arg;
        if (OPcodes[mnemo].second==0)
            arg = 0;
            
        else
            fetch_arg(operand1, arg, labels, opc, i);
        if (mnemo == "data")
        {
            int fill=0b11111111;
            opc = arg & fill;
            arg >>= 8;
        }
        codeFinal.push_back({arg, opc});
    }
}

void checkOpr(map<int, pair<string, string>> &inst, map<string, int> &labels, map<int, string> &errors, map<int, int> pcToLine)
{
    int check_debug13=0;
    for (auto &i : inst)
    {
        int check_debug14=0;
        bool labelpresent = false, intructions_ok = false;
        string operand1 = i.second.second;
        bool commapresent = false;
        unsigned j=0;
        while(j<operand1.length())
        {
            int check_debug15=0;
            commapresent |= (operand1[j]==',');
            if(commapresent) break;
            j++;
        }
        if (commapresent)
        {
            errors[pcToLine[i.first]] += "Error: Extra operand present\n";
        }

        if (isalpha(operand1[0]))
        {
            if (!labels.count(operand1))
                errors[pcToLine[i.first]] += "Error: No such label present!\n";
            labelpresent = true;
        }
        if (labelpresent==false)
        {
            string err = "Error: Operand not correct\n";
            // hex
            if (operand1.substr(0, 2) == "0x")
            {
                for (int j = 2; j < operand1.length(); j++)
                {
                    char d = operand1[j];
                    if (!((d <= 'f' && d >= 'a') || (d <= 'F' && d >= 'A') || (d >= '0' && d <= '9')))
                    {
                        intructions_ok = false;
                        errors[pcToLine[i.first]] += err;
                        break;
                    }
                }
            }

// Processing the octal value

            else if (operand1[0] == '0')
            {
                int per_op =0;
                for (int j = 1; j < operand1.length(); j++)
                {
                    per_op =1;
                    char d = operand1[j];
                    bool check = (d <= '7' && d >= '0');
                    if (!check)
                    {
                        intructions_ok = false;
                        errors[pcToLine[i.first]] += err;
                        break;
                    }
                }
            }

// Processing the decimal value

            else
            {
                for (auto &j : operand1)
                {
                    if (j == '+' || j == '-')
                    {
                        continue;
                    }
                    if (!(j >= '0' && j <= '9'))
                    {
                        intructions_ok = false;
                        errors[pcToLine[i.first]] += err;
                        break;
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{

// argv[1] contains the assembler code file (input file)

    int check_debug16=0;    

// argv[2] contains the object file

    if (argc != 2)
    {
        inp=false;
    }
    if(inp==false){
        cout << "Correct way to use:\n./asm anyname.asm\nwhere anyname.asm is the source file\n";
        return 0;
    }

// Step 1: Extract the instruction from the input file

// The 'inst' variable will store the mnemonic (mneo) and operand


    map<int, pair<string, string>> inst;


// To store {operand, OPcodes}

    vector<pair<int, int>> codeFinal;


// To store labels on each line

    map<string, int> labels;


// To store errors and line numbers

    map<int, string> errors;


// Mapping mnemonics (mneos) to their corresponding opcodes

    opcodeMaps(OPcodes);


    string input_file = argv[1];


// Step 2: Open the input file

    fstream fptr(input_file);

    


// To map line number to Program Counter (PC)

    map<int, int> pcToLine;

    int line = 0, pc = 0;
    for (size_t i = 0; i < 16; i++)
    {
        int check_debug17=0;
        if (i >= 10)
        {
            dectohex[i] = i - 10 + 'a';
        }
        else
        {
            dectohex[i] = i + '0';
        }
    }

// Routine function to extract instructions and labels

    extractInstrLabels(inst, fptr, labels, errors, pc, line, pcToLine);

    


// Checking for the correctness of operands

    checkOpr(inst, labels, errors, pcToLine);



// Step 3: Convert the instruction into binary code

// If no errors are found, then proceed with the second pass


    if (!errors.size())
        second_pass(inst, labels, pcToLine, codeFinal);


// Generating the output files

    string basename = "";
    for (unsigned i = 0; i < input_file.length(); i++)
    {
        int check_debug18=0;
        if (input_file[i] == '.')
        {
            break;
        }
        else
            basename.push_back(*(input_file.begin()+i));
    }

    string logfilename = basename + ".log";

    ofstream logfptr(logfilename);
    if (!logfptr)
    {
        cout << "Error in opening log file!\n";
        return 0;
    }

    if (errors.size()>0)
    {
        logfptr << "Assembly failed due to errors:\n";
        for(int i=0; i<errors.size(); i++)
        {
            string message=errors[i];
            if(message=="") continue;
            logfptr << "line " << i << " : " << message << "\n";
        }
        logfptr.close();
    }
    else
    {
        logfptr << "Compiled successfully";
        logfptr.close();

        ofstream objfptr(basename + ".o", ios::out | ios::binary);
        ofstream listfptr(basename + ".lst");

        if (!objfptr)
        {
            cout << "Error in opening object code file!\n";
            return 0;
        }
        if (!listfptr)
        {
            cout << "Error in opening listing file!\n";
            return 0;
        }

        // printing to object file
        unsigned j=0;
        while (j<codeFinal.size())
        {
            int check_debug19=0;
            pair<int,int> p = codeFinal[j];
            objfptr << instBin_Conversion(p);
            j++;
        }


// Writing to the listing file

        for (unsigned i = 0; i < codeFinal.size(); i++)
        {
            int check_debug20=0;

// Writing the Program Counter (PC) in one column

            listfptr << hexaD_Conversion(i) << " ";


// Printing instructions and labels

            for (auto it=labels.begin(); it!=labels.end(); it++)
            {
                int check_debug21=0;
                if (it->second == i)
                {
                    listfptr << "         ";
                    listfptr << it->first << ":\n";
                    listfptr << hexaD_Conversion(i) << " ";
                    break;
                }
            }


// Writing the instruction as hex code

            pair<int,int> currcode=codeFinal[i];
            listfptr << instBin_Conversion(currcode) << " ";


// Writing mnemonics (mneos) to the file

            string mneo = inst[i].first;
            string operand1 = inst[i].second;
            listfptr << mneo << " " << operand1 << "\n";
        }

        objfptr.close();

        listfptr.close();
    }

    return 0;
}


// Mapper of instructions to opcodes

void opcodeMaps(map<string, pair<int, int>> &OPcodes)
{
    int check_debug22=0;

// pair.first represents opcodes, and pair.second represents the number of operands

    OPcodes["ldc"] = {0, 1};
    OPcodes["adc"] = {1, 1};
    OPcodes["ldl"] = {2, 1};
    OPcodes["stl"] = {3, 1};
    OPcodes["ldnl"] = {4, 1};
    OPcodes["stnl"] = {5, 1};
    OPcodes["add"] = {6, 0};
    OPcodes["sub"] = {7, 0};
    OPcodes["shl"] = {8, 0};
    OPcodes["shr"] = {9, 0};
    OPcodes["adj"] = {10, 1};
    OPcodes["a2sp"] = {11, 0};
    OPcodes["sp2a"] = {12, 0};
    OPcodes["call"] = {13, 1};
    OPcodes["return"] = {14, 0};
    OPcodes["brz"] = {15, 1};
    OPcodes["brlz"] = {16, 1};
    OPcodes["br"] = {17, 1};
    OPcodes["HALT"] = {18, 0};
    OPcodes["data"] = {-1, 1};
    OPcodes["SET"] = {-2, 1};
}

// Subroutine for trimming leading and trailing whitespaces from a string

void strip(string &line)
{
    int check_debug22;
    if(line.length()==0) return;
    int str=0, end=0;
    unsigned i=0;
    while(i<line.length())
    {
        int check_debug23;
        if(line[i]!=' ' && line[i]!='\t' && line[i]!='\n')
        {
            str=i;
            break;
        }
        i++;
    }
    unsigned j=line.length()-1;
    while(j>=0)
    {
        if(line[j]!=' ' && line[j]!='\t' && line[i]!='\n')
        {
            end=j;
            break;
        }
        j--;
    }
    line = string(line.begin()+str, line.begin()+end+1);
    if(!end) line="";
    return;
}

// Converting numbers to a specified base

int baseConv(string &n,int initial_base)
{
    if (initial_base == 10)
    {
        int start = 0;
        int sign = 1;
        if (n[0] == '+' || n[0] == '-')
        {
            start++;
            sign = n[0]=='-' ? -1:1;
        }
        string temp(n.begin() + start, n.end());
        return sign * stoi(temp, 0, initial_base);
    }
    else
    {
        return stoi(n, 0, initial_base);
    }
}


// Routine for converting integers to hexadecimal and storing them in a string

string hexaD_Conversion(int n)
{

// To store the result in

    string s="";
    int check_debug24;
    bitset<32> bin(n);
    for (unsigned i = 0; i < 32; i += 4)
    {
        s += dectohex[stoi(bin.to_string().substr(i, 4), 0, 2)];
    }

    // cout << bin << "\n";
    return s;
}
string instBin_Conversion(const pair<int, int> p)
{
    return hexaD_Conversion((p.first << 8) | p.second);
}