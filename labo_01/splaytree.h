#ifndef SPLAYTREE_H
#define SPAYTREE_H

#include "searchtree.h"

/*
 * virtual function in de bovenklasse
 * */

template <class Sleutel,class Data>
class SplayTree : public Zoekboom<Sleutel, Data>{
public:
    virtual Zoekboom<Sleutel, Data>* voegtoe(const Sleutel& sleutel,const Data& data,bool dubbelsToestaan=false) override;

protected:
    void splay(zoekKnoop<Sleutel, Data> &knoop);
};

template <class Sleutel, class Data>
Zoekboom<Sleutel, Data>* SplayTree<Sleutel, Data>::voegtoe(const Sleutel &sleutel, const Data &data, bool dubbelsToestaan) {
    zoekKnoop<Sleutel, Data>* ptr = Zoekboom<Sleutel, Data>::voegtoe(sleutel, data, dubbelsToestaan);
    splay(*ptr);
}


template <class Sleutel, class Data>
void SplayTree<Sleutel, Data>::splay(zoekKnoop<Sleutel, Data>& knoop) {
    while (knoop->ouder != nullptr) {
        zoekKnoop<Sleutel, Data> ouder = knoop->ouder;
        zoekKnoop<Sleutel, Data> grootOuder = ouder->ouder;

        if (grootOuder == nullptr) {
            // zig
            bool naarRechts = (*ouder)->links->sleutel == knoop->sleutel;
            Zoekboom<Sleutel, Data>::roteer(naarRechts);
        } else {
            bool ouderNaarRechts = (*ouder)->links->sleutel == knoop->sleutel;
            bool grootOuderNaarRechts = (*grootOuder)->links->sleutel == (*ouder)->links->sleutel;

            bool zigZig = (ouderNaarRechts && grootOuderNaarRechts) || (!ouderNaarRechts && !grootOuderNaarRechts);

            if (zigZig) {
                Zoekboom<Sleutel, Data>* first = knoop.geefBoomBovenKnoop(ouder);
                Zoekboom<Sleutel, Data>* second = knoop.geefBoomBovenKnoop(ouder);
                second->roteer(grootOuderNaarRechts);
                first->roteer(ouderNaarRechts);
            } else {
                Zoekboom<Sleutel, Data>* first = knoop.geefBoomBovenKnoop(ouder);
                Zoekboom<Sleutel, Data>* second = knoop.geefBoomBovenKnoop(ouder);
                first->roteer(ouderNaarRechts);
                second->roteer(grootOuderNaarRechts);
            }
        }
    }
}



#endif //SPAYTREE_H
