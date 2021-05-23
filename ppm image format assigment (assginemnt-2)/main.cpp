
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

//max resolutions 1920X1080 
const int _MAX_X_ = 1920;
const int _MAX_Y_ = 1080;

/*this class stores rgb values of one pixel only */
class Pixels{

public:
	Pixels()
		: r(0), g(0), b(0)
	{/*intentionally left blank*/}
	int getR() const {return r;}
	int getG() const {return g;}
	int getB() const {return b;}

	void setR(const int _r) {r = _r;}
	void setG(const int _g) {g = _g;}
	void setB(const int _b) {b = _b;}
private:
	int r;
	int g;
	int b;

};

//stores the ppm image informations
//and ppm funciton
class PpmImage{
public:
	PpmImage() 
	{/*intentionally left blank*/}
	PpmImage(const string _fileName_)
		: fileName(_fileName_)
		{/*intentionally left blank*/}

	PpmImage(const int _dX, const int _dY) 
		: dX(_dX), dY(_dY)
		{/*intentionally left blank*/}

	int pixelInfo(const int x, const int y, const int code) const; //returns rgb values according to the parameters ('r','g','b') !!!
	bool savePpmToFile() const; //saves the current object to the file
	void readPpmFromFile(const string file);	//reads a ppm file and create (maybe) a object
	void printDimensions() const {cout<<dX<<"x"<<dY<<endl;} //prints the dimensions to the screen eg:4X4
	void printPixel(const int _x, const int _y); //prints the specified pixel to screen
	void modifyPixel(const int _x, const int _y, const int r, const int g, const int b);
	void initializePixelVector();
	void resizeVector();
	
	/*getters*/
	vector<vector<Pixels> >& getVector() {return pixelVector;}
	vector<vector<Pixels> > const& getVector()const {return pixelVector;}
	string getVersion()const {return version;}
	int getMax()const {return max;}
	int getdX()const {return dX;}
	int getdY()const {return dY;}

	/*setters*/
	void setVersion(const string& s) {version = s;}
	void setMax(const int x) {max = x;}
	void setdX(const int x) {dX = x;}
	void setdY(const int y) {dY = y;}

private:
	vector <vector <Pixels> >  pixelVector;
	string fileName;
	int dX; //dimension x
	int dY;	//dimension y
	int pixelCount; //stores how many pixels there are 
	string version = "P3";
	int max = 255;

};
int read_ppm(const string source_ppm_file_name, PpmImage& destination_object);
int swap_channels(PpmImage& image_object_to_be_modified, int swap_choice);

//modifies only one specified pixel
void PpmImage::modifyPixel(const int _x, const int _y, const int r, const int g, const int b)
{
	pixelVector[_x][_y].setR(r);
	pixelVector[_x][_y].setG(g);
	pixelVector[_x][_y].setB(b);
}

//prints the specified pixel to screen
void PpmImage::printPixel(const int _x, const int _y) 
{
	cout<<pixelVector[_x][_y].getR()<<" "
	<<pixelVector[_x][_y].getG()<<" "<<pixelVector[_x][_y].getB()<<endl;
}

