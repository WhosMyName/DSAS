#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <iterator>
#include <thread>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/thread/thread.hpp>
#include <boost/predef.h>

boost::filesystem::path OriginalPath = boost::filesystem::current_path(); //Gets The Current Path, which contains the Program
boost::filesystem::path Desktop; //Path which leads to Desktop
boost::filesystem::path FinalPath; //Path where the Savefile should be
boost::filesystem::path MainDir; //Path where all Saves are Stored aka DarkSoulsAutoSave 2 & 3
boost::filesystem::path Year; // DarkSoulsAutoSave\\2016
boost::filesystem::path Month; // DarkSoulsAutoSave\\2016\\May

std::string year;
std::string month;
std::string timestamp;
std::string dsname;

int mint; //Minutes to compare
int premint; //Minutes to compare
bool inplace = false;
bool thread_running = false;

typedef std::vector<boost::filesystem::path> vec; // store paths, so we can store and sort them later
vec v; //Vector for Source-File
vec w; //Vector for Destination-Directory and Sorting

void getDir(boost::filesystem::path p){ //Basically trying to find out where to search for the Saves
	boost::filesystem::path temp = p; //PreVariable to Cycle Back the Path
	boost::filesystem::path pretemp; //PrePreVariable to Cycle Back the Path, should contain the User Folder
	std::string stemp = temp.filename().string(); //Casting Path to String for Comparison
	std::string docs = "Users"; //String
	std::cout << "docs equals: " << docs << std::endl << std::endl; //Optional
	std::cout << "stemp equals: " << stemp << std::endl << std::endl; //Optional
	std::cout << "temp equals: " << temp.string() << std::endl << std::endl; //Optional
	while(stemp.compare(docs) != 0){ //Reverse Clycling through Path
		pretemp = temp; //Assignment of Paths
		std::cout << "pretemp equals: " << pretemp.string() << std::endl << std::endl; //Optional
		temp = temp.parent_path(); //Going up one Level in Path
		std::cout << "temp equals: " << temp.string() << std::endl << std::endl; //Optional
		stemp = temp.filename().string(); //Casting Path to String for Comparison - Recast
		std::cout << "stemp equals: " << stemp << std::endl << std::endl; //Optional
	}
	std::cout << "It's done! " << std::endl << std::endl; //Optional
	std::string constructpath = pretemp.string(); //Combined with suffix to create FinalPath
	std::cout << "pretemp finished equals: " << pretemp.string() << std::endl << std::endl; //Optional
	std::string suffix = "\\AppData\\Roaming\\DarkSoulsII\\"; //Path to DS Savepath
	std::string Conv(constructpath + suffix); //Converted String Equals renamed Folders
 	FinalPath = Conv; // Cast String to Path
	std::string suffix2 = "\\Desktop\\"; //Path to Desktop
	std::string Conv2(constructpath + suffix2); //Combines constructpath and suffix2
	Desktop = Conv2; //Converts String to Path
	std::cout << FinalPath.string() << " is the estimated Name of the Directory!" << std::endl << std::endl; //Optional
}

void makemain(){ //Creating The MainDirectory
	std::string PreMain(FinalPath.string() + "DarkSoulsAutoSave\\");
	MainDir = PreMain;
	if (create_directory(MainDir)) { //Checks if creation of Directories Succeeds
		std::cout << "Success creating Directory called " << MainDir << std::endl; //Outputs Success Message
	}
}

void gettime(){
std::stringstream ss; //Timestamp
std::stringstream ss1; //Year
std::stringstream ss2; //Month
std::stringstream maxi; //Path Year
std::stringstream mini; //Path Month
ss.str(std::string()); //Clears the Stringstream
ss1.str(std::string()); //Clears the Stringstream
ss2.str(std::string()); //Clears the Stringstream
maxi.str(std::string()); //Clears the Stringstream
mini.str(std::string()); //Clears the Stringstream

boost::posix_time::time_facet *facet = new boost::posix_time::time_facet("%d_%b_%Y %H-%M-%S"); //Creates a Timefacet with the Pattern "Day_Mon_Year Hour-Minutes-Seconds" 
ss.imbue(std::locale(ss.getloc(), facet)); //Maps the Facet onto the Stringstream to match the Pattern
ss << boost::posix_time::second_clock::local_time(); //Gets the Time and assigns it to the Stringstream
timestamp = ss.str(); //Casts Stringstream to String
ss.str(std::string()); //Clears the Stringstream

boost::posix_time::time_facet *facetyear = new boost::posix_time::time_facet("%Y"); //Creates a Timefacet with the Pattern "Year"
ss1.imbue(std::locale(ss1.getloc(), facetyear)); //Maps the Facet onto the Stringstream to match the Pattern
ss1 << boost::posix_time::second_clock::local_time(); //Gets the Time and assigns it to the Stringstream
year = ss1.str(); //Casts Stringstream to String
ss1.str(std::string()); //Clears the Stringstream

boost::posix_time::time_facet *facetmonth = new boost::posix_time::time_facet("%B");//Creates a Timefacet with the Pattern "Month"
ss2.imbue(std::locale(ss2.getloc(), facetmonth)); //Maps the Facet onto the Stringstream to match the Pattern
ss2 << boost::posix_time::second_clock::local_time(); //Gets the Time and assigns it to the Stringstream
month = ss2.str(); //Casts Stringstream to String
ss2.str(std::string()); //Clears the Stringstream

std::cout << "Year: " << year << " Month: " << month << std::endl; //Optional

maxi << MainDir.string() << year << "\\"; //Adds Year to MainDir Path
Year = maxi.str();
maxi.str(std::string()); //Clears the Stringstream
mini << Year.string() << month << "\\"; //Adds Month to Year Path
Month = mini.str();
mini.str(std::string()); //Clears the Stringstream

std::cout << "Year equals: " << Year.string() << std::endl << std::endl; //Optional
std::cout << "Month equals: " << Month.string() << std::endl << std::endl; //Optional

}

