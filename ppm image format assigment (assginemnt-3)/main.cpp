
//#################### Hacı Hasan Savan ###########################
//############### Gebze Technical University ######################
//###############  04/05/2021- Kahramanmaraş ######################
//####################### Homework-3 ##############################

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

//max resolutions 500X500  
const int _MAX_X_ = 500;
const int _MAX_Y_ = 500;

/*this class stores rgb values of one pixel only */
class Pixels{

public:
	Pixels()
		: r(0), g(0), b(0)
	{/*intentionally left blank*/}
	int getR() const {return r;}
	int getG() const {return g;}
	int getB() const {return b;}

	int& getR_ref() {return r;}
	int& getG_ref() {return g;}
	int& getB_ref() {return b;}
	
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
	/*Constructors*/
	PpmImage() 
	{/*intentionally left blank*/}
	PpmImage(const string _fileName_)
		: fileName(_fileName_)
		{/*intentionally left blank*/}

	PpmImage(const int _dX, const int _dY) 
		: dX(_dX), dY(_dY)
		{/*intentionally left blank*/}
	//member functions
	int pixelInfo(const int x, const int y, const int code) const; //returns rgb values according to the parameters ('r','g','b') !!!
	bool savePpmToFile() const; //saves the current object to the file
	void readPpmFromFile(const string file);	//reads a ppm file and create (maybe) a object
	void printDimensions() const {cout<<dX<<"x"<<dY<<endl;} //prints the dimensions to the screen eg:4X4
	void printPixel(const int _x, const int _y); //prints the specified pixel to screen
	void modifyPixel(const int _x, const int _y, const int r, const int g, const int b);
	void initializePixelVector();
	void resizeVector();
	
