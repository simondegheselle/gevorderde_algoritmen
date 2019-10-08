#ifndef SPLAYTREE_H
#define SPAYTREE_H

#include "searchtree.h"

/*
 * virtual function in de bovenklasse, in deze klasse overschrijven met de override na de functie header
 * 
 * */

enum ROTATION_DIRECTION {
    LEFT,
    RIGHT,
    INVALID
};

template <class Sleutel,class Data>
class SplayTree : public Zoekboom<Sleutel, Data>{
public:
    zoekKnoop<Sleutel, Data>* voegtoe(const Sleutel& sleutel,const Data& data,bool dubbelsToestaan) override;

protected:
    void splay(zoekKnoop<Sleutel, Data> *);
};

template <class Sleutel, class Data>
zoekKnoop<Sleutel, Data>* SplayTree<Sleutel, Data>::voegtoe(const Sleutel &sleutel, const Data &data, bool dubbelsToestaan) {
    zoekKnoop<Sleutel, Data> * plaatsPtr = Zoekboom<Sleutel, Data>::voegtoe(sleutel, data, dubbelsToestaan);
	this->splay(plaatsPtr);
	return plaatsPtr;
}


template <class Sleutel, class Data>
void SplayTree<Sleutel, Data>::splay(zoekKnoop<Sleutel, Data> * node) {
    while (node->ouder != nullptr) {
        zoekKnoop<Sleutel, Data> * p = node->ouder;
        zoekKnoop<Sleutel, Data> * g = p->ouder;
		
        if (g == nullptr) {
            // zig
            ROTATION_DIRECTION  direction = ROTATION_DIRECTION::INVALID;

            if (p->links && p->links->sleutel == node->sleutel) {
                direction = ROTATION_DIRECTION::RIGHT;
            } else if (p->rechts && p->rechts->sleutel == node->sleutel) {
                direction = ROTATION_DIRECTION::LEFT;
            }

            if (direction != ROTATION_DIRECTION::INVALID) {
                Zoekboom<Sleutel, Data>* subTree = this->geefBoomBovenKnoop(*p);
                subTree->roteer(direction == ROTATION_DIRECTION::RIGHT);
            }
        } else {
            ROTATION_DIRECTION first_dir = ROTATION_DIRECTION::INVALID;
            if (p->links && p->links->sleutel == node->sleutel) {
                first_dir = ROTATION_DIRECTION::RIGHT;
            } else if (p->rechts && p->rechts->sleutel == node->sleutel) {
                first_dir = ROTATION_DIRECTION::LEFT;
            }

            ROTATION_DIRECTION second_dir = ROTATION_DIRECTION::INVALID;
            if (g->links && g->links->sleutel == p->sleutel) {
                second_dir = ROTATION_DIRECTION::RIGHT;
            } else if (g->rechts && g->rechts->sleutel == p->sleutel) {
                second_dir = ROTATION_DIRECTION::LEFT;
            }

            if (first_dir != ROTATION_DIRECTION::INVALID && second_dir != ROTATION_DIRECTION::INVALID) {
                if (first_dir == second_dir) { // check if zig zig
                    Zoekboom<Sleutel, Data> *firstSubTree = this->geefBoomBovenKnoop(*p);
                    Zoekboom<Sleutel, Data> *secondSubTree = this->geefBoomBovenKnoop(*g);
                    secondSubTree->roteer(second_dir == ROTATION_DIRECTION::RIGHT);
                    firstSubTree->roteer(first_dir == ROTATION_DIRECTION::RIGHT);
                } else {
                    Zoekboom<Sleutel, Data> *firstSubTree = this->geefBoomBovenKnoop(*p);
                    Zoekboom<Sleutel, Data> *secondSubTree = this->geefBoomBovenKnoop(*g);
                    firstSubTree->roteer(first_dir == ROTATION_DIRECTION::RIGHT);
                    secondSubTree->roteer(second_dir == ROTATION_DIRECTION::RIGHT);
                }
            }

        }
    }
}

#endif //SPAYTREE_H
