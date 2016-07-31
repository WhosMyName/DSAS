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
#include <Windows.h>
#include <WinBase.h> 

static boost::filesystem::path CallMyName; //Is Path of Executable including it's Name
boost::filesystem::path CurrentPath = boost::filesystem::current_path(); //Gets The Current Path, which contains the Program
boost::filesystem::path Desktop; //Path which leads to Desktop
boost::filesystem::path Symlink; //Symlink to DSAS3 on Desktop
boost::filesystem::path DSSymlink; //Symlink to DS3 on Desktop
boost::filesystem::path DS3; //Path which leads to Roaming\\DarkSouls3
boost::filesystem::path MainDir; //Path where all Saves are Stored aka DarkSoulsAutoSave 2 & 3
boost::filesystem::path Year; // DarkSoulsAutoSave3\\2016
boost::filesystem::path Month; // DarkSoulsAutoSave3\\2016\\May

std::string year;
std::string month;
std::string timestamp;
std::string dsname = "DS30000.sl2";

int mint; //Minutes to compare
int premint; //Minutes to compare
bool inplace = false;
bool thread_running = false;


typedef std::vector<boost::filesystem::path> vec; // store paths, so we can store and sort them later
vec v; //Vector for Source-File
vec w; //Vector for Destination-Directory and Sorting

void getDir(boost::filesystem::path p) { //Basically trying to find out where to search for the Saves
	boost::filesystem::path temp = p; //PreVariable to Cycle Back the Path
	boost::filesystem::path pretemp; //PrePreVariable to Cycle Back the Path, should contain the User Folder
	std::string stemp = temp.filename().string(); //Casting Path to String for Comparison
	std::string docs = "Users"; //String
	std::cout << "docs equals: " << docs << std::endl << std::endl; //Optional
	std::cout << "stemp equals: " << stemp << std::endl << std::endl; //Optional
	std::cout << "temp equals: " << temp.string() << std::endl << std::endl; //Optional
	while (stemp.compare(docs) != 0) { //Reverse Clycling through Path
		pretemp = temp; //Assignment of Paths
		std::cout << "pretemp equals: " << pretemp.string() << std::endl << std::endl; //Optional
		temp = temp.parent_path(); //Going up one Level in Path
		std::cout << "temp equals: " << temp.string() << std::endl << std::endl; //Optional
		stemp = temp.filename().string(); //Casting Path to String for Comparison - Recast
		std::cout << "stemp equals: " << stemp << std::endl << std::endl; //Optional
	}
	std::cout << "It's done! " << std::endl << std::endl; //Optional
	std::string constructpath = pretemp.string(); //Combined with suffix to create various Paths
	std::cout << "pretemp finished equals: " << pretemp.string() << std::endl << std::endl; //Optional

	std::string suffix = "\\AppData\\Roaming\\DarkSoulsIII"; //Path to DS Savepath
	std::string Conv(constructpath + suffix); //Converted String Equals renamed Folders
	DS3 = Conv; // Cast String to Path

	std::string suffix2 = "\\Desktop"; //Path to Desktop
	std::string Conv2(constructpath + suffix2); //Combines constructpath and suffix2
	Desktop = Conv2; //Converts String to Path

	std::string suffix3 = "\\Desktop\\DSAS3"; //Symlink to DSAS3
	std::string Conv3(constructpath + suffix3); //Combines constructpath and suffix2
	Symlink = Conv3; //Converts String to Path

	std::string suffix4 = "\\Desktop\\DarkSoulsIII"; //Symlink to DSAS3
	std::string Conv4(constructpath + suffix4); //Combines constructpath and suffix2
	DSSymlink = Conv4; //Converts String to Path

	std::cout << DS3.string() << " is the estimated Name of the Directory!" << std::endl << std::endl; //Optional
	std::cout << Desktop.string() << std::endl;
}

void makemain() { //Creating The MainDirectory
	std::string PreMain(DS3.string() + "\\DarkSoulsAutoSave3");
	MainDir = PreMain;
	if (boost::filesystem::create_directory(MainDir)) { //Checks if creation of Directories Succeeds
		std::cout << "Success creating Directory called " << MainDir << std::endl; //Outputs Success Message
	}
}

void gettime() {
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

	maxi << MainDir.string() << "\\" << year ; //Adds Year to MainDir Path
	Year = maxi.str();
	maxi.str(std::string()); //Clears the Stringstream
	mini << Year.string() << "\\" << month; //Adds Month to Year Path
	Month = mini.str();
	mini.str(std::string()); //Clears the Stringstream

	std::cout << "Year equals: " << Year.string() << std::endl << std::endl; //Optional
	std::cout << "Month equals: " << Month.string() << std::endl << std::endl; //Optional

}

