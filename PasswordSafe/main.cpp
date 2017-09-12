//
//  main.cpp
//  PasswordSafe
//
//  Created by Justin Thompson on 8/30/17.
//  Copyright © 2017 Justin Thompson. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <termios.h>
#include <unistd.h>

using namespace std;

int installPasswordSafe();
int updatePasswordSafe();
void encryptPasswords();
void decryptPasswords();
void addPasswords(string pWord);
string getGpgPassword();
bool is_number(const string & s);
string processPassword(string pWord);


string password;
char toSystem[4096];
char usrIn;
string userIn;


int main(int argc, char* argv[]){
	
	// Check for install flag, install & return if it's 1st arg
	if(argc == 2){
		if( strncmp(argv[1], "--install", 9) == 0 ){
			return installPasswordSafe();
		}
		
		if( strncmp(argv[1], "--update", 8) == 0 ){
			return updatePasswordSafe();
		}
	}


	password = getGpgPassword();
	
	
	
MENU:
	
	cout << endl << "What would you like to do?" << endl;
	cout << "1. Open safe to see all passwords" << endl;
	cout << "2. Print titles" << endl;
	cout << "3. Print information for a specific title" << endl;
	cout << "4. Copy password from a specific title" << endl;
	cout << "5. Add passwords to the safe" << endl;
	cout << "6. Edit passwords.txt with nano" << endl;
	
	cout << endl << "Enter a number (1-5) or q to quit: ";
	cin >> userIn;
	

	
	switch (is_number(userIn) ? stoi(userIn) : 0) {
  case 1:
		{	// print entire passwords.txt with batch
			sprintf(toSystem, "clear && gpg --decrypt --batch --passphrase \"%s\" $HOME/PasswordSafe/passwords.txt.gpg", password.c_str());
			system(toSystem);
			goto MENU;
		}
			break;
  case 2:
		{
			sprintf(toSystem, "echo \"$(gpg --decrypt --batch --passphrase \"%s\" $HOME/PasswordSafe/passwords.txt.gpg)\" | sed -n '/^$/{n;p;}'", password.c_str());
			system(toSystem);
			goto MENU;
		}
  case 3:
		{
			string srchQ;
			cout << "Enter the title: ";
			cin >> srchQ;
			sprintf(toSystem, "echo \"$(gpg --decrypt --batch --passphrase \"%s\" $HOME/PasswordSafe/passwords.txt.gpg)\" | sed -n '/%s/,/^$/p'", password.c_str(), srchQ.c_str());
			system(toSystem);
			goto MENU;
		}
			break;
  case 4:
		{
			string srchQ;
			cout << "Enter the title: ";
			cin >> srchQ;
			sprintf(toSystem, "echo \"$(gpg --decrypt --batch --passphrase \"%s\" $HOME/PasswordSafe/passwords.txt.gpg)\" | sed -n '/%s/,/^$/p' > thisTitlesInfo", password.c_str(), srchQ.c_str());
			system(toSystem);
			
			ifstream infoFile("thisTitlesInfo");
			string line;
			while( getline(infoFile, line) ){
				if( line.compare(0,5,"pword") == 0 ){
					unsigned long idx = line.find(" ");
//					cout << "Your password should be " << line.substr(idx+1) << endl;
#if defined(__APPLE__) && defined(__MACH__)
					sprintf(toSystem, "printf %s | pbcopy", line.substr(idx+1).c_str());
					system(toSystem);
#elif defined(__unix__) || defined(__linux__)
					sprintf(toSystem, "printf %s > pwdSafeTemp && xsel --clipboard < pwdSafeTemp && rm -f pwdSafeTemp", line.substr(idx+1).c_str());
					system(toSystem);
#endif
					cout << endl << "Password for " << srchQ << " copied to clipboard" << endl;
				}
			}
			infoFile.close();
			sprintf(toSystem, "rm -f thisTitlesInfo");
			system(toSystem);
			goto MENU;
		}
			break;
  case 5:
		{
			addPasswords(password);
			goto MENU;
		}
			break;
  case 6:
		{
			sprintf(toSystem, "clear && gpg --passphrase \"%s\" $HOME/PasswordSafe/passwords.txt.gpg && rm $HOME/PasswordSafe/passwords.txt.gpg && nano $HOME/PasswordSafe/passwords.txt && gpg -c --cipher-algo AES256 --passphrase \"%s\" $HOME/PasswordSafe/passwords.txt && rm $HOME/PasswordSafe/passwords.txt", password.c_str(), password.c_str());
			system(toSystem);
			goto MENU;
		}
			break;
  default:
		{
			cout << "Quitting PasswordSafe." << endl;
		}
			break;
	}
	
	
	return 0;
}