/*
 * A member function which reads given file and fills the object veriables
 * If there is wrong with file format it will writes the console warning and
 * will do nothing
*/
void PpmImage::readPpmFromFile(const string file)	//reads a ppm file and fills the object itself
{
	ifstream fin;
	fin.open(file);
	string temp;
	int d1,d2,maxColor;
	int _r, _g, _b;
	bool fileFormatIsTrue = true;
	if(fin.is_open())
	{
		fin>>temp;
		if(temp =="P3") //we'll use this format in this project
			version = temp;
		else fileFormatIsTrue = false; 
		
		fin>>d1>>d2; //taking the dimensions
		if(d1 <=_MAX_X_ && d2<=_MAX_Y_ && fileFormatIsTrue==true)
		{
			dX = d1;
			dY = d2;
		}
		else fileFormatIsTrue = false;
		fin>>maxColor; //taking max rgb values
		if(maxColor<=255 && fileFormatIsTrue == true)
			max = maxColor;
		else fileFormatIsTrue = false;

		if(fileFormatIsTrue) //resize the vector acording to the new sizes
			resizeVector();
		else cout<<"file format is not true!! please check it"<<endl;

		if(fileFormatIsTrue)
		{
			for(int i=0;i<dY; i++)
			{
				for(int k =0; k<dX; k++)
				{
					fin>>_r>>_g>>_b;
					if( _r>=0 && _r<=255 && 	//if rgb values are proper and there is not 
						_g>=0 && _g<=255 &&		// a problem with other stuff in file
						_b>=0 && _b<=255 &&
					 	fileFormatIsTrue==true )
					{
						pixelVector[i][k].setR(_r);
						pixelVector[i][k].setG(_g);
						pixelVector[i][k].setB(_b);
					}
					else
					{
						fileFormatIsTrue = false;
						cout<<"the rgb values improper!! please check it!!"<<endl;
						break;
					}
				}
				if(!fileFormatIsTrue)
					break;
			}

		}
		fin.close();
	}
	else
	{
		cerr<<"error while opening the file !!"<<endl;
		fileFormatIsTrue = false;	
	}  
}
/*
*it basically takes parameters and
 according to that parameter, it returns red, green or
 blue value of that particular pixel)
 */
int PpmImage::pixelInfo(const int x, const int y, const int code) const
{
	int r_value = -1;
	if(code == 1) //red
		r_value = pixelVector[y][x].getR();
	else if(code == 2) //green
		r_value =  pixelVector[y][x].getG();
	else if(code == 3)//blue
		r_value =  pixelVector[y][x].getB();
	else cerr<<"unexpected color code!!"<<endl;
	return r_value;
}

/*
	intializes the pixel vector and fills it with 255 (white)
*/
void PpmImage::initializePixelVector()
{
	resizeVector(); //resize vector with class's dimenison
	for(int i=0; i<pixelVector.size(); i++)
	{
		for(int k=0; k < pixelVector[i].size(); k++)
		{
			pixelVector[i][k].setR(255 );
			pixelVector[i][k].setG(255 );
			pixelVector[i][k].setB(255 );
		}
	}
		
}

void PpmImage::resizeVector()
{
	pixelVector.resize(dY);
	for(int i=0; i<pixelVector.size(); i++)
		pixelVector[i].resize(dX);
}

//returns true if it's accomplished. else false
//saves the current object to the file
bool PpmImage::savePpmToFile() const 
{
	bool r_value = true;
	ofstream fout;
	fout.open(fileName);

	if(fout.is_open())
	{
		fout<<version<<endl;
		fout<<dX<<" "<<dY<<endl;
		fout<<max<<endl;

		for(int i=0; i<pixelVector.size(); i++)
		{
			for(int k=0; k<pixelVector[i].size(); k++)
			{
				fout<<pixelVector[i][k].getR()<<" ";
				fout<<pixelVector[i][k].getG()<<" ";
				fout<<pixelVector[i][k].getB()<<" ";
				fout<<"   ";
			}
			fout<<endl;
		}
	}
	else
	{
		r_value = false;
		cerr<<"Failed while saving the file!!"<<endl;
	}
	fout.close();

	return r_value;
}


// returns 1 if the operation is successful. otherwise, returns 0.
// reads ppm data from file named as source_ppm_file_name. stores data in destination_object
// which is already created outside of the function.