void showvecs(){ //Optional
	std::cout << "<-----------------------V-------------------------------->" << std::endl << std::endl;
	for (vec::iterator it (v.begin()); it != v.end(); ++it){ //Iteration cycles through Vector V
		std::cout << (*it).filename() << " was the latest added Directory!" << std::endl << std::endl;
	}
	std::cout << "<------------------------W------------------------------->" << std::endl << std::endl;
	for (vec::iterator it2 (w.begin()); it2 != w.end(); ++it2){ //Iteration cycles through Vector W
		std::cout << (*it2).filename() << " was the latest added Directory!" << std::endl << std::endl;
	}
	std::cout << "<------------------------X------------------------------->" << std::endl << std::endl;
}

//................................................................................................\\

void clearvecs(){ //Clears the Vectors to prevent File Duplications, or whatever the Bug I encountered was (hopefully)

	v.erase(v.begin(), v.end());
	w.erase(w.begin(), w.end());
}

//................................................................................................\\

void hitandrun(){ //Thats the actual Save-Function

	std::cout << "It has begun!" << std::endl << std::endl;

	for (vec::iterator it (v.begin()); it != v.end(); ++it){		

		boost::filesystem::path Sourcefile = (*it);
		std::stringstream ss3;
		ss3.str(std::string());
		ss3 << Month.string() << Sourcefile.filename().string();
		boost::filesystem::path Destinationfile = ss3.str();
		std::cout << Destinationfile << std::endl; //Optional
		copy_file(Sourcefile, Destinationfile);
		std::cout << "Finished Copy Process!" << std::endl; //Optional

		std::stringstream renamed;
		renamed << Destinationfile.string();
		std::cout << renamed.str() << std::endl << std::endl;
		size_t found = renamed.str().find_last_of(".");
		std::cout << "Found equals: " << found << std::endl << std::endl;
		std::stringstream pre;
		pre << renamed.str().substr(0, found) << " - " << timestamp << renamed.str().substr(found);

		boost::filesystem::path Renomnom = pre.str();
		std::cout << Renomnom.string() << std::endl << std::endl;
		rename(Destinationfile, Renomnom);
	}

	std::cout << "Outside Loop!" << std::endl; //Optional

	w.erase(w.begin(), w.end());
}

void timegate(){// gets the minutes and uses them for comparison to sync the saves in a X minute cycle
	std::stringstream minutes;
	boost::posix_time::time_facet *facetminutes = new boost::posix_time::time_facet("%M");
	minutes.imbue(std::locale(minutes.getloc(), facetminutes));
	minutes << boost::posix_time::second_clock::local_time();
	mint = atoi(minutes.str().c_str());
	minutes.str(std::string()); //Clears the Stringstream

}

void threadnochill(){
	thread_running = true;
	std::cin.ignore();
	thread_running = false;
}

void checkexist(boost::filesystem::path p);

void createstorage(boost::filesystem::path p);

//##################################################################################################################################################################//
int main(){

	/*-------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!---------------------------------->
	Check if DSASII is inside SteamID Folder
	Yes:Skip
	Else: Create Symlink for DSASII and SteamID Folder on Desktop inside DSASII
	Let User Move DSASII into preferred	SteamID Folder
	Wait for any Input to ensure the Messag is read!!!!
	--------------------
	Normal DSAS-Routine
	???
	Profit
	Praise!
	*/

	/*
	Check if DSASII is beneath Roaming/DSII
	if not create Symlink, notify and exit;
	else check if Savefile exists
		set inplace true and re-run;
		check which save-name exists and set this name to dsname;
	*/

	if(BOOST_OS_WINDOWS){
        std::cout << "Windows" << std::endl;
	}
    else if(BOOST_OS_LINUX){
        std::cout << "Linux" << std::endl;
    }
    else{
        std::cout << "Other" << std::endl;
    }

    clearvecs();
	getDir(OriginalPath);
	if(OriginalPath.string().find(FinalPath.string()) == std::string::npos){
		create_directory_symlink(FinalPath, OriginalPath);
	}
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
	std::thread thread(threadnochill);// Creates Thread (C++11) and calls treadnochill
	boost::this_thread::sleep(boost::posix_time::seconds(1));
	std::cout << "Hit Enter to exit DSAS..." << std::endl;

	while(thread_running){ //Starts a 3 Variables Swap with Time Comparison and Management
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
			std::cout << "Hit Enter to exit DSAS..." << std::endl;
		}
		boost::this_thread::sleep(boost::posix_time::seconds(10)); //Lets The Process sleep to reduce CPU Load
	}

	std::cout << "Saving before Exiting!" << std::endl;
	gettime();
	createstorage(Year);
	createstorage(Month);
	hitandrun();
	std::cout << "Exiting!" << std::endl;

	thread.join();
	return 0;

	//Create optimized Copys for DSII and DS3 - GG


}
//##################################################################################################################################################################//




