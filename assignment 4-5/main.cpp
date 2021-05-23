#include <iostream>
#include <string>
#include "haci_hasan_savan_1901042704.h"
#include <fstream>

int main()
{
	simulateCircuit("circuit.txt","input.txt");

	return 0;
}

Gate::Gate(){}
string Gate::getName()const{return name;}
string Gate::getInput1()const{return "error";}  
void Gate::setName(const string& n){name = n;} 
string Gate::getInput2()const{return "error";}  
int Gate::eval(){return out;}
void Gate::setOut(int o){out = o;}
int Gate::get_out(){return out;}
void Gate::setCheck(int s){check = s;}
int Gate::getCheck(){return check;}

Output::Output(){}  
Output::Output(const string& s1)
		: Gate(0,s1) {/*-------*/}
int Output::eval(){out = inp1->eval(); return out;}   
 
And::And(){}
int And::eval(){return inp1->eval() && inp2->eval();}
string And::getInput1()const{return input1;} 
string And::getInput2()const{return input2;}  
void And::setInput1(const string& n){input1 = n;}  
void And::setInput2(const string& n){input2 = n;}  

Or::Or(){} 
int Or::eval(){return inp1->eval() || inp2->eval();}
string Or::getInput1()const{return input1;} 
string Or::getInput2()const{return input2;}  
void Or::setInput1(const string& n){input1 = n;}  
void Or::setInput2(const string& n){input2 = n;} 

Not::Not(){}  
int Not::eval(){return !inp1->eval();}
string Not::getInput2()const{return input1;}  
string Not::getInput1()const{return input1;} 
void Not::setInput1(const string& n){input1 = n;}  

FlipFlop::FlipFlop(){}
FlipFlop::FlipFlop(const string& s1, const string& s2)
		: Gate(0,s1), input1(s2) {/*-------*/}
int FlipFlop::eval()
{
	int r_value;

	if(inp1->eval() == former_out)
		r_value =0;
	else r_value = 1;

	if(FlipFlop::former_out==0)
		FlipFlop::former_out=1;
	else FlipFlop::former_out=0;
	out = r_value;
	return r_value;

}
string FlipFlop::getInput1()const{return input1;}  
string FlipFlop::getInput2()const{return input1;}  
void FlipFlop::setInput1(const string& n){input1 = n;}
int FlipFlop::former_out = 0;



//typedef Gate* gatePtr;
/*
	counts the whitespace number in a given string and returns it
	ex: "INPUT a b c d"			--> 4 whitespaces (4 object to be created)
	ex: "OUTPUT d1 d2 d3 d4"	--> 4 whitespaces (4 object to be created)
*/
int WhiteSpaceCounter(const string& s)
{
	int x = 0;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == ' ')
			x++;
	}
	return x;
}
/*counts input number, output number and gates number that is in the given file*/
void counter(int& inp, int& out, int& gates, const string& file)
{
	ifstream fin;
	fin.open(file);
	if (fin.is_open())
	{
		string tempLine,inpLine,outLine;

		getline(fin, inpLine);
		inp = WhiteSpaceCounter(inpLine);
		getline(fin, outLine);
		out = WhiteSpaceCounter(outLine);

		while (!fin.eof())
		{
			gates++;
			getline(fin, tempLine);
		}
		fin.close();
	}
	else cout << "error while openning the file! " << endl;
}


//creats gate pointer array: ||--a--|--b--|--c--|--d--|--and1--|--or1--|--n1--|.....||
Gate** createGatePointer(int& inp, int& gates, int& out)
{
	Gate** gateP = new gatePtr[inp + gates+out];
	for (int i = 0; i < inp + gates + out; i++)
		gateP[i] = nullptr;
		
	return gateP;
}
Gate** readCircuit_txt(const string& circuit_txt, Gate** gateP)
{
	ifstream fin;
	fin.open(circuit_txt);
	int i = 0;
	if (fin.is_open())
	{
		string temp,x,y,z;
		string temp2,tempstr;
		while (!fin.eof())
		{
			fin >> temp;
			if (temp == "INPUT" || temp == "OUTPUT" || temp == "AND" || temp == "OR" ||
				temp == "NOT" || temp == "FLIPFLOP" || temp == "DECODER")
				temp2 = temp;

			if (temp2 == "INPUT")
			{
				if (temp != "INPUT")
				{
					Gate* i1 = new Gate(0,temp);
					gateP[i] = i1;
					i++;
				}
			}
			else if (temp2 == "OUTPUT")
			{
				if (temp != "OUTPUT") 
				{
					Gate* i1 = new Gate(0,temp);
					gateP[i] = i1;
					gateP[i]->setCheck(1);
					i++;

				}
			}
			else if (temp2 == "AND")
			{
				fin >> x >> y >> z;

				And* i1 = new And(x,y,z);
				gateP[i] = i1;
				i++;
					//cout<<"i :"<<i<<endl;
			}
			else if (temp2 == "OR")
			{
				fin >> x >> y >> z;
				Or* i1 = new Or(x, y, z);
				gateP[i] = i1;
				i++;
			}
			else if (temp2 == "NOT")
			{
				fin >> x >> z;
				Not *i1 = new Not(x, z);
				gateP[i] = i1;
				i++;
			}
			else if (temp2 == "FLIPFLOP")
			{
				
				fin >> x >> z;
				FlipFlop *i1 = new FlipFlop(x, z);
				gateP[i] = i1;
				i++;
			}
			else if (temp2 == "DECODER")
			{
				cout << "gelecek" << endl;
			}

		}
		fin.close();
	}
	else cout << "error while opening the file (counter)" << endl;


	return gateP;
}

//fills the gate** pointer with input.txt file
void fillInputValues(Gate** gp, const string& input_txt,int inpCount,int size)
{
	ifstream fin;
	ofstream fout;
	fout.open("output.txt");
	fin.open(input_txt);
	int inpValue;
	int counter=0;


	if(fin.is_open() && fout.is_open())
	{
       
        /* Reading input.txt and assigning numbers to gp[i].out */
		while(!fin.eof())
		{            				
            for(int i=0; i<inpCount; i++)
            {   
            	fin >> inpValue;
                gp[i]->setOut(inpValue);
            }
         
			for(int k=0; k<size; k++)
			{	
				//gp[k]->eval();
				if(gp[k]->getCheck()==1)
				{
					//cout<<gp[k]->getName()<<" ";
					fout<< gp[k]->eval()<<endl;
				}

			}
		}

		fout.close();
		fin.close();


	}
}
void simulateCircuit(const string& circuit_txt, const string& input_txt)
{
//1-creating 1d gate pointer array

	//thesee int values needed for keeping the number information:
	int inpCount =0, outCount=0, gateCount=0;
	Gate** gp;
	counter(inpCount,outCount,gateCount,circuit_txt);//fills the veriables
	int size = inpCount+gateCount + outCount; 
	gp = createGatePointer(inpCount,gateCount,outCount);
//2-reading the circuit file and filling the gate**in poter
	gp = readCircuit_txt(circuit_txt,gp);

//3-connecting operation 

	for(int k=0; k <size; k++)
	{ 
	// If input1 or input2 is found, inp1 and inp2(struct pointers) holds the adress of that locations.
		for(int m=0; m < size; m++)
		{
			if(gp[m]->getName() == gp[k]->getInput1()) 
				gp[k]->setPoint1(gp[m]);   // Adress transfer

			if(gp[m]->getName() == gp[k]->getInput2()) 
				gp[k]->setPoint2(gp[m]);		
		}

	}
//4-filling input gates from input.txt

	fillInputValues(gp, input_txt, inpCount, size);


}


