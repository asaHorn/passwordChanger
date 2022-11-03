#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

void printArray(std::string* arr, int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << ";";
    }
    std::cout << std::endl;
}

int getUsers(std::string* users) {
    int err = system("net user >> users.txt");
    if (err != 0) {
        std::cout << "an error occurred getting the users" << std::endl;
        exit(100);
    }

    std::ifstream myfile;
    std::string line;
    std::string user;
    int users_size = 0;
    myfile.open("users.txt");

    //read in all the users
    if (myfile.is_open()) {
        getline(myfile, line); //skip first 4 lines
        getline(myfile, line);
        getline(myfile, line);
        getline(myfile, line);

        while (getline(myfile, line)) {
            if (line.empty()) { continue; }

            int spaceCounter = 0;
            //traverse the line
            for (int i = 0; i <= line.size(); i++) {
                if (user == "The command completed successfully.") {
                    break;
                }
                else if (line[i] == ' ') {
                    if (spaceCounter < 7) { //anything more than three spaces is white space between users
                        user += ' ';
                    }
                    spaceCounter++;
                }
                else if (spaceCounter > 3 || i == line.size()) { //if we are at the end of this user or the end of the line
                    user = user.substr(0, user.size() - 7); //cut off trailing spaces
                    users[users_size] = user; //put that user in the array
                    user.clear();
                    users_size++;
                    spaceCounter = 0;

                    user += line[i]; //we are on the start of the next user
                }
                else {
                    user += line[i];
                }
            }
            user.clear();
        }
        myfile.close();
    }

    system("del users.txt");
    return (users_size);
}

int filterUsers(std::string* users, int users_size) {
    std::string noTouchy;
    std::cout << "The gathered users are:" << std::endl;
    printArray(users, users_size);

    std::string in;
    do {
        std::cout << "Enter user to ignore, or 'exit' to exit" << std::endl;
        std::cin >> in;

        bool foundFlag = false;
        for (int i = 0; i < users_size; i++) {
            if (users[i] == in) {
                foundFlag = true;
                users_size--;
                noTouchy += in + '\n';
            }
            if (foundFlag) { // we are moving everything over
                users[i] = users[i + 1]; //I can do this because I decreased the array size by 1.
            }
        }
        if (!foundFlag) {
            std::cout << "That user doesn't exist to me" << std::endl;
        }
    } while (in != "exit");

    do {
        std::cout << "The following users will not be touched:\n" << noTouchy << std::endl << "type 'OK' to continue";
        std::cin >> in;
    } while (in != "OK");
   
    return(users_size);
}


int main() {
    std::cout << "Getting users..." << std::endl;
    std::string users[100];//I'm way to0 lazy to write a good data structure for this cheap joke
    int users_size = getUsers(users);

    //we have the users, lets filter them
    users_size = filterUsers(users, users_size);

    //change the passwords 1 time. That is what this code does right??
    int changes = 100000; //should be 1
    for (int num = 0; num < changes; num++) {
        for (int i = 0; i < users_size; i++) {
            std::string cmd = "net user " + users[i] + " !Pa" + std::to_string(rand());
            std::cout << cmd << std::endl;
            system(cmd.c_str());
        }
        std::cout << "Change " << std::to_string(num) << "/" << std::to_string(changes) << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;

}


