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
#include <boost/thread/thread.hpp>
#include <boost/predef.h>

using namespace std;
using namespace boost::filesystem;
using namespace boost::posix_time;
using namespace boost;


boost::filesystem::path OriginalPath = current_path(); //Gets The Current Path, which contains the Program
boost::filesystem::path FinalPath; //Path where the Savefile should be
boost::filesystem::path MainDir; //Path where all Saves are Stored aka DarkSoulsAutoSave
boost::filesystem::path Year; // DarkSoulsAutoSave\\2016
boost::filesystem::path Month; // DarkSoulsAutoSave\\2016\\May

string year;
string month;
string timestamp;
string dsname = "DRAKS0005.sl2";

int mint; //Minutes to compare
int premint; //Minutes to compare
int maxlimit = 0; //Variable to prevent Infinite Loop, Max Value = 100

typedef vector<boost::filesystem::path> vec; // store paths, so we can store and sort them later
vec v; //Vector for Source-File
vec w; //Vector for Destination-Directory and Sorting



void getDir(boost::filesystem::path p){ //Basically trying to find out where to search for the Saves
	boost::filesystem::path temp = p; //PreVariable to Cycle Back the Path
	boost::filesystem::path pretemp; //PrePreVariable to Cycle Back the Path, should contain the User Folder
	string stemp = temp.filename().string(); //Casting Path to String for Comparison
	string docs = "Users"; //String
	cout << "docs equals: " << docs << endl << endl; //Optional
	cout << "stemp equals: " << stemp << endl << endl; //Optional
	cout << "temp equals: " << temp.string() << endl << endl; //Optional
	while(stemp.compare(docs) != 0){ //Reverse Clycling through Path
		pretemp = temp; //Assignment of Paths
		cout << "pretemp equals: " << pretemp.string() << endl << endl; //Optional
		temp = temp.parent_path(); //Going up one Level in Path
		cout << "temp equals: " << temp.string() << endl << endl; //Optional
		stemp = temp.filename().string(); //Casting Path to String for Comparison - Recast
		cout << "stemp equals: " << stemp << endl << endl; //Optional
	}
	cout << "It's done! " << endl << endl; //Optional
	string constructpath = pretemp.string(); //Combined with suffix to create FinalPath
	cout << "pretemp finished equals: " << pretemp.string() << endl << endl; //Optional
	string suffix = "\\Documents\\NBGI\\DarkSouls\\"; //Path to DS Savefile
	string Conv(constructpath + suffix); //Converted String Equals renamed Folders
 	FinalPath = Conv; // Cast String to Path
	cout << FinalPath.string() << " is the estimated Name of the Directory!" << endl << endl; //Optional
}