int read_ppm(const string source_ppm_file_name, PpmImage& destination_object)
{
	ifstream fin;
	fin.open(source_ppm_file_name);
	string temp;
	int d1,d2,maxColor;
	vector<vector<Pixels> >& pV = destination_object.getVector();
	int _r, _g, _b;//, i =0, k =0,m =0;
	bool fileFormatIsTrue = true;
	if(fin.is_open())
	{
		fin>>temp; //taking format
		if(temp =="P3") //we'll use this format in this project
			destination_object.setVersion(temp);
		else fileFormatIsTrue = false; 
		
		fin>>d1>>d2; //taking dimesions
		if(d1 <=_MAX_X_ && d2<=_MAX_Y_ && fileFormatIsTrue==true)
		{
			destination_object.setdX(d1);
			destination_object.setdY(d2);
		}
		else fileFormatIsTrue = false;
		fin>>maxColor; //taking max rgb value
		if(maxColor<=255 && fileFormatIsTrue == true)
			destination_object.setMax(maxColor);
		else fileFormatIsTrue = false;

		if(fileFormatIsTrue) //resizeing of the vector
			destination_object.resizeVector(); 
		else cout<<"file format is not true!! please check it"<<endl;

		if(fileFormatIsTrue)
		{

			for(int i=0;i<destination_object.getdY(); i++)
			{
				for(int k =0; k<destination_object.getdX(); k++)
				{
					fin>>_r>>_g>>_b;
					if( _r>=0 && _r<=255 && 	//if rgb values are proper and there is not 
						_g>=0 && _g<=255 &&		// a problem with other stuff in file
						_b>=0 && _b<=255 &&
					 	fileFormatIsTrue==true )
					{
						pV[i][k].setR(_r);
						pV[i][k].setG(_g);
						pV[i][k].setB(_b);
					}
					else //the rg values are not in expected boundry (0-255) terminate!
					{
						fileFormatIsTrue = false;
						cout<<"the rgb values improper!! please check it!!"<<endl;
						break;
					}
				}
				if(!fileFormatIsTrue)
					break;
			}
		}

		fin.close();
	}
	else
	{
		cerr<<"error while opening the file !!"<<endl;
		fileFormatIsTrue = false;	
	}   
	if(!fileFormatIsTrue)
		return 0;
	else return 1;

}

// returns 1 if the operation is successful. otherwise, returns 0.
// writes ppm data from source_object to the file named destination_ppm_file_name.
int write_ppm(const string destination_ppm_file_name,  const PpmImage& source_object)
{
	bool r_value = true;
	ofstream fout;
	fout.open(destination_ppm_file_name);
	vector<vector<Pixels> > const &pV = source_object.getVector();

	if(fout.is_open())
	{
		fout<<source_object.getVersion()<<endl; //P3
		fout<<source_object.getdX()<<" "<<source_object.getdY()<<endl; // dx dy
		fout<<source_object.getMax()<<endl; //255

		//pixel rgb values
		for(int i=0; i<pV.size(); i++) 
		{
			for(int k=0; k<pV[i].size(); k++)
			{
				fout<<pV[i][k].getR()<<" ";
				fout<<pV[i][k].getG()<<" ";
				fout<<pV[i][k].getB()<<" ";
				fout<<"   ";
			}
			fout<<endl;
		}
		fout.close();
	}
	else
	{
		r_value = false;
		cerr<<"Failed while saving the file!!"<<endl;
	}

	if(!r_value)
		return 0;
	else return 1;

}

