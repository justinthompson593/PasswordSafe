//
//  main.cpp
//  PasswordSafe
//
//  Created by Justin Thompson on 8/30/17.
//  Copyright © 2017 Justin Thompson. All rights reserved.
//
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>

using namespace std;

void installPasswordSafe();
void encryptPasswords();
void decryptPasswords();



int main(int argc, char* argv[]){
	// Check for install flag, install & return if it's 1st arg
	if(argc == 2){
		if( strncmp(argv[1], "--install", 9) == 0 ){
			installPasswordSafe();
			return 0;
		}
	}
	
	
	char toSystem[4096];
	
	cout << "What would you like to do?" << endl;
	cout << "1. Open safe to see passwords" << endl;
	
	string usrIn;
	cin >> usrIn;
	
	switch (stoi(usrIn)) {
  case 1:
		{
			sprintf(toSystem, "gpg $HOME/PasswordSafe/passwords.txt.gpg && echo \"$(<$HOME/PasswordSafe/passwords.txt)\" && rm -f $HOME/PasswordSafe/passwords.txt");
			system(toSystem);
		}
			break;
			
  default:
			break;
	}
	
	return 0;
}


void installPasswordSafe(){
	
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
		return;
	}
	else{
		sprintf(toSystem, "echo \"Installing PasswordSafe in $HOME/PasswordSafe/\"");
		system(toSystem);
	}
	
	// make directory & build executable in it
	sprintf(toSystem, "mkdir $HOME/PasswordSafe && cd $HOME/PasswordSafe && curl -o pwdSafe.cpp https://raw.githubusercontent.com/justinthompson593/PasswordSafe/master/PasswordSafe/main.cpp && g++ pwdSafe.cpp -std=c++11 -o PasswordSafe && rm -f pwdSafe.cpp && touch passwords.txt");
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
}


void decryptPasswords(){
	char sysBuf[4096];
	sprintf(sysBuf, "gpg passwords.txt.gpg");
	system(sysBuf);
}

void encryptPasswords(){
	char sysBuf[4096];
	sprintf(sysBuf, "rm passwords.txt.gpg");
	system(sysBuf);
	
	sprintf(sysBuf, "gpg -c --cipher-algo AES256 passwords.txt && rm passwords.txt");
	system(sysBuf);
}