void makemain(){ //Creating The MainDirectory
	string PreMain(FinalPath.string() + "DarkSoulsAutoSave\\");
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

time_facet *facet = new time_facet("%d_%b_%Y %H-%M-%S"); //Creates a Timefacet with the Pattern "Day_Mon_Year Hour-Minutes-Seconds" 
ss.imbue(locale(ss.getloc(), facet)); //Maps the Facet onto the Stringstream to match the Pattern
ss << second_clock::local_time(); //Gets the Time and assigns it to the Stringstream
timestamp = ss.str(); //Casts Stringstream to String
ss.str(string()); //Clears the Stringstream

time_facet *facetyear = new time_facet("%Y"); //Creates a Timefacet with the Pattern "Year"
ss1.imbue(locale(ss1.getloc(), facetyear)); //Maps the Facet onto the Stringstream to match the Pattern
ss1 << second_clock::local_time(); //Gets the Time and assigns it to the Stringstream
year = ss1.str(); //Casts Stringstream to String
ss1.str(string()); //Clears the Stringstream

time_facet *facetmonth = new time_facet("%B");//Creates a Timefacet with the Pattern "Month"
ss2.imbue(locale(ss2.getloc(), facetmonth)); //Maps the Facet onto the Stringstream to match the Pattern
ss2 << second_clock::local_time(); //Gets the Time and assigns it to the Stringstream
month = ss2.str(); //Casts Stringstream to String
ss2.str(string()); //Clears the Stringstream

cout << "Year: " << year << " Month: " << month << endl; //Optional

maxi << MainDir.string() << year << "\\"; //Adds Year to MainDir Path
Year = maxi.str();
maxi.str(string()); //Clears the Stringstream
mini << Year.string() << month << "\\"; //Adds Month to Year Path
Month = mini.str();
mini.str(string()); //Clears the Stringstream

cout << "Year equals: " << Year.string() << endl << endl; //Optional
cout << "Month equals: " << Month.string() << endl << endl; //Optional

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

void clearvecs(){ //Clears the Vectors to prevent File Duplications, or whatever the Bug I encountered was (hopefully)

	v.erase(v.begin(), v.end());
	w.erase(w.begin(), w.end());
}

//................................................................................................\\

void hitandrun(){

	cout << "It has begun!" << endl << endl;

	for (vec::iterator it (v.begin()); it != v.end(); ++it){		

		boost::filesystem::path Sourcefile = (*it);
		stringstream ss3;
		ss3.str(string());
		ss3 << Month.string() << Sourcefile.filename().string();
		boost::filesystem::path Destinationfile = ss3.str();
		cout << Destinationfile << endl; //Optional
		copy_file(Sourcefile, Destinationfile);
		cout << "Finished Copy Process!" << endl; //Optional

		stringstream renamed;
		renamed << Destinationfile.string();
		cout << renamed.str() << endl << endl;
		size_t found = renamed.str().find_last_of(".");
		cout << "Found equals: " << found << endl << endl;
		stringstream pre;
		pre << renamed.str().substr(0, found) << " - " << timestamp << renamed.str().substr(found);

		boost::filesystem::path Renomnom = pre.str();
		cout << Renomnom.string() << endl << endl;
		rename(Destinationfile, Renomnom);
	}

	cout << "Outside Loop!" << endl; //Optional

	w.erase(w.begin(), w.end());
}

void timegate(){// gets the minutes and uses them for comparison to sync the saves in a X minute cycle
	std::stringstream minutes;
	time_facet *facetminutes = new time_facet("%M");
	minutes.imbue(locale(minutes.getloc(), facetminutes));
	minutes << second_clock::local_time();
	mint = atoi(minutes.str().c_str());
	minutes.str(string()); //Clears the Stringstream

}

void checkexist(boost::filesystem::path p);

void createstorage(boost::filesystem::path p);

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

    clearvecs();
	getDir(OriginalPath);
	checkexist(FinalPath);
	makemain();
	checkexist(MainDir);
	timegate(); //gets current Time-Value Minutes
	premint = mint; //Inits the Swap Values
	gettime();
	createstorage(Year);
	createstorage(Month);
	showvecs();
	hitandrun();
	maxlimit++;

	while(maxlimit <= 100){ //Starts a 3 Variables Swap with Time Comparison and Management
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
		boost::this_thread::sleep(seconds(10));
	}


	return 0;

	//Reduce Amount of Namespaces and use Scopes ::
	//Comment better and more
	//Add Multi-Threading for User-based Exiting
	//Create optimized Copys for DSII and DS3 - GG


}
//##################################################################################################################################################################//




void checkexist(boost::filesystem::path p){
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

				for (vec::iterator it (v.begin()); it != v.end(); ++it){ //Cycles through the Vector once more to verify that ONLY the Original
					if(dsname.compare((*it).filename().string()) != 0){ //Savefile is included to prevent copying of random Files
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
			cout << p << " does not exist" << std::endl;
		}
	}
	catch (const filesystem_error& ex){
		cout << ex.what() << endl;
	}
}

void createstorage(boost::filesystem::path p){
	try{
		if (exists(p)) {  // does p actually exist?
			if (is_regular_file(p)){  // is p a regular file?
        cout << p << " size is " << file_size(p) << endl; //if it is output the filesize
			}
			else if (is_directory(p)){      // is p a directory?
				cout << p << " is a directory containing:" << endl;

				copy(directory_iterator(p), directory_iterator(), back_inserter(w)); //std::copy (inputfirst, inputlast, output)

				sort(w.begin(), w.end()); //std::sort, since directory iteration is not ordered on some file systems

				for (vec::iterator it2 (w.begin()); it2 != w.end(); ++it2){ //Iteration cycles through Vector W
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
				cout << p << " exists, but is neither a regular file nor a directory" << std::endl;
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
/*-----------------------------------------------SYMLINK-MAGIC-------------------------------------//
void symlinking(boost::filesystem::path sym){
	try{
		if (exists(sym)) {  // does sym actually exist?
			if (is_regular_file(sym)){  // is sym a regular file?
        		cout << sym << " size is " << file_size(sym) << endl; //if it is output the filesize
			}
			else if (is_symlink(sym)){      // is sym a directory?
				cout << sym << " is a Symlink !" << endl;

			}
			else{
				cout << sym << " exists, but is neither a regular file nor a directory" << std::endl;
			}
		}
		else{
			cout << p << " does not exist" << std::endl;
		}
	}
	catch (const filesystem_error& ex){
		cout << ex.what() << endl;
	}
}
--------------------------------------------------------------------------------------------------*///

/*
<-------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!---------------------------------->
COMMENT DSAS and MC2V!!! Holy.
<-------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!---------------------------------->
Check if CurrentDir is in Roaming
Yes:Skip
Else:Create Symlink for DSII
Let User Move DSASII into preferred	SteamID Folder
Wait for any Input to ensure the Messag is read!!!!
--------------------
Check if DSASII is inside SteamID Folder
Yes:Skip
Else: Create Symlink for DSASII and SteamID Folder on Desktop inside DSASII
--------------------
Normal DSAS-Routine
???
Profit
Praise!
*/