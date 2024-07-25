#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

class MagicCreature {
public:
	MagicCreature(){
	name = "Unnamed";
	type = "Unknown";
	age = 0;
	magicLevel = 0;
	}

	MagicCreature(string Name, string Type,int Age, int magicLevel)
		: name(Name), type(Type), age(Age),magicLevel(magicLevel){}
	
	MagicCreature(const MagicCreature& copy) 
		:name(copy.name), type(copy.type), age(copy.age), magicLevel(copy.magicLevel){
		cout << "Copy constructor conjured!" << endl;
	}
	void display() {
		cout << "Name of creature= " << name << ", type of creature= " << type << ", age of creature= " << age << ", magic level of creature= " << magicLevel << "." << endl;
	}
	void display(bool alive = false)const {
		if (alive) {
			cout << "The creature is alive and well. It resides in a nearby town called Aleport." << endl;
			cout << "Name of creature= " << name << ", type of creature= " << type << ", age of creature= " << age << ", magic level of creature= " << magicLevel << "." << endl;
		}
		else {
			cout << "The creature was slain in battle. It fought gloriously until it's very last moments. " << endl;
		}
	}
	string getName()const {
		return name;
	}
	
	string getType()const {
		return type;
	}
	
	int getAge() const {
		return age;
	}
	
	int getmagicLevel() const{
		return magicLevel;
	}
	
	void setName(const string &newname) {
		name = newname;
	}
	
	void setType(const string &newtype) {
		type = newtype;
	}
	
	void setAge(int newage) {
		age = newage;
	}
	
	void setmagicLevel(int newmagicLevel) {
		magicLevel = newmagicLevel;
	}

private:
	string name;
	string type;
	int age;
	int magicLevel;
};


int main() {
	MagicCreature creature1("Kappa", "Water", 47, 18);
	creature1.display();
	MagicCreature creature2 = creature1;
	creature2.display();

	creature2.setName("Orc");
	creature2.setType("Earth");
	creature2.setAge(65);
	creature2.setmagicLevel(10);
	cout << "Updated values of second creature:" << endl;
	creature2.display();
	creature2.display(false);
}