int updatePasswordSafe(){
	char toSystem[2048];
	sprintf(toSystem, "cd $HOME/PasswordSafe && curl -o update.cpp https://raw.githubusercontent.com/justinthompson593/PasswordSafe/master/PasswordSafe/main.cpp && g++ update.cpp -std=c++11 -o PasswordSafe && rm -f update.cpp && echo \"\n\nUpdate successful! You are now using the newest version of PasswordSafe.\n\"");
	system(toSystem);
	
	return 0;
}


int installPasswordSafe(){
	
	char toSystem[4096];
	char userIn;
	
	sprintf(toSystem, "echo \"\n******Installing Password Safe******\nHit any key to continue or q to quit\n\"");
	system(toSystem);
	sprintf(toSystem, "stty raw");
	system(toSystem);
	userIn = getchar();
	sprintf(toSystem, "stty cooked");
	system(toSystem);
	
	if( userIn == 'q' || userIn == 'Q' ){
		cout << "Installation aborted. Have a nice day." << endl << endl;
		return 8;
	}
	else{
		sprintf(toSystem, "echo \"Installing PasswordSafe in $HOME/PasswordSafe/\"");
		system(toSystem);
	}
	
	// make directory & build executable in it
	sprintf(toSystem, "mkdir $HOME/PasswordSafe && cd $HOME/PasswordSafe && curl -o pwdSafe.cpp https://raw.githubusercontent.com/justinthompson593/PasswordSafe/master/PasswordSafe/main.cpp && g++ pwdSafe.cpp -std=c++11 -o PasswordSafe && rm -f pwdSafe.cpp && echo \"________________Password Safe________________\n\" > passwords.txt");
	system(toSystem);
	
	
	sprintf(toSystem, "echo \"Hit any key to add PasswordSafe to your path or c to cancel\n\"");
	system(toSystem);
	sprintf(toSystem, "stty raw");
	system(toSystem);
	userIn = getchar();
	sprintf(toSystem, "stty cooked");
	system(toSystem);
	
	if( userIn != 'c' && userIn != 'C' ){
#if defined(__APPLE__) && defined(__MACH__)
		sprintf(toSystem, "echo \"export PATH=$PATH:$HOME/PasswordSafe\" >> $HOME/.bash_profile");
		system(toSystem);
#elif defined(__unix__) || defined(__linux__)
		sprintf(toSystem, "echo \"export PATH=$PATH:$HOME/PasswordSafe\" >> $HOME/.bashrc");
		system(toSystem);
#endif
	}
	
	sprintf(toSystem, "echo \"Encrypting passwords.txt\nGet ready to choose a master passphrase for your safe. You will be asked to type it in twice. Do not forget it. If you do, the seas will boil, the sun will become black as sackcloth, and the moon as blood. Earthquakes. Famine. Plagues. Cats and dogs; living together. Mass hysteria.\n\nBut seriously, it is impossible to recover your passwords without this passphrase. Do not forget it. Hit any key to continue.\n\"");
	system(toSystem);
	sprintf(toSystem, "stty raw");
	system(toSystem);
	userIn = getchar();
	sprintf(toSystem, "stty cooked");
	system(toSystem);
	
	encryptPasswords();
	
	sprintf(toSystem, "echo \"Installation successful! If you chose to add PasswordSafe to your path, you'll need to close this terminal and open a new one. Then run the command 'PasswordSafe' from your terminal. Otherwise, run '$HOME/PasswordSafe/PasswordSafe'.\"");
	system(toSystem);
	
	return 0;
}


