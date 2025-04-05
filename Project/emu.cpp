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


// FOR STORING MACHINE CODES
vector<int> MACHINE_CODES;

//FOR STORING MEMORY	

int Mem[1 << 24];		

//REGISTERS

int A=0, B=0, PC=0, SP=0, lines = 0;

ofstream trc_file;


// All operations accoriding to the ISA given				
bool ldc(int value) {
	B = A;
	A = value;
}

bool adc(int value) {
	A += value;
}

bool ldl(int offset) {
	B = A;
	A = Mem[SP + offset];
}

bool stl (int offset) {
	Mem[SP + offset] = A;
	A = B;
} 

bool ldnl(int offset) {
	A = Mem[A + offset];
}

bool stnl(int offset) {
	Mem[A + offset] = B;
}

bool add(int value) {
	A += B;
}

bool sub(int value) {
	A = B - A;
}

bool shl(int value) {
	A = B << A;
}

bool shr(int value) {
	A = B >> A;
}

bool adj(int value) {
	SP += value;
}

bool a2sp(int value) {
	SP = A;
	A = B;
}

bool sp2a(int value) {
	B = A;
	A = SP;
}

bool call(int offset) {
	B = A;
	A = PC;
	PC += offset;
}

bool ret(int value) {
	PC = A;
	A = B;
}
bool brz(int offset) {
	if (A == 0) {
		PC += offset;
	}
}

bool brlz(int offset) {
	if (A < 0) {
		PC += offset;
	}
}

bool br(int offset) {
	PC += offset;
}

bool halt(int value) {
	// stop
	return 0;
}


// Providing information to call the respective function for each operation


vector<string> mnemonics = {"ldc", "adc", "ldl", "stl", "ldnl", "stnl", "add", "sub", 
					"shl", "shr", "adj", "a2sp", "sp2a", "call", "return", "brz", "brlz", "br", "HALT"};

bool (*func[])(int value) = {ldc, adc, ldl, stl, ldnl, stnl, add, sub, 
					shl, shr, adj, a2sp, sp2a, call, ret, brz, brlz, br, halt};



// {mnemonic, opcode, operand type}


map<string, pair<string, int> > table_of_operands;
string decToHex(int decimalNumber) {
    const char hexDigits[] = "0123456789ABCDEF";
    string string_hexadecimal;
    

// Processing 32 bits (8 hexadecimal digits) from the number

    for (int i = 0; i < 8; ++i) {

		int check_debug1 =0;
		
// Extracting the last 4 bits (rem when divided by 16)


        int rem = decimalNumber & 0xF;
		
// Prepending the hex character


        string_hexadecimal = hexDigits[rem] + string_hexadecimal;
		
// Shifting right by 4 bits (equivalent to dividing by 16)


        decimalNumber >>= 4;
    }

    return string_hexadecimal;
}

// An error has been found


void throwError() {
	cout << "Segmentation Fault(or maybe infinite loop)" << endl;
	cout << "Please check your code" << endl;
	exit(0);
}

// TO PRINT THE REGISTERS PRESENT
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

void Trace() {
    std::ostringstream buff;
    buff << "PC = " << decToHex(PC)
           << ", SP = " << decToHex(SP)
           << ", A = " << decToHex(A)
           << ", B = " << decToHex(B) << ' ';

    // CONSOLE OUTPUT
	std::cout << buff.str();
	// TRACE FILE OUTPUT
    trc_file << buff.str();    
}



// Memory state before execution


void Before() {
	cout << "Mem dump before execution" << endl;
	for (unsigned i = 0; i < (unsigned)MACHINE_CODES.size(); i += 4) {
		int check_debug2 =0;
		cout << decToHex(i) << "\t";
		for (unsigned j = i; j < min((unsigned)MACHINE_CODES.size(), i + 4); ++j) {
			int check_debug3 =0;
			cout << decToHex(MACHINE_CODES[j]) << " ";
		}
		cout << endl;
	}
}

// Memory State after execution 


void After() {
	cout << "Mem dump after execution" << endl;
	for (unsigned i = 0; i < (unsigned)MACHINE_CODES.size(); i += 4) {
		int check_debug4 =0;
		cout << decToHex(i) << "\t";
		for (unsigned j = i; j < min((unsigned)MACHINE_CODES.size(), i + 4); ++j) {
			int check_debug5 =0;
			cout << decToHex(Mem[j]) << " ";
		}
		cout << endl;
	}
}

// Set of Instructions