void checkexist(boost::filesystem::path p){
	try{
		if (exists(p)) {  // does p actually exist?
			if (is_regular_file(p)){  // is p a regular file?
        std::cout << p << " size is " << file_size(p) << std::endl; //if it is output the filesize
			}
			else if (is_directory(p)){      // is p a directory?
				std::cout << p << " is a directory containing:" << std::endl;

				copy(boost::filesystem::directory_iterator(p), boost::filesystem::directory_iterator(), std::back_inserter(v)); //std::copy (inputfirst, inputlast, output)

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
					std::cout << (*it).filename() << " was the latest added File!" << std::endl << std::endl;
				}
			}
			else{
				std::cout << p << " exists, but is neither a regular file nor a directory" << std::endl;
			}
		}
		else{
			std::cout << p << " does not exist" << std::endl;
		}
	}
	catch (const boost::filesystem::filesystem_error& ex){
		std::cout << ex.what() << std::endl;
	}
}

void createstorage(boost::filesystem::path p){
	try{
		if (exists(p)) {  // does p actually exist?
			if (is_regular_file(p)){  // is p a regular file?
        std::cout << p << " size is " << file_size(p) << std::endl; //if it is output the filesize
			}
			else if (is_directory(p)){      // is p a directory?
				std::cout << p << " is a directory containing:" << std::endl;

				copy(boost::filesystem::directory_iterator(p), boost::filesystem::directory_iterator(), std::back_inserter(w)); //std::copy (inputfirst, inputlast, output)

				sort(w.begin(), w.end()); //std::sort, since directory iteration is not ordered on some file systems

				for (vec::iterator it2 (w.begin()); it2 != w.end(); ++it2){ //Iteration cycles through Vector W
					if (is_directory(*it2)){
						it2 = w.erase(it2);
						it2--;
					}
				}
				for (vec::iterator it2 (w.begin()); it2 != w.end(); ++it2){ //Iteration cycles through Vector W
					std::cout << (*it2).filename() << " was the latest added File!" << std::endl << std::endl;
				}
			}
			else{
				std::cout << p << " exists, but is neither a regular file nor a directory" << std::endl;
			}
		}
		else{
			if (create_directory(p)) { //Checks if creation of Directories Succeeds
				std::cout << "Success creating Directory called " << p << std::endl; //Outputs Success Message
			}
		}
	}
	catch (const boost::filesystem::filesystem_error& ex){
		std::cout << ex.what() << std::endl;
	}
}

//-----------------------------------------------SYMLINK-MAGIC-------------------------------------//
void symlinking(boost::filesystem::path sym){
	try{
		if (exists(sym)) {  // does sym actually exist?
			if (is_symlink(sym)){      // is sym a directory?
				std::cout << sym << " is a Symlink !" << std::endl;
			}
			else if (is_regular_file(sym)){  // is sym a regular file?
        		std::cout << sym << " size is " << file_size(sym) << std::endl; //if it is output the filesize
			}
			else{
				std::cout << sym << " exists, but is neither a regular file nor a directory" << std::endl;
			}
		}
		else{
			create_symlink(Desktop, OriginalPath);
			std::cout << sym << " does not exist" << std::endl;
		}
	}
	catch (const boost::filesystem::filesystem_error& ex){
		std::cout << ex.what() << std::endl;
	}
}
//--------------------------------------------------------------------------------------------------//



/*----------------------------Testing under Linux------------------------------------------------------//
std::string suffix = "\\Documents\\NBGI\\DarkSouls\\";
!!!
std::string PreMain(FinalPath.string() + "DarkSoulsAutoSave\\");
!!!
maxi << MainDir.string() << year << "\\"; //Adds Year to MainDir Path
Year = maxi.str();
maxi.str(std::string()); //Clears the Stringstream
mini << Year.string() << month << "\\"; //Adds Month to Year Path
Month = mini.str();
mini.str(std::string()); //Clears the Stringstream

to

std::string suffix = "/Documents/NBGI/DarkSouls/";
!!!
std::string PreMain(FinalPath.string() + "DarkSoulsAutoSave/");
!!!
maxi << MainDir.string() << year << "/"; //Adds Year to MainDir Path
Year = maxi.str();
maxi.str(std::string()); //Clears the Stringstream
mini << Year.string() << month << "/"; //Adds Month to Year Path
Month = mini.str();
mini.str(std::string()); //Clears the Stringstream
---------------------------------------------------------------------------------------------------*///