// this function does not create a new object but modifies the given one.
// if swap_choice is 1: swaps red and green
// if swap_choice is 2: swaps red and blue
// if swap_choice is 3: swaps green and blue
// if swap_choice is not 1, 2 or 3: no swaps (this does not mean that the operation is not
// successful. the function should return 1 in this case if everything is normal)
// returns 1 if the operation is successful. otherwise, returns 0.
int swap_channels(PpmImage& image_object_to_be_modified, int swap_choice)
{
	vector<vector<Pixels> >& pV = image_object_to_be_modified.getVector();
	int temp;
	int r_value = 1;

	if(swap_choice==1 ||swap_choice==2 || swap_choice==3 )
	{
		for(int i =0; i<pV.size(); i++)
		{
			for(int k = 0; k<pV[i].size(); k++)
			{
				if(1==swap_choice) // if swap_choice is 1: swaps red and green
				{
					temp = pV[i][k].getR();
					pV[i][k].setR(pV[i][k].getG());
					pV[i][k].setG(temp);
				}
				else if(2==swap_choice)// if swap_choice is 2: swaps red and blue
				{
					temp = pV[i][k].getR();
					pV[i][k].setR(pV[i][k].getB());
					pV[i][k].setB(temp);
				}

				else if(3==swap_choice)// if swap_choice is 3: swaps green and blue
				{
					temp = pV[i][k].getB();
					pV[i][k].setB(pV[i][k].getG());
					pV[i][k].setG(temp);
				}
			}
		}
	}
	else
	{
		r_value = 0;
		cerr<<"invalid swap choice !!"<<endl;	
	} 
	return r_value;
}

// creates and returns a copy of a new ppmImage object which stores only one color at each
//pixel. This simply takes the source and copies only one color information and stores it in
//a new object. The other color channels are simply going to be zeros.
//if color_choice is 1: red channel is preserved
//if color_choice is 2: green channel is preserved
//if color_choice is 3: blue channel is preserved
PpmImage single_color(const PpmImage& source, int color_choice)
{
	PpmImage copy;
	int x,y;
	copy.setVersion(source.getVersion());
	copy.setdX(source.getdX());
	copy.setdY(source.getdY());
	copy.setMax(source.getMax());

	copy.resizeVector(); //resizes vector with dX and dY
	vector<vector<Pixels>>& pV = copy.getVector(); 
	for(int i = 0; i<copy.getdY(); i++)
	{
		for(int k=0; k<copy.getdX(); k++)
		{
			pV[i][k].setR(0);
			pV[i][k].setG(0);
			pV[i][k].setB(0);
			if(1==color_choice)
				pV[i][k].setR(source.pixelInfo(i,k,1));
			if(2==color_choice)
				pV[i][k].setG(source.pixelInfo(i,k,2));
			else if(3==color_choice)
				pV[i][k].setB(source.pixelInfo(i,k,3));
		}
	}

	return copy;
}


int main(int argc, char const *argv[])
{

	int choice;
	string ppmFile;
	PpmImage obj;
	string output;
	bool ArgumentsAreOke = false;

	if(argc == 3){
		choice = stoi(argv[1]);	  
		ppmFile = argv[2];	
		ArgumentsAreOke = true;
	}
	else
	{
		cerr << "Wrong call!" << endl;
		ArgumentsAreOke = false;
	}


	if(ArgumentsAreOke){

		if(choice==1)
		{
			read_ppm(ppmFile, obj);	
			output = "o1.ppm";	
			write_ppm(output, obj);
		}
		else if(choice==2)
		{
			read_ppm(ppmFile, obj);	
			swap_channels(obj, 1);
			output = "o2.ppm";
			write_ppm(output, obj);
		}
		else if(choice==3)
		{
			read_ppm(ppmFile, obj);	
			swap_channels(obj, 2);
			output = "o3.ppm";
			write_ppm(output, obj);
		}
		else if(choice==4)
		{
			read_ppm(ppmFile, obj);	
			swap_channels(obj, 3);
			output = "o4.ppm";
			write_ppm(output, obj);
		}
		else if(choice==5)
		{
			read_ppm(ppmFile, obj);	
			output = "o5.ppm";
			write_ppm(output, single_color(obj, 1));
		}
		else if(choice==6)
		{
			read_ppm(ppmFile, obj);	
			output = "o6.ppm";		
			write_ppm(output, single_color(obj, 2));
		}
		else if(choice==7)
		{
			read_ppm(ppmFile, obj);
			output = "o7.ppm";	
			write_ppm(output, single_color(obj, 3));
		}
		else cerr<<"invalid option!!"<<endl;
	}

	return 0;
}