// An simple example of creating a stream-enabled object
// This example creates a utility designed for writing out logfile entries from 
// command line arguments that takes advantage of some important stream facilities. 

#include <ctime>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;
 
// Timestamp function returns current time as a string 
string timestamp();    
 
// Forward declarations
class LogStatement;
ostream & operator<<(ostream & ost, const LogStatement & ls);
 
class LogStatement
{
	public:

		LogStatement(string s) : data(s), time_string( timestamp() ) { };
 
		// This method handles all the outputs.    
		friend ostream & operator<<(ostream &, const LogStatement &);

	private:

		string data;
		string time_string;
};
 
ostream & operator<<(ostream & ost, const LogStatement & ls)
{
        ost << "Date and time: " << ls.time_string << "\n\nData: " << ls.data << "\n";

        return ost;
}
 
string timestamp() 
{
	// Notice the use of a stringstream, yet another useful stream medium!
	tm * timeinfo;
	time_t rawtime;
	ostringstream stream;    

	time(&rawtime);
	timeinfo = localtime(&rawtime);
 
	stream << (timeinfo->tm_year) + 1900 << " " << timeinfo->tm_mon 
		<< " " << timeinfo->tm_mday <<" " << timeinfo->tm_hour 
		<< " " << timeinfo->tm_min <<" " << timeinfo->tm_sec;

	// The str() function of output stringstreams returns an string.
	return stream.str();   
}
 
int main(int argc, char** argv)
{
	if (argc < 2)
	{
		// A return of -1 denotes an error condition.
		return -1;  
	}

	ostringstream log_data;

	// Take all the char arrays in the argv (except the filename) 
	// and generate an output string stream.
	for (int i = 1; i < argc; i++)    
	{
		log_data << argv[i] << ' '; 
	}
  
	// Create a LogStatement object and initialize it with log data
	LogStatement log_entry(log_data.str());
 
	// Output contents of the log entry to the standard output stream
	cout << log_entry << endl;

	// Output contents of the log entry to the standard logging stream
	clog << log_entry << endl;

	// Open a file (in the project directory) for appending
	ofstream logfile("IOStreamsLogfile.txt", ios::app);
 
	// Check for errors opening the file
	// logfile.fail() is equivalent to simply !logfile and !logfile.good()
	if ( logfile.fail() )	
	{
		return -1;
	}  
 
	// Output contents of the log entry to the file output stream
	logfile << log_entry << endl;

	// Close the file output stream
	logfile.close();

	cin.get();
 
	return 0;
}