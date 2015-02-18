#include<iostream>
#include<fstream>
#include"vector.h"
#include"LinkedList.h"
#include"StackAr.h"
#include"CursorList.h"
#include"QueueAr.h"
#include"StackLi.h"
#include"SkipList.h"
#include "CPUTimer.h"
#include "dsexceptions.h"

using namespace std;

vector<CursorNode <int> > cursorspace(250000);
string word;
int value;

void RunList(string filename){
	List<int> LinkedList;
	ListItr<int> iter = LinkedList.zeroth();
	string line;			//for skipping
	ifstream file;	
	file.open(filename.c_str());
	getline(file, line);		//Skip line 1
	while (file >> word) {
		if (word.at(0) == 'i'){	
			word = word.substr(1,word.length());
			value = atoi(word.c_str());
			LinkedList.insert(value, iter);
		}
		else{
			word = word.substr(1,word.length());
                        value = atoi(word.c_str());
			LinkedList.remove(value);	
		}
	}
	file.close();
}

void RunCursorList(string filename){
	CursorList<int> cursor(cursorspace);
	CursorListItr<int> itr(cursorspace);
	itr=cursor.zeroth();
	string line;                    //for skipping
        ifstream file;
        file.open(filename.c_str());
        getline(file, line);            //Skip line 1
        while (file >> word) {
                if (word.at(0) == 'i'){
                        word = word.substr(1,word.length());
                        value = atoi(word.c_str());
			cursor.insert(value, itr);
		}
		else{
			word = word.substr(1,word.length());
                        value = atoi(word.c_str());
			cursor.remove(value);
		}	
	}
	file.close();
}

void RunStackAr(string filename){
	StackAr<int> stack(250000);
	string line;                    //for skipping
        ifstream file;
        file.open(filename.c_str());
        getline(file, line);            //Skip line 1
        while (file >> word) {
                if (word.at(0) == 'i'){
			word = word.substr(1,word.length());
                        value = atoi(word.c_str());
			stack.push(value);
		}
		else{
			word = word.substr(1,word.length());
                        value = atoi(word.c_str());
			stack.pop();	
		}
	}
	file.close();
}
void RunStackLi(string filename){
	StackLi<int> stackli;
	string line;                    //for skipping
        ifstream file;
        file.open(filename.c_str());
        getline(file, line);            //Skip line 1
        while (file >> word) {
                if (word.at(0) == 'i'){
                        word = word.substr(1,word.length());
                        value = atoi(word.c_str());
			stackli.push(value);
		}
		else{
			word = word.substr(1,word.length());
                        value = atoi(word.c_str());
                        stackli.pop();	
		}	
	}
	file.close();
}

void RunQueueAr(string filename){
	Queue<int> que(250000);
	string line;                    //for skipping
  ifstream file;
  file.open(filename.c_str());
  getline(file, line);            //Skip line 1

	int i = 0;
  while (file >> word) {
		if (word.at(0) == 'i'){
			word = word.substr(1,word.length());
			value = atoi(word.c_str());
			que.enqueue(value);
		}
		else{
			word = word.substr(1,word.length());
			value = atoi(word.c_str());
			que.dequeue();
		}
		
		i++;
	}

	file.close();
}
void RunSkipList(string filename){
	SkipList<int> skip(0, 250000);
	string line;                    //for skipping
        ifstream file;
        file.open(filename.c_str());
        getline(file, line);            //Skip line 1
        while (file >> word) {
                if (word.at(0) == 'i'){
                        word = word.substr(1,word.length());
                        value = atoi(word.c_str());
			skip.insert(value);
		}
		else{
			word = word.substr(1,word.length());
                        value = atoi(word.c_str());
 			skip.deleteNode(value);
		}	
	}
	file.close();
}

int getChoice(){
	int choice;
        cout << endl <<"      ADT Menu" << endl;
        cout <<"0. Quit" << endl << "1. LinkedList" <<endl <<"2. CursorList" << endl;
        cout <<"3. StackAr" << endl << "4. StackLi" << endl << "5. QueueAr" << endl;
        cout << "6. SkipList" << endl << "Your Choice >> ";

	cin >> choice;
	return choice;
}

int main(void)
{
	int choice;
	CPUTimer ct;
	string filename;
       	cout << "Filename >> ";
        cin >> filename;
	do
	{
		choice = getChoice();
		ct.reset();
		switch(choice)
		{
			case 1: RunList(filename); break;
			case 2: RunCursorList(filename); break;
			case 3: RunStackAr(filename); break;
			case 4: RunStackLi(filename); break;
			case 5: RunQueueAr(filename); break;
			case 6: RunSkipList(filename); break;
		}
		cout << "CPU time: " << ct.cur_CPUTime()<< endl;
	} while (choice > 0);
	return 0;
}
