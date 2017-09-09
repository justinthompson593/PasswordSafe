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

string getGpgPassword(){
	string out;
	cout << "Enter your passphrase (the one I said to never EVER forget): ";
	cin >> out;
	return out;
}


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
	cout << "2. Add a password to safe" << endl;
	
//	string usrIn;
//	cin >> usrIn;
	sprintf(toSystem, "stty raw");
	system(toSystem);
	char usrIn = getchar();
	sprintf(toSystem, "stty cooked");
	system(toSystem);
	
	switch (usrIn) {
  case '1':
		{
			string pwd = getGpgPassword();
			sprintf(toSystem, "clear && gpg --decrypt --batch --passphrase \"%s\" $HOME/PasswordSafe/passwords.txt.gpg", pwd.c_str());
			system(toSystem);
		}
			break;
  case '2':
		{
//			sprintf(toSystem, "gpg $HOME/PasswordSafe/passwords.txt.gpg && nano $HOME/PasswordSafe/passwords.txt && rm $HOME/PasswordSafe/passwords.txt");
			string pwd = getGpgPassword();
			sprintf(toSystem, "clear && gpg --passphrase \"%s\" $HOME/PasswordSafe/passwords.txt.gpg && rm $HOME/PasswordSafe/passwords.txt.gpg && nano $HOME/PasswordSafe/passwords.txt && gpg -c --cipher-algo AES256 --passphrase \"%s\" $HOME/PasswordSafe/passwords.txt && rm $HOME/PasswordSafe/passwords.txt", pwd.c_str(), pwd.c_str());
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
	sprintf(toSystem, "mkdir $HOME/PasswordSafe && cd $HOME/PasswordSafe && curl -o pwdSafe.cpp https://raw.githubusercontent.com/justinthompson593/PasswordSafe/master/PasswordSafe/main.cpp && g++ pwdSafe.cpp -std=c++11 -o PasswordSafe && rm -f pwdSafe.cpp && echo \"______________Password Safe______________\" > passwords.txt && echo \"\" >> passwords.txt");
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