void decryptPasswords(){
	char sysBuf[512];
	sprintf(sysBuf, "gpg $HOME/PasswordSafe/passwords.txt.gpg");
	system(sysBuf);
}

void encryptPasswords(){
	char sysBuf[512];
	sprintf(sysBuf, "rm $HOME/PasswordSafe/passwords.txt.gpg 2> /dev/null");
	system(sysBuf);
	
	sprintf(sysBuf, "gpg -c --cipher-algo AES256 $HOME/PasswordSafe/passwords.txt && rm $HOME/PasswordSafe/passwords.txt");
	system(sysBuf);
}

void addPasswords(string pWord){
	string uIn;
	
	char sysBuf[2048];
	sprintf(sysBuf, "gpg --passphrase \"%s\" $HOME/PasswordSafe/passwords.txt.gpg && echo $HOME/PasswordSafe/passwords.txt > PwdSafePathAndFile", pWord.c_str());
	system(sysBuf);
	
	ifstream pathIn("PwdSafePathAndFile");
	string PathAndFile((istreambuf_iterator<char>(pathIn)),istreambuf_iterator<char>());
	PathAndFile = PathAndFile.substr(0,PathAndFile.length()-1);
	
	fstream pwds;
	pwds.open(PathAndFile.c_str(), ios::app);
	
	bool keepGoing = true;
	while(keepGoing){
	
		if(pwds.is_open()){
			cout << "Enter a one-word title for this password (first-gmail, NewBank, phone): ";
			cin >> uIn;
			string thisTitle = uIn;
			pwds << uIn << endl;
			cout << endl << "Enter your username (it may be an email in some cases): ";
			cin >> uIn;
			pwds << "uname: " << uIn << endl;
			cout << endl << "Enter your password: ";
			cin >> uIn;
			pwds << "pword: " << uIn << endl;
			
			cout << endl << "Would you like to add info to "<<  thisTitle << "? (i.e. a PIN #, security question, website) Enter y to add info or any other key to complete " << thisTitle << ": ";
			cin >> uIn;
			
			while( uIn.compare("y") == 0 ){
				cout << endl << "Enter a one-word label to appear under " << thisTitle << " (i.e. PIN, sec-question, Website, Contact_Name): ";
				cin >> uIn;
				pwds << uIn << ": ";
				cout << endl << "Enter the note to follow " << uIn << ". If there are multiple words, separate them-with-dashes or_with_subs): ";
				cin >> uIn;
				pwds << uIn << endl;
				
				
				cout << endl << "Enter y to add more info or any other key to complete " << thisTitle << ": ";
				cin >> uIn;
			}
			pwds << endl;
			
			cout << "Enter y to add another password to the safe or any other key to quit: ";
			cin >> uIn;
			if( uIn.compare("y") != 0 )
				keepGoing = false;
			
			cout << endl;
		}
		else{
			cout << "Could not open passwords.txt. You may have entered an invalid password." << endl;
			keepGoing = false;
		}
	
	}
	pwds.close();
	
	
	
	sprintf(sysBuf, "rm -f PwdSafePathAndFile 2> /dev/null");
	system(sysBuf);
	
	sprintf(sysBuf, "rm $HOME/PasswordSafe/passwords.txt.gpg 2> /dev/null && gpg -c --cipher-algo AES256 --passphrase \"%s\" $HOME/PasswordSafe/passwords.txt && rm $HOME/PasswordSafe/passwords.txt", pWord.c_str());
	system(sysBuf);
}


string getGpgPassword(){
	
	string out;
	cout << "Enter your passphrase (the one I said to never EVER forget): ";
	
	
	
	termios oldt;
	tcgetattr(STDIN_FILENO, &oldt);
	termios newt = oldt;
	newt.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	
	getline(cin, out);
	
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // restore terminal state
	
	cout << endl;
	
	// Process password:   $ --> \$   for bash strings
	size_t idx = out.find("$");
	if( idx != string::npos ){
		out.insert(idx, "\\");
	}
	
	
	return out;
}


// From https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c#4654718
bool is_number(const string& s)
{
	return !s.empty() && find_if(s.begin(), s.end(), [](char c) { return !isdigit(c); }) == s.end();
}
