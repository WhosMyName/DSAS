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
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/predef.h>

using namespace std;
using namespace boost::filesystem;
using namespace boost::posix_time;


path OriginalPath = current_path(); //Gets The Current Path, which contains the Program
path FinalPath; //Path where the Savefile should be
path MainDir; //Path where all Saves are Stored
path Year;
path Month;

string year;
string month;
string timestamp;

int mint; //Minutes to compare
int premint; //Minutes to compare
int maxlimit = 0;

typedef vector<path> vec; // store paths, so we can store and sort them later
vec v; //Vector for Source-File
vec w; //Vector for Destination-Directory and Sorting



void getDir(path p){
	path temp = p;
	path pretemp;
	string stemp = temp.filename().string();
	string docs = "Documents and Settings";
	cout << "docs equals: " << docs << endl << endl; //Optional
	cout << "stemp equals: " << stemp << endl << endl; //Optional
	cout << "temp equals: " << temp.string() << endl << endl; //Optional
	while(stemp.compare(docs) != 0){
		pretemp = temp;
		cout << "pretemp equals: " << pretemp.string() << endl << endl; //Optional
		temp = temp.parent_path();
		cout << "temp equals: " << temp.string() << endl << endl; //Optional
		stemp = temp.filename().string();
		cout << "stemp equals: " << stemp << endl << endl; //Optional
	}
	cout << "It's done! " << endl << endl; //Optional
	string constructpath = pretemp.string();
	cout << "pretemp finished equals: " << pretemp.string() << endl << endl; //Optional
	string suffix = "/Documents/NBGI/DarkSouls/";
	string Conv(constructpath + suffix); //Converted String Equals renamed Folders
	path Dir;
	Dir = Conv; // Cast String to Path
	FinalPath = Dir;
	cout << FinalPath.string() << " is the estimated Name of the Directory!" << endl << endl;
}

void makemain(){
	string PreMain(FinalPath.string() + "DarkSoulsAutoSave/");
	MainDir = PreMain;
	if (create_directory(MainDir)) { //Checks if creation of Directories Succeeds
		cout << "Success creating Directory called " << MainDir << endl; //Outputs Success Message
	}
}

void gettime(){
stringstream ss; //Timestamp
stringstream ss1; //Year
stringstream ss2; //Month
stringstream maxi; //Path Year
stringstream mini; //Path Month
ss.str(string()); //Clears the Stringstream
ss1.str(string()); //Clears the Stringstream
ss2.str(string()); //Clears the Stringstream
maxi.str(string()); //Clears the Stringstream
mini.str(string()); //Clears the Stringstream

time_facet *facet = new time_facet("%d-%b-%Y %H:%M:%S");
ss.imbue(locale(ss.getloc(), facet));
ss << second_clock::local_time() << endl;
timestamp = ss.str();
ss.str(string()); //Clears the Stringstream

time_facet *facetyear = new time_facet("%Y");
ss1.imbue(locale(ss1.getloc(), facetyear));
ss1 << second_clock::local_time();
year = ss1.str();
ss1.str(string()); //Clears the Stringstream

time_facet *facetmonth = new time_facet("%B");
ss2.imbue(locale(ss2.getloc(), facetmonth));
ss2 << second_clock::local_time();
month = ss2.str();
ss2.str(string()); //Clears the Stringstream

cout << "Year: " << year << " Month: " << month << endl;

maxi << MainDir.string() << year << "/";
Year = maxi.str();
maxi.str(string()); //Clears the Stringstream
mini << Year.string() << month <<"/";
Month = mini.str();
mini.str(string()); //Clears the Stringstream

cout << "Year equals: " << Year.string() << endl << endl;
cout << "Month equals: " << Month.string() << endl << endl;

}

void showvecs(){ //Optional
	cout << "<-----------------------V-------------------------------->" << endl << endl;
	for (vec::iterator it (v.begin()); it != v.end(); ++it){ //Iteration cycles through Vector V
		cout << (*it).filename() << " was the latest added Directory!" << endl << endl;
	}
	cout << "<------------------------W------------------------------->" << endl << endl;
	for (vec::iterator it2 (w.begin()); it2 != w.end(); ++it2){ //Iteration cycles through Vector W
		cout << (*it2).filename() << " was the latest added Directory!" << endl << endl;
	}
	cout << "<------------------------X------------------------------->" << endl << endl;
}
//................................................................................................\\

