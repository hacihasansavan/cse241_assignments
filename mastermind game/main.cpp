/*
*__________________________________ MASTERMIND GAME _____________________________|
*_________________Object-Oriented-Programming with C++ Language HW-1 ____________|
*_____________________________ Created by Hacı Hasan Savan ______________________|
*___________________________________ 1901042704 _________________________________|
*___________________________________ 11.03.2021 _________________________________|
*_____________________________ Compiler: g++ -std=c++11 _________________________|
*/

#include <iostream> 
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std; 
 
bool checkLengthsAreOk(const string, const int);
bool checkIfItIsDigitNum(const string);
string generateSecretNum(int x);
bool compareTheNums(string secretNum, string estimation);
bool isContainSameDigit(string s,int x);
string removeFirstDigitZero(string s);
bool isErrorExist(const int _argc,char** _argv);
/*
*argv[0]: ./mastermind
*argv[1]: -r or -u
*argv[2]: digit size like 1 ... 6,7..9
*/
int counter = 0; //counter for iterations
const int __MAX__ = 100; //max iteration number

int main(int argc, char** argv) 
{ 
	srand(time(NULL));
	/*instead of working with argv the veriables are easier*/
	int digit_size; //if option is -r fills digit_size; if option is -U fills secret_num
	string gameOption; //-r or -u
	string prediction; //prediction of user
	string secret_num; //secret_num that created by computer or entered by user
	bool thereIsError = false; // for controling the while loop

	thereIsError = isErrorExist(argc,argv); //if an error exist returns true
	if(thereIsError)
		return 1;
	gameOption = argv[1];

	if(gameOption == "-r"){	//not secret num is entered but length of that is entered
		digit_size = argv[2][0]-'0'; //char-to-int
		secret_num = generateSecretNum(digit_size);
	}
	else if(gameOption == "-u"){ //secret_num is entered by user!
		secret_num = (string)argv[2];
		digit_size = ((string)argv[2]).length();
		thereIsError = isContainSameDigit(argv[2],1);

	} 			

	//cout<<"secret num: "<<secret_num<<endl; //for controlling
	if(thereIsError)
		return 1;
	thereIsError = checkIfItIsDigitNum(argv[2]); //for controlling the main's arguments for the first time
	
	while(!thereIsError) //while there isn't any error
	{
		cin>>prediction;
		if(prediction[0]=='0') //if user entered a number start with 0 
			prediction = removeFirstDigitZero(prediction);

		if(isContainSameDigit(prediction,2))
			break;
		if(checkIfItIsDigitNum(prediction))//if an error exist returns true
			break;
      	if(checkLengthsAreOk(prediction,digit_size))//if an error exist returns true
			break;
      	cin.ignore(10000,'\n'); //to clear the stdin
		cin.clear();			//" 	"	 "
		counter++;
		thereIsError = compareTheNums(secret_num,prediction);
		if(counter==__MAX__)
		{
			cout<<"FAILED";
			break;
		}
	}

    return 0; 
}