void showvecs() { //Optional
	std::cout << "<-----------------------V-------------------------------->" << std::endl << std::endl;
	for (vec::iterator it(v.begin()); it != v.end(); ++it) { //Iteration cycles through Vector V
		std::cout << (*it).filename() << " was the latest added Directory!" << std::endl << std::endl;
	}
	std::cout << "<------------------------W------------------------------->" << std::endl << std::endl;
	for (vec::iterator it2(w.begin()); it2 != w.end(); ++it2) { //Iteration cycles through Vector W
		std::cout << (*it2).filename() << " was the latest added Directory!" << std::endl << std::endl;
	}
	std::cout << "<------------------------X------------------------------->" << std::endl << std::endl;
}

//................................................................................................\\

void clearvecs() { //Clears the Vectors to prevent File Duplications, or whatever the Bug I encountered was (hopefully)

	v.erase(v.begin(), v.end());
	w.erase(w.begin(), w.end());
}

//................................................................................................\\

void hitandrun() { //Thats the actual Save-Function

	std::cout << "It has begun!" << std::endl << std::endl;

	for (vec::iterator it(v.begin()); it != v.end(); ++it) {

		boost::filesystem::path Sourcefile = (*it);
		std::stringstream ss3;
		ss3.str(std::string());

		std::cout << Month << std::endl;
		std::cout << Sourcefile << std::endl;

		ss3 << Month.string() << "\\" << Sourcefile.filename().string();
		boost::filesystem::path Destinationfile = ss3.str();
		std::cout << "Destinationfile: " << Destinationfile << std::endl; //Optional
		boost::filesystem::copy_file(Sourcefile, Destinationfile);
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
		boost::filesystem::rename(Destinationfile, Renomnom);
	}

	std::cout << "Outside Loop!" << std::endl; //Optional

	w.erase(w.begin(), w.end());
}

void timegate() {// gets the minutes and uses them for comparison to sync the saves in a X minute cycle
	std::stringstream minutes;
	boost::posix_time::time_facet *facetminutes = new boost::posix_time::time_facet("%M");
	minutes.imbue(std::locale(minutes.getloc(), facetminutes));
	minutes << boost::posix_time::second_clock::local_time();
	mint = atoi(minutes.str().c_str());
	minutes.str(std::string()); //Clears the Stringstream

}

void threadnochill() {
	thread_running = true;
	std::cin.ignore();
	thread_running = false;
}

void checkexist(boost::filesystem::path p);

void createstorage(boost::filesystem::path p);

void symlinking(boost::filesystem::path sym, boost::filesystem::path dest, int type);

void verifysave(boost::filesystem::path p);

//##################################################################################################################################################################//
int main(int argc, char** argv) {

	if (BOOST_OS_WINDOWS) {
		std::cout << "Windows" << std::endl;
	
	CallMyName = boost::filesystem::system_complete(argv[0]);

	clearvecs();
	getDir(CurrentPath);
	std::cout << CallMyName.parent_path() << std::endl; //Optional
	std::cout << DS3.filename() << std::endl; //Optional
	std::cout << CallMyName.string().find(DS3.filename().string()) << std::endl; //Optional

	if (CallMyName.string().find(DS3.filename().string()) == std::string::npos) {
		symlinking(DSSymlink, DS3, 1);
		std::cout << "DSAS3 is not in the DankSouls-Folder, please move it to " << DS3 << ". I have automagically created a Symlink for that on your Desktop!" << std::endl;
		std::cout << "Press Enter to Leave and Restart DSAS3 after moving!" << std::endl;
		std::cin.ignore();
		return 0;
	}

	symlinking(Symlink, CallMyName, 0);
	verifysave(CurrentPath); //Because lies in HUGENUMBERS in
	//MAINPROCS
	makemain(); //Should be in DS3\\DarkSoulsAutoSave3
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

	while (thread_running) { //Starts a 3 Variables Swap with Time Comparison and Management
		timegate();
		if (premint + 15 >= 60) {
			premint = premint - 45;
		}
		if (mint == premint + 45) {
			premint = mint;
			showvecs();
			gettime();
			hitandrun();
			std::cout << "Hit Enter to exit DSAS..." << std::endl;
		}
		boost::this_thread::sleep(boost::posix_time::seconds(10)); //Lets The Process sleep to reduce CPU Load
	}

	std::cout << "Saving before Exiting!" << std::endl;
	gettime();
	hitandrun();
	std::cout << "Exiting!" << std::endl;
	thread.join();
	return 0;

	}
	else if (BOOST_OS_LINUX) {
		std::cout << "This is a Windows only Application!" << std::endl;
	}
	else {
		std::cout << "This is a Windows only Application!" << std::endl;
	}
}
//##################################################################################################################################################################//

void checkexist(boost::filesystem::path p) {
	try {
		if (exists(p)) {  // does p actually exist?
			if (is_regular_file(p)) {  // is p a regular file?
				std::cout << p << " size is " << file_size(p) << std::endl; //if it is output the filesize
			}
			else if (is_directory(p)) {      // is p a directory?
				std::cout << p << " exists!" << std::endl;

			}
			else {
				std::cout << p << " exists, but is neither a regular file nor a directory" << std::endl;
			}
		}
		else {
			std::cout << p << " does not exist" << std::endl;
		}
	}
	catch (const boost::filesystem::filesystem_error& ex) {
		std::cout << ex.what() << std::endl;
	}
}