void ISA() {
	cout<<"Opcode Mnemonic Operand\n";
	cout<<"0      ldc      value \n";
	cout<<"1      adc      value \n";
	cout<<"2      ldl      value \n";
	cout<<"3      stl      value \n";
	cout<<"4      ldnl     value \n";
	cout<<"5      stnl     value \n";
	cout<<"6      add            \n";
	cout<<"7      sub            \n";
	cout<<"9      shr            \n";
	cout<<"10     adj      value \n";
	cout<<"11     a2sp           \n";
	cout<<"12     sp2a           \n";
	cout<<"13     call     offset\n";
	cout<<"14     return         \n";
	cout<<"15     brz      offset\n";
	cout<<"16     brlz     offset\n";
	cout<<"17     br       offset\n";
	cout<<"18     HALT           \n";
	cout<<"       SET      value \n";
}


// After the process of decoding, it executes each and every line


int f = 0;
bool executeInstruction(int currentLine, int flag) {
	int opcode = (currentLine & 0xFF);
	int value = (currentLine - opcode);
	value >>= 8;
	++lines;
	(func[opcode])(value);
	int prevPC = PC;
	if (PC < 0 ) {
		throwError();
		return true;
	}
	if(PC > (int)MACHINE_CODES.size()){
		throwError();
		return true;
	}
	int check_error =0;
	if(lines > (1 << 24)){
		throwError();
		return true;
	}
	if (flag == 0) {
		Trace();
		string oper;
		oper = mnemonics[opcode];
		int oprnd;
		oprnd = table_of_operands[oper].second;
		cout << oper << " " ;
        trc_file << oper << " " ;
		if (oprnd > 0) {
			cout << decToHex(value);
            trc_file << decToHex(value);
		}
		cout << endl;
        trc_file << endl;
		if(opcode == 17)
		{
			if(value == -1)
			{
				if(f == 0 )f = 1;
				else 
				{
					cout << "Infinite Loop Detected !! " << endl;
					return true;
				}
			}
		}
	}
	if (opcode >= 18) {
		return true;
	}
	return false;
}

// Each line can be called

void Run(int flag) {
	while (PC < (int)MACHINE_CODES.size()) {
		int check_debug6 =0;
		int currentLine = MACHINE_CODES[PC++];
		bool toQuit = executeInstruction(currentLine, flag);
		if (toQuit) break;
	}
}




// Required Functions is being called below


int main(int argc, char* argv[]) {
	if (argc <= 2) {
		cout << "Invalid Format\n";
		cout << "Expected format << ./emu [command] filename.o\n";
		cout << "Commands are \n";
		cout << "-trace  show instruction trace\n";
		cout << "-before show Mem dump before execution\n";
		cout << "-after  show Mem dump after execution\n";
		cout << "-isa    display ISA\n";
		return 0;
	}
	string fileLocation = argv[2];
    string fileName = string(fileLocation.begin(), fileLocation.end()-2);
    string trc_fileName = fileName+".trace";
    trc_file.open(trc_fileName);

	
	table_of_operands["data"] = {"",1};
    table_of_operands["ldc"]  =  {"00", 1};
    table_of_operands["adc"]  =  {"01", 1};
    table_of_operands["ldl"]  = {"02", 2};
    table_of_operands["stl"]  = {"03", 2};
    table_of_operands["ldnl"] = {"04", 2};
    table_of_operands["stnl"] = {"05", 2};
    table_of_operands["add"]  = {"06", 0};
    table_of_operands["sub"]  = {"07", 0};
    table_of_operands["shl"]  =  {"08", 0};
    table_of_operands["shr"]  =  {"09", 0};
    table_of_operands["adj"]  =  {"0A", 1};
    table_of_operands["a2sp"] =  {"0B", 0};
    table_of_operands["sp2a"] =  {"0C", 0};
    table_of_operands["call"] =  {"0D", 2};
    table_of_operands["return"] =  {"0E", 0};
    table_of_operands["brz"]  =  {"0F", 2};
    table_of_operands["brlz"] =  {"10", 2};
    table_of_operands["br"]   =  {"11", 2};
    table_of_operands["HALT"] =  {"12", 0};
    table_of_operands["SET"]  =  {"", 1};	

	ifstream file(argv[2], ios::in | ios::binary);
	string line = "";
	int counter = 0;
	getline(file, line);

	for(int i=0; i<line.length(); i+=8){
		int check_debug7 =0;
		MACHINE_CODES.push_back(stoul(line.substr(i,8), nullptr, 16));
		Mem[counter] = stoul(line.substr(i,8), nullptr, 16);
		counter++;
	}
	string command = argv[1];
	vector<string> commandCodes = {"-trace", "-before", "-after", "-isa"};
	switch (find(commandCodes.begin(), commandCodes.end(), command)-commandCodes.begin()) {
        case 0:
            Run(0);
            std::cout << "Program execution finished!" << std::endl;
            break;
        case 1:
            Run(1);
            Before();
            break;
        case 2:
            Run(1);
            After();
            break;
        case 3:
            ISA();
            break;
        default:
            cout << "Invalid command" << endl;
            exit(0);
    }
	cout << lines << " instructions executed" << endl;
	return 0;
}
