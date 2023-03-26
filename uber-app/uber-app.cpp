#include "Classes.h"
#include "Validations.h"
#include "Menus.h"
#include <string>

int main()
{
	// TODO: any string input must not have a comma
	cout << endl
		 << "Welcome to Uber Application!" << endl;
	int opt1, opt2;

	do
	{
		opt1 = menu1();
		if (opt1 == 3)
			return 0; // exit
		opt2 = menu2();
	} while (opt2 == 3);

	string firstName, lastName, phoneNum, password, confirmPassword;
	int day, month, year;
	string nic;

	bool is_err;
	if (opt1 == 1)
	{
		do
		{
			cout << endl
				 << "Enter first name: ";
			cin >> firstName;

			cout << endl
				 << "Enter last name: ";
			cin >> lastName;

			is_err = !isNameValid(firstName) || !isNameValid(lastName);

			if (is_err)
				cerr << endl << "Error: both names must contain at least 2 and at most 30 characters" << endl;
		} while (is_err);

		do
		{
			cout << endl
				 << "Enter date of your date of birth: ";
			cin >> day;
			cout << endl
				 << "Enter month [1-12] of your date of birth: ";
			cin >> month;
			cout << endl
				 << "Enter year of your date of birth: ";
			cin >> year;

			is_err = day < 1 || day > 31 || month < 1 || month > 12 || year > 2023 || year < 1920;

			if (is_err)
				cerr << endl
					 << "Error: invalid date of birth" << endl;

			// TODO: make 2023 and 1920 into a variable
			// TODO: check if driver is of at least 18 years old
		} while (is_err);

		bool flag1;
		bool flag2;
		bool phoneNumExists;

		do
		{
			flag1 = false;
			flag2 = false;
			phoneNumExists = false;

			cout << endl
				 << "Enter phone number (start with 0): ";
			cin >> phoneNum;

			is_err = !isPhoneNumValid(phoneNum);

			if (is_err)
				cerr << endl
					 << "Error: invalid phone number" << endl;
			else
			{
				if (opt2 == 1)
					phoneNumExists = doesPhoneNumExist(phoneNum, "passengers.txt");

				else if (opt2 == 2)
					phoneNum = doesPhoneNumExist(phoneNum, "drivers.txt");
			}

			if (phoneNumExists)
			{
				cerr << endl
					 << "Error: this phone number is already in use" << endl;
			}

		} while (is_err || phoneNumExists);

		if (opt2 == 2)
		{
			do
			{
				cout << endl
					 << "Enter NIC number (without dashes): ";
				cin >> nic;

				is_err = !isValidNIC(nic);

				if (is_err)
					cerr << endl
						 << "Error: invalid NIC" << endl;
			} while (is_err);
		}

		do
		{
			cout << endl
				 << "Enter password: ";
			cin >> password;

			cout << endl
				 << "Confirm password: ";
			cin >> confirmPassword;

			if (password.length() < 6)
				cerr << endl
					 << "Error: password must be at least six characters long" << endl;

			if (password != confirmPassword)
				cerr << endl
					 << "Error: passwords do not match" << endl;

		} while (password.length() < 6 || password != confirmPassword);

		if (opt2 == 2)
		{ // driver
			int model, noOfSeats;
			string plateNum, color, type, name;

			cout << endl
				 << "Enter name of the vehicle: ";
			cin >> name;

			cout << endl
				 << "Enter model of the vehicle: ";
			cin >> model;

			cout << endl
				 << "Enter number of seats in the vehicle: ";
			cin >> noOfSeats;

			cout << endl
				 << "Enter color of the vehicle: ";
			cin >> color;

			cout << endl
				 << "Enter plate Number of the vehicle: ";
			cin >> plateNum;
			// TODO: validation of name, model, noOfSeats, color, plateNum

			type = typesMenu();

			// TODO: keep check of id and change 0 to id in next line
			Vehicle vehicle(model, noOfSeats, name, plateNum, color, type);
			Driver driver(0, day, month, year, firstName, lastName, phoneNum, password, vehicle, nic);
			driver.appendToFile();
		}
		else
		{ // passenger
			// TODO: keep check of id and change 0 to id in next line
			Passenger passenger(0, day, month, year, firstName, lastName, phoneNum, password);
			passenger.appendToFile();
		}

		// signed up
		// TODO: show main menu again
	}
	else
	{ // opt1 = 2 (login)
		// it is already stored in opt2 whether the user wants to be logged in as driver or passenger
		cout << endl
			 << "Enter phone number: " << endl;
		cin >> phoneNum;

		cout << endl
			 << "Enter password: " << endl;
		cin >> password;

		bool found = 0;
		if (opt2 == 1)
		{
			// TODO: try to remove code duplication in login
			ifstream passengers_in("passengers.txt");
			string idStr, dayStr, monthStr, yearStr, firstNameStr, lastNameStr, phoneNumStr, passwordStr;
			string line;

			if (passengers_in)
			{
				istringstream ss;
				while (!found && getline(passengers_in, line))
				{
					/*
						found	getline	run while loop
						0		0		0
						0		1		1
						1		0		0
						1		1		0
					*/

					ss.clear();
					ss.str(line);

					getline(ss, idStr, ',');
					getline(ss, dayStr, ',');
					getline(ss, monthStr, ',');
					getline(ss, yearStr, ',');
					getline(ss, firstNameStr, ',');
					getline(ss, lastNameStr, ',');
					getline(ss, phoneNumStr, ',');
					getline(ss, passwordStr, ',');

					if (phoneNum == phoneNumStr && password == passwordStr)
					{
						found = 1;
						break;
					}
				}
			}

			passengers_in.close();

			if (!found)
			{
				system("cls");
				cerr << endl
					 << "Error: invalid credentials" << endl;
				// show first menu again
				//	main();
				// TODO: think of a more efficient way of going back to start of main
			}
			else
			{
				// logged in as passenger
				system("cls");
				cout << endl
					 << "Logged in as passenger" << endl;
				Passenger passenger(stoi(idStr), stoi(dayStr), stoi(monthStr), stoi(yearStr), firstNameStr, lastNameStr, phoneNumStr, passwordStr);
				int opt = passenger.displayMenu();
				return 0;
			}
		}
		else if (opt2 == 2)
		{
			ifstream drivers_in("drivers.txt");
			string idStr, dayStr, monthStr, yearStr, firstNameStr, lastNameStr, phoneNumStr, passwordStr;
			string modelStr, noOfSeatsStr, nameStr, plateNumStr, colorStr, typeStr;
			string nicStr;
			string line;

			if (drivers_in)
			{
				istringstream ss;
				while (!found && getline(drivers_in, line))
				{
					/*
						found	getline	run while loop
						0		0		0
						0		1		1
						1		0		0
						1		1		0
					*/

					ss.clear();
					ss.str(line);

					getline(ss, idStr, ',');
					getline(ss, dayStr, ',');
					getline(ss, monthStr, ',');
					getline(ss, yearStr, ',');
					getline(ss, firstNameStr, ',');
					getline(ss, lastNameStr, ',');
					getline(ss, phoneNumStr, ',');
					getline(ss, passwordStr, ',');
					getline(ss, modelStr, ',');
					getline(ss, noOfSeatsStr, ',');
					getline(ss, nameStr, ',');
					getline(ss, plateNumStr, ',');
					getline(ss, colorStr, ',');
					getline(ss, typeStr, ',');
					getline(ss, nicStr, ',');

					if (phoneNum == phoneNumStr && password == passwordStr)
					{
						found = 1;
						break;
					}
				}
			}

			drivers_in.close();

			if (!found)
			{
				system("cls");
				cerr << endl
					 << "Error: invalid credentials" << endl;
				// show first menu again
				// main();
				// TODO: think of a more efficient way of going back to start of main
			}
			else
			{
				// logged in as driver
				system("cls");
				cout << endl
					 << "Logged in as driver" << endl;
				
				Vehicle vehicle(stoi(modelStr), stoi(noOfSeatsStr), nameStr, plateNumStr, colorStr, typeStr);
				Driver driver(stoi(idStr), stoi(dayStr), stoi(monthStr), stoi(yearStr), firstNameStr, lastNameStr, phoneNumStr, passwordStr, vehicle, nicStr);
				int opt = driver.displayMenu();
				return 0;
			}
		}
	}
}