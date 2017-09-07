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
	
	sprintf(toSystem, "mkdir $HOME/PasswordSafe && cd $HOME/PasswordSafe && curl -o pwdSafe.cpp ");
}