	//overloaded operators
	friend const PpmImage operator+(const PpmImage& left, const PpmImage& right);
	friend const PpmImage operator-(const PpmImage& left, const PpmImage& right);
	friend ostream& operator<<(ostream& out, const PpmImage& obj);
	int& operator()(int row, int col, int channel);

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
//hmw2 functions
int read_ppm(const string source_ppm_file_name, PpmImage& destination_object);
int swap_channels(PpmImage& image_object_to_be_modified, int swap_choice);	//re-implement with () operator
PpmImage single_color(const PpmImage& source, int color_choice);	//re-implement with () operator
int write_ppm(const string destination_ppm_file_name,  const PpmImage& source_object);

//hmw3 functions
int test_addition(const string filename_image1, const string filename_image2, const string filename_image3);
int test_subtraction(const string filename_image1, const string filename_image2, const string filename_image3);
int test_print(const string filename_image1);


// returns 1 if the operation is successful. otherwise, returns 0.
// reads images from filename_image1 and filename_image2.
// Adds them and saves the resulting image to filename_image3
int test_addition(const string filename_image1, const string filename_image2, const string filename_image3)
{
	int control = 1;
	PpmImage image1, image2, image3(0,0);
	//for avoidng segmentation fault in the case the sizes are not equal
	vector<vector<Pixels> >& s = image3.getVector();
	s.resize(1);

	control = read_ppm(filename_image1,image1);
	control = read_ppm(filename_image2,image2);
	if((image1.getdX() == image2.getdX()) && (image1.getdY() == image2.getdY()) )
		image3 = image1 + image2;

	control = write_ppm(filename_image3,image3);
	if(control==1)
		return 1;
	return 0;
}

// returns 1 if the operation is successful. otherwise, returns 0.
// reads images from filename_image1 and filename_image2. 
//Subtracts filename_image2 from filename_image1 saves the resulting image to filename_image3
int test_subtraction(const string filename_image1, const string filename_image2, const string filename_image3)
{
	int control = 1;
	PpmImage image1, image2, image3(0,0);
	//for avoidng segmentation fault in the case the sizes are not equal
	vector<vector<Pixels> >& s = image3.getVector();
	s.resize(1);

	control = read_ppm(filename_image1,image1);
	control = read_ppm(filename_image2,image2);
	if(image1.getdX() == image2.getdX() && image1.getdY() == image2.getdY() )
		image3 = image1 - image2;
	
	control = write_ppm(filename_image3,image3);
	if(control==1)
		return 1;
	return 0;
}

// returns 1 if the operation is successful. otherwise, returns 0.
// reads images from filename_image1 and prints it to stdout
int test_print(const string filename_image1)
{
	int control = 1;
	PpmImage obj;
	control = read_ppm(filename_image1, obj);
	cout<<obj;

	if(control==1)
		return 1;
	return 0;
}

/*
Function-call operator. This operator takes three parameters. 
The first parameter is the index of the row number. The second 
parameter is the index of the column number. The third parameter is the
color channel. It can either 1, 2 or 3. This operator returns a 
reference to the pixel value.
*/
int& PpmImage::operator()(int row, int col, int channel)
{	
	if(channel == 1)
		return pixelVector[row][col].getR_ref(); 
	else if(channel == 2)
		return pixelVector[row][col].getG_ref(); 
	else if (channel == 3)
		return pixelVector[row][col].getB_ref(); 
	return pixelVector[0][0].getR_ref(); //we assume that there is no choice other than 1, 2,3
	//for that reason just returns something
}

//Prints image details and image data to stdout
ostream& operator<<(ostream& out, const PpmImage& obj)
{
	out<<obj.version<<endl;
	out<<obj.dX<<" "<<obj.dY<<endl;
	out<<obj.max<<endl;

	for(int i=0; i<obj.pixelVector.size(); i++)
	{
		for(int k=0; k<obj.pixelVector[i].size(); k++)
		{
			out<<obj.pixelVector[i][k].getR()<<" ";
			out<<obj.pixelVector[i][k].getG()<<" ";
			out<<obj.pixelVector[i][k].getB()<<" ";
			out<<"   ";
		}
		out<<endl;
	}
	return out;
}
//Adds two ppmImage objects. Adds them pixel-by-pixel. 
//They have to be the same size otherwise it returns an empty ppmImage object.
const PpmImage operator+(const PpmImage& left, const PpmImage& right)
{
	int r,g,b;
	PpmImage newObj;
	newObj.setdX(left.getdX()); 
	newObj.setdY(left.getdY());
	newObj.setVersion(left.getVersion());
	newObj.setMax(left.getMax()); 
	if(right.pixelVector.size() == left.pixelVector.size())
	{
		vector <vector <Pixels> >& vec = newObj.getVector();; 	 
		vec.resize(left.pixelVector.size());
		for(int i=0; i<right.pixelVector.size(); i++)
		{
			vec[i].resize(left.pixelVector[i].size());
			for(int k=0; k<right.pixelVector[i].size(); k++)
			{
				//addition operation happens here
				r = right.pixelVector[i][k].getR() + left.pixelVector[i][k].getR();
				g = right.pixelVector[i][k].getG() + left.pixelVector[i][k].getG();
				b = right.pixelVector[i][k].getB() + left.pixelVector[i][k].getB();
				
				/*we are assuming that both have same max value which is 255*/
				if(r>255) r=255;
				if(g>255) g=255;
				if(b>255) b=255;
				//setting
				vec[i][k].setR(r);
				vec[i][k].setG(g);
				vec[i][k].setB(b);
			}
		}

	}
	return newObj;
}
//subtracts one image from the other. Color data does not go below 0.
const PpmImage operator-(const PpmImage& left, const PpmImage& right)
{
	int r,g,b;
	PpmImage newObj;
	newObj.setdX(left.getdX()); 
	newObj.setdY(left.getdY());
	newObj.setVersion(left.getVersion());
	newObj.setMax(left.getMax()); 
	if(right.pixelVector.size() == left.pixelVector.size())
	{
		vector <vector <Pixels> >& vec = newObj.getVector();; 	 
		vec.resize(left.pixelVector.size());

		for(int i=0; i<left.pixelVector.size(); i++)
		{
			vec[i].resize(left.pixelVector[i].size());
			for(int k=0; k<left.pixelVector[i].size(); k++)
			{
				//substraction happens here
				r = left.pixelVector[i][k].getR() - right.pixelVector[i][k].getR();
				g = left.pixelVector[i][k].getG() - right.pixelVector[i][k].getG();
				b = left.pixelVector[i][k].getB() - right.pixelVector[i][k].getB();
				/*we are assuming that both have same min value which is 0*/
				if(r<0) r=0;
				if(g<0) g=0;
				if(b<0) b=0;
				vec[i][k].setR(r);
				vec[i][k].setG(g);
				vec[i][k].setB(b);
			}
		}
	}
	return newObj;
}

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
			for(int i=0;i<dX; i++)
			{
				for(int k =0; k<dY; k++)
				{
					fin>>_r>>_g>>_b;
					if( fileFormatIsTrue==true )
					{
						if(_r<0 ) _r = 0;
						if(_r>255 ) _r = 255;
						if(_g<0 ) _g = 0;
						if(_g>255 ) _g = 255;
						if(_b<0 ) _b = 0;
						if(_b>255 ) _b = 255;
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
		r_value = pixelVector[x][y].getR();
	else if(code == 2) //green
		r_value =  pixelVector[x][y].getG();
	else if(code == 3)//blue
		r_value =  pixelVector[x][y].getB();
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
	pixelVector.resize(dX);
	for(int i=0; i<pixelVector.size(); i++)
		pixelVector[i].resize(dY);
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

			for(int i=0;i<destination_object.getdX(); i++)
			{
				for(int k =0; k<destination_object.getdY(); k++)
				{
					fin>>_r>>_g>>_b;
					if( fileFormatIsTrue==true )
					{
						if(_r<0 ) _r = 0;
						else if(_r>255 ) _r = 255;
						if(_g<0 ) _g = 0;
						else if(_g>255 ) _g = 255;
						if(_b<0 ) _b = 0;
						else if(_b>255 ) _b = 255;
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
	int temp;
	if(swap_choice==1 ||swap_choice==2 || swap_choice==3 )
	{
		for(int i=0;i<image_object_to_be_modified.getdX(); i++)
		{
			for(int k=0;k<image_object_to_be_modified.getdY(); k++)
			{
				if(1==swap_choice) // if swap_choice is 1: swaps red and green
				{
					int& r = image_object_to_be_modified(i,k,1);			
					int& g = image_object_to_be_modified(i,k,2);
					temp = r; 		
					r=g;
					g = temp;
				}
				else if(2==swap_choice)// if swap_choice is 2: swaps red and blue
				{
					int& r = image_object_to_be_modified(i,k,1);			
					int& b = image_object_to_be_modified(i,k,3);
					temp = r; 		
					r=b;
					b = temp;
				}

				else if(3==swap_choice)// if swap_choice is 3: swaps green and blue
				{
					int& b = image_object_to_be_modified(i,k,3);			
					int& g = image_object_to_be_modified(i,k,2);
					temp = b; 		
					b=g;
					g = temp;
				}
			}
		}
		return 1;
	}//if
	else
		cerr<<"invalid swap choice !!"<<endl;	
	
	
	return 0;
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
	copy = source; //coppying of the source object
	for(int i = 0; i<copy.getdX(); i++)
	{
		for(int k=0; k<copy.getdY(); k++)
		{
			int &r = copy(i,k,1);
			int &g = copy(i,k,2);
			int &b = copy(i,k,3);
			r = 0;
			g = 0;
			b = 0;
			if(1==color_choice)
				r = source.pixelInfo(i,k,1);
			if(2==color_choice)
				g = source.pixelInfo(i,k,2);
			else if(3==color_choice)
				b = source.pixelInfo(i,k,3);
		}
	}

	return copy;
}


int main(int argc, char const *argv[])
{

	int choice;
	string ppm_file_name1;
	string ppm_file_name2;
	string ppm_file_name3;
	PpmImage obj;
	string output;
	bool ArgumentsAreOke = true;

	if(argc>2)
		choice = stoi(argv[1]);	  
	if(argc == 4 ){
		ppm_file_name1 = argv[2];	
		ppm_file_name2 = argv[3];	
	}
	
	else if(argc == 5 )
	{
		ppm_file_name1 = argv[2];	
		ppm_file_name2 = argv[3];
		ppm_file_name3 = argv[4];	
	}
	else 
	{
		cerr << "Wrong call!" << endl;
		ArgumentsAreOke = false;
	}
	
	if(ArgumentsAreOke){
		if(choice == 1)
		{
			if(argc == 5)
				test_addition(ppm_file_name1,ppm_file_name2,ppm_file_name3);
			else cerr << "Wrong call!" << endl;
		}
		else if(choice == 2)
		{
			if(argc == 5)
				test_subtraction(ppm_file_name1,ppm_file_name2,ppm_file_name3);
			else cerr << "Wrong call!" << endl;
		}
		else if(choice==3)
		{
			read_ppm(ppm_file_name1, obj);	
			swap_channels(obj, 2);
			write_ppm(ppm_file_name2, obj);
		}
		else if(choice==4)
		{
			read_ppm(ppm_file_name1, obj);	
			swap_channels(obj, 3);
			write_ppm(ppm_file_name2, obj);
		}
		else if(choice==5)
		{
			read_ppm(ppm_file_name1, obj);	
			write_ppm(ppm_file_name2, single_color(obj, 1));
		}
		else if(choice==6)
		{
			read_ppm(ppm_file_name1, obj);	
			write_ppm(ppm_file_name2, single_color(obj, 2));
		}
		else if(choice==7)
		{
			read_ppm(ppm_file_name1, obj);
			write_ppm(ppm_file_name2, single_color(obj, 3));
		}
		else cerr<<"invalid option!!"<<endl;
	}

	return 0;
}