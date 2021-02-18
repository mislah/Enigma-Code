#include<iostream>
#include<fstream>
#include<conio.h>
#include<string.h>
#include<stdio.h>
#include<Windows.h>
#include<process.h>
using namespace std;
const char projectname[] = { "Enigma" };
const char ufile[] = { "Settings.dll" };
const char dfile[] = { "data.crypt" };
const int sleeptime = 800;
void uencrypt(char* a, int b)
{
	for (int i = 0; i < a[i] != '\0'; i++)
	{
		a[i] = a[i] % 2 * b + a[i] + b;
	}
}
class userdata
{
	char* user, * pswd;
public:
	char* duser, * dpswd;
	userdata()
	{
		user = new char[20];
		pswd = new char[20];
		duser = new char[20]{ "admin" };
		dpswd = new char[20]{ "root" };
	}
	void login()
	{
		details();
		int pass = 0;
		char* a, * b;
		a = new char[20];
		b = new char[20];
		do
		{
			system("cls");
			cout << "Enter login details : \n";
			cout << "Username : ";
			cin >> a;
			uencrypt(a, 100);
			cout << "Password : ";
			cin >> b;
			uencrypt(b, 200);
			uencrypt(b, 300);
			if (!strcmp(user, a) && !strcmp(pswd, b))
			{
				cout << "Login Successfull!";
				pass = 1;
			}
			else
				cout << "Login Failure!";
			Sleep(sleeptime);
		} while (pass != 1);
	}
	void details()
	{
		uencrypt(duser, 100);
		uencrypt(dpswd, 200);
		uencrypt(dpswd, 300);
		ifstream loga(ufile, ios::in);
		char* in;
		in = new char[20];
		loga.seekg(0, ios::end);
		if (!loga || !loga.tellg())
		{
			ofstream create_log(ufile, ios::out);
			create_log << duser << endl << dpswd;
		}
		loga.close();
		ifstream log(ufile, ios::in);
		log >> in;
		strcpy_s(user, 30, in);
		log >> in;
		strcpy_s(pswd, 30, in);
		log.close();
	}
	void changeuser()
	{
		char* nuser = new char[30];
		char* cpswd = new char[30];
		cout << "Enter new username : ";
		cin >> nuser;
		if (strlen(nuser) > 30)
		{
			cout << "Error 1A: Username must not be more than 30 characters long!";
			Sleep(sleeptime);
			return;
		}
		ifstream passbak(ufile, ios::in);
		passbak >> cpswd >> cpswd;
		uencrypt(nuser, 100);
		ofstream changeuser(ufile, ios::out);
		changeuser << nuser << endl << cpswd;
		cout << "Username changed!";
		Sleep(sleeptime);
	}
	void changepswd()
	{
		char* cuser = new char[20];
		char* npswd = new char[20];
		char* cpswd = new char[20];
		cout << "Enter current password : ";
		cin >> cpswd;
		uencrypt(cpswd, 200);
		uencrypt(cpswd, 300);
		if (!strcmp(pswd, cpswd))
		{
			ifstream userbak(ufile, ios::in);
			userbak >> cuser;
			cout << "Enter new password : ";
			cin >> npswd;
			if (strlen(pswd) > 30)
			{
				cout << "Error 1B: Password must not be more than 30 characters long!";
				Sleep(sleeptime);
				return;
			}
			uencrypt(npswd, 200);
			uencrypt(npswd, 300);
			ofstream changeuser(ufile, ios::out);
			changeuser << cuser << endl << npswd;
			cout << "Password changed!";
		}
		else
			cout << "The current password entered is incorrect!";
		Sleep(sleeptime);
	}
	long key()
	{
		long a = 0;
		for (int i = 0; user[i] != '\0'; i++)
		{
			a = a + ((int)user[i] * (i + 1));
		}
		return a;
	}
}U;
void encrypt()
{
	int e, * aint, * part1, * part2, line = 0;
	long c = U.key();
	char* a = new char[1]();
	char filename[30];
	cout << "Note : The file must be in the folder ";
	system("cd");
	cout << "Please enter filename including extention : ";
	cin >> filename;
	if (!strcmp(filename, dfile))
	{
		cout << "Error 2A: Reserved filename!";
		Sleep(sleeptime);
		return;
	}
	ifstream checkfile(dfile, ios::in);
	if (checkfile)
	{
		checkfile.close();
		remove(dfile);
	}
	checkfile.close();
	ifstream datain(filename, ios::in);
	ofstream dataout(dfile, ios::out | ios::app);
	if (!datain)
	{
		cout << "Error 3A: The file does not exist!";
		Sleep(sleeptime);
		return;
	}
	cout << "Enter encryption key (a non zero integer) : ";
	cin >> e;
	if (e == 0)
	{
		cout << "Error 0A: invalid input! ";
		Sleep(sleeptime);
		return;
	}
	while (!datain.eof())
	{
		datain.getline(a, 1000);
		aint = new int[strlen(a)];
		part1 = new int[strlen(a)];
		part2 = new int[strlen(a)];
		for (int i = 0; i < strlen(a); i++)
		{
			aint[i] = (int)a[i];
			part1[i] = aint[i] / e + c;
			part2[i] = aint[i] % e + c;
			dataout << part1[i] << endl;
			dataout << part2[i] << endl;
			line++;
		}
		int a = (int)'\n';
		dataout << (a / e) + c << endl;
		dataout << (a % e) + c << endl;
		line++;
	}
	datain.close();
	dataout.close();
	char filenameout[30];
	cout << "Enter filename to save encrypted data : ";
	cin >> filenameout;
	if (!strcmp(filenameout, dfile))
	{
		cout << "File saved successfully in the folder ";
		system("cd");
		cout << "Press any key to continue...";
		return;
	}
	ifstream fileout(filenameout, ios::in);
	ifstream datain2(dfile, ios::in);
	ofstream dataout2(filenameout, ios::out);
	if (fileout)
	{
		char ch;
		cout << filenameout << " already exist, do you want to replace it (y/n)? ";
		cin >> ch;
		if (ch == 'y' || ch == 'Y')
		{
			fileout.close();
			remove(filenameout);
			for (int i = 0; i < 2 * line - 3; i++)
			{
				datain2 >> a;
				dataout2 << a << endl;
			}
			datain2.getline(a, 1000);
			dataout2 << a;
			datain2.close();
			dataout2.close();
			remove(dfile);
			cout << "File saved successfully in the folder ";
			system("cd");
			cout << "Press any key to continue...";
		}
		else
		{
			fileout.close();
			cout << "Error 4A: File cannot be saved!";
		}
	}
	else
	{
		fileout.close();
		for (int i = 0; i < 2 * line - 3; i++)
		{
			datain2 >> a;
			dataout2 << a << endl;
		}
		datain2 >> a;
		dataout2 << a;
		datain2.close();
		dataout2.close();
		remove(dfile);
		cout << "File saved sucessfully in the folder ";
		system("cd");
		cout << "Press any key to continue...";
	}
}
void decrypt()
{
	int d;
	char filename[30];
	char* a = new char[1]();
	long c = U.key();
	cout << "Note : The file must be in the folder ";
	system("cd");
	cout << "Please enter file name including extention : ";
	cin >> filename;
	cout << "Enter decryption key (a non zero integer) : ";
	cin >> d;
	ifstream data(filename, ios::in);
	if (!data)
	{
		cout << "Error 3A: The file does not exist!";
		Sleep(sleeptime);
	}
	else
	{
		int part1, part2, aint;
		char a1, ch;
		ofstream dataout(dfile, ios::out);
		cout << "Decrypted Data : \n";
		while (!data.eof())
		{
			data >> part1 >> part2;
			aint = (part1 - c) * d + (part2 - c);
			a1 = (char)aint;
			cout << a1;
			dataout << a1;
		}
		cout << "\n\nDo you want to save decrypted data (y/n)? :";
		cin >> ch;
		if (ch == 'Y' || ch == 'y')
		{
			char filenameout[30];
			cout << "Enter filename to save decrypted data : ";
			cin >> filenameout;
			ifstream checkfile(filenameout, ios::in);
			if (checkfile)
			{
				char ch;
				cout << filenameout << " already exist, do you want to replace it (y/n)? ";
				cin >> ch;
				if (ch == 'y' || ch == 'Y')
				{
					checkfile.close();
					dataout.close();
					remove(filenameout);
					rename(dfile, filenameout);
				}
			}
			else
			{
				checkfile.close();
				dataout.close();
				rename(dfile, filenameout);
			}
		}
		else
		{
			dataout.close();
			remove(dfile);
		}
	}
}
void view()
{
	system("cls");
	char* filename = new char[1]();
	char* a = new char[1]();
	cout << "Note : The file must be in the folder ";
	system("cd");
	cout << "Enter filename to view : ";
	cin >> filename;
	ifstream data(filename, ios::in);
	if (!data)
	{
		cout << "Error 3A: The file does not exist!";
		return;
	}
	ifstream file(filename, ios::in);
	cout << "Content of the file :\n";
	while (!file.eof())
	{
		file.getline(a, 1000);
		cout << a << endl;
	}
}
void main()
{
	system("color f0");
	system("title Enigma");
	int ch;
	U.login();
	do
	{
		system("cls");
		cout << projectname << " Menu\n";
		cout << "1-Encryption\n2-Decryption\n3-View file\n4-Account Settings\n5-Help\n6-Exit\n";
		cout << "Enter an input : ";
		cin >> ch;
		switch (ch)
		{
		case 1:
			int ch1;
			do
			{
				system("cls");
				cout << "Encryption Menu\n";
				cout << "1-Import file\n2-Go to main menu\nEnter an input : ";
				cin >> ch1;
				switch (ch1)
				{
				case 1:
					encrypt();
					_getch();
					ch1 = 2;
					break;
				case 2:
					break;
				default:
					cout << "Error 0A: Invalid Input!";
					Sleep(sleeptime);
				}
			} while (ch1 != 2);
			break;
		case 2:
			int ch2;
			do
			{
				system("cls");
				cout << "Decryption Menu\n";
				cout << "1-Import file\n2-Go to main menu\nEnter an input : ";
				cin >> ch2;
				switch (ch2)
				{
				case 1:
					decrypt();
					ch2 = 2;
					break;
				case 2:
					break;
				default:
					cout << "Error 0A: Invalid Input!";
					Sleep(sleeptime);
				}
			} while (ch2 != 2);
			break;
		case 3:
			view();
			_getch();
			break;
		case 4:
			int ch4;
			do
			{
				system("cls");
				cout << "Account Settings\n";
				cout << "1-Change username\n2-Change password\n3-Go to main menu\nEnter an input : ";
				cin >> ch4;
				switch (ch4)
				{
				case 1:
					U.changeuser();
					ch4 = 3;
					break;
				case 2:
					U.changepswd();
					ch4 = 3;
					break;
				case 3:
					break;
				default:
					cout << "Error 0A: Invalid Input!";
					Sleep(sleeptime);
				}
			} while (ch4 != 3);
			break;
		case 5:
			int ch5;
			do
			{
				system("cls");
				cout << "Help Menu\n";
				cout << "1-Help\n2-Error List\n3-Go to main menu\nEnter an input : ";
				cin >> ch5;
				switch (ch5)
				{
				case 1:
					system("cls");
					cout << "This software is to encrypt text files and read encrypted files.";
					cout << "\nUses an advanced version of Caesar's shift cypher with a block size of 8 bits to encrypt text data.";
					cout << "\nUses a simple hash algorithm to encrypt user accounts password.";
					cout << "\nGenerates a username dependant key and asks a private key for encrypting and decrypting the files.";
					cout << "\n\nThe default username is 'admin' and password is 'root'.";
					cout << "\nChange the username and password first use.";
					cout << "\nUser can decrypt the file only with the same username which encrypted the file.";
					cout << "\nThe user can change to previous username for decrypting file encrypted with that username.";
					cout << "\nDelete the file 'Settings.dll' on the application folder to reset to default username and password.";
					cout << "\nIf the encryption key is lost, the file can't be decrypted.";
					cout << "\nThe filename must include it's extension.";
					cout << "\nView file can only view text files.";
					cout << "\n\nCredits : Mislah Rahman";
					cout << "\nPress any key to continue...";
					_getch();
					ch5 = 3;
					break;
				case 2:
					system("cls");
					cout << "Error 0A: Invalid Input!";
					cout << "\nError 1A: Username must not be more than 30 characters long!";
					cout << "\nError 1B: Password must not be more than 30 characters long!";
					cout << "\nError 2A: Reserved filename!";
					cout << "\nError 3A: The file does not exist!";
					cout << "\nError 4A: File cannot be saved!";
					cout << "\nPress any key to continue...";
					_getch();
					ch5 = 3;
					break;
				}
			} while (ch5 != 3);
			break;
		case 6:
			exit(0);
			break;
		default:
			cout << "Error 0A: Invalid Input!";
			Sleep(sleeptime);
		}
	} while (ch != 6);
	_getch();
}