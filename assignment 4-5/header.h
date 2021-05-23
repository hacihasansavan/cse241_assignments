#ifndef _HACI_HASAN_SAVAN_1901042704_H
#define _HACI_HASAN_SAVAN_1901042704_H
#include <iostream>

using namespace std;



class Gate
{
public:
	Gate();
	Gate(int a, const string& n): out(a), name(n){/**/}
	string getName()const;
	virtual void setName(const string& n); 
	virtual string getInput2()const;
	virtual string getInput1()const;
	virtual int eval(); 
	virtual void setOut(int o);
	int get_out();
	void setPoint1(Gate* p1){inp1 = p1; /*cout<<inp1<<endl;*/} 
	void setPoint2(Gate* p2){inp2 = p2; /*cout<<inp2<<endl;*/} 
	Gate* get_point1(){return inp1;} 
	Gate* get_point2(){return inp2;} 
	void setCheck(int s);
	int getCheck();
protected:
	int check=0;
	int out;
	string name; /*input_name*/
	Gate* inp1;
	Gate* inp2;
};

class Output : public Gate
{
public:
	Output();  // Output(){}
	Output(const string& s1);
	virtual int eval(); 
};
class And : public Gate
{
public:
	And();  
	And(const string& s1, const string& s2, const string& s3)
		: Gate(0,s1), input1(s2), input2(s3) {/*-------*/}
	virtual int eval();
	virtual string getName()const{return name;}
	virtual string getInput1()const;  
	virtual string getInput2()const;  
	virtual void setInput1(const string& n);  
	virtual void setInput2(const string& n);  

private:
	string input1;
	string input2;
};



class Or : public Gate
{
public:
	Or();
	Or(const string& s1, const string& s2, const string& s3)
		: Gate(0,s1), input1(s2), input2(s3) {/*-------*/}
	virtual int eval();
	virtual string getInput1()const;  
	virtual string getInput2()const;  
	virtual void setInput1(const string& n); 
	virtual void setInput2(const string& n); 

private:
	string input1;
	string input2;
};


class Not : public Gate
{
public:
	Not();
	Not(const string& s1, const string& s2)
		: Gate(0,s1), input1(s2) {/*-------*/}
	virtual int eval();
	virtual string getInput2()const;  
	virtual string getInput1()const;  //return name;
	virtual void setInput1(const string& n);  //return name;

private:
	string input1;
};

class FlipFlop : public Gate
{
public:
	FlipFlop();
	FlipFlop(const string& s1, const string& s2);
	~FlipFlop();
	virtual int eval();
	virtual string getInput1()const;  
	virtual string getInput2()const;  
	virtual void setInput1(const string& n); 

private:
	string input1;
	static int former_out;
};


int WhiteSpaceCounter(const string& s);
typedef Gate* gatePtr;
void counter(int& inp, int& out, int& gates, const string& file);
Gate** createGatePointer(int& inp, int& gates, int& out);
Gate** readCircuit_txt(const string& circuit_txt, Gate** gateP);
void fillInputValues(Gate** gp, const string& input_txt,int inpCount,int size);
void simulateCircuit(const string& circuit_txt, const string& input_txt);

#endif