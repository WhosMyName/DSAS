/*//Dark Sous Auto Save - DSAS

Implement Auto Name Search
Implement Time Retrieval
Implement Looping Backup
Implement Copying
Implement Renaming
Implement Exiting

Check for Stored Path and determine Name or
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <iterator>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/predef.h>

using namespace std;
using namespace boost::filesystem;

path OriginalPath = current_path(); //Gets The Current Path, which contains the Program
path FinalPath; //Path where the Savefile should be

typedef vector<path> vec; // store paths, so we can store and sort them later
vec v; //Vector for Source-Directory
vec x; //Vector for Source-Directory without Zeros for Comparison
vec u; //Vector for Files
vec w; //Vector for Destination-Directory


path getDir(path p){
	path temp = p;
	path pretemp = p;
	string users = "Users";
	if(temp.filename().string() != users){
		pretemp = temp;
		temp = temp.parent_path();
	}
		string constructpath = pretemp.string();
		string suffix = "/Documents/NBGI/Dark Souls/"
		string Conv(constructpath + suffix); //Converted String Equals renamed Folders
		path Dir;
		Dir = Conv; // Cast String to Path
		FinalPath = Dir;
}





void checkexist(path p);

//##################################################################################################################################################################//
int main(){

	if(BOOST_OS_WINDOWS){
        cout << "Windows" << endl;
	}
    else if(BOOST_OS_LINUX){
        cout << "Linux" << endl;
    }
    else{
        cout << "Other" << endl;
    }

	return 0;
}
//##################################################################################################################################################################//




void checkexist(path p){
	DiffDirs = 0;
	Fill = 0;
	try{
		if (exists(p)) {  // does p actually exist?
			if (is_regular_file(p)){  // is p a regular file?
        cout << p << " size is " << file_size(p) << endl; //if it is output the filesize
			}
			else if (is_directory(p)){      // is p a directory?
				cout << p << " is a directory containing:" << endl;

				copy(directory_iterator(p), directory_iterator(), back_inserter(v)); //std::copy (inputfirst, inputlast, output)

				sort(v.begin(), v.end()); //std::sort, since directory iteration is not ordered on some file systems

			}
			else{
				cout << p << " exists, but is neither a regular file nor a directory\n";
			}
		}
		else{
			cout << p << " does not exist\n";
		}
	}
	catch (const filesystem_error& ex){
		cout << ex.what() << endl;
	}
}