/*
*precondition: takes two string one is secret_num other is user estimation
*postcondition: return true if the given string nums are equal
*compares two num which are string if they are equal return true otherwise 
returns false and prints the hint

*/
bool compareTheNums(string secretNum, string estimation)
{
	int leftHint=0, rigthHint=0; // like in example: 4 2 
	int temp;
	bool _r = false;
	if(estimation.compare(secretNum) ==0) //if they are equal
	{
		cout<<"FOUND"<<" "<<counter;
		_r = true;
	}
	else
	{
		for(int i=0; i<secretNum.length(); i++)
		{
			if(secretNum[i]==estimation[i])
				leftHint++;
			else{
				temp = estimation[i];
				//this for loop looks all numbers in string for each estimation member
				for(int k=0; k<secretNum.length(); k++)
					if(temp==secretNum[k])
						rigthHint++;
			}
		}
		cout<<leftHint<<" "<<rigthHint<<endl;
	}
	return _r;
}
/*
*precondition: takes mains arguments (argc and argv)
*postcondition: Controls the initial values which are entered with progrom calling.
 if there is an unexpected value or missing value or something wrong, returns true
 else returns false. And also doesn't change the given values.
*/
bool isErrorExist(const int _argc, char** _argv) //if there is an error returns true
{
	bool r_value = false;
	/*we don't want to take extra or less arguments*/

	if(_argc!=3){ //argument numbers must be 3
		cout<<"E0";
		return true;
	}
	else if((string)_argv[1]=="-u" && _argv[2][0] == '0')
	{
		cout<<"E0";
		r_value = true;
	}
	//digit cannot be a zero or negatif value otherwise terminate!
	else if((string)_argv[1]=="-r")
	{
		if(!isdigit(_argv[2][0]))
		{
			cout<<"E0";
			r_value = true;
		}
		else if(_argv[2][0]-'0'<=0 || ((string)_argv[2]).length()>1) 
		{
			cout<<"E0";
			r_value = true;
		}
	}
	
	//the name of game is mastermind. otherwise terminate!
	else if((string)_argv[0]!="./mastermind")  
	{
		cout<<"E0";
		r_value = true;
	}
	//there are only 2 option for playing game otherwise terminate!
	else if((string)_argv[1]!="-r" && (string)_argv[1]!="-u") 
	{
		cout<<"E0";
		r_value = true;
	}
	
	return r_value;

}
/*
*precondition: takes an int which is length of the requested secret num
*postcondition: creates and sends a secret number in string format
*in each iteration creates only one number and add that up to the string
*/
string generateSecretNum(int x)
{
	string s;
	int randNum;
	string tempString;
	bool isUsedBefore = false;

	randNum = rand()%9+1; //1....9 the first number should not be 0
	tempString= to_string(randNum);
	s+=tempString;
	if(x==1) //if length of the expected num is 1 
		return s;
	//in the 
	while(1)
	{
		randNum = rand()%10; //0....9. creates only one number in each iteration
		tempString = to_string(randNum); //change int to a tempstring 
		//this for loop checks if the just created number is used before 
		for(int i=0; i<s.length(); i++) 
		{
			if(s[i] == tempString[0]) //if this number is used before, exit from loop 
			{
				isUsedBefore = true;
				break;
			}
		}
		if(!isUsedBefore) //else add that up to the string
			s+=tempString;
		else isUsedBefore = false;
		
		if(s.length()==x) //is expected length is reached exit from while
			break;
	}
	return s;
}

/*
*preconditon: takes a string which is the user input 
*postcondition: returns true if it's not a digit num
 does nothing otherwise
*/
bool checkIfItIsDigitNum(const string s)
{
	bool _r = false;
	if(s[0]=='-')	//if the entered number is negative
	{
		cout<<"E0";
		return true;
	}
	for(int i=0; i<s.length(); i++)
	{
		if(!isdigit(s[i])) //if it's not a digit
		{
			cout<<"E2";
			_r = true;
			break;
		}
	}
	return _r;
}

/*
*preconditon: Takes a string and int (expected length)
*postcondition: returns true if the expected length and 
 string length are not equals does nothing otherwise
*/
bool checkLengthsAreOk(const string s, const int x)
{
	bool _r = false;
	if(s[0]=='0') //if first digit is 0 there is a problem there
	{
		if(s.length()-1 != x) //0123 is a number which has 3 digit
		{
			cout<<"E1";
			_r = true;
		}

	}
	else if(s.string::length() != x)
	{
		cout<<"E1";
		_r = true;
	}
	return _r;
}
/*removes first digit if it's zero while playing in -r*/
string removeFirstDigitZero(string s)
{
	string temp;
	for(int i=1;i<s.length(); i++)
		temp+=s[i];
	if(temp[0]=='0')
		removeFirstDigitZero(temp);
	return temp;
}
/*
Checks if there are any same numbers. If returns true this means yes
*/ 
bool isContainSameDigit(string s, int x)
{
	if(s.length()==0)
		return false;
	for(int i =0; i<s.length(); i++)
	{
		for(int k=i+1; k<s.length(); k++)
		{
			if(s[i]==s[k]){
				if(x==1)
					cout<<"E0";
				else cout<<"E1";
				return true;
			}
		}
	}
	return false;
}