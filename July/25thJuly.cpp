#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

// 1.
/*class Shape {
public:
	virtual void draw() const{
		std::cout << "Shape." << endl;
	}
	virtual ~Shape() = default;
};

class Circle : public Shape {
public:
	void draw()const override {
		cout << "Circle." << endl;
	}
};

class Rectangle : public Shape {
public:
	void draw()const override {
		cout << "Rectangle." << endl;
	}
};




int main() {
	Shape* shape1 = new Circle();
	Shape* shape2 = new Rectangle();

	shape1->draw();
	shape2->draw();

	delete shape1;
	delete shape2;
}*/



// 2.
/*class AbstractBase {
public:
	virtual void pureFunction() const = 0;
	virtual ~AbstractBase() = default;
};

class Concrete :public AbstractBase {
public:
	void pureFunction() const override {
		cout << "Concrete implementation." << endl;
	}
};

int main() {
	AbstractBase* obj = new Concrete();
	obj->pureFunction();
	delete obj;
}*/

//3.
class Printable {
public:
	virtual void print() const = 0;
	virtual ~Printable() = default;
};

class Document : public Printable {
public:
	void print()const override {
		cout << "Printing the document." << endl;
	}
};

class Image :public Printable {
public:
	void print()const override {
		cout << "Printing the image." << endl;
	}
};

int main() {
	Printable* doc = new Document();
	Printable* img = new Image();

	doc->print();
	img->print();

	delete doc;
	delete img;
}