#include <stdio.h>

class Visitor;

class Animal {
    public:
        virtual void accept(Visitor * v) {printf("emmm ..");};

};

class Dog : public Animal {
    public:
        virtual void accept(Visitor *);
};

class Cat : public Animal {
    public:
        virtual void accept(Visitor *);
};

class Fox : public Animal {
    public:
        virtual void accept(Visitor *);
};

class Visitor {
    public:
        void visit(Animal * animal) {};
        virtual void visit(Dog * animal) {};
        virtual void visit(Cat * animal) {};
        virtual void visit(Fox * animal) {};

};

class Speaker : public Visitor {
    public:
        void visit(Animal * pa) {
            pa -> accept(this);
        }

        virtual void visit(Dog * pd) {
            printf("wang\n");
        }

        virtual void visit(Cat * pc) {
            printf("miao\n");
        }

        virtual void visit(Fox * pf) {
            printf("woo\n");
        }


};

class Feeding : public Visitor {
    public:
        void visit(Animal * pa) {
            pa ->accept(this);
        }

        virtual void visit(Dog * pd) {
            printf("bone\n");
        }

        virtual void visit(Cat * pc) {
            printf("fish\n");
        }

        virtual void visit(Fox * pf) {
            printf("meat\n");
        }
};

void Dog::accept(Visitor * v) {
    v ->visit(this);
}

void Cat::accept(Visitor * v) {
    v ->visit(this);
}

void Fox::accept(Visitor * v) {
    v ->visit(this);
}


int main() {
    Animal * animals[] = {new Dog(),new Cat(), new Fox()};
    Speaker s;
    for (int i = 0; i < sizeof(animals)/ sizeof(Animal *); i ++) {
        s.visit(animals[i]);
    }
    Feeding f;
    for (int i = 0; i < sizeof(animals)/ sizeof(Animal *); i ++) {
        f.visit(animals[i]);
    }
    return 0;
}