void hitandrun(){

	cout << "It has begun!" << endl << endl;

	for (vec::iterator it (v.begin()); it != v.end(); ++it){

		path Sourcefile = (*it);
		stringstream ss3;
		ss3.str(string());
		ss3 << Month.string() << Sourcefile.filename().string();
		path Destinationfile = ss3.str();
		cout << Destinationfile << endl; //Optional
		copy_file(Sourcefile, Destinationfile);
		cout << "Finished Copy Process!" << endl; //Optional

		stringstream renamed;
		renamed << Destinationfile.string();
		cout << renamed.str() << endl << endl;
		size_t found = renamed.str().find_last_of(".");
		cout << "Found equals: " << found << endl << endl;
		stringstream pre;
		//stringstream ext;
		pre << renamed.str().substr(0,found) << " - " << timestamp << renamed.str().substr(found);

		path Renomnom = pre.str();
		cout << Renomnom.string() << endl << endl;
		rename(Destinationfile, Renomnom);
	}

	cout << "Outside Loop!" << endl; //Optional

	w.erase(w.begin(), w.end());
}

void timegate(){// gets the minutes and uses them for comparison to sync the saves in a 10 minute cycle
	stringstream minutes;
	time_facet *facetminutes = new time_facet("%M");
	minutes.imbue(locale(minutes.getloc(), facetminutes));
	minutes << second_clock::local_time();
	mint = atoi(minutes.str().c_str());
	minutes.str(string()); //Clears the Stringstream

}

void checkexist(path p);

void createstorage(path p);

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

    
	getDir(OriginalPath);
	checkexist(FinalPath);
	makemain();
	checkexist(MainDir);
	timegate();
	premint = mint;
	gettime();
	createstorage(Year);
	createstorage(Month);
	hitandrun();
	maxlimit++;

	while(maxlimit <= 100){
		timegate();
		if(premint + 10 >= 60){
			premint = premint - 50;
		}
		if(mint == premint + 10){
			premint = mint;
			showvecs();
			gettime();
			createstorage(Year);
			createstorage(Month);
			hitandrun();
			maxlimit++;
		}
		sleep(10); 
	}


	return 0;


	//Create optimized Copys for DSII and DS3 - GG


}
//##################################################################################################################################################################//




void checkexist(path p){
	try{
		if (exists(p)) {  // does p actually exist?
			if (is_regular_file(p)){  // is p a regular file?
        cout << p << " size is " << file_size(p) << endl; //if it is output the filesize
			}
			else if (is_directory(p)){      // is p a directory?
				cout << p << " is a directory containing:" << endl;

				copy(directory_iterator(p), directory_iterator(), back_inserter(v)); //std::copy (inputfirst, inputlast, output)

				sort(v.begin(), v.end()); //std::sort, since directory iteration is not ordered on some file systems

				for (vec::iterator it (v.begin()); it != v.end(); ++it){ //Iteration cycles through Vector V
					if (is_directory(*it)){
						it = v.erase(it);
						it--;
					}
				}

				for (vec::iterator it (v.begin()); it != v.end(); ++it){ //Iteration cycles through Vector V
					cout << (*it).filename() << " was the latest added File!" << endl << endl;
				}
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

void createstorage(path p){
	try{
		if (exists(p)) {  // does p actually exist?
			if (is_regular_file(p)){  // is p a regular file?
        cout << p << " size is " << file_size(p) << endl; //if it is output the filesize
			}
			else if (is_directory(p)){      // is p a directory?
				cout << p << " is a directory containing:" << endl;

				copy(directory_iterator(p), directory_iterator(), back_inserter(w)); //std::copy (inputfirst, inputlast, output)

				sort(w.begin(), w.end()); //std::sort, since directory iteration is not ordered on some file systems

				for (vec::iterator it2 (w.begin()); it2 != w.end(); ++it2){ //Iteration cycles through Vector V
					if (is_directory(*it2)){
						it2 = w.erase(it2);
						it2--;
					}
				}

				for (vec::iterator it2 (w.begin()); it2 != w.end(); ++it2){ //Iteration cycles through Vector W
					cout << (*it2).filename() << " was the latest added File!" << endl << endl;
				}
			}
			else{
				cout << p << " exists, but is neither a regular file nor a directory\n";
			}
		}
		else{
			if (create_directory(p)) { //Checks if creation of Directories Succeeds
				cout << "Success creating Directory called " << p << endl; //Outputs Success Message
			}
		}
	}
	catch (const filesystem_error& ex){
		cout << ex.what() << endl;
	}
}