void createstorage(boost::filesystem::path p) {
	try {
		if (boost::filesystem::exists(p)) {  // does p actually exist?
			if (boost::filesystem::is_regular_file(p)) {  // is p a regular file?
				std::cout << p << " size is " << boost::filesystem::file_size(p) << std::endl; //if it is output the filesize
			}
			else if (boost::filesystem::is_directory(p)) {      // is p a directory?
				std::cout << p << " exists!" << std::endl;
			}
			else {
				std::cout << p << " exists, but is neither a regular file nor a directory" << std::endl;
			}
		}
		else {
			if (boost::filesystem::create_directory(p)) { //Checks if creation of Directories Succeeds
				std::cout << "Success creating Directory called " << p << std::endl; //Outputs Success Message
			}
		}
	}
	catch (const boost::filesystem::filesystem_error& ex) {
		std::cout << ex.what() << std::endl;
	}
}

//-----------------------------------------------SYMLINK-MAGIC-------------------------------------//

void symlinking(boost::filesystem::path sym, boost::filesystem::path dest, int type) { //Tricky McMicky Pseudo-Create Symlink, check against Existance of Sym and create if not existing
	try {
		if (boost::filesystem::exists(sym)) {  // does sym actually exist?
			if (boost::filesystem::is_symlink(sym)) {      // is sym a directory?
				std::cout << sym << " is a Symlink!" << std::endl;
			}
			else if (boost::filesystem::is_regular_file(sym)) {  // is sym a regular file?
				std::cout << sym << " size is " << boost::filesystem::file_size(sym) << std::endl; //if it is output the filesize
			}
			else {
				std::cout << sym << " exists, but is neither a regular Symlink nor a directory" << std::endl;
			}
		}
		else {
			std::cout << sym << " does not exist! Creating new Symlink!" << std::endl;
			//General Syntax = (Outputfile, Symlink-Destination, 0);
			//The Last Flag stands either for File (0) or Directory (1)
			//To convert Boost Path to LPCWSTR just add "path".wstring().c_str()
			CreateSymbolicLink(sym.wstring().c_str(), dest.wstring().c_str(), type); //Creates a Symbolic Link from DSAS3 to Desktop
			std::cout << "Sheeesh!" << std::endl; //Optional
			if (GetLastError() != 0) { //Checks if LastError (Symlinkcreation) is not "0" 
				std::cout << "GetLastError returned: \"" << GetLastError() << "\"" << std::endl;
				std::cout << "Please make sure you Run DSAS3 with Administrator Privileges !" << std::endl;
				std::cout << "If the Errorcode is not \"0\" or \"1314\", please raise an Issue or Contact me directly!" << std::endl;
				std::cin.ignore();
			}
		}
	}
	catch (const boost::filesystem::filesystem_error& ex) {
		std::cout << ex.what() << std::endl;
	}
}

//--------------------------------------------------------------------------------------------------//

void verifysave(boost::filesystem::path p) {
	try {
		if (boost::filesystem::exists(p)) {  // does p actually exist?
			if (boost::filesystem::is_regular_file(p)) {  // is p a regular file?
				std::cout << p << " size is " << boost::filesystem::file_size(p) << std::endl; //if it is output the filesize
			}
			else if (boost::filesystem::is_directory(p)) {      // is p a directory?
				std::cout << p << " is a directory containing:" << std::endl;

				std::copy(boost::filesystem::directory_iterator(p), boost::filesystem::directory_iterator(), std::back_inserter(v)); //std::copy (inputfirst, inputlast, output)

				std::sort(v.begin(), v.end()); //std::sort, since directory iteration is not ordered on some file systems

				for (vec::iterator it(v.begin()); it != v.end(); ++it) { //Iteration cycles through Vector V
					if (boost::filesystem::is_directory(*it)) {
						it = v.erase(it);
						--it;
					}
				}

				for (vec::iterator it(v.begin()); it != v.end(); ++it) { //Cycles through the Vector once more to verify that ONLY the Original
					if (dsname.find((*it).filename().string()) != std::string::npos) { //Savefile is included to prevent copying of random Files
						v.clear();
						boost::filesystem::path path1 = dsname;
						v.insert(v.begin(), path1);
						break;
					}
				}

				for (vec::iterator it(v.begin()); it != v.end(); ++it) { //Cycles through the Vector once more to verify that ONLY the Original
					if (dsname.find((*it).filename().string()) == std::string::npos) { //Savefile is included to prevent copying of random Files
						exit(1);
					}
				}

				for (vec::iterator it(v.begin()); it != v.end(); ++it) { //Iteration cycles through Vector V
					std::cout << (*it).filename() << " was the latest added File!" << std::endl << std::endl;
				}
				std::cout << "end" << std::endl << std::endl;
			}
			else {
				std::cout << p << " exists, but is neither a regular file nor a directory" << std::endl;
			}
		}
		else {
			std::cout << p << " does not exist" << std::endl;
		}
	}
	catch (const boost::filesystem::filesystem_error& ex) {
		std::cout << ex.what() << std::endl;
	}
}

